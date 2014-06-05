#ifndef LOCUSINFO_H
#define LOCUSINFO_H
#include <vector>
#include <string>
#include "pvalues.h"

using namespace std;

class LocusInfo
{
public:
    LocusInfo(char newbase);
    void inc_calls(char base, int phred);
    void populate();

    char getBase() const;
    int getCoverage() const;
    int getAvePhred() const;
    long double getPValue(PValues::Method method) const;
    char bestbaseEx() const;
    int countBestEx() const;
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
    std::vector<unsigned int> base_qualities;
    //std::vector<char> base_calls;
    //std::vector<string> read_id;

    vector<long double> pValues;

    //std::vector<unsigned int> DeletionQV;
    //std::vector<unsigned int> InsertionQV;
    //std::vector<unsigned int> MergeQV;
    //std::vector<unsigned int> SubstitutionQV;
};

#endif // LOCUSINFO_H
