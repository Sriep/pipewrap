#include <cassert>
#include <fstream>
#include "H5Cpp.h"
#include <H5IdComponent.h>
#include <H5Location.h>
#include <H5Object.h>
#include <H5DataType.h>
#include <H5AtomType.h>
#include <H5PredType.h>
#include "simbax.h"
#include "main.h"
#include "baxh5.h"

SimBax::SimBax(const string& tFilename,
               const string &prefix,
               const int depth)
    : tFilename(tFilename)
    , baxFilename(prefix + ".bax.h5")
    , fastqFilename(prefix + ".fastq")
    , depth(depth)
    , gen(rd())
    , rndProb(0,1)
    , rndErrType(0, ratioSize)
    , rndBase(1, 4*3)
{
    t = getFileContents(tFilename.c_str());
    basesFromFasta(t);
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

void SimBax::operator()()
{
    uniform_int_distribution<int> readStart(0, t.size());
    ofstream fastq(fastqFilename);
    BaxH5 baxh5(baxFilename);
    resetStrings();
    const int numReads = t.size() * depth / readLen;
    for (int i = 0 ; i < numReads ; i++ )
    {        
        string newRead;
        makeRead(newRead, readStart(gen));
        reads.push_back(newRead.size());
        writeFastq(newRead, fastq, i);
        baseCall += newRead;
    }
    deletionQV.insert(0, baseCall.size(), deletionPhred);
    insertionQV.insert(0, baseCall.size(), insertionPhred);
    substitutionQV.insert(0, baseCall.size(), substituionPhred);
    qualityValue.insert(0, baseCall.size(), qualityPhred);
    baxh5.writeRead(baseCall, deletionQV, deletionTag,
                    insertionQV, mergeQV, substitutionQV,
                    subsititutionTag, qualityValue, reads);
}

void SimBax::makeRead(string& r, unsigned int startPos)
{
    assert(startPos < t.size());
    unsigned int locus = startPos;
    for ( int i = 0 ; i < readLen ; i++ )
    {
        if (t.size() == locus) locus = 0;
        if (rndProb(gen) < qualityProb)
        {
            switch (getErrorType(locus))
            {
            case Deletion:
                deletionTag.push_back(t[locus]);
                subsititutionTag.push_back(t[locus]);
                mergeQV.push_back(MergePosible(locus) ? mergePhred : 100);
                locus++;
                break;
            case Insertion:
                r.push_back(randomBase());
                break;
            case Merge:                
                deletionTag.push_back(t[locus]);
                subsititutionTag.push_back(t[locus]);
                mergeQV.push_back(mergePhred);
                r.push_back(t[locus++]);
                locus = (locus == t.size()) ? t[1] : locus+1;
                break;
            case Substitution:
                deletionTag.push_back(t[locus]);
                subsititutionTag.push_back(t[locus]);
                mergeQV.push_back(MergePosible(locus) ? mergePhred : 100);
                r.push_back(randomBase(t[locus++]));
                break;
            default:
                assert(false);
            }
        }
        else
        {            
            deletionTag.push_back(t[locus]);
            subsititutionTag.push_back(t[locus]);
            mergeQV.push_back(MergePosible(locus) ? mergePhred : 100);
            r.push_back(t[locus++]);
        }
    }
}

SimBax::BaseErrorType SimBax::getErrorType(unsigned int tPos)
{
    long rndL = rndErrType(gen);
    if (MergePosible(tPos))
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

char SimBax::randomBase(char notThis)
{
    short r = rndBase(gen);
    if ('-' == notThis)
    {
        if (1 == r || 2 == r || 3 == r) return 'A';
        if (4 == r || 5 == r || 6 == r) return 'C';
        if (7 == r || 8 == r || 9 == r) return 'G';
        return 'T';
    }
    else
    {
        if (1 == r || 2 == r || 3 == r || 4 == r)
            return ('A' == notThis || 'a' == notThis) ? 'T' : 'A';
        if (5 == r || 6 == r || 7 == r || 8 == r)
            return ('C' == notThis || 'c' == notThis) ? 'T' : 'C';
        return ('G' == notThis || 'g' == notThis) ? 'T' : 'G';
    }
}

inline void SimBax::resetStrings()
{
    baseCall.clear(); baseCall.resize(0);
    deletionQV.clear(); deletionQV.resize(0);
    deletionTag.clear(); deletionTag.resize(0);
    insertionQV.clear(); insertionQV.resize(0);
    qualityValue.clear(); qualityValue.resize(0);
    mergeQV.clear(); mergeQV.resize(0);
    substitutionQV.clear(); substitutionQV.resize(0);
    subsititutionTag.clear(); subsititutionTag.resize(0);
    numEvent.clear(); numEvent.resize(0);
    reads.clear(); reads.resize(0);
}

inline bool SimBax::MergePosible(unsigned int tpos) const
{
    assert(tpos<t.size());
    return tpos+1 < t.size() ? t[tpos+1] == t[tpos] : t[tpos] == t[0];
}

void SimBax::writeFastq(string &r, ofstream& fastq, int readNum)
{
    fastq << "@S1_" << std::to_string(readNum)  << '\n';
    fastq << r << '\n';
    fastq << "+S1_" << std::to_string(readNum) << '\n';
    for (unsigned int i = 0 ; i < r.size() ; i++ ) fastq << fastqPhred;
    fastq << '\n';
}



































