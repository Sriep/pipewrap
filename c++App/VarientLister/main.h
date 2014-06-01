#ifndef MAIN_H
#define MAIN_H
#include <QCoreApplication>
#include <QCommandLineParser>
#include <fstream>
#include <string>

using namespace std;

void configCommandLineParser(QCommandLineParser& parser);
void runProgram(const string& in_file,
                const string& tp, int err_t,
                ostream& out_stream);

//http://insanecoding.blogspot.co.uk/2011/11/how-to-read-in-file-in-c.html
string get_file_contents(const char *filename);

#endif // MAIN_H
