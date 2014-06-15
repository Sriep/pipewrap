#ifndef MATCHMISMATCHES_H
#define MATCHMISMATCHES_H
#include "api/BamAlignment.h"

using namespace std;
using namespace BamTools;

class MatchMismatches : public std::string
{
public:
    MatchMismatches(const string& alignedBases,
                    const vector<CigarOp>& cigar);
    int offset(unsigned int position) const;
private:
    void untangle();

    const vector<CigarOp>& cigar;
    vector<int> offsets;
    //const string& alignedBases;
};

#endif // MATCHMISMATCHES_H
