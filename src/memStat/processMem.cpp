# include <cstdio>
# include <cstring>
# include <iostream>
# include <fstream>
# include <cstdlib>
# include <dirent.h>
# include <vector>

# include "processMem.h"
# include "Mem.h"
# include "utils.h"

using namespace std;

bool isProcess(string str){
    for (int i = 0; i<str.size(); i++) {
        if (!isdigit(str[i])) return false;
    }
    return true;
}

void getProcessList(string path, vector<string>& processList){
    DIR *pDir;
    struct dirent* ptr;
    if (!(pDir = opendir(path.c_str()))) return;
    while ((ptr = readdir(pDir))!=0) {
        if (strcmp(ptr->d_name, ".") && strcmp(ptr->d_name, "..") && isProcess(ptr->d_name)) {
            processList.push_back(ptr->d_name);
        }
    }
}

void getProcessInfo(string pid, struct processMemInfo* info) {
    string statFilePath = "/proc/" + pid + "/status";
    fstream statFile (statFilePath, ios_base::in);

    if (statFile.fail()) {
        printf ("Fail to open %s\n",statFilePath);
        return;
    }

    /* To retrieve the infomation we want */
    string key;
    while ( !statFile.eof() ) {
        statFile >> key;
        // Caused by the last empty line
        if (statFile.fail()) break;

        /* The key we are looking for */
        if ( key == "Name:" ) {
            statFile >> info->name;
        } else if ( key == "State:" ) {
            statFile >> info->state;
        } else if ( key == "Pid:" ) {
            getInteger(statFile, info->pid);
        } else if ( key == "VmSize:" ) {
            getInteger(statFile, info->vmsize);
        } else if ( key == "VmRSS:" ) {
            getInteger(statFile, info->vmrss);
        }
        clearLine(statFile);
    }

    getUtilization(info);
}

void getUtilization(struct processMemInfo* info) {
    info->vmsize_per = (info->vmsize * 100.0) / TOTAL_VM_SIZE;
    info->vmrss_per = (info->vmrss * 100.0) / MemInfo::m_total;
}