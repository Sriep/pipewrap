#ifndef OPTIONS_H
#define OPTIONS_H
#include <unistd.h>
#include <string>
#include "getopt.h"

using namespace std;

//const int required_argument = 0;
//const int no_argument = 0;
//const int optional_argument = 0;

class Options
{
public:
    enum OptionTypes
    {
        InTemplate
        ,Depth
        ,OutPrefix
        ,Help
        ,Version
        ,NumOptionTypes
    };
    //static const int showHelp = 1;
    //static const int showVersion = 1;

    Options();
    static string get(OptionTypes option);
    static int flag(OptionTypes flag);
    static void readOptions(int argc, char *argv[]);


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
