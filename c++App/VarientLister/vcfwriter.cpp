#include <ctime>
#include <chrono>
#include <iomanip>
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <main.h>

#include "locusinfo.h"
#include "vcfwriter.h"

VcfWriter::~VcfWriter()
{

}

VcfWriter::VcfWriter(const string &VcfOutFile
                     , const vector<unique_ptr<LocusInfo> > &als_info
                     , const string &source
                     , const string& reference
                     , const string &referenceFile
                     , int pVThreshold
                     , PValues::Method calMethod = PValues::KnownFrequency)

    : VcfOutFile(VcfOutFile + ".vcf")
    , als_info(als_info)
    , vSource(source)
    , ref(reference)
    , refFilename(referenceFile)
    , pVThreshold(pVThreshold)
    , calMethod(calMethod)
{
}

void VcfWriter::operator()()
{
    vout.open(VcfOutFile, ios_base::out | ios_base::trunc);
    WriteMetaLine(fileFormat, Vcf40);
    WriteMetaLine(fileDate, date());
    WriteMetaLine(source, vSource);
    WriteMetaLine(reference, refFilename);
    WriteMetaInfo("DP", "1", "Integer", "Raw Depth","","");
    WriteMetaInfo("AF", "1", "Float", "Allele Frequency","","");
    WriteMetaFormat("GT", "1", "Integer", "Genotype");
    WriteMetaFormat("GQ", "1", "Integer", "Genotype Quality");
    WriteDataHeader();

    for (unsigned int pos = 0 ; pos < als_info.size() ; pos++)
    {
        long double logPValue = log10(als_info[pos]->getPValue(calMethod));
        if (logPValue < pVThreshold)
        {
            WriteDataLine(pos);
        }
    }
    if (vout.is_open())
        vout.close();
}

void VcfWriter::WriteMetaLine(const string &left, const string &right)
{
    vout << "##" << left << "=" << right << "\n";
}

void VcfWriter::WriteMetaInfo(const string& id
                             , const string& number
                             , const string& type
                             , const string& description
                             , const string& source = ""
                             , const string& version = "")
{
    vout << "##INFO<" << "ID=" << id <<  ",Number=" << number
         << ",Type=" << type << ",Description=""" << description << """";
    if (source != "")
    {
        vout << ",Source""" << source << """";
    }
    if (version != "")
    {
        vout << ",Version""" << version << """";
    }
    vout << ">" << "\n";
    hasInfo = true;
}

void VcfWriter::WriteMetaFilter(const string &id, const string &description)
{
    vout << "##FILTER<" << "ID=" << id <<  ",Description=""" << description << """";
    vout << ">" << "\n";
    hasFilter = true;
}

void VcfWriter::WriteMetaFormat(const string &id, const string &number, const string &type, const string &description)
{
    vout << "##FORMAT<" << "ID=" << id <<  ",Number=" << number
         << ",Type=" << type << ",Description=""" << description << """";
    vout << ">" << "\n";
    hasFormat = true;
}

void  VcfWriter::WriteDataHeader()
{
    vout << "#CHROM" << "\t" << "POS" << "\t" << "ID" << "\t"
         << "REF"   << "\t"  << "ALT" << "\t" << "QUAL" << "\t"
         << "FILTER";
    //if (hasFilter) vout << "\t" "FILTER";
    if (hasInfo) vout << "\t" "INFO";
    if (hasFormat) vout << "\t" "FORMAT" <<  "\t"  << "1";
    vout <<  "\n";
}

void VcfWriter::WriteDataLine(unsigned int pos)
{
    long double pValue = als_info[pos]->getPValue(PValues::Method::KnownFrequency);
    int varQual = prob2phred(pValue);
    if (varQual>100) varQual = 100;

    vout << "Sc_consensus"
         << "\t" << pos
         << "\t" << "."
         << "\t" << ref[pos]
         << "\t" << als_info[pos]->bestbaseEx()
         << "\t" << als_info[pos]->getAvePhred()
         << "\t" << ".";

    vout << "\t"
         << "DP=" << als_info[pos]->countBestEx()
         << ";AF=" << pValue;

    vout << "\t" << "GT:GQ"
         << "\t" << "0|1:" << varQual;
    vout << "\n";
}

string VcfWriter::date()
{
    time_t t = time(NULL);
    tm* timePtr = localtime(&t);
    string date = to_string(timePtr->tm_year+1900);
    if (timePtr->tm_mon < 9) date += "0";
    date += to_string(timePtr->tm_mon+1);
    if (timePtr->tm_mday < 10) date += "0";
    date += to_string(timePtr->tm_mday);

    return date;
}



























