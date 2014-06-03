#include <algorithm>
#include "locusalingmentinfo.h"
#include "main.h"

LocusInfo::LocusInfo(char newbase)
    : base(newbase)
{
    base = newbase;
}

void LocusInfo::inc_calls(char new_base, int phred)
{
    coverage++;
    total_phred += phred;
    if (ave_phred) ave_phred = 0;
    switch (new_base)
    {
    case 'a':
    case'A':
        a_calls++;
        break;
    case 'c':
    case'C':
        c_calls++;
        break;
    case 'g':
    case'G':
        g_calls++;
        break;
    case 't':
    case 'T':
        t_calls++;
        break;
    default:
        bad_calls++;
        base = '-';
    }
}

int LocusInfo::countBestEx()
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

char LocusInfo::bestbaseEx()
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
long double LocusInfo::getPVFisher() const
{
    return p_value_fisher;
}

void LocusInfo::populate()
{
    if (coverage)
    {
        ave_phred = total_phred/coverage;
    }
    if (countBestEx()>0)
    {
        p_value_bionomial = pBionomial(coverage, countBestEx(), ave_phred);
        p_value_poission = pPoisson(coverage, countBestEx(), ave_phred);
        p_value_fisher = pFisher(coverage, countBestEx(), ave_phred);
        p_value_b_poisson = 0;
    }
    else
    {
        p_value_bionomial = 1;
        p_value_poission = 1;
        p_value_fisher = 1;
        p_value_b_poisson = 1;
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
long double LocusInfo::getPVBionomial() const
{
    return  p_value_bionomial;
}
long double LocusInfo::getPVPoission() const
{
    return p_value_poission;
}
long double LocusInfo::getPVBionomialPiossion() const
{
    return p_value_b_poisson;
}

long double pBionomial(unsigned int N, unsigned int K, unsigned int ave_phred)
{
    long double avQ = (long double) ave_phred;
    long double p = pow(10.0,-avQ/10.0);
    long double pvalue = 0.0;
    long double logPk;
    if (K >= N/2)
    {
        for (unsigned int i = K ; i <= N ; i++)
        {
            logPk = log_n_C_r(N,i) - i*avQ/10.0 + (N-i)*log10((long double)1-p);
            pvalue += pow((long double)10,logPk);
            //pvalue += n_C_r(N,i) * pow((long double)10.0,-i*avQ/10.0) * pow((long double)1-p,N-i);

        }        
        return pvalue;
    }
    else
    {

        for (unsigned int i = 0 ; i < K ; i++)
        {
            logPk = log_n_C_r(N,i) - i*avQ/10 + (N-i)*log10((long double)1-p);
            pvalue += pow((long double)10,logPk);
            //pvalue += n_C_r(N,i) * pow(10,-i*avQ/10.0) * pow(1-p,N-i);
        }
        return 1-pvalue;
    }
}

long double pPoisson(unsigned int N, unsigned int K, unsigned int ave_phred)
{
    long double avQ = (long double) ave_phred;
    long double p = pow(10,-avQ/10.0);
    long double pvalue = 0.0;
    long double logPk;
    if (K>=N/2)
    {
        for (unsigned int i = K ; i <= N ; i++)
        {
            logPk = - ((long double)i)*avQ/10.0
                    - p * log10(exp((long double) 1.0))
                    - log_fac(i);
            pvalue += pow((long double)10.0, logPk);
            //pvalue += pow(avQ, i)*exp(-avQ)/(long double)factorial(i);
        }
        return pvalue;
    }
    else
    {
        for (unsigned int i = 0 ; i < N ; i++)
        {
            logPk = - ((long double)i)*avQ/10.0
                    - p * log10(exp((long double) 1.0))
                    - log_fac(i);
            pvalue += pow((long double)10.0, logPk);
            //pvalue += pow(avQ, i)*exp(-avQ)/(long double)factorial(i);
        }
        return 1-pvalue;
    }
}

long double pFisher(unsigned int N, unsigned int K, unsigned int ave_phred)
{
    //long double avQ = (long double) ave_phred;
    //long double p = pow(10,-avQ/10.0);
    //long int pN = 1;
    long int OmpN = pow(10, ((long double) ave_phred)/10.0);
    //long int a = (1-p)/p;
    //long double pv_log = log_fac(pN+K) + log_fac(OmpN+N-K) + log_fac(N)
    //            - log_fac(pN) - log_fac(K) - log_fac(OmpN) - log_fac(N-K);
    //long double pv_log = log_n_C_r(pN+K, pN) + log_n_C_r(OmpN+N-K, OmpN)
    //                                - log_n_C_r(2*N, OmpN*N);
    //ong double pv_log = slog_fac();
    long double pv_log = log((long double) K)
                            + log_sfac(N-K, N)
                            - log_sfac(N+OmpN-K, N+OmpN+1);

    return pow((long double)10.0, pv_log);
}







































