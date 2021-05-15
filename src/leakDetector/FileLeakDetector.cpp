# include <string.h>
# include <stdio.h>
# include <pthread.h>
# include <string>
# include <iostream>
# include <unistd.h>
# include <sys/stat.h>
# include <fcntl.h>

# include "FileLeakDetector.hpp"
using namespace std;

static File_leak * leak_start = NULL;
static File_leak * leak_next = NULL;
static File_Leak_info leak_info = {0};
static string FILENAME;

int wrapper_open(const char * str, int flag, const char * file, unsigned int line) {
    int fd = open(str, flag);
    if (fd != -1) {
        add_file_info(fd, file, line);
    }
    return fd;
}

int wrapper_close (int fd, const char * file, unsigned int line) {
    int res = close(fd);
    if (res != -1) {
        remove_file_info(fd, file, line);
    }
    return res;
}

void add_file_info(int fd, const char * file, unsigned int line) {
    File_info file_info;
    memset( &file_info, 0, sizeof(file_info) );
    file_info.fd = fd;
    file_info.file_name = file;
    file_info.line = line;

    if (!leak_info.is_first) {
        leak_info.is_first ++;
        MUTEX_CREATE(leak_info.mutex);
        FILENAME = string(strtok((char *)(string(file)).c_str(), "."));
        string DIRNAME = string(getcwd(NULL, 0)) + "/Output/";
        if (access(DIRNAME.c_str(), F_OK)) {
            mkdir(DIRNAME.c_str(),ALLPERMS);
        }
        FILENAME = DIRNAME + string(basename(FILENAME.c_str())) + "_File.txt";
        FILE *f = fopen(FILENAME.c_str(), "w");
        fclose(f);
    }

    MUTEX_LOCK(leak_info.mutex);
    insert_leak(file_info);
    MUTEX_UNLOCK(leak_info.mutex);

    FILE *f = fopen(FILENAME.c_str(), "a");
    if (f!=NULL) {
        fprintf(f, "[Open File]\nFile: %s\nFile Descriptor: %d\nLine: %d\n\n", file, fd, line);
        fclose(f);
    }
}

void remove_file_info(int fd, const char * file, unsigned int line) {
    File_leak * file_leak = leak_start;
    bool success = 0;

    MUTEX_LOCK(leak_info.mutex);
    for (unsigned int i = 0; file_leak!=NULL; ++i) {
        if (file_leak->file_info.fd == fd) {
            delete_leak(i);
            success = 1;
            break;
        }
        file_leak = file_leak->next;
    }
    MUTEX_UNLOCK(leak_info.mutex);

    if (!success) {
        printf("Failure: close file.\n");
    } else {
        FILE *f = fopen(FILENAME.c_str(), "a");
        if (f!=NULL) {
            fprintf(f, "[Close File]\nFile: %s\nFile Descriptor: %d\nLine: %d\n\n", file, fd, line);
            fclose(f);
        }
    }
}

void insert_leak(File_info fd_info) {
    File_leak * file_leak = new File_leak();
    file_leak->file_info = fd_info;
    file_leak->next = NULL;

    if (leak_start == NULL) {
        leak_start = file_leak;
        leak_next = file_leak;
    } else {
        leak_next->next = file_leak;
        leak_next = leak_next->next;
    }

    leak_info.num ++;
}

void delete_leak(int pos) {
    File_leak *fd_info = leak_start;
    File_leak *close_info = leak_start;
    if (pos == 0) {
        leak_start = leak_start->next;
    } else {
        unsigned int i = pos - 1;
        while (i--) fd_info = fd_info->next;
        close_info = fd_info->next;
        fd_info->next = close_info->next;

        if (leak_next == close_info) {
            leak_next = fd_info;
        }
    }

    free(close_info);
    leak_info.num --;
}

void file_report() {
    File_leak * leak_item;
    FILE *f = fopen(FILENAME.c_str(), "a");
    if (f!=NULL) {
        fprintf(f, "   File Descriptor Leak Summary\n");
        fprintf(f, "-----------------------------------\n");
        fprintf(f, "Leak total: %d\n\n", leak_info.num);
        
        for (leak_item = leak_start; leak_item!= NULL; leak_item = leak_item->next) {
            fprintf(f, "File Descriptor: %d\nFile Name: %s\nLine: %d\n", 
                leak_item->file_info.fd, leak_item->file_info.file_name.c_str(), leak_item->file_info.line);
            fprintf(f, "-----------------------------------\n");
        }
        fclose(f);
    }
    file_clear();
    MUTEX_DESTROY(leak_info.mutex);
}

void file_clear() {
	File_leak * leak_item = leak_start;
	File_leak * open_info = leak_start;

	while(open_info != NULL) 
	{
		open_info = open_info->next;
		free(leak_item);
		leak_item = open_info;
	}
}