#include <string>
#include <iostream>
#include <cctype>
#include <sstream>
#include <iomanip>
#include  <algorithm>
#include "chrono"
#include "ctime"
//#include <qdebug.h>
#include "H5Cpp.h"
#include "varientcaller.h"
#include "main.h"
#include "locusinfo.h"
#include "matchmismatches.h"
#include "hdf5basfile.h"
#include "options.h"
#include "vcfwriter.h"

using namespace H5;

VarientCaller::VarientCaller()
    : freqPartition("", Options::get(Options::NumFrequencyPartitions))
      //,basH5file(Options::get(Options::BaxH5File))
      ,readOutfile(Options::get(Options::ReadOutFile))
      ,lociOutfile(Options::get(Options::LociOutFile))
      ,vcfOutfile(Options::get(Options::VcfOutput))
      ,insThreshold(stoul(Options::get(Options::FilterInsThreshold)))
      ,delThreshold(stoul(Options::get(Options::FilterDelsThreshold)))
      ,subsThreshold(stoul(Options::get(Options::FilterSubsThreshold)))
      ,preQualThreshold(stoul(Options::get(Options::PreFilterQuality)))
      ,postQualThrehold(stoul(Options::get(Options::PostFilterQuality)))
      ,pVThreshold(stoul(Options::get(Options::PValueThreshold)))
{    
    pvMethodsFilename.clear();
    //if (fisherFilename.size() > 0)
    //{
    //    pvMethodsFilename[PValues::FisherExact] = fisherFilename;
    //    methods.insert(PValues::FisherExact);
    //}
    if (Options::get(Options::BionomialFile).size() > 0)
    {
        pvMethodsFilename[PValues::Bionomial]
                = Options::get(Options::BionomialFile);
        methods.insert(PValues::Bionomial);
    }
    if (Options::get(Options::PoissonFile).size() > 0)
    {
        pvMethodsFilename[PValues::Poisson]
                = Options::get(Options::PoissonFile);
        methods.insert(PValues::Poisson);
    }
    //if (poissonBinomialFilename.size() > 0)
    //{
    //    pvMethodsFilename[PValues::PiossonBionomial] = poissonBinomialFilename;
    //    methods.insert(PValues::PiossonBionomial);
    //}
    if (Options::get(Options::KfFile).size() > 0)
    {
        pvMethodsFilename[PValues::KnownFrequency]
                = Options::get(Options::KfFile);
        methods.insert(PValues::KnownFrequency);
    }
    bam_reader.Open(Options::get(Options::InBamFile));
    t = getFileContents(Options::get(Options::TemplateFile).c_str());
    basesFromFasta();
    init();
}

VarientCaller::~VarientCaller()
{
}

void VarientCaller::operator()()
{
    write();
}

void VarientCaller::init()
{
    for (unsigned int i = 0 ; i < t.length() ; i++ )
    {
        als_info.push_back(unique_ptr<LocusInfo>(
                                new LocusInfo(t[i], &freqPartition, methods)));
    }
    filterReads();
    populateLociInfo();
    calculatePvalues();

}

void VarientCaller::filterReads()
{
    freqPartition.setParmeters();
}

void VarientCaller::basesFromFasta()
{
    if ('>' == t[0])
        t.erase(0, t.find_first_of('\n'));

    //http://en.cppreference.com/w/cpp/algorithm/remove
    t.erase(std::remove_if(t.begin(),
                              t.end(),
                              [](char x){return std::isspace(x);}),
               t.end());
}

char VarientCaller::visBase(char bamChar)
{
    if (bamChar == 'a' || bamChar == 'A' || bamChar == 'c' || bamChar == 'C'
            || bamChar == 't' || bamChar == 'T' || bamChar == 'g'
            || bamChar == 'G' || bamChar == 'u' || bamChar == 'U')
    {
        return bamChar;
    }
    return '-';
}

bool VarientCaller::goodEnoughRead(char phred)
{
    return phred-33 >= preQualThreshold;
}

