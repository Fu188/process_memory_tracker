#include <bits/stdc++.h>

#include "../leakDetector/wrapper.hpp"
#include "../leakDetector/MemLeakDetector.hpp"
#include "../leakDetector/FileLeakDetector.hpp"

using namespace std;

int main() {
    void *ptr;
    while(true) {
        ptr=malloc(128);
        int p = open("test_file.cpp",0);
        ptr=malloc(128);
        free(ptr);
        close(p);
    }
}