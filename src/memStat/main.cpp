# include <iostream>
# include <vector>
# include <unistd.h>
# include <csignal>

# include "display.hpp"
# include "Mem.hpp"
# include "processMem.hpp"

using namespace std;

void signalHandler(int signum) {
    clearScreen();
    showCursor();
    exit(signum);
}

int main(int argc, char *argv[]) {
    /* register exception function */
    signal(SIGINT, signalHandler);

    int displayNum = 10;
    int sortRegulation = 1;

    /* parameter parser 
     * n: display number
     * s: sort regulation */
    const char *optstring = "n::s::h::";
    int o;
    while ((o = getopt(argc, argv, optstring)) != -1) {
        switch (o) {
            case 'n':
                displayNum = atoi(optarg);
                break;
            case 's':
                sortRegulation = atoi(optarg);
                break;
            default:
                displayHelp();
                return 0;
        }
    }

    if (!checkArgs(displayNum, sortRegulation)) return 0;

    vector<processMemInfo> (*memStatFunc)();
    switch (sortRegulation) {
        case 0: { memStatFunc = &getMemStatByPid; break; }
        case 1: { memStatFunc = &getMemStatByVmrss; break; }
        case 2: { memStatFunc = &getMemStatByVmsize; break; }
        case 3: { memStatFunc = &getMemStatByState; break; }
        default: memStatFunc = &getMemStatByVmrss;
    }

    /* Begin detecting the usage of memory */
    hideCursor();
    while(true) {
        displayTitle();
        displayEachItem(memStatFunc, displayNum);
        UpCursor(displayNum+1);
        sleep(1);
    }

    return 0;
}