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
    VarientCaller();
    ~VarientCaller();
    void operator() ();
protected:
    bool goodEnoughRead(unsigned short position, const BamAlignment& al);
    virtual void populateLociInfo();
private:
    void write();
    void writeReadInfo();
    void writeLociInfo();
    void writeVcfFile();
    void write(PValues::Method method);
    bool goodEnoughRead(char phred);

    void calculatePvalues();

    void init();
    void filterReads();
    void basesFromFasta();
    char visBase(char bamChar);    
    char baseToCompare(BamAlignment al, int base);

    FrequencyPartition freqPartition;

protected: //should sort this out, make privazte somehow.
    BamReader bam_reader;
    string t;
    string readOutfile;
    string lociOutfile;
    string vcfOutfile;

    map<PValues::Method, string> pvMethodsFilename;
    set<PValues::Method> methods;

    //unsigned int errorThreshold;
    long long totalBaseReads = 0;
    long long totalReadVareints = 0;
    long double averagePhred = 0.0;   

    unsigned int windowSize = 20;
    unsigned char insThreshold = 5;
    unsigned char delThreshold = 0;
    unsigned char subsThreshold = 0;
    unsigned char preQualThreshold = 10;
    unsigned char postQualThrehold = 10;
    int pVThreshold = 0;

    bool trimEndsFactor(int alPos, int mBase, int tSize);
    int trimEnds = 300;
    const int numTRepeats = 2;

    vector<unique_ptr<LocusInfo>> als_info;
    unordered_set<string> invalid;
};

#endif // VARIENTCALLER_H
