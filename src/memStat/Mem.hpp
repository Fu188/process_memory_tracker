# ifndef MEM_H
# define MEM_H

# include <string>

using namespace std;

const __int64_t TOTAL_VM_SIZE = (0xffffffffffffffff);

class MemInfo {
public:
    static long m_total;
    static long m_free;
    static long m_buffers;
    static long m_cached;
    static long m_used;

public:
    static void getCurMemInfo();
};



# endif