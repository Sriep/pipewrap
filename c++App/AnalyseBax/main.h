#ifndef MAIN_H
#define MAIN_H

#include <QCommandLineParser>
#include <QCommandLineOption>
#include <QStringList>
#include <fstream>
#include <string>
#include <vector>

using namespace std;
static const long double Ten = 10.0;
static const long double One = 1.0;
static const long double Zero = 0.0;
void configCommandLineParser(QCommandLineParser& parser);
void runProgram(QCommandLineParser& parser);

const QCommandLineOption inBaxh5(
                    QStringList() << "h" << "in-bax",
                   "Input bax.h5 file",
                   "filename",
                   "new2.bax.h5");

#endif // MAIN_H
