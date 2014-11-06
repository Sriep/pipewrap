#include <string>
#include <iostream>
#include <cctype>
#include <sstream>
#include <iomanip>
#include "chrono"
#include "ctime"
#include <qdebug.h>
#include "H5Cpp.h"
#include "varientcaller.h"
#include "varientcallerHdf5.h"
#include "main.h"
#include "locusinfo.h"
#include "matchmismatches.h"
#include "hdf5basfile.h"
#include "options.h"

using namespace H5;

VarientCallerHdf5::VarientCallerHdf5()
    : VarientCaller(), basH5file(Options::get(Options::BaxH5File))
{
}

VarientCallerHdf5::~VarientCallerHdf5()
{
}

bool VarientCallerHdf5::goodEnoughRead(unsigned short position
                                    , const BamAlignment& al
                                    , const Hdf5BasFile *baxFile)
{
    if (NULL == baxFile)
    {
        return VarientCaller::goodEnoughRead(position, al);
        //return al.Qualities[position] - 33 >= preQualThreshold;
    }
    else
    {
        return  baxFile->getInsertionQV(position) >= insThreshold
                && baxFile->getDeletionQV(position) >= delThreshold
                && baxFile->getSubstitutionQV(position) >= subsThreshold
                && al.Qualities[position] - 33 >= preQualThreshold;
    }
}

void VarientCallerHdf5::populateLociInfo()
{
   if (basH5file == "")
   {
       VarientCaller::populateLociInfo();
       return;
   }

    bam_reader.Rewind();
    BamAlignment al;
    cout << "Untangling reads.\n";
    int readCount = 0;


    Hdf5BasFile* basFile = NULL;
    basFile = new Hdf5BasFile(basH5file);

    while(bam_reader.GetNextAlignment(al))
    {
        // Position = 0 means read has not been mapped succesfully
        if (0 < al.Position)
        {
            basFile->setReadFromId(al.Name);
            MatchMismatches tMatch(al.QueryBases, al.CigarData);
            for (unsigned int mBase = 0 ; mBase < tMatch.size() ; mBase++)
            {
                const unsigned int locus = (al.Position+ mBase) % t.size();
                const unsigned int alBase = mBase + tMatch.offset(mBase);

                if (goodEnoughRead(alBase, al, basFile))
                {
                    if (!basesDiffer(tMatch[mBase], t[locus]))
                    {
                        int phard = basFile->getPhred(alBase);
                        als_info[locus]->inc_calls(al.QueryBases[alBase],phard);
                    }
                    else if (tMatch[mBase] != '-')
                    {
                        int phard;
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
    delete(basFile);
}



































