#ifndef FREQUENCYPARTITION_H
#define FREQUENCYPARTITION_H
#include <vector>
#include <string>

using namespace std;

const int MatchPartition = 0;
class FrequencyPartition
{
public:
    FrequencyPartition(const string& filename, const string &numBins = "100");
    void setParmeters(long long tReads = 20000,
                      long long tVarients = 300);
    long double getMatchPartition(int par);
    long double getPsnpPartition(int par);
    long double getPartionFreq(int par,
                               int coverage,
                               std::vector<unsigned int>& phreds);
private:
    long double logPartionRatio(int partition,
                                int coverage,
                                std::vector<unsigned int> &phreds);
    long double logNormlisingFactor(int coverage,
                                    std::vector<unsigned int> &phreds);

    long long totalReads;
    long long totalPsnps;
    //long long totalErrors;
    string dataFile;

    vector<long double> pMatch;
    vector<int> ratioPartitions;
    unsigned int numBins;
};

#endif // FREQUENCYPARTITION_H
