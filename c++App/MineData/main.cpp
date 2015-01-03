#include <iostream>
#include <cstdio>
#include <cerrno>
#include <fstream>
#include <string>
#include "main.h"
#include "options.h"
#include "generatedpsnps.h"
#include "positiondata.h"

int main(int argc, char *argv[])
{
    try
    {
        try
        {
            Options::readOptions(argc, argv);
            runProgram();
        }
        catch (const exception& ex)
        {
            perror(ex.what());
            return errno;
        }
    }
    catch (...)
    {
        perror("");
        return errno;
    }
}

void runProgram()
{
    GeneratedPsnps truePsnps(Options::get(Options::TrueData));
    int windowSize = atoi(Options::get(Options::WindowSize).c_str());
    PositionData variantData(Options::get(Options::VariantData), windowSize);

    ofstream outf(Options::get(Options::OutData).c_str(), ios_base::out | ios_base::app);
    //ofstream outf(Options::get(Options::OutData).c_str(), ios_base::out | ios_base::trunc);
    //int test = outf.tellp();
    if (outf.tellp() == 0)
        outf << variantData.getDataHeaderARFF();

    //int logPvThreshold = atoi(Options::get(Options::LogPValueThreshold).c_str());
    //if (logPvThreshold > 0) logPvThreshold = logPvThreshold * -1;
    double freqThrehold =  stod(Options::get(
                                Options::FrequencyThreholdPct).c_str())/100.0;

    while (variantData.next())
    {
        //if (logPvThreshold > variantData.currentLogPvalue())
        if (freqThrehold < variantData.currentPsnpFrequency())
        {
            outf << variantData.getWindowData() << ",";
            outf << truePsnps.hasPosition(variantData.currentPosition());
            outf << "\n";
        }
    }

}


string stripQuotes(string theBuffer, char quote)
{
    if ( theBuffer.front() == quote )
    {
        theBuffer.erase(0, 1);
        theBuffer.erase(theBuffer.size()-1);
    }
    return theBuffer;
}
