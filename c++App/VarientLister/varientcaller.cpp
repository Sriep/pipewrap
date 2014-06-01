#include <algorithm>
#include <cctype>
#include <qdebug.h>
#include <algorithm>
#include <string>
#include <iostream>
#include <cctype>
#include <QDebug>
#include <sstream>

#include "api/BamAlignment.h"
#include "varientcaller.h"
#include "main.h"


VarientCaller::VarientCaller(const string& in_file,
                             const string& t_filename,
                             int e_threshold,
                             ostream& out_stream)
    : out(out_stream), error_threshold(e_threshold)
{
    bam_reader.Open(in_file);
    t = get_file_contents(t_filename.c_str());
    basesFromFasta();
}

void VarientCaller::callVarants()
{

    std::vector<unsigned int> coverage(t.length(), 0);
    std::vector<unsigned int> ave_quality(t.length(), 0);
    BamAlignment al;
    while(bam_reader.GetNextAlignment(al))
    {
        for (unsigned int base = 0 ; base < al.Length ; base++)
        {
            if (base + al.Position  < t.length())
            {
                coverage[base + al.Position] += 1;
                ave_quality[base + al.Position] += al.Qualities[base]-33;
            }
        }
    }
    for (unsigned int i = 0 ; i < t.length() ; i++ )
    {
        ave_quality[i] = (int)(((double) ave_quality[i])
                               / ((double) coverage[i]));
    }

    bam_reader.Rewind();
    out << "Read\tPosRead\tBaseRead\tPosTemp\tBaseTemp\tPhred"
        << "\tAveQuality\tCoverage\n";
    while(bam_reader.GetNextAlignment(al))
    {

        unsigned int num_mismatching = 0;
        stringstream out_str;
        for (unsigned int base = 0 ; base < al.Length ; base++)
        {
            if (al.AlignedBases[base] != t[base + al.Position])
            {
                out_str << "\"" << al.Name
                    << "\"\t\"" << base +1
                    << "\"\t\"" << visBase(al.AlignedBases[base])
                    << "\"\t\"" << base + al.Position +1
                    << "\"\t\"" << visBase(t[base + al.Position])
                    << "\"\t\"" << al.Qualities[base]-33;
                if (base + al.Position  < t.length())
                {
                    out_str << "\"\t\"" << ave_quality[base + al.Position]
                            << "\"\t\"" << coverage[base + al.Position];
                }
                else
                {
                    out_str  << "\"\t\"0\"\t\"0";
                }
                out_str  << "\"\n";
                num_mismatching++;
            }
            if (error_threshold < (100 * num_mismatching) / al.Length )
                break;
        }
        if (error_threshold > (100 * num_mismatching) / al.Length )
        {
            out << out_str.str();
        }
        else
        {
            // too many mismatches
        }

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

std::vector<unsigned int> VarientCaller::getCoverage()
{

}


































