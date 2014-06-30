#include "baxh5.h"

BaxH5::BaxH5(const string &filename)
    : H5File(filename, H5F_ACC_TRUNC)
{
    pulseData = createGroup("PulseData");
    baseCalls = pulseData.createGroup("BaseCalls");
    zmw = baseCalls.createGroup("ZMW");
}

void BaxH5::writeReads(const string& bases,
                      const string& delPhred,
                      const string& delTag,
                      const string& insPhred,
                      const string& mergePhreds,
                      const vector<unsigned short> &perBaseFrame,
                      const vector<unsigned int> &pulseIndex,
                      const string& subsPhred,
                      const string& subsTag,
                      const string& qualPhred,
                      const vector<unsigned short> &widthInFrames,
                      vector<unsigned int> &reads)
{
    hsize_t dimsBases[1];
    dimsBases[0]= bases.size();
    DataSpace ds(1, dimsBases);
    dsBasecall = baseCalls.createDataSet("Basecall",PredType::NATIVE_UINT8, ds);
    dsDeletionQV = baseCalls.createDataSet("DeletionQV",
                                            PredType::NATIVE_UINT8, ds);
    dsDeletionTag = baseCalls.createDataSet("DeletionTag",
                                            PredType::NATIVE_UINT8, ds);
    dsInsertionQV = baseCalls.createDataSet("InsertionQV",
                                            PredType::NATIVE_UINT8, ds);
    dsMergeQV = baseCalls.createDataSet("MergeQV",
                                        PredType::NATIVE_UINT8, ds);
    dsPerBaseFrame = baseCalls.createDataSet("PreBaseFrames",
                                             PredType::NATIVE_UINT16, ds);
    dsPulseIndex = baseCalls.createDataSet("PulseIndex",
                                             PredType::NATIVE_INT32, ds);
    dsQualtiyValue = baseCalls.createDataSet("QualtiyValue",
                                             PredType::NATIVE_UINT8, ds);
    dsSubstitutionQV = baseCalls.createDataSet("SubstitutionQV",
                                               PredType::NATIVE_UINT8, ds);
    dsSubstitutionTag = baseCalls.createDataSet("SubstitutionTag",
                                                PredType::NATIVE_UINT8, ds);
    dsPerBaseFrame = baseCalls.createDataSet("WidthInFrames",
                                             PredType::NATIVE_UINT16, ds);

    dsBasecall.write(bases.c_str(), PredType::NATIVE_UINT8);
    dsDeletionQV.write(delPhred.c_str(), PredType::NATIVE_UINT8);
    dsDeletionTag.write(delTag.c_str(), PredType::NATIVE_UINT8);
    dsInsertionQV.write(insPhred.c_str(), PredType::NATIVE_UINT8);
    dsMergeQV.write(mergePhreds.c_str(), PredType::NATIVE_UINT8);
    dsPerBaseFrame.write(&perBaseFrame[0], PredType::NATIVE_UINT16);
    dsPulseIndex.write(&pulseIndex[0], PredType::NATIVE_INT32);
    dsSubstitutionQV.write(subsPhred.c_str(), PredType::NATIVE_UINT8);
    dsSubstitutionTag.write(subsTag.c_str(), PredType::NATIVE_UINT8);
    dsQualtiyValue.write(qualPhred.c_str(), PredType::NATIVE_UINT8);
    dsWidthInFrame.write(&widthInFrames[0], PredType::NATIVE_UINT16);

    hsize_t numEvent[1];
    numEvent[0] = reads.size();
    DataSpace dsNE(1, numEvent);
    dsNumEvent = zmw.createDataSet("NumEvent", PredType::NATIVE_INT32, dsNE);
    dsNumEvent.write(&reads[0], PredType::NATIVE_INT32);
}


























