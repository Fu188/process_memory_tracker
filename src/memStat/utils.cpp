# include <iostream>
# include <string>

# include "utils.h"

using namespace std;

void clearLine(istream& stream) {
    char ch;
    do {
        stream.get(ch);
    } while ((!stream.eof()) && (ch != '\n') );
}

template<typename T> void getInteger(istream& stream, T& info) {
    string value;
	stream >> value;
    info = stoi(value);
}