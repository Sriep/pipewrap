#include <iostream>
#include <cstdio>
#include <cerrno>
#include <fstream>
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
    PositionData variantData(Options::get(Options::VariantData), 5);

    ofstream outf(Options::get(Options::OutData).c_str(), ios_base::out | ios_base::app);
    outf << variantData.getDataHeader();

    int logPvThreshold = atoi(Options::get(Options::LogPValueThreshold).c_str());
    if (logPvThreshold > 0) logPvThreshold = logPvThreshold * -1;
    while (variantData.next())
    {
        if (logPvThreshold > variantData.currentLogPvalue())
        {
            outf << variantData.getWindowData() << ",";
            outf << truePsnps.hasPosition(variantData.currentPosition());
            outf << "\n";
        }
    }

}


void stripQuotes(string theBuffer, char quote)
{
    if ( theBuffer.front() == quote )
    {
        theBuffer.erase(0, 1);
        theBuffer.erase(theBuffer.size()-1);
    }
}
