# include <iostream>
# include <fstream>
# include <cstring>

# include "Mem.h"
# include "utils.h"

using namespace std;

void getCurMemInfo(){
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
            getInteger(statFile, MemInfo::m_total);
        } else if ( key == "MemFree:" ) {
            getInteger(statFile, MemInfo::m_free);
        } else if ( key == "Buffers:" ) {
            getInteger(statFile, MemInfo::m_buffers);
        } else if ( key == "Cached:" ) {
            getInteger(statFile, MemInfo::m_cached);
        }
        clearLine(statFile);
    }

    MemInfo::m_used = MemInfo::m_total - MemInfo::m_free -MemInfo::m_buffers - MemInfo::m_cached;
}