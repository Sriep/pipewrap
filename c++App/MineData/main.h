#ifndef MAIN_H
#define MAIN_H
#include <string>
using namespace std;

static string programName = "AnalyseBax";
static string version = "1.0.0.0";
typedef unsigned int uint;
const int bufferSize = 256;

void runProgram();

void stripQuotes(string theBuffer, char quote = '"');

#endif // MAIN_H
