#include <algorithm>
#include <cctype>
#include <qdebug.h>
#include <algorithm>
#include <string>
#include <iostream>
#include <cctype>
#include <QDebug>
#include <sstream>
#include <iomanip>
#include  <cstdlib>

#include "api/BamAlignment.h"
#include "varientcaller.h"
#include "main.h"
#include "locusinfo.h"


VarientCaller::VarientCaller(const string& bamInfile,
                             const string& tepInile,
                             const string &freqPartFile,
                             const string &numFreqPart,
                             const string &readOutfile,
                             const string &lociOutfile,
                             const string &fisherFilename,
                             const string &bionomialFilename,
                             const string &poissonFilename,
                             const string &poissonBinomialFilename,
                             const string &knownFrequencyFilename,
                             int errorThreshold)
    : freqPartition(freqPartFile, std::stoi(numFreqPart)),
      readOutfile(readOutfile),
      lociOutfile(lociOutfile),
      errorThreshold(errorThreshold)
{
    pvMethodsFilename.clear();
    if (fisherFilename.size() > 0)
    {
        pvMethodsFilename[PValues::FisherExact] = fisherFilename;
        methods.insert(PValues::FisherExact);
    }
    if (bionomialFilename.size() > 0)
    {
        pvMethodsFilename[PValues::Bionomial] = bionomialFilename;
        methods.insert(PValues::Bionomial);
    }
    if (poissonFilename.size() > 0)
    {
        pvMethodsFilename[PValues::Poisson] = poissonFilename;
        methods.insert(PValues::Poisson);
    }
    if (poissonBinomialFilename.size() > 0)
    {
        pvMethodsFilename[PValues::PiossonBionomial] = poissonBinomialFilename;
        methods.insert(PValues::PiossonBionomial);
    }
    if (knownFrequencyFilename.size() > 0)
    {
        pvMethodsFilename[PValues::KnownFrequency] = knownFrequencyFilename;
        methods.insert(PValues::KnownFrequency);
    }
    bam_reader.Open(bamInfile);
    t = getFileContents(tepInile.c_str());
    basesFromFasta();
    Init();
}

VarientCaller::~VarientCaller()
{
}

void VarientCaller::Init()
{
    for (unsigned int i = 0 ; i < t.length() ; i++ )
    {
        als_info.push_back(unique_ptr<LocusInfo>(
                                new LocusInfo(t[i], &freqPartition, methods)));
    }
    filterReads();
    populateLociInfo();
}

void VarientCaller::filterReads()
{
    bam_reader.Rewind();
    BamAlignment al;
    unsigned long long totalPhred = 0;
    while(bam_reader.GetNextAlignment(al))
    {
        unsigned int numMismatching = 0;
        unsigned int base = 0;
        unsigned int varients = 0;
        unsigned long phredsAl;
        bool tooManyMissmatches = false;
        if (0 == al.AlignedBases.length())
            tooManyMissmatches = true;
        else
        {
            while ((base < (unsigned int) al.Length))// && !tooManyMissmatches)
            {
                //if (al.AlignedBases[base] != t[base + al.Position])
                if (!compareBases(al.AlignedBases[base],
                                  t[base + al.Position]))
                {
                    numMismatching++;
                    if (((100 * numMismatching) / al.Length) > errorThreshold)
                        tooManyMissmatches = true;
                    varients++;
                }
                base++;
                phredsAl += al.Qualities[base];
            }
        }
        if (tooManyMissmatches) invalid.insert(al.Name);
        else
        {
            totalBaseReads += base;
            totalReadVareints += varients;
            totalPhred += phredsAl;
        }
        numMismatching++;
     }
    averagePhred = (long double) totalPhred / (long double) totalBaseReads;
    freqPartition.setParmeters(totalBaseReads, totalReadVareints, averagePhred);
}
void VarientCaller::write()
{
    if (!readOutfile.empty()) writeReadInfo();
    if (!lociOutfile.empty()) writeLociInfo();

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
                int locus = base + al.Position;
                //if (al.AlignedBases[base] != t[locus])
                if (!compareBases(al.AlignedBases[base], t[locus]))
                {
                    rout << "\"" << al.Name
                        << "\"\t\"" << base +1
                        << "\"\t\"" << visBase(al.AlignedBases[base])
                        << "\"\t\"" << base + al.Position +1
                        << "\"\t\"" << visBase(t[locus])
                        << "\"\t\"" << al.Qualities[base]-33;
                    if (locus  < t.length() && locus >= 0)
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
    lout << "Locus\tBase\tCoverage\tAveQuality\tpSNP\tpSNPCount\tpSNP%"
        << "\tpvBionomial\tpvPoissionan\tKF\n";

    for (unsigned int locus = 0 ; locus < t.length() ; locus++)
    {
        int pct;
        if (als_info[locus]->countBestEx() != 0)
            pct = 100*als_info[locus]->countBestEx()/als_info[locus]->getCoverage();
        else pct = 0;

        lout    << "\"" << locus
                << "\"\t\"" << visBase(t[locus])
                << "\"\t\"" << als_info[locus]->getCoverage()
                << "\"\t\"" << als_info[locus]->getAvePhred()
                << "\"\t\"" << als_info[locus]->bestbaseEx()
                << "\"\t\"" << als_info[locus]->countBestEx()
                << "\"\t\"" << pct
                << "\"\t\"" << setprecision(64)
                << als_info[locus]->getPValue(PValues::Bionomial)
                << "\"\t\"" << setprecision(64)
                << als_info[locus]->getPValue(PValues::Poisson)
                << "\"\t\"" << setprecision(64)
                << als_info[locus]->getPValue(PValues::KnownFrequency)
                << "\"\n";
    }

}

void VarientCaller::write(PValues::Method method)
{
    ofstream pvout;
    pvout.open(pvMethodsFilename[method] + ".pvalues.csv", ios_base::out | ios_base::trunc);
    for (unsigned int locus = 0 ; locus < t.length() ; locus++)
    {
        pvout << setprecision(64) << als_info[locus]->getPValue(method) << "\n";
    }
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

void VarientCaller::populateLociInfo()
{
    bam_reader.Rewind();
    BamAlignment al;
    while(bam_reader.GetNextAlignment(al))
    {
        if (!invalid.count(al.Name))
        {
            for (unsigned int base = 0 ; base < al.Length ; base++)
            {
                if (base + al.Position  < t.length())
                {
                    int locus = base + al.Position;
                    als_info[locus]->inc_calls(al.AlignedBases[base],
                                               al.Qualities[base]-33);
                }
            }
        }
    }

    for (unsigned int i = 0 ; i < t.length() ; i++ )
    {
        als_info[i]->populate();
    }
}



































