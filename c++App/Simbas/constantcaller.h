#ifndef CONSTANTCALLER_H
#define CONSTANTCALLER_H
#include "basecaller.h"

class ConstantCaller : public BaseCaller
{
public:
    ConstantCaller(const string& t, unsigned int locus = 0);
    ConstantCaller(const string& t,
                   unsigned short deletionPhred,
                   unsigned short insrtionPhred,
                   unsigned short mergePhred,
                   unsigned short substitutionPhred,
                   unsigned int deletionRatio,
                   unsigned int insertionRatio,
                   unsigned int substitutionRatio,
                   unsigned int mergeRatio,
                   unsigned int locus = 0);
    virtual ~ConstantCaller();

protected:
    virtual void simulateData();
private:
    BaseErrorType getErrorType(unsigned int tPos);
    uniform_int_distribution<long> rndErrType;

    short deletionPrior = 13;
    short substitutionPrior = 20;
    short readLen = 4000;
    short deletionPhred = 11;
    short insertionPhred = 9;
    short mergePhred = 20;
    short substituionPhred = 10;
    short qualityPhred;
    char fastqPhred;
    long double qualityProb;

    const long ratioSize = 1000000000;

    long deletionRatio = 10;
    long insertionRatio = 9;
    long substituionRatio = 5;

    long deletionMRatio = 10;
    long insertionMRatio = 9;
    long mergeMRatio = 1;
    long substituionMRatio = 5;

};

#endif // CONSTANTCALLER_H
