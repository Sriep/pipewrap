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

using namespace std;
using namespace BamTools;

class LocusInfo;

class VarientCaller
{
public:
    VarientCaller(const string& in_file,
                  const string& t_filename,                  
                  int ethreshold = 100,
                  ostream& read_stream = cout,
                  ostream& loci_steam = cout);
    ~VarientCaller();
    void Init();
    void writeReadInfo();
    void writeLociInfo();

private:
    void filterReads();
    void basesFromFasta();
    char visBase(char bamChar);
    void populateLociInfo();
    double pBionomial(unsigned int N,unsigned int K,unsigned int aphred);
    double pPoisson(unsigned int N, unsigned int K, unsigned aphred);
    double pBionomialPoisson(int t_pos, int N, int K);

    BamReader bam_reader;
    string t;
    vector<unique_ptr<LocusInfo>> als_info;
    unordered_set<string> invalid;

    unsigned int error_threshold;
    ostream& rout;
    ostream& lout;
};

#endif // VARIENTCALLER_H
