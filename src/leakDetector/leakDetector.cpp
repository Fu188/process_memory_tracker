# include <string.h>
# include <stdio.h>

# include "leakDetector.hpp"

using namespace std;

static Mem_leak * leak_start = NULL;
static Mem_leak * leak_next = NULL;
static Leak_info leak_info = {0};

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
            remove_mem_info(str);
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

void * wrapper_free(void * mem_ref) {
    remove_mem_info(mem_ref);
    free(mem_ref);
}

void add_mem_info(void * mem_ref, unsigned int size, char * file, unsigned int line) {
    Mem_info mem_info;
    memset( &mem_info, 0, sizeof(mem_info) );
    mem_info.address = mem_ref;
    mem_info.size = size;
    mem_info.file_name = file;
    mem_info.line = line;

    if (!leak_info.is_first) {
        leak_info.is_first ++;
        MUTEX_CREATE(leak_info.mutex);
    }

    FILE *f = fopen(strtok(file, "."), "w");

    fprintf(f, "File: %s\nLine: %d\nSize: %zu bytes\n", file, line, size);
	fclose(f);

    MUTEX_LOCK(leak_info.mutex);
    insert_leak(mem_info);
    MUTEX_UNLOCK(leak_info.mutex);
}

void remove_mem_info(void * mem_ref) {
    Mem_leak * mem_leak = leak_start;
    bool success = 0;

    MUTEX_LOCK(leak_info.mutex);
    for (unsigned int i = 0; mem_leak!=NULL; ++i) {
        if (mem_leak->mem_info.address == mem_ref) {
            delete_leak(i);
            success = 1;
            break;
        }
        mem_leak = mem_leak->next;
    }
    MUTEX_UNLOCK(leak_info.mutex);

    if (!success) {
        printf("Failure: remove memory.\n");
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

void delete_leak(unsigned int pos) {
    Mem_leak *alloc_info = leak_start;
    Mem_leak *free_info = leak_start;
    if (pos == 0) {
        leak_start = leak_start->next;
    } else {
        unsigned int i = pos - 1;
        while (i--) alloc_info = alloc_info->next;
        free_info = alloc_info->next;

        if (leak_next == free_info) {
            leak_next = alloc_info;
        }
    }

    leak_info.total_memory -= free_info->mem_info.size;
    free(free_info);
    leak_info.num --;
}