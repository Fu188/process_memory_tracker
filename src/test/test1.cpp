#include <bits/stdc++.h>
# include <string.h>
# include <stdio.h>
# include <pthread.h>
# include <string>
# include <iostream>
# include <unistd.h>
# include <sys/stat.h>
# include <fcntl.h>

#include "../leakDetector/wrapper.hpp"
#include "../leakDetector/FileLeakDetector.hpp"
#include "../leakDetector/MemLeakDetector.hpp"

using namespace std;

int main() {
    int index = 5;
    int p[5];
    void * ptr[5];
    while(index --) {
        p[index] = open("test_file.cpp",0);
        ptr[index]=malloc(32*index);
        sleep(2);
    }
    close(p[1]);
    free(ptr[1]);
    sleep(2);
    close(p[2]);
    free(ptr[2]);
    sleep(2);
    mem_report();
    file_report();
}