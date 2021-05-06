# include <cstdio>
# include <cstring>
# include <iostream>
# include <fstream>
# include <cstdlib>
# include <dirent.h>
# include <vector>
# include <cmath>
# include <algorithm>

# include "processMem.hpp"
# include "Mem.hpp"
# include "utils.hpp"

using namespace std;

vector<processMemInfo> getAllMemStat() {
    // update currentMemInfo
    MemInfo::getCurMemInfo();

    vector<string> processList;
    vector<processMemInfo> processMemInfoList;

    getProcessList("/proc", processList);
    for(int i=0;i<processList.size();++i){
        getProcessInfo(processList[i],processMemInfoList);
    }

    return processMemInfoList;
}

vector<processMemInfo> getMemStatByPid() {
    vector<processMemInfo> processMemInfoList = getAllMemStat();
    sort(processMemInfoList.begin(), processMemInfoList.end(), MemCmpPid);
    return processMemInfoList;
}

vector<processMemInfo> getMemStatByState() {
    vector<processMemInfo> processMemInfoList = getAllMemStat();
    sort(processMemInfoList.begin(), processMemInfoList.end(), MemCmpState);
    return processMemInfoList;
}

vector<processMemInfo> getMemStatByVmsize() {
    vector<processMemInfo> processMemInfoList = getAllMemStat();
    sort(processMemInfoList.begin(), processMemInfoList.end(), MemCmpVmsize);
    return processMemInfoList;
}

vector<processMemInfo> getMemStatByVmrss() {
    vector<processMemInfo> processMemInfoList = getAllMemStat();
    sort(processMemInfoList.begin(), processMemInfoList.end(), MemCmpVmrss);
    return processMemInfoList;
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

void getProcessInfo(string pid, vector<processMemInfo>& processMemInfoList) {
    string statFilePath = "/proc/" + pid + "/status";
    fstream statFile (statFilePath, ios_base::in);

    if (statFile.fail()) {
        return;
    }

    processMemInfo info;

    /* To retrieve the infomation we want */
    string key;
    while ( !statFile.eof() ) {
        statFile >> key;
        // Caused by the last empty line
        if (statFile.fail()) break;

        /* The key we are looking for */
        if ( key == "Name:" ) {
            statFile >> info.name;
        } else if ( key == "State:" ) {
            getState(statFile, info.state);
        } else if ( key == "Pid:" ) {
            getInteger(statFile, info.pid);
        } else if ( key == "VmSize:" ) {
            getInteger(statFile, info.vmsize);
        } else if ( key == "VmRSS:" ) {
            getInteger(statFile, info.vmrss);
        }
        clearLine(statFile);
    }

    getUtilization(&info);
    processMemInfoList.push_back(info);
}

void getUtilization(processMemInfo* info) {
    info->vmsize_per = round((info->vmsize * 10000.0) / TOTAL_VM_SIZE) / 100.0;
    info->vmrss_per = round((info->vmrss * 10000.0) / MemInfo::m_total) / 100.0;
}

bool isProcess(string str){
    for (int i = 0; i<str.size(); i++) {
        if (!isdigit(str[i])) return false;
    }
    return true;
}

/* Compare Function */
bool MemCmpPid(const processMemInfo& a,const processMemInfo& b) {
    return a.pid < b.pid;
}

bool MemCmpState(const processMemInfo& a,const processMemInfo& b) {
    if (a.state == b.state) return a.pid < b.pid;
    return a.state < b.state;
}

bool MemCmpVmsize(const processMemInfo& a,const processMemInfo& b) {
    return a.vmsize > b.vmsize;
}

bool MemCmpVmrss(const processMemInfo& a,const processMemInfo& b) {
    return a.vmrss > b.vmrss;
}