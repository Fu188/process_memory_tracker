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
    int * ptr = (int*)malloc(10*sizeof(int));
    ptr[10] = 5;
    checkAssign();
    mem_report();
    file_report();
}