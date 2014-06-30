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
    void writeReads(const string& bases,
                   const string& delPhred,
                   const string& delTag,
                   const string& insPhred,
                   const string& mergePhreds,
                   const vector<unsigned short>  &perBaseFrame,
                   const vector<unsigned int>  &pulseIndex,
                   const string& subsPhred,
                   const string& subsTag,
                   const string& qualPhred,
                   const vector<unsigned short> &widthInFrames,
                   vector<unsigned int> &reads);

private:
    Group pulseData;
    Group baseCalls;
    Group zmw;
    DataSet dsBasecall;
    DataSet dsDeletionQV;
    DataSet dsDeletionTag;
    DataSet dsInsertionQV;
    DataSet dsMergeQV;
    DataSet dsPerBaseFrame;
    DataSet dsPulseIndex;
    DataSet dsQualtiyValue;
    DataSet dsSubstitutionQV;
    DataSet dsSubstitutionTag;
    DataSet dsWidthInFrame;
    DataSet dsNumEvent;
};

#endif // BAXH5_H
