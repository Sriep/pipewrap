#include <array>
#include <cmath>
#include "main.h"
#include "options.h"
#include "positiondata.h"

PositionData::PositionData(const string &dataFilename, uint windowSize = 5)
    : ifstream(dataFilename.c_str()),
      windowSize(windowSize),
      skip(atoi(Options::get(Options::SnipEnds).c_str()))
{
    if (is_open())
    {
        init();
    }
}

void PositionData::init()
{
    string inDataFile = Options::get(Options::VariantData);
    totalLines = getLineCount(inDataFile);
    ignore(2560,'\n');
    linesRead++;

    //uint skip = atoi(inDataFile.c_str());
    if (skip > 0)
    {
        if (totalLines < 2*skip + 2*windowSize +1) return;
        for (uint i = 0 ; i < skip ; i++ )
        {
            ignore(2560,'\n');
            linesRead++;
        }
    }

    for (uint i = 0 ; i < 2*windowSize+1 ; i++ )
    {
        pushline();
    }
}

//http://stackoverflow.com/questions/3137094/how-to-count-lines-in-a-document
int PositionData::getLineCount(const string &filename)
{
    string command = "wc -l < " + filename;
    FILE * f = popen( command.c_str(), "r" );
    if ( f == 0 ) {
        fprintf( stderr, "Could not execute\n" );
        return -1;
    }
    const int BUFSIZE = 1000;
    char buf[ BUFSIZE ];
    while( fgets( buf, BUFSIZE,  f ) ) {
        fprintf( stdout, "%s", buf  );
    }
    pclose( f );
    return atoi(buf);
}

void PositionData::popline()
{
    position.pop_front();
    intPosition.pop_front();
    refBase.pop_front();
    coverage.pop_front();
    intCoverage.pop_front();
    quality.pop_front();
    psnp.pop_front();
    psnpCount.pop_front();
    intPsnpCount.pop_front();
    psnpFrequ.pop_front();
    ldPValue.pop_front();
    pValue.pop_front();
}

void PositionData::pushline()
{
    string inBuffer;
    string inData;

    std::getline(*this, inBuffer, '\t');
    inData = stripQuotes(inBuffer);
    //int pos = atoi(inData.data());
    intPosition.push_back(atoi(inData.data()));
    position.push_back(inData.data());

    std::getline(*this, inBuffer, '\t');
    inData = stripQuotes(inBuffer);
    if (inData[0] > 'Z') inData[0] = inData[0] - ('a' -'A');
    refBase.push_back(inData.data());

    std::getline(*this, inBuffer, '\t');
    inData = stripQuotes(inBuffer);
    intCoverage.push_back(atoi(inData.data()));
    coverage.push_back(inData.data());

    std::getline(*this, inBuffer, '\t');
    inData = stripQuotes(inBuffer);
    quality.push_back(inData.data());

    std::getline(*this, inBuffer, '\t');
    inData = stripQuotes(inBuffer);
    if (inData[0] > 'Z') inData[0] = inData[0] - ('a' -'A');
    psnp.push_back(inData.data());

    std::getline(*this, inBuffer, '\t');
    inData = stripQuotes(inBuffer);
    intPsnpCount.push_back(atoi(inData.data()));
    psnpCount.push_back(inData.data());

    std::getline(*this, inBuffer, '\t');
    inData = stripQuotes(inBuffer);
    psnpFrequ.push_back(inData.data());

    std::getline(*this, inBuffer, '\n');
    inData = stripQuotes(inBuffer);
    pValue.push_back(inData.data());
    //long double ld = stold(inData.c_str());
    ldPValue.push_back(stold(inData.c_str()));

    linesRead++;
}

bool PositionData::next()
{
    if (eof() || linesRead > totalLines - skip) return false;
    popline();
    pushline();
    return true;
}

int PositionData::currentPosition() const
{
    return intPosition[windowSize];
}
#include "options.h"
long double PositionData::currentLogPvalue() const
{
    long double logPV = log10(ldPValue[windowSize]);
    return logPV;
}

double PositionData::currentPsnpFrequency() const
{
    double freq =  ((double)intPsnpCount[windowSize])
                    /((double)intCoverage[windowSize]);
    return freq;
}

