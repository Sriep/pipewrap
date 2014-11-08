#ifndef PositionData_H
#define PositionData_H
#include <fstream>
#include <deque>
#include <string>

using namespace std;

class PositionData : public ifstream
{
public:
    PositionData(const string& dataFilename, uint windowSize);
    bool next();
    long double  currentLogPvalue() const;
    double currentPsnpFrequency() const;
    string getWindowData() const;
    string getDataHeader() const;
    string getWindowDataARFF() const;
    string getDataHeaderARFF() const;
    int currentPosition() const;

private:
    /*deque<int> getPosition() const;
    deque<char> getRefBase() const;
    deque<int> getCoverage() const;
    deque<int> getQuality() const;
    deque<char> getPsnp() const;
    deque<int> getPsnpCount() const;
    deque<long double> getPValue() const;*/

    void init();
    const uint windowSize;
    const uint skip;
    uint linesRead = 0;
    uint totalLines = 0;

    deque<string> position;
    deque<string> refBase;
    deque<string> coverage;
    deque<string> quality;
    deque<string> psnp;
    deque<string> psnpCount;
    deque<string> psnpFrequ;
    deque<string> pValue;

    deque<int> intPosition;
    deque<int> intCoverage;
    deque<int> intPsnpCount;
    /*deque<char> refBase;  
    deque<int> quality;
    deque<char> psnp;  
    deque<float> psnpFrequ;*/
    deque<long double> ldPValue;

    int getLineCount(const string &filename);
    void pushline();
    void popline();
};

#endif // PositionData_H
