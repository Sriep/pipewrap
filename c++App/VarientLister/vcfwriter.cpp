#include <ctime>
#include <chrono>
#include <iomanip>
#include "vcfwriter.h"
#include <iostream>
#include <iomanip>


VcfWriter::VcfWriter(const string &VcfOutFile
                     , const vector<unique_ptr<LocusInfo> > &als_info
                     , const string &source
                     , const string& reference
                     , const string &referenceFile)
    : VcfOutFile(VcfOutFile)
    , als_info(als_info)
    , vSource(source)
    , ref(reference)
    , refFilename(referenceFile)
{
}

void VcfWriter::operator()()
{
    vout.open(VcfOutFile, ios_base::out | ios_base::trunc);
    WriteMetaLine(fileFormat, Vcf40);
    WriteMetaLine(fileDate, __DATE__);//std::put_time(std::localtime(&now_c), "%F %T"));
    WriteMetaLine(source, vSource);
    WriteMetaLine(reference, refFilename);
    WriteMetaInfo("DP", "1", "Integer", "Raw Depth","","");
    WriteMetaInfo("SB", "1", "Float", "Allele Frequency","","");
    WriteDataHeader();

    for (unsigned int pos = 0 ; pos < ref.length() ; pos++)
    {

    }
}

void VcfWriter::WriteMetaLine(const string &left, const string &right)
{
    vout << "##" << left << "=" << right << "\"\n";
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
    vout << ">";
}

void  VcfWriter::WriteDataHeader()
{
    vout << "#CHROM" << "\"\t\"" << "POS" << "\"\t\"" << "ID" << "\"\t\""
         << "REF"   << "\"\t\""  << "ALT" << "\"\t\"" << "QUAL" << "\"\t\""
         << "FILTER" << "\"\t\"" "INFO";
    vout <<  "\"\n";
}

