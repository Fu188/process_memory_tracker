# include <iostream>
# include <string>
# include <vector>

# include "processMem.hpp"
# include "display.hpp"
# include "Mem.hpp"

const float MEMSAFE = 10;
const float MEMWARN = 20;
const float MEMDANGER = 30;

int checkArgs(int displayNum, int sortRegulation) {
    if (displayNum < 1 || displayNum > 20) {
        ForeGroundColor FGColor = FG_GREEN;
        setForeGoundColor(FG_GREEN);
        setFontBold();
        printf("Display Number should be in range [1,20].\n");
        resetDisplay();
        return 0;
    }

    if (sortRegulation < 0 || sortRegulation >3) {
        ForeGroundColor FGColor = FG_GREEN;
        setForeGoundColor(FG_GREEN);
        setFontBold();
        printf("Sort Regulation should be in (0, 1, 2, 3).\n");
        printf("0: Sort by PID    1: Sort by Vmrss    2: Sort by Vmsize    3: Sort by State\n");
        resetDisplay();
        return 0;
    }

    return 1;
}

void displayHelp(){
    ForeGroundColor FGColor = FG_GREEN;
    setForeGoundColor(FG_GREEN);
    setFontBold();
    printf("You can use -n or -s to show the result.\n");
    printf("Use -n to set the number of displayed item. Range: [1, 20].\n");
    printf("Use -s to set the sort regulation. Range: [0, 3].\n");
    printf("0: Sort by PID    1: Sort by Vmrss    2: Sort by Vmsize    3: Sort by State\n");
    resetDisplay();
}

void displayTitle(){
    moveCursorLineHead();
    BackGroundColor BGColor = BG_BLACK;
    ForeGroundColor FGColor = FG_WHITE;
    printf("%21s%23ld%21s%22ld\n",
           "Total Memory:",MemInfo::m_total,"Used Memory:",MemInfo::m_used);
    printf("%21s%23ld%21s%22ld\n",
           "Cached Memory:",MemInfo::m_cached,"Free Memory:",MemInfo::m_free);
    setFontBold();
    setBackGroundColor(BGColor);
    setForeGoundColor(FGColor);
    printf ("%7s%15s%15s%15s%15s%10s%10s",
            "PID","NAME","STATE","VMSIZE","VMRSS","%VMSIZE","%VMRSS");
    resetDisplay();
    printf("\n");
}

void displayEachItem(vector<processMemInfo> (*memStatFunc)(), int displayNum) {
    vector<processMemInfo> processMemInfoList = (*memStatFunc)();
    for (int i=0;i<displayNum;++i){
        displayMemoryInfo(processMemInfoList[i]);
    }
}

void displayMemoryInfo(processMemInfo info){
    BackGroundColor BGColor = BG_BLACK;
    ForeGroundColor FGColor = FG_BLACK;

    if (info.vmrss_per > MEMDANGER) {
        setFlash();
        BGColor = BG_RED;
    } else if (info.vmrss_per > MEMWARN) {
        BGColor = BG_RED;
    } else if (info.vmrss_per > MEMSAFE) {
        BGColor = BG_YELLOW;
    } else {
        BGColor = BG_GREEN;
    }

    setBackGroundColor(BGColor);
    setForeGoundColor(FGColor);
    moveCursorLineHead();

    info.name[10] = '\0';
    printf ("%7d%15s%15s%15d%15d%10f%10f",
            info.pid, info.name.c_str(), ProcessStateString[info.state].c_str(),
            info.vmsize, info.vmrss, info.vmsize_per, info.vmrss_per);
    resetDisplay();
    printf("\n");
}

void setForeGoundColor(ForeGroundColor color) {
    printf(ForeGroundColorString[color].c_str());
    fflush(stdout);
}

void setBackGroundColor(BackGroundColor color) {
    printf(BackGroundColorString[color].c_str());
    fflush(stdout);
}

void setFontBold() {
    printf(FONTBOLD.c_str());
    fflush(stdout);
}

void setFlash() {
    printf(FLASH.c_str());
    fflush(stdout);
}

void resetDisplay() {
    printf(RESET.c_str());
    fflush(stdout);
}

void clearScreen() {
    system("clear");
}

void moveCursorLineHead() {
    printf("\r");
    fflush;
}

void showCursor() {
    printf("\033[?25h");
    fflush(stdout);
}

void hideCursor() {
    printf("\033[?25l");
    fflush(stdout);
}

void UpCursor(int up) {
    printf("\033[%dA", up);
    fflush(stdout);
}