#ifndef DISCRETECALLER_H
#define DISCRETECALLER_H
#include "basecaller.h"
#include "main.h"
#include <array>

class DiscreteCaller : public BaseCaller
{
public:
    DiscreteCaller(const string &t, unsigned int locus = 0);
    DiscreteCaller(const string &t
                   , const array<unsigned char, nPhreds> &delitionsDist
                   , const array<unsigned char, nPhreds> &insertionDist
                   , const array<unsigned char, nPhreds> &mergeDist
                   , const array<unsigned char, nPhreds> &substitutionDist
                   , unsigned int locus = 0);
    virtual ~DiscreteCaller();

protected:
    virtual void simulateData();
    virtual void simulateData(bool setDeleteTag);
private:
    void deletionError();
    void insertionError(bool setDeleteTag);
    void substitutinoError(bool setDeleteTag);
    void reconfigDist(const array<unsigned char, nPhreds> inDist
                      , array<unsigned char, nPhreds>& outDist
                      , unsigned short& background);

    array<unsigned char, nPhreds> delitionsDist;
    array<unsigned char, nPhreds> insertionDist;
    array<unsigned char, nPhreds> mergeDist;
    array<unsigned char, nPhreds> substitutionDist;
    unsigned short backgroundDel;
    unsigned short backgroudMerge;
    unsigned short backgroundIns;
    unsigned short backgourndSub;

    uniform_int_distribution<short> rndPct;
};

#endif // DISCRETECALLER_H