string PositionData::getWindowData() const
{
    string data;
    //const string sep = ",";
    int aCount = 0;
    int cCount = 0;
    int gCount = 0;
    for ( uint i = 0 ; i <= 2*windowSize ; i++)
    {
        int logPValue = (int) log10(ldPValue[i]);

        data += to_string(logPValue);
        data += ",";
        data += refBase[i];
        aCount += refBase[i][1] == 'a' || refBase[i][1] == 'A' ? 1 : 0;
        cCount += refBase[i][1] == 'c' || refBase[i][1] == 'C' ? 1 : 0;
        gCount += refBase[i][1] == 'g' || refBase[i][1] == 'G' ? 1 : 0;
        data += ",";
        data += psnp[i];
        data += ",";
        data += to_string((double)intPsnpCount[i]/(double)intCoverage[i]);
        //data += psnpFrequ[i];
        data += ",";
    }
    data += coverage[windowSize];
    data += ",";
    data += psnpCount[windowSize];
    data += ",";
    data += quality[windowSize];
    data += ",";
    data += to_string(aCount);
    data += ",";
    data += to_string(cCount);
    data += ",";
    data += to_string(gCount);

    return data;
}

string PositionData::getDataHeader() const
{
    string data;

    for ( uint i = 1 ; i <= 2*windowSize+1 ; i++)
    {
        data += "log pValue " + to_string(i);
        data += ",";
        data += "Ref Base " + to_string(i);
        data += ",";
        data += "pSNP Base " + to_string(i);
        data += ",";
        data += "pSNP Freq " + to_string(i);
        data += ",";
    }
    data += "coverage";
    data += ",";
    data += "psnpCount";
    data += ",";
    data += "quality";
    data += ",";
    data += "A count";
    data += ",";
    data += "C count";
    data += ",";
    data += "G count";
    data += "\n";
    return data;
}

string PositionData::getWindowDataARFF() const
{
    string data;
    //const string sep = ",";
    int aCount = 0;
    int cCount = 0;
    int gCount = 0;
    int tCount = 0;
    for ( uint i = 0 ; i <= 2*windowSize ; i++)
    {
        int logPValue = (int) log10(ldPValue[i]);

        data += to_string(logPValue);
        data += ",";
        data += refBase[i];
        aCount += refBase[i][0] == 'a' || refBase[i][0] == 'A' ? 1 : 0;
        cCount += refBase[i][0] == 'c' || refBase[i][0] == 'C' ? 1 : 0;
        gCount += refBase[i][0] == 'g' || refBase[i][0] == 'G' ? 1 : 0;
        tCount += refBase[i][0] == 't' || refBase[i][0] == 'T' ? 1 : 0;
        data += ",";
        data += psnp[i];
        data += ",";
        data += to_string((double)intPsnpCount[i]/(double)intCoverage[i]);
        //data += psnpFrequ[i];
        data += ",";
    }
    data += coverage[windowSize];
    data += ",";
    data += psnpCount[windowSize];
    data += ",";
    data += quality[windowSize];
    data += ",";
    data += to_string(aCount);
    data += ",";
    data += to_string(cCount);
    data += ",";
    data += to_string(gCount);
    data += ",";
    data += to_string(tCount);
    return data;
}

string PositionData::getDataHeaderARFF() const
{
    const string relation = "@RELATION ";
    const string attribute = "@ATTRIBUTE ";
    const string numeric = "NUMERIC";

    string data;
    data += "% Data from reads where a variant has probablity of being a\n";
    data += "% sequencing error bellow the threshold value of ";
    data +=  Options::get(Options::LogPValueThreshold) + "\n";
    data += "% Percentage frequency ";
    data +=  Options::get(Options::FrequencyThreholdPct) + "\n";
    data += "% Input read data file output from varientLister ";
    data += Options::get(Options::VariantData) + "\n";
    data += "% True postive postions from ";
    data += Options::get(Options::TrueData) + "\n";
    data += relation + "varientData/n";
    data += "\n";
    for ( uint i = 1 ; i <= 2*windowSize+1 ; i++)
    {
        data += attribute +  "logPValue" + to_string(i) + " " + numeric + "\n";
        data += attribute +  "RefBase" + to_string(i) + " {A,G,C,T}" + "\n";
        data += attribute +  "psnpBase" + to_string(i) + " {A,G,C,T,=}" + "\n";
        data += attribute +  "psnpFreq" + to_string(i) + " " + numeric + "\n";
    }
    data += attribute +  "coverage " + numeric + "\n";
    data += attribute +  "psnpCount " + numeric + "\n";
    data += attribute +  "quality " + numeric + "\n";
    data += attribute +  "Acount " + numeric + "\n";
    data += attribute +  "Ccount " + numeric + "\n";
    data += attribute +  "Gcount " + numeric + "\n";
    data += attribute +  "Tcount " + numeric + "\n";
    data += attribute +  "truePSNP {0,1}\n";
    data += "\n";
    data += "@data\n";

    return data;
}




