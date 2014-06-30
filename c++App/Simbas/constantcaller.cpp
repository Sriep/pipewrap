#include "constantcaller.h"
#include "main.h"
#include <cassert>
#include <random>

ConstantCaller::ConstantCaller(const string& t, unsigned int locus)
    : BaseCaller(t, locus)
{
}

ConstantCaller::ConstantCaller(const string &t,
                               unsigned short deletionPhred,
                               unsigned short insertionPhred,
                               unsigned short mergePhred,
                               //unsigned short perBaseFrameVale,
                               //unsigned int pulseIndexVale,
                               unsigned short substitutionPhred,
                               //unsigned short widthinFramesVale,
                               unsigned int deletionRatio,
                               unsigned int insertionRatio,
                               unsigned int substitutionRatio,
                               unsigned int mergeRatio,
                               unsigned int position)
    : BaseCaller(t, position)
    ,deletionPhred(deletionPhred)
    ,insertionPhred(insertionPhred)
    ,mergePhred(mergePhred)
    ,substituionPhred(substitutionPhred)
    ,deletionRatio(deletionRatio)
    ,insertionRatio(insertionRatio)
    ,substituionRatio(substitutionRatio)
    ,mergeMRatio(mergeRatio)
{
    qualityProb = phred2prob(deletionPhred)
                + phred2prob(insertionPhred)
                + phred2prob(substituionPhred);

    qualityPhred = prob2phred(qualityProb);
    fastqPhred = char (qualityPhred + 33);

    deletionRatio = (phred2prob(deletionPhred)/qualityProb)*ratioSize;
    insertionRatio = (phred2prob(insertionPhred)/qualityProb)*ratioSize;
    substituionRatio = (phred2prob(substituionPhred)/qualityProb)*ratioSize;

    long double qulPlusMProb = qualityProb + phred2prob(mergePhred);
    deletionMRatio = (phred2prob(deletionPhred)/qulPlusMProb)*ratioSize;
    insertionMRatio = (phred2prob(insertionPhred)/qulPlusMProb)*ratioSize;
    substituionMRatio = (phred2prob(substituionPhred)/qulPlusMProb)*ratioSize;
    substituionMRatio = (phred2prob(substituionPhred)/qulPlusMProb)*ratioSize;
}

ConstantCaller::~ConstantCaller()
{
}

void ConstantCaller::simulateData()
{
    bool foundNextBase = false;
    do
    {
        if (rndProb(gen) < qualityProb)
        {
            switch (getErrorType(locus))
            {
            case Deletion:
                deletionTag =t[locus];
                substitutionTag = t[locus];
                mergeQV = mergePosible(locus) ? mergePhred : 100;
                nextLocus();
                break;
            case Insertion:
                baseCall = randomBase();
                foundNextBase = true;
                moveLocus = false;
                break;
            case Merge:
                deletionTag = t[locus];
                substitutionTag = t[locus];
                mergeQV  = mergePhred;
                baseCall = t[locus++];
                foundNextBase = true;
                break;
            case Substitution:
                deletionTag = t[locus];
                substitutionTag = t[locus];
                mergeQV = mergePosible(locus) ? mergePhred : 100;
                baseCall = randomBase(t[locus++]);
                foundNextBase = true;
                break;
            default:
                assert(false);
            }
        }
        else
        {
            deletionTag = t[locus];
            substitutionTag = t[locus];
            mergeQV = mergePosible(locus) ? mergePhred : 100;
            baseCall = t[locus++];
            foundNextBase = true;
        }
    } while(!foundNextBase);

}

BaseCaller::BaseErrorType ConstantCaller::getErrorType(unsigned int tPos)
{
    long rndL = rndErrType(gen);
    if (mergePosible(tPos))
    {
        if (rndL < deletionMRatio) return Deletion;
        if (rndL < deletionMRatio + insertionMRatio) return Insertion;
        if (rndL < deletionMRatio + insertionMRatio + mergeMRatio) return Merge;
        return Substitution;
    }
    else
    {
        if (rndL < deletionRatio) return Deletion;
        if (rndL < deletionRatio + insertionRatio) return Insertion;
        return Substitution;
    }
}




