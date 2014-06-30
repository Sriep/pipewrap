#include "bipolercaller.h"
#include "main.h"

BiPolerCaller::BiPolerCaller(const string& t, unsigned int locus)
    : BaseCaller(t, locus)
{
}

BiPolerCaller::BiPolerCaller(const string &t
                             , char lowDelPhred
                             , char highDelPhred
                             , char lowInsPhred
                             , char highInsPhred
                             , char lowSubPhred
                             , char highSubPhred
                             , short lowDelPct
                             , short lowInsPct
                             , short lowSubPct
                             , unsigned int locus)
    : BaseCaller(t, locus)
    ,lowDelPhred(lowDelPhred)
    ,highDelPhred(highDelPhred)
    ,lowInsPhred(lowInsPhred)
    ,highInsPhred(highInsPhred)
    ,lowSubPhred(lowSubPhred)
    ,highSubPhred(highSubPhred)
    ,lowDelPct(lowDelPct)
    ,lowInsPct(lowInsPct)
    ,lowSubPct(lowSubPct)
    ,probLowDel(phred2prob(lowDelPhred))
    ,probHighDel(phred2prob(highDelPhred))
    ,probLowIns(phred2prob(lowInsPhred))
    ,probHighIns(phred2prob(highInsPhred))
    ,probLowSub(phred2prob(lowSubPhred))
    ,probHighSub(phred2prob(highSubPhred))
    ,rndPct(1,100)
{
        simulateData();
}

BiPolerCaller::~BiPolerCaller()
{
}

void BiPolerCaller::simulateData()
{
    simulateData(true);
}

void BiPolerCaller::simulateData(bool setDeleteTag)
{
    deletionQV = rndPct(gen) <= lowDelPct ? lowDelPhred : highDelPhred;
    insrtionQV = rndPct(gen) <= lowInsPct ? lowInsPhred : highInsPhred;
    substitutionQV = rndPct(gen) <= lowSubPct ? lowSubPhred : highSubPhred;
    const long double pDeletion = phred2prob(deletionQV);
    const long double pInsertion = phred2prob(insrtionQV);
    const long double pSubstitution = phred2prob(substitutionQV);
    const long double pError = pDeletion + pInsertion + pSubstitution;
    qualityValue = prob2phred(pError);

    if (rndProb(gen) < pError)
    {
        const long double pErrorType = rndProb(gen);
        if (pErrorType < pDeletion/pError)
            deletionError();
        else if (pErrorType < (pDeletion + pInsertion)/pError)
            insertionError(setDeleteTag);
        else
            substitutinoError(setDeleteTag);
    }
    else
    {
        baseCall = t[locus];
        if (setDeleteTag) deletionTag = randomBase(baseCall);
        substitutionTag = randomBase(baseCall);
    }
}

void BiPolerCaller::deletionError()
{
    if (rndProb(gen) < probLowDel/(probLowDel+3*probHighDel))
        deletionTag = t[locus];
    else
        deletionTag = randomBase(t[locus]);
    nextLocus();
    simulateData(false);
}

void BiPolerCaller::insertionError(bool setDeleteTag)
{
    baseCall = randomBase();
    moveLocus = false;
    if (setDeleteTag) deletionTag = randomBase();
    substitutionTag = randomBase();
}

void BiPolerCaller::substitutinoError(bool setDeleteTag)
{
    baseCall = randomBase(t[locus]);
    if (setDeleteTag) deletionTag = randomBase(baseCall);
    if (rndProb(gen) < probLowSub/(probLowSub+3*probHighSub))
        substitutionTag = t[locus];
    else
        substitutionTag = randomBase(t[locus]);
}











