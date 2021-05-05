# ifndef PROCESSMEM_H
# define PROCESSMEM_H

# include <string>
# include <vector>

using namespace std;

struct processMemInfo {
    string name;
    string state;
    int pid;
    long vmsize;
    int vmsize_per;
    long vmrss;
    int vmrss_per;
};

bool isProcess(string str);
void getProcessList(string path, vector<string>& processList);
void getProcessInfo(string pid, struct processMemInfo* info);
void getUtilization(struct processMemInfo* info);

# endif