bool VarientCaller::goodEnoughRead(unsigned short position
                                    , const BamAlignment& al)
{
   return al.Qualities[position] - 33 >= preQualThreshold;
}

bool VarientCaller::trimEndsFactor(int alPos, int mBase, int tSize)
{
    return alPos + mBase < trimEnds
            || alPos + mBase > tSize * numTRepeats - trimEnds;
}


void VarientCaller::populateLociInfo()
{
    bam_reader.Rewind();
    BamAlignment al;
    cout << "Untangling reads.\n";
    int readCount = 0;
    while(bam_reader.GetNextAlignment(al))
    {
        // Position = 0 means read has not been mapped succesfully
        if (0 < al.Position)
        {
            MatchMismatches tMatch(al.QueryBases, al.CigarData);
            for (unsigned int mBase = 0 ; mBase < tMatch.size() ; mBase++)
            {
                const unsigned int locus = (al.Position+ mBase) % t.size();
                const unsigned int alBase = mBase + tMatch.offset(mBase);
                if (goodEnoughRead(alBase, al))
                {
                    if (!basesDiffer(tMatch[mBase], t[locus]))
                    {
                        //int phard = al.Qualities[alBase]-33;
                        //als_info[locus]->inc_calls(al.QueryBases[alBase],phard);
                        als_info[locus]->inc_calls(
                                    al.QueryBases[alBase],al.MapQuality);
                    }
                    else if (tMatch[mBase] != '-')
                    {
                        int phard;
                        phard = al.Qualities[alBase]-33;
                        if (phard > postQualThrehold)
                            //als_info[locus]->inc_calls( al.QueryBases[alBase],phard);
                            als_info[locus]->inc_calls(
                                       al.QueryBases[alBase],al.MapQuality);
                    }
                }
            }
        }
        if (0 == (++readCount % 10000))
            cout << readCount << " reads processed\n";
    }
    cout << "Total reads" << readCount << "\n";
}


void VarientCaller::calculatePvalues()
{
    unsigned int nextOut = t.length() /10;
    unsigned int pct = 0;
    chrono::time_point<chrono::system_clock> start;
    start = chrono::system_clock::now();
    cout << "Calculating pvalues.\n";

    for (unsigned int i = 0 ; i < t.length() ; i++ )
    {
        als_info[i]->populate();

        if (i == nextOut)
        {
            chrono::time_point<chrono::system_clock> current;
            current = chrono::system_clock::now();
            chrono::duration<double> elapsed = current - start;
            nextOut += t.length()/10;
            pct += 10;
            cout << pct << "% done. Time ellapsed "
                 << elapsed.count() << " seconds\n";
        }
    }
    cout << "Wrinting data to file.\n";
}

void VarientCaller::write()
{
    if (!readOutfile.empty()) writeReadInfo();
    if (!lociOutfile.empty()) writeLociInfo();
    if (!vcfOutfile.empty()) writeVcfFile();

    for (int m = PValues::FisherExact;
             m != PValues::NumOfMethods;
             m++)
    {
        PValues::Method method = static_cast<PValues::Method>(m);
        if (methods.find(method) != methods.end())
            write(method);
    }
}

