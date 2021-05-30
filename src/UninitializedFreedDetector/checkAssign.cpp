# include <string.h>
# include <stdio.h>
# include <pthread.h>
# include <string>
# include <iostream>
# include <unistd.h>
# include <sys/stat.h>
# include <fcntl.h>
# include <vector>
# include <cmath>
# include <algorithm>
# include <time.h>
#include <dirent.h>

#include "../leakDetector/MemLeakDetector.hpp"
#include "../leakDetector/FileLeakDetector.hpp"
#include "../leakDetector/wrapper.hpp"

using namespace std;

enum BackGroundColor {BG_BLACK, BG_RED, BG_GREEN, BG_YELLOW, BG_BLUE, BG_PURPLE, BG_WHITE};
const string BackGroundColorString[] = {"\033[40m", "\033[41m", "\033[42m", "\033[43m", "\033[44m","\033[45m","\033[47m"};

enum ForeGroundColor {FG_BLACK, FG_RED, FG_GREEN, FG_YELLOW, FG_BLUE, FG_PURPLE, FG_WHITE};
const string ForeGroundColorString[] = {"\033[30m", "\033[31m", "\033[32m", "\033[33m", "\033[34m", "\033[35m", "\033[37m"};

const string FONTBOLD = "\033[1m";
const string RESET = "\033[0m";
const string FLASH = "\033[5m";
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


string FILEPATHRECORD = "../leakDetector/Output";
vector<Mem_info> memAllocatedGlobal;
vector<Mem_info> memFreedGlobal;
vector<File_info> fileOpenedGlobal;
vector<File_info> fileClosedGlobal;

void updateMemStat(){
    vector<Mem_info> memAllocated;
    vector<Mem_info> memFreed;
    DIR* dir = opendir(FILEPATHRECORD.c_str());
    dirent* p = NULL;
    string format = "_Mem.txt";
    while((p = readdir(dir)) != NULL)
    {
        if(p->d_name[0] != '.')
        {
            string name = FILEPATHRECORD + "/" + string(p->d_name);
            if(strstr(name.c_str(), format.c_str()) != NULL){

                FILE *f = fopen(name.c_str(), "r");
                if(f!=NULL) {
                    int mode = 0;
                    void *address;
                    int numTen = 0;
                    unsigned int size;
                    string file_name;
                    unsigned int line;
                    time_t start;

                    string str = "";
                    char in[100];
                    while (NULL != fgets(in, 99, f)) {
                        if (in[0] == '\n')
                            continue;
                        str = in;
                        if (str == "[Allocate Memory]\n") {
                            mode = 1;
                            continue;
                        } else if (str == "[Free Memory]\n") {
                            mode = -1;
                            continue;
                        } else if (str == "       Memory Leak Summary\n")
                            break;
                        str.erase(remove(str.begin(), str.end(), '\n'), str.end());
                        if (str.substr(0, 7) == "Address"){
                            numTen = strtoull(str.substr(9).c_str(), NULL, 16);
                            address = (void*) numTen;
                        }
                        else if (str.substr(0, 4) == "File") {
                            file_name = str.substr(6);
                        } else if (str.substr(0, 4) == "Line") {
                            line = atoi(str.substr(6).c_str());
                        } else if (str.substr(0, 4) == "Size") {
                            size = atoi(str.substr(6).c_str());
                        } else if (str.substr(0, 5) == "Start") {
                            start = atol(str.substr(7).c_str());
                            if (mode == 1) {
                                Mem_info mem_info;
                                memset(&mem_info, 0, sizeof(mem_info));
                                mem_info.address = address;
                                mem_info.size = size;
                                mem_info.file_name = file_name;
                                mem_info.line = line;
                                mem_info.start = start;
                                memAllocated.push_back(mem_info);
                            }
                            else if(mode == -1){
                                int find = -1;
                                for (int i = 0; i < memAllocated.size(); i++) {
                                    if (memAllocated[i].address == address) {
                                        find = i;
                                        break;
                                    }
                                }
                                if (find > -1){
                                    memFreed.push_back(memAllocated[find]);
                                    memAllocated.erase(memAllocated.begin() + find);
                                }
                            }
                        }
                    }
                    fclose(f);
                }
            }
        }
    }
    closedir(dir);
    memAllocatedGlobal = memAllocated;
    memFreedGlobal = memFreed;
}

