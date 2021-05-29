#ifndef MEMSTAT_DISPLAYMEMLEAK_H
#define MEMSTAT_DISPLAYMEMLEAK_H

# include "MemLeakDetector.hpp"
# include "string"

using namespace std;

extern int leakSize;

int checkArgsMemLeak(int displayNum, int sortRegulation);

void displayHelpMemLeak();
void displayTitleMemLeak();
void displayEachMem(vector<Mem_info> (*memLeakFunc)(), int displayNum);
void displayMemoryLeak(Mem_info leak, time_t second);

#endif //MEMSTAT_DISPLAYMEMLEAK_H
