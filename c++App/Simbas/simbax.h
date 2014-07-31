#ifndef SIMBAX_H
#define SIMBAX_H
#include <string>
#include <random>
#include <memory>
#include "basecaller.h"

#include "H5Cpp.h"

using namespace H5;
using namespace std;

class SimBax
{
public:
    SimBax(const string& t,
           const string& prefix,
           const int depth,
           unique_ptr<BaseCaller> baseCaller);
    SimBax(const string& t,
           unique_ptr<BaseCaller> baseCaller);
    void operator() ();
private:

    void makeRead(unsigned int readLen
                  , unsigned int startLocus
                  , string &fastqPhreds);
    void makeReadOld(string& r, unsigned int startPos);
    void writeFastq(const string &r, const string &q, ofstream &fastq, int readNum);
    inline bool MergePosible(unsigned int tpos) const;
    char randomBase(char notThis = '-');
    inline void resetStrings();

    //string tFilename;
    const string& t;
    string baxFilename;
    string fastqFilename;

    short depth;
    short readLen;
    unique_ptr<BaseCaller> baseCaller;    

    string baseCall;
    string deletionQV;
    string deletionTag;
    string insertionQV;
    string qualityValue;
    string mergeQV;
    vector<unsigned short> preBaseFrame;
    vector<unsigned int> pulseIndex;
    string substitutionQV;
    string subsititutionTag;
    vector<unsigned short> widthInFrame;
    vector<unsigned int> reads;

    std::random_device rd;
    std::mt19937 gen;
    //uniform_real_distribution<long double> rndProb;



};

#endif // SIMBAX_H