void updateFileStat(){
    vector<File_info> fileOpened;
    vector<File_info> fileClosed;
    DIR* dir = opendir(FILEPATHRECORD.c_str());
    dirent* p = NULL;
    string format = "_File.txt";
    while((p = readdir(dir)) != NULL)
    {
        if(p->d_name[0] != '.')
        {
            string name = FILEPATHRECORD + "/" + string(p->d_name);
            if(strstr(name.c_str(), format.c_str()) != NULL){
                FILE *f = fopen(name.c_str(), "r");
                if(f!=NULL) {
                    int mode = 0;
                    int fd;
                    string file_name;
                    unsigned int line;
                    time_t start;

                    string str = "";
                    char in[100];
                    while (NULL != fgets(in, 99, f)) {
                        if (in[0] == '\n')
                            continue;
                        str = in;
                        if (str == "[Open File]\n") {
                            mode = 1;
                            continue;
                        } else if (str == "[Close File]\n") {
                            mode = -1;
                            continue;
                        } else if (str == "   File Descriptor Leak Summary\n")
                            break;
                        str.erase(remove(str.begin(), str.end(), '\n'), str.end());

                        if (str.substr(0, 5) == "File:")
                            file_name = str.substr(6);
                        else if (str.substr(0, 5) == "File ") {
                            fd = atoi(str.substr(17).c_str());
                        } else if (str.substr(0, 4) == "Line") {
                            line = atoi(str.substr(6).c_str());
                        } else if (str.substr(0, 5) == "Start") {
                            start = atol(str.substr(7).c_str());
                            if (mode == 1) {
                                File_info file_info;
                                memset(&file_info, 0, sizeof(file_info) );
                                file_info.file_name = file_name;
                                file_info.fd = fd;
                                file_info.line = line;
                                file_info.start = start;
                                fileOpened.push_back(file_info);
                            } else if (mode == -1) {
                                int find = -1;
                                for (int i = 0; i < fileOpened.size(); i++) {
                                    if (fileOpened[i].fd == fd) {
                                        find = i;
                                        break;
                                    }
                                }
                                if (find > -1){
                                    fileClosed.push_back(fileOpened[find]);
                                    fileOpened.erase(fileOpened.begin() + find);
                                }
                            }
                        }

                    }
                    fclose(f);
                }
            }
        }
    }
    closedir(dir);
    fileClosedGlobal = fileClosed;
    fileOpenedGlobal = fileOpened;

}

vector<Mem_info> getAllMemAllocated(){
    updateMemStat();
    return memAllocatedGlobal;
}

vector<Mem_info> getAllMemFreed(){
    updateMemStat();
    return memFreedGlobal;
}

template<typename T> void checkAssignMem(T* address){
    BackGroundColor BGColor = BG_GREEN;
    ForeGroundColor FGColor = FG_BLACK;
    setBackGroundColor(BGColor);
    setForeGoundColor(FGColor);
    int size = sizeof(T); // sizeof(*address) is also ok
    Mem_info mem;
    int start = *((int *) &address);
    int lower = 0, upper = 0;
    updateMemStat();
    for(int i = 0; i < memAllocatedGlobal.size(); i++){
        mem = memAllocatedGlobal[i];
        lower = *((int *)(&mem.address));
        upper = *((int *)(&mem.address)) + mem.size -1;
        if(lower<=start && upper >= start+size-1){
            printf("%15s%20s%5s0x%08x","Memory", "Success","",address);
            resetDisplay();
            printf("\n");
            return;
        }
    }
    for(int i = 0; i < memFreedGlobal.size(); i++){
        mem = memFreedGlobal[i];
        lower = *((int *)&mem.address);
        upper = *((int *)&mem.address) + mem.size -1;
        if(lower<=start && upper >= start+size-1){
            printf("%15s%20s%5s0x%08x","Memory", "Already Freed","",address);
            resetDisplay();
            printf("\n");
            return;
        }
    }
    printf("%15s%20s%5s0x%08x","Memory", "Uninitialized","",address);
    resetDisplay();
    printf("\n");
    return;
}

void checkAssignFile(int fd){
    BackGroundColor BGColor = BG_GREEN;
    ForeGroundColor FGColor = FG_BLACK;
    setBackGroundColor(BGColor);
    setForeGoundColor(FGColor);
    File_info file;
    updateFileStat();
    for(int i = 0; i < fileOpenedGlobal.size(); i++){
        file = fileOpenedGlobal[i];
        if(file.fd == fd){
            printf("%15s%20s%15d","File", "Opened",fd);
            resetDisplay();
            printf("\n");
            return;
        }
    }
    for(int i = 0; i < fileClosedGlobal.size(); i++){
        file = fileClosedGlobal[i];
        if(file.fd == fd){
            printf("%15s%20s%15d","File", "Already Closed",fd);
            resetDisplay();
            printf("\n");
            return;
        }
    }
    printf("%15s%20s%15d","File", "Not Accessed", -1);
    resetDisplay();
    printf("\n");
    return;
}


int main(){
    BackGroundColor BGColor = BG_BLACK;
    ForeGroundColor FGColor = FG_WHITE;
    moveCursorLineHead();
    setFontBold();
    setBackGroundColor(BGColor);
    setForeGoundColor(FGColor);
    printf("%15s%20s%15s", "CLASS", "TYPE", "ADDRESS/FD");
    resetDisplay();
    printf("\n");
    int index = 5;
    void* ptr[index];
    int p[index+1];
    p[6] = 1000;
    while(index --) {
        ptr[index] = malloc( sizeof(int) );
        p[index] = open("../test/test_file.cpp",0);
    }
    checkAssignMem(ptr[5]);
    checkAssignFile(p[5]);
    sleep(2);
    free(ptr[2]);
    checkAssignMem(ptr[2]);
    close(p[2]);
    checkAssignFile(p[2]);
    sleep(2);
    checkAssignMem(ptr[3]);
    checkAssignFile(p[3]);
    mem_report();
    file_report();
    return 0;
}

