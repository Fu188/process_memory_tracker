# include <iostream>
# include <string>

# include "utils.hpp"

using namespace std;

void clearLine(istream& stream) {
    char ch;
    do {
        stream.get(ch);
    } while ((!stream.eof()) && (ch != '\n') );
}