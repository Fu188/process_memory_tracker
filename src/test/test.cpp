#include <bits/stdc++.h>

#include "../leakDetector/wrapper.hpp"
#include "../leakDetector/MemLeakDetector.hpp"

using namespace std;

int main() {
    void *ptr;
    while(true) {
        ptr=malloc(128);
        ptr=malloc(128);
        free(ptr);
    }
}