#ifndef SIMBAX_H
#define SIMBAX_H
#include <string>
#include <random>

#include "H5Cpp.h"

using namespace H5;
using namespace std;

class SimBax
{
public:
    SimBax(const string& tFilename,
           const string& prefix,
           const int depth);
    void operator() ();
private:
    enum BaseErrorType {
        Deletion,
        Insertion,
        Merge,
        Substitution
    };

    void makeRead(string& r, unsigned int startPos);
    void writeFastq(string &r, ofstream &fastq, int readNum);
    BaseErrorType getErrorType(unsigned int tPos);
    inline bool MergePosible(unsigned int tpos) const;
    char randomBase(char notThis = '-');
    inline void resetStrings();

    string tFilename;
    string baxFilename;
    string fastqFilename;
    string t;

    short depth;
    short readLen = 4000;
    short deletionPhred = 7;
    short insertionPhred = 11;
    short mergePhred = 20;
    short substituionPhred = 25;
    short qualityPhred;
    char fastqPhred;
    long double qualityProb;

    string baseCall, deletionQV, deletionTag, insertionQV, qualityValue, mergeQV;
    string substitutionQV, subsititutionTag, numEvent;
    vector<int> reads;

    std::random_device rd;
    std::mt19937 gen;
    uniform_real_distribution<long double> rndProb;

    long deletionRatio;
    long insertionRatio;
    long substituionRatio;

    long deletionMRatio;
    long insertionMRatio;
    long mergeMRatio;
    long substituionMRatio;
    const long ratioSize = 1000000000;
    uniform_int_distribution<long> rndErrType;
    uniform_int_distribution<short> rndBase;
};

#endif // SIMBAX_H
