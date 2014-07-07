#include "options.h"
#include "cassert"
#include "iostream"
#include "main.h"

static const int showHelp = 1;
static const int showVersion = 1;

const string Options::shortOptions = "i:o:dchv";
const string Options::optionIndexes = "iodchv";
const struct option Options::longOptions[NumOptionTypes+1] =
{
    {"in-bax", required_argument, NULL, shortOptions[0]}
    ,{"out-prefix", required_argument, NULL, shortOptions[1]}
    ,{"distribution", required_argument, &flags[2], true}
    ,{"confusion-table", required_argument, &flags[3], true}
    ,{"help", no_argument, &flags[4], showHelp}
    ,{"version", no_argument, &flags[5], showVersion}
    ,{NULL, 0, NULL, 0}
};
const string Options::descriptions[NumOptionTypes] =
{
    "Input bax file."
    ,"Prefix for output files."
    ,"Output data distributon for bax file. Can be used as input to simbas"
    ,"Display confusion table for bax file, showing dependancy between data."
    ,"Show Help information"
    ,"Show version number"
};
const string Options::defaults[NumOptionTypes] =
{
    ""
    ,""
    ,""
    ,""
    ,""
    ,""
};

string Options::values[NumOptionTypes] = defaults;
int Options::flags[NumOptionTypes] = {0,0,1,0,0,0};

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






















