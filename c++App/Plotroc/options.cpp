#include "options.h"
#include "cassert"
#include "iostream"
#include "main.h"

static const int showHelp = 1;
static const int showVersion = 1;
const string Options::shortOptions = "s:phv";
const string Options::optionIndexes = "sphv";
const struct option Options::longOptions[NumOptionTypes+1] =
{
    {"pSNPs", required_argument, NULL, shortOptions[0]}
    ,{"popup", required_argument, &flags[4], 1}
    ,{"help", no_argument, &flags[4], showHelp}
    ,{"version", no_argument, &flags[5], showVersion}
    ,{NULL, 0, NULL, 0}
};
const string Options::descriptions[NumOptionTypes] =
{
    "List of real pSNP postionts, output file from pSNPgenerator.R."
    "On  finish show ROC plot in window."
    ,"Help information."
    ,"Show program version information."
};
const string Options::defaults[NumOptionTypes] =
{
    "changes.csv"
    ,""
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






















