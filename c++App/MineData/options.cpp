#include "options.h"
#include "cassert"
#include "iostream"
#include "main.h"

static const int showHelp = 1;
static const int showVersion = 1;

const string Options::shortOptions = "V:T:O:t:F:s:hv";
const string Options::optionIndexes = "VTOtFshv";
const struct option Options::longOptions[NumOptionTypes+1] =
{
    {"variant-data", required_argument, NULL, shortOptions[0]}
    ,{"true-data", required_argument, NULL, shortOptions[1]}
    ,{"Out-data", required_argument, NULL, shortOptions[2]}
    ,{"PV-threshold", required_argument, NULL, shortOptions[3]}
    ,{"Freq-threshold", required_argument, NULL, shortOptions[4]}
    ,{"Snip-ends", required_argument, NULL, shortOptions[5]}
    ,{"help", no_argument, &flags[6], showHelp}
    ,{"version", no_argument, &flags[7], showVersion}
    ,{NULL, 0, NULL, 0}
};
const string Options::descriptions[NumOptionTypes] =
{
    "Data file output by VariantLister."
    ,"True pSNP data from pSNPgenerator.R"
    ,"Output arff file, new data ammended at end."
    ,"Log of thresold pValue, store data if log10 of pvalue is this below threshold."
    ,"Percentage frequency thresold pValue, store data if psnp frequecy is this below the threshold."
    ,"Trim reads off each end"
    ,"Show Help information"
    ,"Show version number"
};
const string Options::defaults[NumOptionTypes] =
{
    ""
    ,""
    ,"outdata.csv"
    ,""
    ,"0.4"
    ,"0"
    ,""
    ,""
};

string Options::values[NumOptionTypes] = defaults;
int Options::flags[NumOptionTypes] = {0,0,0,0,0};

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

int Options::readOptions(int argc, char *argv[])
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
    return optind;
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






















