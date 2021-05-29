# include <iostream>
# include <string>
# include <vector>
# include <time.h>

# include "MemLeakDetector.hpp"
# include "displayMemLeak.hpp"
# include "../memStat/display.hpp"

const float LEAKSAFE = 10;
const float LEAKWARN = 20;
const float LEAKDANGER = 30;
int leakSize = 0;

int checkArgsMemLeak(int displayNum, int sortRegulation) {
    if (displayNum < 1 || displayNum > 10) {
        ForeGroundColor FGColor = FG_GREEN;
        setForeGoundColor(FG_GREEN);
        setFontBold();
        printf("Display Number should be in range [1,10].\n");
        resetDisplay();
        return 0;
    }

    if (sortRegulation < 0 || sortRegulation >2) {
        ForeGroundColor FGColor = FG_GREEN;
        setForeGoundColor(FG_GREEN);
        setFontBold();
        printf("Sort Regulation should be in (0, 2).\n");
        printf("0: Sort by Name    1: Sort by Size/FD    2: Sort by Time\n");
        resetDisplay();
        return 0;
    }

    return 1;
}

void displayHelpMemLeak(){
    ForeGroundColor FGColor = FG_GREEN;
    setForeGoundColor(FG_GREEN);
    setFontBold();
    printf("You can use -n or -s to show the result.\n");
    printf("Use -n to set the number of displayed item. Range: [1, 10].\n");
    printf("Use -s to set the sort regulation. Range: [0, 2].\n");
    printf("0: Sort by Name    1: Sort by Size    2: Sort by Time\n");
    resetDisplay();
}

void displayTitleMemLeak(){
    BackGroundColor BGColor = BG_BLACK;
    ForeGroundColor FGColor = FG_WHITE;
    moveCursorLineHead();
    setFontBold();
    setBackGroundColor(BGColor);
    setForeGoundColor(FGColor);
    printf ("%15s%15s%15s%15s",
            "NAME-MEM","LINE","SIZE","TIME");
    resetDisplay();
    printf("\n");
}

void displayEachMem(vector<Mem_info> (*memLeakFunc)(), int displayNum) {
    time_t nowtime = time(NULL);
    vector<Mem_info> memLeakList = (*memLeakFunc)();
    leakSize = memLeakList.size();
//    printf("inside:%d\n",leakSize);
    for (int i=0;i<displayNum && i<leakSize;++i){
        time_t second = nowtime-memLeakList[i].start;
        displayMemoryLeak(memLeakList[i], second);
    }
}

void displayMemoryLeak(Mem_info leak, time_t second){
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

//    info.name[10] = '\0';
    printf ("%15s%15d%15d%15lld",
            leak.file_name.c_str(), leak.line, leak.size, second);
    resetDisplay();
    printf("\n");
}

