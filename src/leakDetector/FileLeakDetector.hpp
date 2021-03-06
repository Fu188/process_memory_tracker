# ifndef FILELEAKDETECTOR_H
# define FILELEAKDETECTOR_H

# include <string>
# include <vector>
#include <time.h>

using namespace std;

# define MUTEX_CREATE(handle)       pthread_mutex_init(&(handle), NULL)
# define MUTEX_LOCK(handle)         pthread_mutex_lock(&(handle))
# define MUTEX_UNLOCK(handle)       pthread_mutex_unlock(&(handle))
# define MUTEX_DESTROY(handle)      pthread_mutex_destroy(&(handle))

typedef struct _file_info {
    int fd;
    std::string file_name;
    unsigned int line;
    time_t start;
} File_info ;

typedef struct _file_leak {
    File_info file_info;
    struct _file_leak *next;
} File_leak ;

typedef struct _leak_info {
    int num;
    bool is_first;
    pthread_mutex_t mutex;
} File_Leak_info ;

void add_file_info(int fd, const char * file, unsigned int line);
void remove_file_info(int fd, const char * file, unsigned int line);
void insert_leak(File_info fd_info);
void delete_leak(int pos);

void file_report();
void file_clear();

vector<File_info> getAllFileLeak();
vector<File_info> getFileLeakByName();
vector<File_info> getFileLeakByTime();

bool LeakCmpName(const File_info& a,const File_info& b);
bool LeakCmpTime(const File_info& a,const File_info& b);

# endif