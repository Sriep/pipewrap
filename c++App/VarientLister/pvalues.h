#ifndef PVALUES_H
#define PVALUES_H

class PValues
{
public:
    PValues();

    enum Method
    {
        FisherExact = 0,
        Bionomial,
        Poisson,
        PiossonBionomial,
        NumOfMethods
    };
    static long double pValue(Method method,
                       unsigned int N,
                       unsigned int K,
                       unsigned int Q);

    static long double pBionomial(unsigned int N, unsigned int K,
                           unsigned int ave_phred);
    static long double pPoisson(unsigned int N, unsigned int K,
                         unsigned int ave_phred);
    static long double pFisher(unsigned int N, unsigned int K,
                        unsigned int ave_phred);

};

#endif // PVALUES_H
