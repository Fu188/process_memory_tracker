# include <iostream>
# include <string>

# include "utils.hpp"
# include "processMem.hpp"

using namespace std;

void clearLine(istream& stream) {
    char ch;
    do {
        stream.get(ch);
    } while ((!stream.eof()) && (ch != '\n') );
}

void getState(istream& stream, ProcessState& info) {
    char value;
    stream >> value;
    switch (value) {
        case 'R': { info = RUNNING; break; }
        case 'S': { info = SLEEPING; break; }
        case 'Z': { info = ZOMBIE; break; }
        case 'T': { info = STOPPED; break; }
        case 'D': { info = WAITING; break; }
        case 'I': { info = IDLE; break; }
    }
}