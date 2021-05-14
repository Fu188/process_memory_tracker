# ifndef LEAKDETECTOR_H
# define LEAKDETECTOR_H

# include <string>
# include <pthread.h>

using namespace std;

# define malloc(size)               wrapper_malloc(size, __FILE__, __LINE__)
# define calloc(element, size)      wrapper_calloc(element, size, __FILE__, __LINE__)
# define realloc(str, size)         wrapper_realloc(str, size, __FILE__, __LINE__)
# define strdup(str)                wrapper_strdup(str, __FILE__, __LINE__)
# define free(mem_ref)              wrapper_free(mem_ref)

# define MUTEX_CREATE(handle)       pthread_mutex_init(&(handle), NULL)
# define MUTEX_LOCK(handle)         pthread_mutex_lock(&(handle))
# define MUTEX_UNLOCK(handle)       pthread_mutex_unlock(&(handle))
# define MUTEX_DESTROY(handle)      pthread_mutex_destroy(&(handle))

typedef struct _mem_info {
    void * address;
    unsigned int size;
    string file_name;
    unsigned int line;
} Mem_info ;

typedef struct _mem_leak {
    Mem_info mem_info;
    struct _mem_leak *next;
} Mem_leak ;

typedef struct _leak_info {
    int num;
    unsigned int total_memory;
    unsigned int used_max;
    unsigned int once_max;
    bool is_first;
    pthread_mutex_t mutex;
} Leak_info ;

void * wrapper_malloc(unsigned int size, const char * file, unsigned int line);
void * wrapper_calloc(unsigned int element, unsigned int size, const char * file, unsigned int line);
void * wrapper_realloc(void * str, unsigned int size, const char * file, unsigned int line);
char * wrapper_strdup(const char * str, const char * file, unsigned int line);
void * wrapper_free(void * mem_ref);

void add_mem_info(void * mem_ref, unsigned int size, char * file, unsigned int line);
void remove_mem_info(void * mem_ref);
void insert_leak(Mem_info alloc_info);
void delete_leak(unsigned int pos);

# endif
