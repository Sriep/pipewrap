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

using namespace std;
using namespace BamTools;

class LocusInfo;

class VarientCaller
{
public:
    VarientCaller(const string& bamInfile,
                  const string& tepInile,
                  const string& readOutfile,
                  const string& lociOutfile,
                  const string& fisherFilename,
                  const string& bionomialFilename,
                  const string& poissonFilename,
                  const string& poissonBinomialFilename,
                  int errorThreshold);
    ~VarientCaller();
    void write();

private:
    void writeReadInfo();
    void writeLociInfo();
    //void writeFisherPValues();
    //void writeBionomialPValues();
    //void writePoissionPValues();
   // void writePoissonBinomialPValues();
    void write(PValues::Method method);

    void Init();
    void filterReads();
    void basesFromFasta();
    char visBase(char bamChar);
    void populateLociInfo();
    double pBionomial(unsigned int N,unsigned int K,unsigned int aphred);
    double pPoisson(unsigned int N, unsigned int K, unsigned aphred);
    double pBionomialPoisson(int t_pos, int N, int K);

    BamReader bam_reader;
    string t;
    string readOutfile;
    string lociOutfile;

    vector<string> pvMethodsFilename;
    //string fisherFilename;
    //string bionomialFilename;
    //string poissonFilename;
    //string poissonBinomialFilename;

    unsigned int errorThreshold;

    vector<unique_ptr<LocusInfo>> als_info;
    unordered_set<string> invalid;
};

#endif // VARIENTCALLER_H
