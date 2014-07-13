#include <algorithm>
#include <vector>
#include <QDebug>
#include <QProcess>
#include  <cstdlib>
#include "pvalues.h"
#include "main.h"
#include "frequencypartition.h"

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
                   std::vector<unsigned int> Phreds,
                   FrequencyPartition* freqPartition)
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
        return pPoissionBionomial(N,K,Phreds);
    case KnownFrequency:
        return pKnownFrequency(N,K,Phreds,freqPartition);
    default:
        return 0.0;
    }
}


long double PValues::pBionomial(unsigned int N,
                                unsigned int K, unsigned int ave_phred)
{
    //return 0.0;
    long double avQ = (long double) ave_phred;
    long double p = pow(Ten,-avQ/Ten);
    long double pvalue = Zero;
    long double logPk;
    //if (K >= N/2)
    {
        for ( unsigned int i = K ; i <= N ; i++)
        {
            logPk = log_n_C_r(N,i) - i*avQ/Ten + (N-i)*log10(One-p);

            long double p10 = pow(Ten,logPk);
            pvalue += p10;//pow(Ten,logPk);            
        }
        return pvalue;
    }
    /*else
    {

        for ( int i = 0 ; i < K ; i++)
        {
            logPk = log_n_C_r(N,i) - i*avQ/Ten + (N-i)*log10(One-p);

            long double p10 = pow(Ten,logPk);
            pvalue += p10;//pow(Ten,logPk);
            //pvalue += n_C_r(N,i) * pow(10,-i*avQ/10.0) * pow(1-p,N-i);
        }
        long double rtv = One-pvalue;
        return rtv;
    }*/
}

long double PValues::pPoisson(unsigned int N,
                              unsigned int K,
                              unsigned int ave_phred)
{
    long double avQ = (long double) ave_phred;
    long double p = pow(Ten,-avQ/Ten);
    long double pvalue = Zero;
    long double logPk;
    long double p10;
    //vector<long double> dpois;
    //if (K>=N/2)
    {
        for (unsigned int i = K ; i <= N ; i++)
        //for (int i = N ; i >= K ; i--)
        {
            logPk = - ((long double)i)*avQ/Ten
                    - p * log10(exp(One))
                    - log_fac(i);
            p10 = pow(Ten, logPk);
            //dpois.push_back(pow(Ten, logPk));
            pvalue += p10;//pow(Ten, logPk);
            //pvalue += pow(avQ, i)*exp(-avQ)/(long double)factorial(i);
        }
        //pvalue = kahanSum(dpois);
        return pvalue;
    }
    /*else
    {
        //for (unsigned int i = 0 ; i < K ; i++)
        for (int i = K-1 ; i >= 0 ; i--)
        {
            logPk = -((long double)i)*avQ/Ten - p*log10(exp(One)) - log_fac(i);
            //pvalue = static_cast<long double>(pvalue + pow(Ten, logPk));
            p10 = pow(Ten, logPk);// * pow(Ten, (long double) 100);
            //dpois.push_back(pow(Ten, logPk));
            pvalue += (long double)(p10);
            //pvalue = ( double) ((( double) (pvalue)) + (( double) (pow(Ten, logPk))));
            //pvalue += pow(Ten, logPk);
        }
        //pvalue = kahanSum(dpois);
        //pvalue = pvalue / pow(Ten, (long double) 100);
        long double rtv = One-pvalue;
        return rtv;
    }*/
}