void VarientCaller::writeReadInfo()
{
    bam_reader.Rewind();
    BamAlignment al;
    ofstream rout;
    rout.open(readOutfile, ios_base::out | ios_base::trunc);
    rout << "Read\tPosRead\tBaseRead\tPosTemp\tBaseTemp\tPhred"
        << "\tAveQuality\tCoverage\tpvBionomial\tpvPoissionan\tFisher\n";
    while(bam_reader.GetNextAlignment(al))
    {
        if (!invalid.count(al.Name))
        {
            for (int base = 0 ; base < al.Length ; base++)
            {
                uint locus =(base + al.Position) % t.size();
                //if (al.AlignedBases[base] != t[locus])
                if (basesDiffer(al.AlignedBases[base], t[locus]))
                //if (!compareBases(al.AlignedBases[base], t[locus]))
                {
                    rout << "\"" << al.Name
                        << "\"\t\"" << base +1
                        << "\"\t\"" << visBase(al.AlignedBases[base])
                        << "\"\t\"" << (base + al.Position +1)% t.size()
                        << "\"\t\"" << visBase(t[locus])
                        << "\"\t\"" << al.Qualities[base]-33;
                    if (locus  < t.length())// && locus >= 0)
                    {
                        rout << "\"\t\"" << als_info[locus]->getAvePhred()
                            << "\"\t\"" << als_info[locus]->getCoverage();
                            //<< "\"\t\"" << setprecision(64)
                            //<< als_info[locus]->getPValue(PValues::Bionomial)
                            //<< "\"\t\"" << setprecision(64)
                           //<< als_info[locus]->getPValue(PValues::Poisson);
                            //<< "\"\t\"" << setprecision(64)
                            //<< als_info[locus]->getPValue(PValues::FisherExact);
                    }
                    else
                    {
                        rout << "\"\t\"0\"\t\"0";
                        //rout << "\"\t\"0\"\t\"0";
                        //rout << "\"\t\"0";
                    }
                    rout  << "\"\n";
                }
            }
        }
     }
}

void VarientCaller::writeLociInfo()
{
    ofstream lout;
    lout.open(lociOutfile, ios_base::out | ios_base::trunc);
    lout << "Locus\tBase\tCoverage\tAveQuality\tpSNP\tpSNPCount\tpSNP%";
    if (methods.find(PValues::Bionomial) != methods.end())
        lout <<  "\tpvBionomial";
    if (methods.find(PValues::Poisson) != methods.end())
        lout << "\tpvPoissionan";
    if (methods.find(PValues::KnownFrequency) != methods.end())
        lout << "\tpvKF";
    lout << "\n";

    for (unsigned int locus = 0 ; locus < t.length() ; locus++)
    {
        int pct;
        if (als_info[locus]->countBestEx() != 0)
            pct = 100*als_info[locus]->countBestEx()/als_info[locus]->getCoverage();
        else pct = 0;

        lout    << "\"" << locus+1
                << "\"\t\"" << visBase(t[locus])
                << "\"\t\"" << als_info[locus]->getCoverage()
                << "\"\t\"" << als_info[locus]->getAvePhred()
                << "\"\t\"" << als_info[locus]->bestbaseEx()
                << "\"\t\"" << als_info[locus]->countBestEx()
                << "\"\t\"" << pct;
        if (methods.find(PValues::Bionomial) != methods.end())
            lout << "\"\t\"" << setprecision(64)
                 << als_info[locus]->getPValue(PValues::Bionomial);
        if (methods.find(PValues::Poisson) != methods.end())
            lout << "\"\t\"" << setprecision(64)
                 << als_info[locus]->getPValue(PValues::Poisson);
        if (methods.find(PValues::KnownFrequency) != methods.end())
            lout << "\"\t\"" << setprecision(64)
                 << als_info[locus]->getPValue(PValues::KnownFrequency);
        lout << "\"\n";
    }
}

void VarientCaller::writeVcfFile()
{
    if (pVThreshold > 0) pVThreshold = -1*pVThreshold;
    VcfWriter vcfw(Options::get(Options::VcfOutput)
                   , als_info
                   , Options::commandLine
                   , t
                   , Options::get(Options::TemplateFile)
                   , pVThreshold
                   , PValues::Method::KnownFrequency);
    vcfw();
}

void VarientCaller::write(PValues::Method method)
{
    ofstream pvout;
    pvout.open(pvMethodsFilename[method] + ".pvalues.csv",
               ios_base::out | ios_base::trunc);
    for (unsigned int locus = 0 ; locus < t.length() ; locus++)
    {
        pvout << setprecision(64) << als_info[locus]->getPValue(method) << "\n";
    }
}
































