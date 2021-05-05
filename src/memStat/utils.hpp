#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <string>

using namespace std;

/* clear a used line in stream */
void clearLine(istream& stream);

/* read an integer from stream */
template<typename T> void getInteger(istream& stream, T& info) {
    string value;
	stream >> value;
    info = stoi(value);
}

#endif