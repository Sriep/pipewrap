#ifndef OPTIONS_H
#define OPTIONS_H
#include <unistd.h>
#include <string>
#include "getopt.h"

using namespace std;

class Options
{
public:
    enum OptionTypes
    {
        InBax
        ,OutPrefix
        ,Distribution
        ,ConfusionTable
        ,Help
        ,Version
        ,NumOptionTypes
    };

    Options();
    static string get(OptionTypes option);
    static int flag(OptionTypes flag);
    static int readOptions(int argc, char *argv[]);


private:
    static void writeHelpInfo(ostream &outs);
    static void writeVersion(ostream& outs);

    static const string shortOptions;
    static const string optionIndexes;
    static const struct option longOptions[NumOptionTypes+1];
    static const string descriptions[NumOptionTypes];
    static int flags[NumOptionTypes];
    static string values[NumOptionTypes];
    static const string defaults[NumOptionTypes];
};

#endif // OPTIONS_H
