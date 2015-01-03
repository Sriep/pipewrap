#ifndef VCFWRITER_H
#define VCFWRITER_H
#include <string>
#include <vector>
#include <memory>
#include <fstream>
#include "pvalues.h"

using namespace std;
class LocusInfo;

class VcfWriter
{
public:

    const string fileFormat = "fileformat";
    const string fileDate = "filedate";
    const string source = "source";
    const string reference = "reference";
    const string contig = "contig";
    const string phasing = "phasing";

    const string Vcf40 = "VCFv4.0";
    const string Vcf41 = "VCFv4.1";
    const string Vcf42 = "VCFv4.2";
    const string Bcf21 = "BCFv2.1";

    const string typeInteger = "Integer";
    const string typeFloat = "Float";
    const string typeFlag = "Flag";
    const string typeCharacter = "Character";
    const string typeString = "String";

    VcfWriter(const string& VcfOutFile
              , const vector<unique_ptr<LocusInfo>>&als_info
              , const string& source
              , const string& ref
              , const string& refFilename
              , int pVThreshold
              , PValues::Method calMethod);
    void operator() ();
private:
    const string VcfOutFile;
    const vector<unique_ptr<LocusInfo>>& als_info;
    string vSource;
    string ref;
    string refFilename;
    int pVThreshold;
    PValues::Method calMethod;

    ofstream vout;


    void WriteMetaLine(const string& left, const string& right);
    void WriteMetaInfo(const string& id
                      ,const string& number
                      ,const string& type
                      ,const string& description
                      ,const string& source
                      ,const string& version);
    void WriteMetaFilter();
    void WriteMetaFormat();
    void WriteDataHeader();
    void WriteDataLine(uint pos);
    string date();
};

#endif // VCFWRITER_H
