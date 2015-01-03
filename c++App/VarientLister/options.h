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
        /// In Files
        InBamFile
        ,TemplateFile
        ,BaxH5File

        /// Out Files
        ,ReadOutFile
        ,LociOutFile
        ,BionomialFile
        ,PoissonFile
        ,KfFile
        ,VcfOutput

        /// Algorithm parameters
        ,NumFrequencyPartitions
        ,FilterDelsThreshold
        ,FilterInsThreshold
        ,FilterSubsThreshold
        ,PreFilterQuality
        ,PostFilterQuality
        ,PreCalculateFactorials
        ,PValueThreshold
        ,CoverageThreshold

        ,Help
        ,Version
        ,NumOptionTypes
    };

    Options();
    static string get(OptionTypes option);
    static int flag(OptionTypes flag);
    static void readOptions(int argc, char *argv[]);    
    static string commandLine;

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
