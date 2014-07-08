#include "locusinfo.h"
#include "pvalues.h"

LocusInfo::LocusInfo(char newbase,
                     FrequencyPartition *freqPartition,
                     set<PValues::Method> methods)
    : base(newbase), freqPartition(freqPartition), methods(methods)
{
    base = newbase;
}

void LocusInfo::inc_calls(char new_base, int phred)
{
    coverage++;
    total_phred += phred;
    //baseQualities.push_back(phred);
    //alignedBases.push_back(new_base);
    if (ave_phred) ave_phred = 0;
    switch (new_base)
    {
    case 'a':
    case'A':
        a_calls++;
        aPhred.push_back(phred);
        break;
    case 'c':
    case'C':
        c_calls++;
        cPhred.push_back(phred);
        break;
    case 'g':
    case'G':
        g_calls++;
        gPhred.push_back(phred);
        break;
    case 't':
    case 'T':
        t_calls++;
        tPhred.push_back(phred);
        break;
    default:
        bad_calls++;
        //base = '-';
    }
}

int LocusInfo::countBestEx() const
{
    switch (base)
    {
    case 'a':
    case'A':
        return max(c_calls, max(g_calls, t_calls));
    case 'c':
    case'C':
        return max(a_calls, max(g_calls, t_calls));
    case 'g':
    case'G':
        return max(a_calls, max(c_calls, t_calls));
    case 't':
    case 'T':
        return max(a_calls, max(c_calls, g_calls));
    default:
        return 0;
    }
}

std::vector<unsigned int> LocusInfo::phredBestEx() const
{
    switch (bestbaseEx())
    {
    case 'a':
    case'A':
        return aPhred;
    case 'c':
    case'C':
        return cPhred;
    case 'g':
    case'G':
        return gPhred;
    case 't':
    case 'T':
        return tPhred;
    default:
        std::vector<unsigned int> empty;
        return empty;
    }
}

char LocusInfo::bestbaseEx() const
{
    if ( (0 == a_calls || 'a' == base || 'A' == base)
         && (0 == t_calls || 't' == base || 'T' == base)
         && (0 == g_calls || 'g' == base || 'G' == base)
         && (0 == c_calls || 'c' == base || 'C' == base) )
        return '=';

    switch (base)
    {
    case 'a':
    case'A':
        if (c_calls > t_calls && c_calls > g_calls)
            return 'c';
        return t_calls > g_calls ? 't' : 'g';
    case 'c':
    case'C':
        if (a_calls > t_calls && a_calls > g_calls)
            return 'a';
        return t_calls > g_calls ? 't' : 'g';
    case 'g':
    case'G':
        if (c_calls > t_calls && c_calls > a_calls)
            return 'c';
        return t_calls > a_calls ? 't' : 'a';
    case 't':
    case 'T':
        if (c_calls > a_calls && c_calls > g_calls)
            return 'c';
        return a_calls > g_calls ? 'a' : 'g';
    default:
        return '-';
    }
}

long double LocusInfo::getPValue(PValues::Method method) const
{
    return pValues[method];
}

void LocusInfo::populate()
{
    if (coverage)
    {
        ave_phred = total_phred/coverage;
    }

    for (int m = PValues::FisherExact ; m != PValues::NumOfMethods ; m++)
    //for ( unsigned int i = 0 ; i < methods.size() ; i++ )
    {
        //pValues.push_back(PValues::pValue(static_cast<PValues::Method>(method),
        PValues::Method method = static_cast<PValues::Method>(m);
        if (methods.find(method) != methods.end())
        {
            pValues.push_back(PValues::pValue(method,
                                             coverage,
                                             countBestEx(),
                                             ave_phred,
                                             phredBestEx(),
                                             freqPartition));
        }
        else
        {
            pValues.push_back(-1);
        }
    }
}

int LocusInfo::getCoverage() const
{
    return coverage;
}

int LocusInfo::getAvePhred() const
{
    return ave_phred;
}

char LocusInfo::getBase() const
{
    return base;
}










































