#include "options.h"
#include "cassert"
#include "iostream"
#include "main.h"

static const int showHelp = 1;
static const int showVersion = 1;
const string Options::shortOptions = "t:p:d:l:o:hv";
const string Options::optionIndexes = "tpdlohv";
const struct option Options::longOptions[NumOptionTypes+1] =
{
    {"in-template", required_argument, NULL, shortOptions[0]}
    ,{"phred-dist", required_argument, NULL, shortOptions[1]}
    ,{"depth", required_argument, NULL, shortOptions[2]}
    ,{"length", required_argument, NULL, shortOptions[3]}
    ,{"out-prefix", required_argument, NULL, shortOptions[4]}
    ,{"help", no_argument, &flags[4], showHelp}
    ,{"version", no_argument, &flags[5], showVersion}
    ,{NULL, 0, NULL, 0}
};
const string Options::descriptions[NumOptionTypes] =
{
    "Template sequence to base reads upon."
    ,"File containing phred distribution for error parameters."
    ,"Depth of simulated sequence."
    ,"Length of reads"
    ,"Output prefix for bax.h5 and fastq filenames."
    ,"Help information."
    ,"Show program version information."
};
const string Options::defaults[NumOptionTypes] =
{
    ""
    ,""
    ,"100"
    ,"4000"
    ,"simbas"
    ,""
    ,""
};

string Options::values[NumOptionTypes] = defaults;
int Options::flags[NumOptionTypes] = {};

Options::Options()
{
}

string Options::get(Options::OptionTypes option)
{
    return values[option];
}

int Options::flag(Options::OptionTypes flag)
{
    return flags[flag];
}

void Options::readOptions(int argc, char *argv[])
{
    int optionIndex = 0;
    int option = -1;
    bool finished = false;
    do
    {
        option = getopt_long(argc, argv
                                  ,shortOptions.c_str()
                                  ,longOptions
                                  ,&optionIndex);

        finished = (option == -1);
        if (!finished && 0 != option && '?' != option)
        {
            const unsigned int index = optionIndexes.find_first_of(option);
            if (NULL == optarg)
            {
                flags[index] = longOptions[index].val;
            }
            else
            {
                values[index].assign(optarg);
            }
        }
    } while(!finished);

    if (flag(Options::Help)) writeHelpInfo(cout);
    if (flag(Options::Version)) writeVersion(cout);
}

void Options::writeHelpInfo(ostream& outs)
{
    outs << "Help infomation for " << programName << " " << version << '\n';
    for ( unsigned int i = 0 ; i < NumOptionTypes ; i++ )
    {
        outs << "[" << optionIndexes[i] << ']'
             << '\t' << "[" << longOptions[i].name << ']'
             << '\t'<<  "default: " << defaults[i] << '\n';
        outs << '\t' << '\t' << descriptions[i] << '\n';
    }
    outs << '\n';
}


void Options::writeVersion(ostream& outs)
{
    outs << "Version " << version << '\n';
}






