long double PValues::pFisher(unsigned int N,
                             unsigned int K, unsigned int ave_phred)
{
    //long double avQ = (long double) ave_phred;
    //long double p = pow(10,-avQ/10.0);
    //long int pN = 1;
    return 0.0;
    long int OmpN = pow(Ten, ((long double) ave_phred)/Ten);
    long double pv_log = log_sfac(K,K+1)
                        + log_sfac(N-K, N)
                        - log_sfac(N+OmpN-K, N+OmpN+1);
    long double rtv = pow(Ten, pv_log);
#ifdef QT_DEBUG
    //Rscript fisherExact.R
    /*QString headSh2 =    "echo " +  QVariant((double)rtv).toString()
                        + " <- Me R ->"
                        + "Rscript fisherExact.R -N " + QVariant(N).toString()
                        + " -K " + QVariant(K).toString()
                        + " -Q " + QVariant(ave_phred).toString() + "\n";
    string fisherShD =  "Rscript fisherExact.R -N " + QVariant(N).toString()
                        + " -K " + QVariant(K).toString()
                        + " -Q " + QVariant(ave_phred).toString() + "\n";
    string fisherSh =  "Rscript fisherExact.R -N " + QVariant(N).toString()
                        + " -K " + QVariant(K).toString()
                        + " -Q " + QVariant(ave_phred).toString()
                        + "| cat >> pFisherExactR.pvalues.csv\n";

    string headSh =    "echo " +  to_string((long double)rtv)  + "  Me R "                       + "Rscript fisherExact.R -N " + to_string(N)
                        + " -K " + to_string(K)
                        + " -Q " + to_string(ave_phred) + "\n";
    string fisherShD =  "Rscript fisherExact.R -N " + to_string(N)
                        + " -K " + to_string(K)
                        + " -Q " + to_string(ave_phred) + "\n";*/

    //string fisherSh =  "Rscript fisherExact.R -N " + to_string(N)
    //                    + " -K " + to_string(K)
    //                    + " -Q " + to_string(ave_phred)
    //                    + "| cat >> pFisherExactR.pvalues.csv\n";
    //QProcess process;
    //process.execute(headSh);
   // process.execute(fisherShD);
    //process.execute(fisherSh);

    //std::system(string(headSh2.toStdString()).c_str());
    //std::system(fisherShD.c_str());
    //std::system(fisherSh.c_str());
#endif
    return rtv;
}


long double PValues::pPoissionBionomial(unsigned int N,
                                        unsigned int K,
                                        std::vector<unsigned int>& phreds)
{
    return 0.0;
    std::vector<std::vector<long double>> lP;
    for (unsigned int n = 1 ; n<=N ; n++)
    {
        for (unsigned int k = 0 ; k <= (n<K ? n : K) ; k++)
        {
            if (n == k)
            {
                lP[n].at(k-1) = logAllErrors(n, phreds);
            }
            else if (0 == k)
            {
                std::vector<long double> newV;
                newV.push_back(logNoErrors(n, phreds));
                lP.push_back(newV);
            }
            else
            {
                long double Pnk;
                Pnk = pow(Ten, lP[n-1].at(k-1) - (long double)phreds[n]/Ten)
                        + pow(Ten, lP[n-1].at(k))
                        - pow(Ten, lP[n-1].at(k) - (long double)phreds[n]/Ten);
                Pnk = log10(Pnk);
                lP[n].push_back(Pnk);
            }
        }
    }

    long double lT = lP[K].at(K);
    for (unsigned int k = K+1 ; k <= N ; k++)
    {
        long double Tnk = 0.0;
        Tnk = pow(Ten,lT) + pow(Ten, lP[k-1].at(K-1)-(long double) phreds[k]/Ten);
        lT += log(Tnk);
    }
    return pow(Ten, lT);
}

long double PValues::pKnownFrequency(unsigned int N,
                                   unsigned int K,
                                   std::vector<unsigned int>& phreds,
                                   FrequencyPartition *P)
{
    return P->getPartionFreq(MatchPartition, N, phreds);
}

long double PValues::logAllErrors(int n, std::vector<unsigned int>& phreds)
{
    long double allErrors = 0.0;
    for (int i = 0 ; i < n ; i++)
    {
        allErrors += -(long double)phreds[i]/Ten;
    }
    return allErrors;
}

long double PValues::logNoErrors(int n, std::vector<unsigned int>& phreds)
{
    long double noErrros = 0.0;
    for (int i = 0 ; i < n ; i++)
    {
        noErrros += log10(One + pow(Ten, -(long double)phreds[i]/Ten));
    }
    return noErrros;
}











































