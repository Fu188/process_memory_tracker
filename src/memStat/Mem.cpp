# include <iostream>
# include <fstream>
# include <cstring>

# include "Mem.hpp"
# include "utils.hpp"

using namespace std;

long MemInfo::m_total;
long MemInfo::m_free;
long MemInfo::m_buffers;
long MemInfo::m_cached;
long MemInfo::m_used;

void MemInfo::getCurMemInfo(){
    fstream statFile ("/proc/meminfo", ios_base::in);

    if (statFile.fail()) {
        printf ("Fail to open /proc/meminfo\n");
        return;
    }

    /* To retrieve the infomation we want */
    string key, value;
    while ( !statFile.eof() ) {
        statFile >> key;
        // Caused by the last empty line
        if (statFile.fail()) break;

        /* The key we are looking for */
        if ( key == "MemTotal:" ) {
            getInteger(statFile, m_total);
        } else if ( key == "MemFree:" ) {
            getInteger(statFile, m_free);
        } else if ( key == "Buffers:" ) {
            getInteger(statFile, m_buffers);
        } else if ( key == "Cached:" ) {
            getInteger(statFile, m_cached);
        }
        clearLine(statFile);
    }

    m_used = m_total - m_free -m_buffers - m_cached;
}