#ifndef LOCUSINFO_H
#define LOCUSINFO_H
#include <vector>
#include <string>
#include <set>
#include "pvalues.h"

using namespace std;
class FrequencyPartition;

class LocusInfo
{
public:
    LocusInfo(char newbase,
              FrequencyPartition* freqPartition,
              set<PValues::Method> methods);
    void inc_calls(char base, int phred);
    void populate();

    char getBase() const;
    int getCoverage() const;
    int getAvePhred() const;
    long double getPValue(PValues::Method method) const;
    char bestbaseEx() const;
    int countBestEx() const;
    std::vector<unsigned int> phredBestEx() const;
private:
    char base;
    int coverage = 0;
    int ave_phred = 0;
    int total_phred = 0;
    int a_calls = 0;
    int c_calls = 0;
    int g_calls = 0;
    int t_calls = 0;
    int bad_calls = 0;
    std::vector<unsigned int> aPhred;
    std::vector<unsigned int> tPhred;
    std::vector<unsigned int> gPhred;
    std::vector<unsigned int> cPhred;
    //std::vector<char> base_calls;
    //std::vector<string> read_id;

    vector<long double> pValues;
    FrequencyPartition* freqPartition;
    set<PValues::Method> methods;
    //std::vector<unsigned int> DeletionQV;
    //std::vector<unsigned int> InsertionQV;
    //std::vector<unsigned int> MergeQV;
    //std::vector<unsigned int> SubstitutionQV;
};

#endif // LOCUSINFO_H
