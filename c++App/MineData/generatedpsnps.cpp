#include <fstream>
#include <array>
#include <cstdlib>
#include "main.h"
#include "generatedpsnps.h"

GeneratedPsnps::GeneratedPsnps(const string& psnpFilename)
    : inFilemane(psnpFilename)
{
    init();
}

bool GeneratedPsnps::hasPosition(int pos) const
{
    return positions.find(pos) != positions.end();
}

void GeneratedPsnps::init()
{
    ifstream inf(inFilemane.c_str());
    if (inf.is_open())
    {
        int index = 0;
        array<char, bufferSize> inBuffer;

        inf.getline(inBuffer.data(),bufferSize,'\n');

        while (!inf.eof())
        {
            inf.getline(inBuffer.data(),bufferSize,'\t');
            stripQuotes(inBuffer.data());
            inf.getline(inBuffer.data(),bufferSize,'\t');
            position.push_back(atoi(inBuffer.data()));
            positions.insert(atoi(inBuffer.data()));
            inf.getline(inBuffer.data(),bufferSize,'\t');
            refBase.push_back(inBuffer[1]);
            inf.getline(inBuffer.data(),bufferSize,'\t');
            psnp.push_back((inBuffer[1]));
            inf.getline(inBuffer.data(),bufferSize,'\t');
            frequency.push_back(atoi(&inBuffer[0]));
            inf.getline(inBuffer.data(),bufferSize,'\n');
            index++;
        }
    }
}

vector<int> GeneratedPsnps::getPosition() const
{
    return position;
}

vector<int> GeneratedPsnps::getFrequency() const
{
    return frequency;
}

string GeneratedPsnps::getRefBase() const
{
    return refBase;
}

string GeneratedPsnps::getPsnp() const
{
    return psnp;
}

uint GeneratedPsnps::length() const
{
    return position.size();
}







