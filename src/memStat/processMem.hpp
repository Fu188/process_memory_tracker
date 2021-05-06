# ifndef PROCESSMEM_H
# define PROCESSMEM_H

# include <string>
# include <vector>

using namespace std;

/* Define the state of each process */
enum ProcessState {RUNNING, SLEEPING, IDLE, ZOMBIE, WAITING, STOPPED};
static const string ProcessStateString[] = {"RUNNING", "SLEEPING", "IDLE", "ZOMBIE", "WAITING", "STOPPED"};

/* define the info of each process */
typedef struct {
    string name;
    ProcessState state;
    int pid;
    long vmsize;
    float vmsize_per;
    long vmrss;
    float vmrss_per;
} processMemInfo;

vector<processMemInfo> getAllMemStat();
vector<processMemInfo> getMemStatByPid();
vector<processMemInfo> getMemStatByState();
vector<processMemInfo> getMemStatByVmsize();
vector<processMemInfo> getMemStatByVmrss();

bool isProcess(string str);
void getProcessList(string path, vector<string>& processList);
void getProcessInfo(string pid, vector<processMemInfo>& processMemInfoList);
void getUtilization(processMemInfo* info);

bool MemCmpPid(const processMemInfo& a,const processMemInfo& b);
bool MemCmpState(const processMemInfo& a,const processMemInfo& b);
bool MemCmpVmsize(const processMemInfo& a,const processMemInfo& b);
bool MemCmpVmrss(const processMemInfo& a,const processMemInfo& b);

# endif