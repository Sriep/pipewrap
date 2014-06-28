#ifndef VARIENTCALLER_H
#define VARIENTCALLER_H
#include <ostream>
#include <fstream>
#include <string>
#include <vector>
#include <set>
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
                  const string& basH5file,
                  const string& numFreqPart,
                  const string& readOutfile,
                  const string& lociOutfile,
                  const string& fisherFilename,
                  const string& bionomialFilename,
                  const string& poissonFilename,
                  const string& poissonBinomialFilename,
                  const string &knownFrequencyFilename);
    ~VarientCaller();
    void write();

private:
    void writeReadInfo();
    void writeLociInfo();
    void write(PValues::Method method);
    void hdf5();

    void Init();
    void filterReads();
    void basesFromFasta();
    char visBase(char bamChar);
    void populateLociInfo();
    char baseToCompare(BamAlignment al, int base);

    FrequencyPartition freqPartition;
    BamReader bam_reader;
    string t;
    string basH5file;
    string readOutfile;
    string lociOutfile;


    map<PValues::Method, string> pvMethodsFilename;
    set<PValues::Method> methods;

    //unsigned int errorThreshold;
    long long totalBaseReads = 0;
    long long totalReadVareints = 0;
    long double averagePhred = 0.0;
    int windowSize = 20;

    vector<unique_ptr<LocusInfo>> als_info;
    unordered_set<string> invalid;
};

#endif // VARIENTCALLER_H
