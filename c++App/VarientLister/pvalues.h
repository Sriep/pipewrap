#ifndef PVALUES_H
#define PVALUES_H
#include <vector>

const long double Ten = 10.0;
const long double One = 1.0;

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
        KnownFrequency,
        NumOfMethods
    };
    static long double pValue(Method method,
                       unsigned int N,
                       unsigned int K,
                       unsigned int Q,
                       std::vector<unsigned int>& Phreds);

    static long double pBionomial(unsigned int N, unsigned int K,
                           unsigned int ave_phred);
    static long double pPoisson(unsigned int N, unsigned int K,
                         unsigned int ave_phred);
    static long double pFisher(unsigned int N, unsigned int K,
                        unsigned int ave_phred);
    static long double pPoissionBionomial(unsigned int N,
                                          unsigned int K,
                                          std::vector<unsigned int>& phreds);
    static long double pKnownFrequency(unsigned int N,
                                       unsigned int K,
                                       std::vector<unsigned int>& phreds);
    private:
        static long double logAllErrors(int n,
                                        std::vector<unsigned int>& phreds);
        static long double logNoErrors(int n,
                                       std::vector<unsigned int>& phreds);
};

#endif // PVALUES_H
