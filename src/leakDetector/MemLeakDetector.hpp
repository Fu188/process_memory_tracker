# ifndef MEMLEAKDETECTOR_H
# define MEMLEAKDETECTOR_H

# include <string>
#include <vector>
#include <time.h>

using namespace std;

# define MUTEX_CREATE(handle)       pthread_mutex_init(&(handle), NULL)
# define MUTEX_LOCK(handle)         pthread_mutex_lock(&(handle))
# define MUTEX_UNLOCK(handle)       pthread_mutex_unlock(&(handle))
# define MUTEX_DESTROY(handle)      pthread_mutex_destroy(&(handle))

typedef struct _mem_info {
    void * address;
    unsigned int size;
    std::string file_name;
    unsigned int line;
    time_t start;
} Mem_info ;

typedef struct _mem_leak {
    Mem_info mem_info;
    struct _mem_leak *next;
} Mem_leak ;

typedef struct _mem_leak_info {
    int num;
    unsigned int total_memory;
    unsigned int used_max;
    unsigned int once_max;
    bool is_first;
    pthread_mutex_t mutex;
} Mem_Leak_info ;

void add_mem_info(void * mem_ref, unsigned int size, const char * file, unsigned int line);
void remove_mem_info(void * mem_ref, const char * file, unsigned int line);
void insert_leak(Mem_info alloc_info);
unsigned int delete_leak(unsigned int pos);

void mem_report();
void mem_clear();

vector<Mem_info> getAllMemLeak();
vector<Mem_info> getMemLeakByName();
vector<Mem_info> getMemLeakBySize();
vector<Mem_info> getMemLeakByTime();

bool LeakCmpName(const Mem_info& a,const Mem_info& b);
bool LeakCmpSize(const Mem_info& a,const Mem_info& b);
bool LeakCmpTime(const Mem_info& a,const Mem_info& b);

# endif