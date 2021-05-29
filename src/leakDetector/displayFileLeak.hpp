#ifndef MEMSTAT_DISPLAYFILELEAK_H
#define MEMSTAT_DISPLAYFILELEAK_H

# include "FileLeakDetector.hpp"
# include "string"
# include <vector>

using namespace std;

extern int FileLeakSize;

int checkArgsFileLeak(int displayNum, int sortRegulation);

void displayHelpFileLeak();
void displayTitleFileLeak();
void displayEachFile(vector<File_info> (*fileLeakFunc)(), int displayNum);
void displayFileLeak(File_info leak, time_t second);

#endif //MEMSTAT_DISPLAYFILELEAK_H
