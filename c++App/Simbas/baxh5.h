#ifndef BAXH5_H
#define BAXH5_H
#include <string>
#include <vector>
#include <array>
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
                   const vector<unsigned int> &reads);

private:
    void writePulseData(const string& bases,
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
                       const vector<unsigned int> &reads);

    void writeBaseCalls(const string& bases,
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
                        const vector<unsigned int> &reads);
    void writeZmw(const vector<unsigned int> &reads);
    void writeRegions(const vector<unsigned int> &reads);
    void writeBaseCallAttrbutes(const vector<unsigned int> &reads);
    void writeScanData();

    const vector<unsigned short> genHoleChipLookDs(
            const vector<unsigned int> &reads);
    const vector<int> genHoleNumberData(const vector<unsigned int> &reads);
    const vector<unsigned char> genHoleStatusData(
            const vector<unsigned int> &reads);
    const vector<array<short,2>> genHoleXYData(
            const vector<unsigned int> &reads);

    const vector<array<int, 5> > genRegionsData(
            const vector<unsigned int> &reads);

    Group pulseData;
        Group baseCalls;
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
            Group zmw;
                DataSet dsHoleChipLook;
                DataSet dsHoleNumber;
                DataSet dsHoleStatus;
                DataSet dsHoleXY;
                DataSet dsNumEvent;
        DataSet dsRegions;
    Group scanData;
        Group acqParams;
        Group dyeSet;
        Group runInfo;


};

#endif // BAXH5_H
