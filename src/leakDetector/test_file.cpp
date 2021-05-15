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
    }
    close(p[2]);
    free(ptr[1]);
    close(p[3]);
    free(ptr[4]);
    mem_report();
    file_report();
}