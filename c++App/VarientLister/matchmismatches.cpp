#include "matchmismatches.h"

MatchMismatches::MatchMismatches(const string& alignedBases,
                                 const vector<CigarOp>& cigar)
 : string(alignedBases), cigar(cigar)//, offsets(alignedBases.size())
{
    untangle();
}

int MatchMismatches::offset(unsigned int position) const
{
    if (position < offsets.size())
        return offsets[position];
    else
        throw (range_error("MatchMismatches::offset postion too high: "
                           + to_string(position)));
}

/// CIGAR operation type (MIDNSHPX=)
/// NHSI    remove  (insertion)
/// MX=     keep    (Match mismatch)
/// DP      add gap (Deletion)
void MatchMismatches::untangle()
{
    unsigned int sequPos = 0;
    int offset = 0;
    for (unsigned int i = 0 ; i < cigar.size() ; i++)
    {
        switch (cigar[i].Type)
        {
        case 'N':
        case 'H':
        case 'S':
        case 'I':
            erase(sequPos, cigar[i].Length);
            offset += cigar[i].Length;
            break;
        case 'M':
        case 'X':
        case '=':
            for (unsigned int j = 0 ; j < cigar[i].Length ; j++)
                offsets.push_back(offset);
            sequPos += cigar[i].Length;
            break;
        case 'D':
        case 'P':
            insert(sequPos, cigar[i].Length, '-');            
            for (unsigned int j = 0 ; j < cigar[i].Length ; j++)
                offsets.push_back(offset--);
            sequPos += cigar[i].Length;
            break;
        default:
            throw(std::out_of_range("cigar wrong value: " + cigar[i].Type));
            break;
        }
    }
    if (cigar.size() == 0)
            offsets.resize(size(),0);
}
