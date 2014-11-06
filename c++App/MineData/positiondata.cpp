#include <array>
#include <cmath>
#include "main.h"

#include "positiondata.h"

PositionData::PositionData(const string &dataFilename, uint windowSize = 5)
    : ifstream(dataFilename.c_str()),
      windowSize(windowSize)
{
    if (is_open())
    {
        init();
    }
}

void PositionData::init()
{
    const int bufferSize = 256;
    array<char, bufferSize> inBuffer;

    getline(inBuffer.data(),bufferSize,'\n');
    for (uint i = 0 ; i < 2*windowSize+1 ; i++ )
    {
        pushline();
    }
}

void PositionData::popline()
{
    position.pop_front();
    intPosition.pop_front();
    refBase.pop_front();
    coverage.pop_front();
    quality.pop_front();
    psnp.pop_front();
    psnpCount.pop_front();
    psnpFrequ.pop_front();
    ldPValue.pop_front();
    pValue.pop_front();
}

void PositionData::pushline()
{
    //const int bufferSize = 256;
    //array<char, bufferSize> inBuffer;
    string inBuffer;

    //getline(inBuffer.c_str(),bufferSize,'\t');
    std::getline(*this, inBuffer, '\t');
    stripQuotes(inBuffer);
    intPosition.push_back(atoi(&inBuffer[1]));
    position.push_back(inBuffer.data());

    std::getline(*this, inBuffer, '\t');
    //getline(inBuffer.data(),bufferSize,'\t');
    stripQuotes(inBuffer);
    refBase.push_back(inBuffer.data());
    //getline(inBuffer.data(),bufferSize,'\t');
    std::getline(*this, inBuffer, '\t');
    stripQuotes(inBuffer);
    coverage.push_back(inBuffer.data());
    //getline(inBuffer.data(),bufferSize,'\t');
    std::getline(*this, inBuffer, '\t');
    stripQuotes(inBuffer);
    quality.push_back(inBuffer.data());
    //getline(inBuffer.data(),bufferSize,'\t');
    std::getline(*this, inBuffer, '\t');
    stripQuotes(inBuffer);
    psnp.push_back(inBuffer.data());
    //getline(inBuffer.data(),bufferSize,'\t');
    std::getline(*this, inBuffer, '\t');
    stripQuotes(inBuffer);
    psnpCount.push_back(inBuffer.data());
    //getline(inBuffer.data(),bufferSize,'\t');
    std::getline(*this, inBuffer, '\t');
    stripQuotes(inBuffer);
    psnpFrequ.push_back(inBuffer.data());
    //getline(inBuffer.data(),bufferSize,'\n');
    std::getline(*this, inBuffer, '\n');
    stripQuotes(inBuffer);
    pValue.push_back(inBuffer.data());
    string pv = &inBuffer[0];
    ldPValue.push_back(stold(&inBuffer[0]));
}

bool PositionData::next()
{
    if (eof()) return false;
    popline();
    pushline();
    return true;
}

int PositionData::currentPosition() const
{
    return intPosition[windowSize];
}

long double PositionData::currentLogPvalue() const
{
    long double logPV = log10(ldPValue[windowSize]);
    return logPV;
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
        data += psnpFrequ[i];
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




