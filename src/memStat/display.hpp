# ifndef DISPLAY_H
# define DISPLAY_H

# include <string>

# include "processMem.hpp"

using namespace std;

enum BackGroundColor {BG_BLACK, BG_RED, BG_GREEN, BG_YELLOW, BG_BLUE, BG_PURPLE, BG_WHITE};
const string BackGroundColorString[] = {"\033[40m", "\033[41m", "\033[42m", "\033[43m", "\033[44m","\033[45m","\033[47m"};

enum ForeGroundColor {FG_BLACK, FG_RED, FG_GREEN, FG_YELLOW, FG_BLUE, FG_PURPLE, FG_WHITE};
const string ForeGroundColorString[] = {"\033[30m", "\033[31m", "\033[32m", "\033[33m", "\033[34m", "\033[35m", "\033[37m"};

const string FONTBOLD = "\033[1m";
const string RESET = "\033[0m";
const string FLASH = "\033[5m";

int checkArgs(int displayNum, int sortRegulation);

void displayHelp();
void displayTitle();
void displayEachItem(vector<processMemInfo> (*memStatFunc)(), int displayNum);
void displayMemoryInfo(processMemInfo info);

void setForeGoundColor(ForeGroundColor color);
void setBackGroundColor(BackGroundColor color);
void setFontBold();
void setFlash();
void resetDisplay();

void clearScreen();
void moveCursorLineHead();
void showCursor();
void hideCursor();
void UpCursor(int up);

# endif