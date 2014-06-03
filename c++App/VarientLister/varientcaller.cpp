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

#include "api/BamAlignment.h"
#include "varientcaller.h"
#include "main.h"
#include "locusalingmentinfo.h"


VarientCaller::VarientCaller(const string& in_file,
                             const string& t_filename,
                             int e_threshold,
                             ostream& read_stream,
                             ostream& loci_steam)
    : error_threshold(e_threshold), rout(read_stream), lout(loci_steam)
{
    bam_reader.Open(in_file);
    t = get_file_contents(t_filename.c_str());
    Init();
}

VarientCaller::~VarientCaller()
{
}

void VarientCaller::Init()
{
    basesFromFasta();
    filterReads();
    for (unsigned int i = 0 ; i < t.length() ; i++ )
    {
        als_info.push_back( unique_ptr<LocusInfo>(new LocusInfo(t[i])) );
    }
    populateLociInfo();
}

void VarientCaller::filterReads()
{
    bam_reader.Rewind();
    BamAlignment al;

    while(bam_reader.GetNextAlignment(al))
    {
        unsigned int num_mismatching = 0;
        unsigned int base = 0;
        bool too_many_missmatches = false;
        while ((base < (unsigned int) al.Length) && !too_many_missmatches)
        {
            if (al.AlignedBases[base] != t[base + al.Position])
            {
                num_mismatching++;
                if (((100 * num_mismatching) / al.Length) > error_threshold)
                    too_many_missmatches = true;
            }
            base++;
        }
        if (too_many_missmatches) invalid.insert(al.Name);
        num_mismatching++;
     }
}

void VarientCaller::writeReadInfo()
{
    bam_reader.Rewind();
    BamAlignment al;
    rout << "Read\tPosRead\tBaseRead\tPosTemp\tBaseTemp\tPhred"
        << "\tAveQuality\tCoverage\tpvBionomial\tpvPoissionan\tFisher\n";
    while(bam_reader.GetNextAlignment(al))
    {
        if (!invalid.count(al.Name))
        {
            for (int base = 0 ; base < al.Length ; base++)
            {
                int locus = base + al.Position;

                if (al.AlignedBases[base] != t[locus])
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
                            << "\"\t\"" << als_info[locus]->getCoverage()
                            << "\"\t\"" << setprecision(64)
                            << als_info[locus]->getPVBionomial()
                            << "\"\t\"" << setprecision(64)
                            << als_info[locus]->getPVPoission()
                            << "\"\t\"" << setprecision(64)
                            << als_info[locus]->getPVFisher();
                    }
                    else
                    {
                        rout  << "\"\t\"0\"\t\"0" << "\"\t\"0\"\t\"0";
                    }
                    rout  << "\"\n";
                }
            }
        }
     }
}

void VarientCaller::writeLociInfo()
{
    lout << "Locus\tBase\tCoverage\tAveQuality\tpSNP\tpSNPCount\rpSNP%"
        << "\tpvBionomial\tpvPoissionan\tFisher\n";

    for (int locus = 0 ; locus < t.length() ; locus++)
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
                << als_info[locus]->getPVBionomial()
                << "\"\t\"" << setprecision(64)
                << als_info[locus]->getPVPoission()
                << "\"\t\"" << setprecision(64)
                << als_info[locus]->getPVFisher()
                << "\"\n";
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
        else
        {
            int i =5;
            i++;
        }
    }

    for (unsigned int i = 0 ; i < t.length() ; i++ )
    {
        als_info[i]->populate();
    }
}




































