# include <iostream>
# include <vector>
# include <unistd.h>
# include <csignal>

# include "displayMemLeak.hpp"
# include "MemLeakDetector.hpp"
# include "wrapper.hpp"
# include "displayFileLeak.hpp"
# include "FileLeakDetector.hpp"
# include "../memStat/display.hpp"
#include "../memStat/utils.hpp"

using namespace std;

int main(int argc, char *argv[]) {
    /* register exception function */
    signal(SIGINT, clearAndExit);
    signal(SIGQUIT, clearAndExit);
    signal(SIGTSTP, clearAndExit);

    int displayNum = 10;
    int sortRegulationMem = 2;
    int sortRegulationFile = 1;

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
                sortRegulationMem = atoi(optarg);
                sortRegulationFile = sortRegulationMem;
                break;
            default:
                displayHelp();
                return 0;
        }
    }

    if (!checkArgsMemLeak(displayNum, sortRegulationMem)) return 0;

    vector<Mem_info> (*memLeakFunc)();
    switch (sortRegulationMem) {
        case 0: { memLeakFunc = &getMemLeakByName; break; }
        case 1: { memLeakFunc = &getMemLeakBySize; break; }
        case 2: { memLeakFunc = &getMemLeakByTime; break; }
        default: memLeakFunc = &getMemLeakByTime;
    }

    if (!checkArgsFileLeak(displayNum, sortRegulationFile)) return 0;

    vector<File_info> (*fileLeakFunc)();
    switch (sortRegulationFile) {
        case 0: { fileLeakFunc = &getFileLeakByName; break; }
        case 1: { fileLeakFunc = &getFileLeakByTime; break; }
        default: fileLeakFunc = &getFileLeakByName;
    }

    /* Begin detecting the usage of memory */
    hideCursor();
    clearScreen();
    while(true) {
        displayTitleMemLeak();
        displayEachMem(memLeakFunc, displayNum);
        displayTitleFileLeak();
        displayEachFile(fileLeakFunc, displayNum);
        sleep(1);
        clearScreen();
//        UpCursor(leakSize+1);
//        printf("size %d\t",leakSize);
    }

    clearAndExit(0);
    return 0;
}