#include "discretecaller.h"
#include "main.h"
#include <cmath>
#include <cassert>

DiscreteCaller::DiscreteCaller(const string& t, unsigned int locus)
    : BaseCaller(t, locus), rndPct(0,100)
{
}

DiscreteCaller::DiscreteCaller(const string &t
                           , const array<unsigned char, nPhreds>& delitions
                           , const array<unsigned char, nPhreds>& insertions
                           , const array<unsigned char, nPhreds>& merges
                           , const array<unsigned char, nPhreds>& substitutions
                           , unsigned int locus)
    : BaseCaller(t, locus), rndPct(0,100)
{
    reconfigDist(delitions, delitionsDist, backgroundDel);
    reconfigDist(insertions, insertionDist, backgroundIns);
    reconfigDist(merges, mergeDist, backgroudMerge);
    reconfigDist(substitutions, substitutionDist, backgourndSub);
    simulateData();
}

void DiscreteCaller::reconfigDist(const array<unsigned char, nPhreds> inDist
                                 , array<unsigned char, nPhreds>& outDist
                                 , unsigned short &background)
{
    unsigned int total = 0;
    background = 0;
    for ( unsigned int i = 0 ; i < inDist.size() ; i++ )
    {
        total += inDist[i];
        if (inDist[i] > 0) background = i;
    }
    if (total > 0)
    {
        unsigned int index = 0;
        long double sum = 0.0;
        long double toPct = 100.0;
        toPct = toPct / (long double) total;

        for ( unsigned int i = 0 ; i < inDist.size() ; i++ )
        {
            sum += (long double) inDist[i] * toPct;
            for ( ; (index < (unsigned int)(round(sum)) && (index < nPhreds))
                  ; index++ )
            {
                outDist[index] = i;
            }
        }
        for (  ; index < nPhreds; index++ )
            outDist[index] = background;
    }
    else
    {
        for ( unsigned int i = 0 ; i < outDist.size() ; i++ )
            outDist[i] = 100;
    }
}

DiscreteCaller::~DiscreteCaller()
{
}

void DiscreteCaller::simulateData()
{
    simulateData(true);
}

void DiscreteCaller::simulateData(bool setDeleteTag)
{
    deletionQV = delitionsDist[rndPct(gen)];
    insrtionQV = insertionDist[rndPct(gen)];
    mergeQV = 100;//mergeDist[rndPct(gen)];
    substitutionQV = substitutionDist[rndPct(gen)];

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

void DiscreteCaller::deletionError()
{
    if (rndProb(gen) < deletionQV/(deletionQV+3*backgroundDel))
        deletionTag = t[locus];
    else
        deletionTag = randomBase(t[locus]);
    nextLocus();
    simulateData(false);
}

void DiscreteCaller::insertionError(bool setDeleteTag)
{
    baseCall = randomBase();
    moveLocus = false;
    if (setDeleteTag) deletionTag = randomBase();
    substitutionTag = randomBase();
}

void DiscreteCaller::substitutinoError(bool setDeleteTag)
{
    baseCall = randomBase(t[locus]);
    if (setDeleteTag) deletionTag = randomBase(baseCall);
    if (rndProb(gen) < substitutionQV/(substitutionQV+3*backgourndSub))
        substitutionTag = t[locus];
    else
        substitutionTag = randomBase(t[locus]);
}


