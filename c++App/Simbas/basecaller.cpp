#include <cassert>
#include "main.h"
#include "basecaller.h"
#include "options.h"


BaseCaller::BaseCaller()
    : t(getFileContents(Options::get(Options::InTemplate).c_str()))
    , locus(0)
    , rndBase(1, 4*3)
    , rndProb(0,1)
{
    locus = 0;
    basesFromFasta(t);
    for ( unsigned int i = 0 ; i < t.size() ; i++)
        if ('a' <= t[i] && t[i] <= 'z') t[i] -= 'a' - 'A';
}

BaseCaller::BaseCaller(const string& tp, unsigned int locus)
    : t(tp), locus(locus), rndBase(1, 4*3), rndProb(0,1)
{
    for ( unsigned int i = 0 ; i < t.size() ; i++)
        if ('a' <= tp[i] && tp[i] <= 'z') t[i] -= 'a' - 'A';
}

void BaseCaller::next()
{
    if (moveLocus)
        nextLocus();
    else
        moveLocus = true;
    simulateData();
}

void BaseCaller::nextLocus()
{
    locus++;
    if (t.size() == locus) locus = 0;
}

bool BaseCaller::mergePosible(unsigned int tpos) const
{
    assert(tpos<t.size());
    return tpos+1 < t.size() ? t[tpos+1] == t[tpos] : t[tpos] == t[0];
}

char BaseCaller::randomBase(char notThis)
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

unsigned char BaseCaller::getBaseCall() const
{
    return baseCall;
}
unsigned char BaseCaller::getDeletionQV() const
{
    return deletionQV;
}
unsigned char BaseCaller::getDeletionTag() const
{
    return deletionTag;
}
unsigned char BaseCaller::getInsrtionQV() const
{
    return insrtionQV;
}
unsigned short BaseCaller::getPreBaseFrame() const
{
    return preBaseFrame;
}
unsigned int BaseCaller::getPulseIndex() const
{
    return pulseIndex;
}
unsigned char BaseCaller::getQualityValue() const
{
    return qualityValue;
}
unsigned char BaseCaller::getMergeQV() const
{
    return mergeQV;
}
unsigned char BaseCaller::getSubstitutionQV() const
{
    return substitutionQV;
}
unsigned char BaseCaller::getSubstitutionTag() const
{
    return substitutionTag;
}
unsigned short BaseCaller::getWidthInFrames() const
{
    return widthInFrames;
}
unsigned int BaseCaller::getLocus() const
{
    return locus;
}

void BaseCaller::setLocus(unsigned int value)
{
    locus = value;
    moveLocus = true;
    simulateData();
}

