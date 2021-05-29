# include <iostream>
# include <string>
# include <vector>
# include <time.h>

#include "FileLeakDetector.hpp"
#include "displayFileLeak.hpp"
# include "../memStat/display.hpp"
using namespace std;

const float LEAKSAFE = 10;
const float LEAKWARN = 20;
const float LEAKDANGER = 30;
int fileLeakSize = 0;

int checkArgsFileLeak(int displayNum, int sortRegulation){
    if (displayNum < 1 || displayNum > 10) {
        ForeGroundColor FGColor = FG_GREEN;
        setForeGoundColor(FG_GREEN);
        setFontBold();
        printf("Display Number should be in range [1,10].\n");
        resetDisplay();
        return 0;
    }

    return 1;
}

void displayHelpFileLeak(){
    ForeGroundColor FGColor = FG_GREEN;
    setForeGoundColor(FG_GREEN);
    setFontBold();
    printf("You can use -n or -s to show the result.\n");
    printf("Use -n to set the number of displayed item. Range: [1, 10].\n");
    printf("Use -s to set the sort regulation. Range: [0, 1].\n");
    printf("0: Sort by Name    2: Sort by Time\n");
    resetDisplay();
}
void displayTitleFileLeak(){
    BackGroundColor BGColor = BG_BLACK;
    ForeGroundColor FGColor = FG_WHITE;
    moveCursorLineHead();
    setFontBold();
    setBackGroundColor(BGColor);
    setForeGoundColor(FGColor);
    printf ("%15s%15s%15s",
            "NAME-FILE","LINE","TIME");
    resetDisplay();
    printf("\n");
}
void displayEachFile(vector<File_info> (*fileLeakFunc)(), int displayNum){
    time_t nowtime = time(NULL);
    vector<File_info> fileLeakList = (*fileLeakFunc)();
    fileLeakSize = fileLeakList.size();
    for (int i=0;i<displayNum && i<fileLeakSize;++i){
        time_t second = nowtime-fileLeakList[i].start;
        displayFileLeak(fileLeakList[i], second);
    }
}
void displayFileLeak(File_info leak, time_t second){
    BackGroundColor BGColor = BG_GREEN;
    ForeGroundColor FGColor = FG_BLACK;

    if (second > LEAKDANGER) {
        setFlash();
        BGColor = BG_RED;
    } else if (second > LEAKWARN) {
        BGColor = BG_RED;
    } else if (second > LEAKSAFE) {
        BGColor = BG_YELLOW;
    } else {
        BGColor = BG_GREEN;
    }

    setBackGroundColor(BGColor);
    setForeGoundColor(FGColor);
    moveCursorLineHead();

    printf ("%15s%15d%15lld",
            leak.file_name.c_str(), leak.line, second);
    resetDisplay();
    printf("\n");
}