#ifndef BIPOLERCALLER_H
#define BIPOLERCALLER_H
#include "basecaller.h"

class BiPolerCaller : public BaseCaller
{
public:
    BiPolerCaller(const string &t, unsigned int locus = 0);
    BiPolerCaller(const string &t
                  ,char lowDelPhred
                  ,char highDelPhred
                  ,char lowInsPhred
                  ,char highInsPhred
                  ,char lowSubPhred
                  ,char highSubPhred
                  ,short lowDelPct
                  ,short lowInsPct
                  ,short lowSubPct
                  ,unsigned int locus = 0);
    virtual ~BiPolerCaller();
protected:
    virtual void simulateData();
    virtual void simulateData(bool setDeleteTag);
private:
    void deletionError();
    void insertionError(bool setDeleteTag);
    void substitutinoError(bool setDeleteTag);

    char lowDelPhred = 5;
    char highDelPhred = 16;
    char lowInsPhred = 3;
    char highInsPhred = 17;
    char lowSubPhred = 6;
    char highSubPhred = 27;

    short lowDelPct = 30;
    short lowInsPct = 60;
    short lowSubPct = 60;

    long double probLowDel;
    long double probHighDel;
    long double probLowIns;
    long double probHighIns;
    long double probLowSub;
    long double probHighSub;

    uniform_int_distribution<short> rndPct;
};

#endif // BIPOLERCALLER_H
