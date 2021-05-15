# include <string.h>
# include <stdio.h>
# include <pthread.h>
# include <string>
# include <iostream>
# include <unistd.h>
# include <sys/stat.h>

# include "MemLeakDetector.hpp"
using namespace std;

static Mem_leak * leak_start = NULL;
static Mem_leak * leak_next = NULL;
static Mem_Leak_info leak_info = {0};
static string FILENAME;

void * wrapper_malloc(unsigned int size, const char * file, unsigned int line) {
    void * ptr = malloc(size);
    if (ptr != NULL) {
        add_mem_info (ptr, size, file, line);
    }
    return ptr;
}

void * wrapper_calloc(unsigned int element, unsigned int size, const char * file, unsigned int line) {
    void * ptr = calloc(element, size);
    if (ptr != NULL) {
        add_mem_info (ptr, element*size, file, line);
    }
    return ptr;
}

void * wrapper_realloc(void * str, unsigned int size, const char * file, unsigned int line) {
    void * ptr = realloc(str, size);
    if (ptr != NULL) {
        if (str == NULL){
            add_mem_info(ptr, size, file, line);
        } else if ( ptr != str ) {
            remove_mem_info(str, file, line);
            add_mem_info(ptr, size, file, line);
        }
    }
    return ptr;
}

char * wrapper_strdup(const char * str, const char * file, unsigned int line) {
    char * ptr = strdup(str);
    if (ptr != NULL) {
        add_mem_info(ptr, strlen(str)+1, file, line);
    }
    return ptr;
}

void * wrapper_free(void * mem_ref, const char * file, unsigned int line) {
    remove_mem_info(mem_ref, file, line);
    free(mem_ref);
}

void add_mem_info(void * mem_ref, unsigned int size, const char * file, unsigned int line) {
    Mem_info mem_info;
    memset( &mem_info, 0, sizeof(mem_info) );
    mem_info.address = mem_ref;
    mem_info.size = size;
    mem_info.file_name = file;
    mem_info.line = line;
    if (!leak_info.is_first) {
        leak_info.is_first ++;
        MUTEX_CREATE(leak_info.mutex);
        FILENAME = string(strtok((char *)(string(file)).c_str(), "."));
        string DIRNAME = string(getcwd(NULL, 0)) + "/Output/";
        if (access(DIRNAME.c_str(), F_OK)) {
            mkdir(DIRNAME.c_str(),ALLPERMS);
        }
        FILENAME = DIRNAME + string(basename(FILENAME.c_str())) + "_Mem.txt";
        FILE *f = fopen(FILENAME.c_str(), "w");
        fclose(f);
    }

    MUTEX_LOCK(leak_info.mutex);
    insert_leak(mem_info);
    MUTEX_UNLOCK(leak_info.mutex);

    FILE *f = fopen(FILENAME.c_str(), "a");
    if (f!=NULL) {
        fprintf(f, "[Allocate Memeory]\nFile: %s\nLine: %d\nSize: %zu bytes\n\n", file, line, size);
        fclose(f);
    }
}

void remove_mem_info(void * mem_ref, const char * file, unsigned int line) {
    Mem_leak * mem_leak = leak_start;
    bool success = 0;
    unsigned int size;

    MUTEX_LOCK(leak_info.mutex);
    for (unsigned int i = 0; mem_leak!=NULL; ++i) {
        if (mem_leak->mem_info.address == mem_ref) {
            size = delete_leak(i);
            success = 1;
            break;
        }
        mem_leak = mem_leak->next;
    }
    MUTEX_UNLOCK(leak_info.mutex);

    if (!success) {
        printf("Failure: remove memory.\n");
    } else {
        FILE *f = fopen(FILENAME.c_str(), "a");
        if (f!=NULL) {
            fprintf(f, "[Free Memeory]\nFile: %s\nLine: %d\nSize: %zu bytes\n\n", file, line, size);
            fclose(f);
        }
    }
}

void insert_leak(Mem_info alloc_info) {
    Mem_leak * mem_leak = new Mem_leak();
    mem_leak->mem_info = alloc_info;
    mem_leak->next = NULL;

    if (leak_start == NULL) {
        leak_start = mem_leak;
        leak_next = mem_leak;
    } else {
        leak_next->next = mem_leak;
        leak_next = leak_next->next;
    }

    leak_info.num ++;
    leak_info.total_memory += alloc_info.size;
    if (leak_info.total_memory > leak_info.used_max) {
        leak_info.used_max = leak_info.total_memory;
    }
    if (alloc_info.size > leak_info.once_max) {
        leak_info.once_max = alloc_info.size;
    }

}

unsigned int delete_leak(unsigned int pos) {
    Mem_leak *alloc_info = leak_start;
    Mem_leak *free_info = leak_start;
    if (pos == 0) {
        leak_start = leak_start->next;
    } else {
        unsigned int i = pos - 1;
        while (i--) alloc_info = alloc_info->next;
        free_info = alloc_info->next;
        alloc_info->next = free_info->next;

        if (leak_next == free_info) {
            leak_next = alloc_info;
        }
    }

    unsigned int free_size = free_info->mem_info.size;
    leak_info.total_memory -= free_size;
    free(free_info);
    leak_info.num --;
    return free_size;
}

void mem_report() {
    Mem_leak * leak_item;
    FILE *f = fopen(FILENAME.c_str(), "a");
    if (f!=NULL) {
        fprintf(f, "       Memory Leak Summary\n");
        fprintf(f, "-----------------------------------\n");
        fprintf(f, "Leak Total Num: %d, Leak Total Size: %u Bytes\n", leak_info.num, leak_info.total_memory);
        fprintf(f, "Max used: %u Bytes, Once Max: %u Bytes\n\n", leak_info.used_max, leak_info.once_max);
        
        for (leak_item = leak_start; leak_item!= NULL; leak_item = leak_item->next) {
            fprintf(f, "Address: 0x%8x\nSize: %u Bytes\n", leak_item->mem_info.address, leak_item->mem_info.size);
            fprintf(f, "File Name: %s\nLine: %u\n", leak_item->mem_info.file_name.c_str(), leak_item->mem_info.line);
            fprintf(f, "-----------------------------------\n");
        }
        fclose(f);
    }
}

