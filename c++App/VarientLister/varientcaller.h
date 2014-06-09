#ifndef VARIENTCALLER_H
#define VARIENTCALLER_H
#include <ostream>
#include <fstream>
#include <string>
#include <vector>
#include <memory>
#include <unordered_set>
#include <api/BamReader.h>
#include <api/BamAlignment.h>
#include "pvalues.h"
#include "frequencypartition.h"

using namespace std;
using namespace BamTools;

class LocusInfo;

class VarientCaller
{
public:
    VarientCaller(const string& bamInfile,
                  const string& tepInile,
                  const string& freqPartFile,
                  const string& readOutfile,
                  const string& lociOutfile,
                  const string& fisherFilename,
                  const string& bionomialFilename,
                  const string& poissonFilename,
                  const string& poissonBinomialFilename,
                  const string &knownFrequencyFilename,
                  int errorThreshold);
    ~VarientCaller();
    void write();

private:
    void writeReadInfo();
    void writeLociInfo();
    void write(PValues::Method method);

    void Init();
    void filterReads();
    void basesFromFasta();
    char visBase(char bamChar);
    void populateLociInfo();
    char baseToCompare(BamAlignment al, int base);

    BamReader bam_reader;
    string t;
    string readOutfile;
    string lociOutfile;
    FrequencyPartition freqPartition;
    vector<string> pvMethodsFilename;

    unsigned int errorThreshold;
    long long totalBaseReads = 0;
    long long totalReadVareints = 0;
    long double averagePhred = 0.0;

    vector<unique_ptr<LocusInfo>> als_info;
    unordered_set<string> invalid;

};

#endif // VARIENTCALLER_H
