# ifndef WRAPPER_H
# define WRAPPER_H

# define malloc(size)               wrapper_malloc(size, __FILE__, __LINE__)
# define calloc(element, size)      wrapper_calloc(element, size, __FILE__, __LINE__)
# define realloc(str, size)         wrapper_realloc(str, size, __FILE__, __LINE__)
# define strdup(str)                wrapper_strdup(str, __FILE__, __LINE__)
# define free(mem_ref)              wrapper_free(mem_ref, __FILE__, __LINE__)

void * wrapper_malloc(unsigned int size, const char * file, unsigned int line);
void * wrapper_calloc(unsigned int element, unsigned int size, const char * file, unsigned int line);
void * wrapper_realloc(void * str, unsigned int size, const char * file, unsigned int line);
char * wrapper_strdup(const char * str, const char * file, unsigned int line);
void * wrapper_free(void * mem_ref, const char * file, unsigned int line);

# define open(str, flag)            wrapper_open(str, flag, __FILE__, __LINE__)
# define close(fd)                  wrapper_close(fd, __FILE__, __LINE__)

int wrapper_open(const char * str, int flag, const char * file, unsigned int line);
int wrapper_close (int fd, const char * file, unsigned int line);

# endif
