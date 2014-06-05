#include <algorithm>
#include <vector>
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
                   unsigned int Q,
                   std::vector<unsigned int>& Phreds)
{
    if (K == 0) return 1.0;
    //if (K == N) return 0.0;
    switch (method)
    {
    case FisherExact:
        return pFisher(N,K,Q);
    case Bionomial:
        return pBionomial(N,K,Q);
    case Poisson:
        return pPoisson(N,K,Q);
    case PiossonBionomial:
        //return pPoissionBionomial(N,K,Phreds);
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
            logPk = - ((long double)i)*avQ/Ten
                    - p * log10(exp(One))
                    - log_fac(i);
            pvalue += pow(Ten, logPk);
            //pvalue += pow(avQ, i)*exp(-avQ)/(long double)factorial(i);
        }
        return pvalue;
    }
    else
    {
        for (unsigned int i = 0 ; i < N ; i++)
        {
            logPk = - ((long double)i)*avQ/Ten
                    - p * log10(exp((long double) 1.0))
                    - log_fac(i);
            pvalue += pow(Ten, logPk);
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
    long int OmpN = pow(Ten, ((long double) ave_phred)/Ten);
    long double pv_log = log_sfac(K,K+1)
                        + log_sfac(N-K, N)
                        - log_sfac(N+OmpN-K, N+OmpN+1);
    return pow(Ten, pv_log);
}


long double PValues::pPoissionBionomial(unsigned int N,
                                        unsigned int K,
                                        std::vector<unsigned int>& phreds)
{
    //std::vector<std::vector<long double>> lP(N, std::vector<long double>(K));
    std::vector<std::vector<long double>> lP;
    for (int n = 1 ; n<=N ; n++)
    {
        for (int k = 0 ; k <= (n<K ? n : K) ; k++)
        {
            if (n == k)
            {
                //lP.at(n)).at(k-1).appened(logAllErrors(n, phreds));
                //std::vector<long double> aa = lP[n];
                //aa.at(k-1) = logAllErrors(n, phreds);
                lP[n].at(k-1) = logAllErrors(n, phreds);
            }
            else if (0 == k)
            {
                std::vector<long double> newV;
                newV.push_back(logNoErrors(n, phreds));
                lP[n].push_back(newV);
            }
            else
            {
                long double Pnk;
                Pnk = pow(Ten, lP[n-1].at(k-1) - (long double)phard[n]/Ten)
                        + pow(Ten, lP[n-1].at(k))
                        - pow(Ten, lP[n-1].at(k) - (long double)phard[n]/Ten);
                Pnk = log10(Pnk);
                lT[n].at(k-1).appened(Pnk);
            }
        }
    }

    long double lT = lT[K].at(K);
    for (int k = K+1 ; k <= N ; k++)
    {
        long double Tnk = 0.0;
        Tnk = pow(Ten,T) + pow(Ten, lP(n-1,k-1) - (long double) phreds[n]/Ten);
        lT += log(Tnk);
    }
    return pow(ten, lT);
}

long double PValues::logAllErrors(int n, std::vector<unsigned int>& phreds)
{
    long double allErrors = 0.0;
    for (int i = 0 ; i < n ; i++)
    {
        allErrors += -(long double)phreds[i]/Ten;
    }
}

long double PValues::logNoErrors(int n, std::vector<unsigned int>& phreds)
{
    long double allErrors = 0.0;
    for (int i = 0 ; i < n ; i++)
    {
        allErrors += log10(One + pow(Ten, -(long double)phreds[i]/Ten
    }
}











































