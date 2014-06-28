#ifndef BAXH5_H
#define BAXH5_H
#include <string>
#include <vector>
#include "H5Cpp.h"

using namespace H5;
using namespace std;

class BaxH5 : public H5File
{
public:
    BaxH5(const string& filename);
    void writeRead(const string& bases,
                   const string& delPhred,
                   const string& delTag,
                   const string& insPhred,
                   const string& MergePhreds,
                   const string& subsPhred,
                   const string& subsTag,
                   const string& qualPhred,
                   vector<int> &reads);

private:
    Group pulseData;
    Group baseCalls;
    Group zmw;
    DataSet dsBasecall;
    DataSet dsDeletionQV;
    DataSet dsDeletionTag;
    DataSet dsInsertionQV;
    DataSet dsMergeQV;
    DataSet dsQualtiyValue;
    DataSet dsSubstitutionQV;
    DataSet dsSubstitutionTag;
    DataSet dsNumEvent;
};

#endif // BAXH5_H
