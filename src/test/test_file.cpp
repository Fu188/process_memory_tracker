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

using namespace std;

int main() {
    while(true) {
        int p = open("test_file.cpp",0);
        close(p);
    }
}