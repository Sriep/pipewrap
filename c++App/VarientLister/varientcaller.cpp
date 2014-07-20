#include <string>
#include <iostream>
#include <cctype>
#include <sstream>
#include <iomanip>
#include "chrono"
#include "ctime"

#include "H5Cpp.h"
#include "varientcaller.h"
#include "main.h"
#include "locusinfo.h"
#include "matchmismatches.h"
#include "hdf5basfile.h"
#include "options.h"

using namespace H5;


VarientCaller::VarientCaller(const string& bamInfile,
                             const string& tepInile,
                             const string &freqPartFile,
                             const string& basH5file,
                             const string &numFreqPart,
                             const string &readOutfile,
                             const string &lociOutfile,
                             const string &fisherFilename,
                             const string &bionomialFilename,
                             const string &poissonFilename,
                             const string &poissonBinomialFilename,
                             const string &knownFrequencyFilename)
    : freqPartition(freqPartFile, numFreqPart),
      basH5file(basH5file),
      readOutfile(readOutfile),
      lociOutfile(lociOutfile)
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
    init();
}

VarientCaller::VarientCaller()
    : freqPartition("", Options::get(Options::NumFrequencyPartitions))
      ,basH5file(Options::get(Options::BaxH5File))
      ,readOutfile(Options::get(Options::ReadOutFile))
      ,lociOutfile(Options::get(Options::LociOutFile))
      ,insThreshold(stoul(Options::get(Options::FilterInsThreshold)))
      ,delThreshold(stoul(Options::get(Options::FilterDelsThreshold)))
      ,subsThreshold(stoul(Options::get(Options::FilterSubsThreshold)))
      ,preQualThreshold(stoul(Options::get(Options::PreFilterQuality)))
      ,postQualThrehold(stoul(Options::get(Options::PostFilterQuality)))
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
    //Hdf5BasFile* basFile = new Hdf5BasFile(basH5file);
    for (unsigned int i = 0 ; i < t.length() ; i++ )
    {
        als_info.push_back(unique_ptr<LocusInfo>(
                                new LocusInfo(t[i], &freqPartition, methods)));
    }
    filterReads();
    populateLociInfo();
    calculatePvalues();

}

void VarientCaller::hdf5()
{
    /*H5::H5File h5file(basH5file, H5F_ACC_RDONLY);
    FileAccPropList propList = h5file.getAccessPlist();
    hsize_t nob = h5file.getNumObjs();
    vector<H5std_string> names;
    for ( unsigned int i = 0 ; i < h5file.getNumObjs() ; i++ )
    {
        H5std_string name = h5file.getObjnameByIdx(i);
        names.push_back(name);
    }

    h5file.close();*/
}

void VarientCaller::filterReads()
{
    bam_reader.Rewind();
    BamAlignment al;
    //unsigned long long totalPhred = 0;
    while(bam_reader.GetNextAlignment(al))
    {
        // Position = 0 means read has not been mapped succesfully
        if (0 < al.Position)
        {
            MatchMismatches tMatch(al.QueryBases, al.CigarData);
            unsigned int varients = 0;
                for ( unsigned int base = 0 ; base < tMatch.size() ; base++)
                {
                    if (basesDiffer(tMatch[base]
                                    , t[(base + al.Position) % t.size()]))
                    //if (!compareBases(tMatch[base]
                    //                  , t[(base + al.Position) % t.size()]))
                    {
                        varients++;
                    }
                   // assert(base + tMatch.offset(base) < al.Qualities.size());
                }
                totalBaseReads += tMatch.size();
                totalReadVareints += varients;
        }
     }
    //averagePhred = (long double) totalPhred / (long double) totalBaseReads;
    freqPartition.setParmeters(totalBaseReads, totalReadVareints);//, averagePhred);
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
                int locus =(base + al.Position) % t.size();
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
    lout << "Locus\tBase\tCoverage\tAveQuality\tpSNP\tpSNPCount\tpSNP%";
    if (methods.find(PValues::Bionomial) != methods.end())
        lout <<  "\tpvBionomial";
    if (methods.find(PValues::Poisson) != methods.end())
        lout << "\tpvPoissionan";
    if (methods.find(PValues::KnownFrequency) != methods.end())
        lout << "\tKF\n";
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
                                    , const BamAlignment& al
                                    , const Hdf5BasFile *baxFile)
{
    if (NULL == baxFile)
    {
        return al.Qualities[position] - 33 >= preQualThreshold;
    }
    else
    {
        //char q = al.Qualities[position] - 33;
        //char i = baxFile->getInsertionQV(position);
        //char d = baxFile->getDeletionQV(position);
        //char s = baxFile->getSubstitutionQV(position);
        //return i>insThreshold && d >delThreshold && s>subsThreshold;
        return  baxFile->getInsertionQV(position) >= insThreshold
                && baxFile->getDeletionQV(position) >= delThreshold
                && baxFile->getSubstitutionQV(position) >= subsThreshold
                && al.Qualities[position] - 33 >= preQualThreshold;
    }
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

    //unsigned int nextOut = /t.length() /10;
    //unsigned int pct = 0;
    //chrono::time_point<chrono::system_clock> start;
    //start = chrono::system_clock::now();
    cout << "Untangling reads.\n";
    int readCount = 0;


    Hdf5BasFile* basFile = NULL;
    if (basH5file != "") basFile = new Hdf5BasFile(basH5file);

    while(bam_reader.GetNextAlignment(al))
    {
        // Position = 0 means read has not been mapped succesfully
        if (0 < al.Position)
        {
            if (NULL != basFile) basFile->setReadFromId(al.Name);
            MatchMismatches tMatch(al.QueryBases, al.CigarData);
            for (unsigned int mBase = 0 ; mBase < tMatch.size() ; mBase++)
            {
                const unsigned int locus = (al.Position+ mBase) % t.size();
                const unsigned int alBase = mBase + tMatch.offset(mBase);

                //if (goodEnoughRead(al.Qualities[alBase]))
                if (goodEnoughRead(alBase, al, basFile))
                        //|| !trimEndsFactor(al.Position, mBase, t.size()))
                {
                    if (!basesDiffer(tMatch[mBase], t[locus]))
                    //if (compareBases(tMatch[mBase], t[locus]))
                    {
                        int phard = (NULL == basFile)
                                    ? al.Qualities[alBase]-33
                                    : basFile->getPhred(alBase);
                        als_info[locus]->inc_calls(al.QueryBases[alBase],phard);
                    }
                    else if (tMatch[mBase] != '-')
                    {
                        int phard;
                        if(NULL == basFile)
                        {
                            phard = al.Qualities[alBase]-33;
                        }
                        else
                        {
                            if (0 == windowSize)
                            {
                                phard = basFile->getPhred(alBase);
                            }
                            else
                            {
                                const unsigned int start =
                                        (unsigned int)max(0, (int)(locus-windowSize));
                                const unsigned int end =
                                                t.size() < locus + windowSize
                                              ? t.size() : locus + windowSize;
                                const unsigned int l = (locus < windowSize)
                                                        ? locus : windowSize;
                                string window = t.substr(start, end - start);
                                phard = basFile->getPhred(alBase, window, l);
                            }
                        }
                        if (phard > postQualThrehold)
                            als_info[locus]->inc_calls(
                                        al.QueryBases[alBase],phard);
                    }
                }
            }
        }
        if (0 == (++readCount % 10000))
            cout << readCount << " reads processed\n";
    }
    if (NULL != basFile) delete(basFile);
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


































