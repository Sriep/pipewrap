#include <algorithm>
#include "pvalues.h"
#include "main.h"

PValues::PValues()
{
}

///
/// Proabablity of getting more than or equal to K errors of
/// probabilty with phard Q across N reads.
long double PValues::pValue(Method method,
                   unsigned int N,
                   unsigned int K,
                   unsigned int Q)
{
    if (K == 0) return 1.0;
    switch (method)
    {
    case FisherExact:
        return pFisher(N,K,Q);
    case Bionomial:
        return pBionomial(N,K,Q);
    case Poisson:
        return pPoisson(N,K,Q);
    default:
        return 0.0;
    }
}


long double PValues::pBionomial(unsigned int N,
                                unsigned int K, unsigned int ave_phred)
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

long double PValues::pPoisson(unsigned int N,
                              unsigned int K, unsigned int ave_phred)
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

long double PValues::pFisher(unsigned int N,
                             unsigned int K, unsigned int ave_phred)
{
    //long double avQ = (long double) ave_phred;
    //long double p = pow(10,-avQ/10.0);
    //long int pN = 1;
    long int OmpN = pow(10, ((long double) ave_phred)/10.0);    
    long double pv_log = log_sfac(K,K+1)
                        + log_sfac(N-K, N)
                        - log_sfac(N+OmpN-K, N+OmpN+1);
    return pow((long double)10.0, pv_log);
}



























