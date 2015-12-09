#include <stdexcept>
#include "options.h"
#include "cassert"
#include "iostream"
#include "main.h"

static const int showHelp = 1;
static const int showVersion = 1;
const string Options::shortOptions = "i:t:x:r:l:b:p:k:V:f:d:e:u:q:a:F:P:c:hv";
const string Options::optionIndexes = "itxrlbpkVfdeuqaFPchv";
string Options::commandLine="";
const struct option Options::longOptions[NumOptionTypes+1] =
{
    {"in-bam", required_argument, NULL, shortOptions[0]}
    ,{"in-template", required_argument, NULL, shortOptions[1]}
    ,{"in-bax", required_argument, NULL, shortOptions[2]}

    ,{"readfile", required_argument, NULL, shortOptions[3]}
    ,{"locusfile", required_argument, NULL, shortOptions[4]}
    ,{"bionomial", required_argument, NULL, shortOptions[5]}
    ,{"poisson", required_argument, NULL, shortOptions[6]}
    ,{"kf", required_argument, NULL, shortOptions[7]}
    ,{"VcfOutput", required_argument, NULL, shortOptions[8]}

    ,{"freq-partionts", required_argument, NULL, shortOptions[9]}
    ,{"del-threshold", required_argument, NULL, shortOptions[10]}
    ,{"ins-threshold", required_argument, NULL, shortOptions[11]}
    ,{"subs-threshold", required_argument, NULL, shortOptions[12]}
    ,{"prequal-threshold", required_argument, NULL, shortOptions[13]}
    ,{"postqual-threshold", required_argument, NULL, shortOptions[14]}
    ,{"calculate-factorials", required_argument, NULL, shortOptions[15]}
    ,{"pValue-threshold", required_argument, NULL, shortOptions[16]}
    ,{"Coverage-threshold", required_argument, NULL, shortOptions[17]}

    ,{"help", no_argument, &flags[4], showHelp}
    ,{"version", no_argument, &flags[5], showVersion}
    ,{NULL, 0, NULL, 0}
};
const string Options::descriptions[NumOptionTypes] =
{
    "Input BAM file to be searched for pSNPs"
    ,"Input template fasta file"
    ,"Input bas.h5 input file"

    ,"Output filename for Read centric output data"
    ,"Output filename for loci centric output data"
    ,"Output filename prefix for bionomial p_values"
    "<.pvalues.csv> stem will be added to filename"
    ,"Output filename prefix for poission p_values"
    "<.pvalues.csv> stem will be added to filename"
    ,"Output filename prefix for known freqyency p_values"
    "<.pvalues.csv> stem will be added to filename"
    ,"Output results as VCF file"

    ,"Number of frequency bins used in FK pvalue estimation"
    ,"Threhold for pre filtering by deletetionQV"
    ,"Threhold for pre filtering by insertionQV"
    ,"Threhold for pre filtering by substitutionQV"
    ,"Threhold for pre filtering by aggregate quality value"
    ,"Threhold for post filtering by aggreagate quality value"
    ,"Precalculate factorails to improve poisson and bionomial methds"
    ,"Threhold pValue for calling a pSNP"
    ,"Threshold coverfage for calling a pSNP"

    ,"Help information"
    ,"Show program version information"
};
const string Options::defaults[NumOptionTypes] =
{
    "" //InBamFile
    ,"" //TemplateFile
    ,"" //BaxH5File

    /// Out Files
    ,"" //ReadOutFile
    ,"" //LociOutFile
    ,"" //BionomialFile
    ,"" //PoissonFile
    ,"" //KfFile
    ,"" //VcfOutput

    /// Algorithm parameters
    ,"10" //NumFrequencyPartitions
    ,"0" //FilterDelsThreshold
    ,"0" //FilterInsThreshold
    ,"0" //FilterSubsThreshold
    ,"0" //PreFilterQuality
    ,"0" //PostFilterQuality
    ,"0" //PreCalculateFactorials
    ,"20" //PValueThreshold
    ,"0" //CoverageThreshold

    ,"" //Help
    ,"" //Version
    //NumOptionTypes
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
    for (int i = 0 ; i < argc ; i++) commandLine += argv[i];

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
            if (index < optionIndexes.size())
            {
                if (NULL == optarg)
                {
                    flags[index] = longOptions[index].val;
                }
                else
                {
                    values[index].assign(optarg);
                }
            }
            else
            {
                throw(std::invalid_argument("Unrecognised option" + option));
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






















