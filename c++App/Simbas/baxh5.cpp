#include "baxh5.h"
#include "ctime"
#include "main.h"
#include "options.h"

BaxH5::BaxH5(const string &filename)
    : H5File(filename, H5F_ACC_TRUNC)
{
}

void BaxH5::writeReads(const string& bases
                       ,const string& delPhred
                       ,const string& delTag
                       ,const string& insPhred
                       ,const string& mergePhreds
                       ,const vector<unsigned short> &perBaseFrame
                       ,const vector<unsigned int> &pulseIndex
                       ,const string& subsPhred
                       ,const string& subsTag
                       ,const string& qualPhred
                       ,const vector<unsigned short> &widthInFrames
                       ,const vector<unsigned int> &reads)
{
    pulseData = createGroup("PulseData");
    writePulseData(bases
                  ,delPhred
                  ,delTag
                  ,insPhred
                  ,mergePhreds
                  ,perBaseFrame
                  ,pulseIndex
                  ,subsPhred
                  ,subsTag
                  ,qualPhred
                  ,widthInFrames
                  ,reads);
    scanData = createGroup("ScanData");
    writeScanData();
}

void BaxH5::writeScanData()
{
    Attribute a;

    acqParams = scanData.createGroup("AcqParams");
    double frameRate = 75.0;
    a = acqParams.createAttribute("FrameRate"
                                  ,PredType::NATIVE_FLOAT
                                  ,H5S_SCALAR);
    a.write(PredType::NATIVE_DOUBLE, &frameRate);
    unsigned int numFrames = 1000000;
    a = acqParams.createAttribute("NumFrames"
                                  ,PredType::NATIVE_UINT32
                                  ,H5S_SCALAR);
    a.write(PredType::NATIVE_UINT32, &numFrames);
    time_t now;
    time(&now);
    char buf[20];
    strftime(buf,20,"%FT%TZ", gmtime(&now));
    const string whenStarted= buf;
    a = acqParams.createAttribute("WhenStarted"
                                  ,StrType(0,whenStarted.size())
                                  ,H5S_SCALAR);
    a.write(StrType(0,whenStarted.size()), whenStarted.c_str());

    dyeSet = scanData.createGroup("DyeSet");
    const string baseMap= "TGAC";
    a = dyeSet.createAttribute("BaseMap"
                                  ,StrType(0,baseMap.size())
                                  ,H5S_SCALAR);
    a.write(StrType(0,baseMap.size()), baseMap.c_str());
    unsigned int numAnalog = 4;
    a = dyeSet.createAttribute("NumAnalog"
                                  ,PredType::NATIVE_UINT32
                                  ,H5S_SCALAR);
    a.write(PredType::NATIVE_UINT32, &numAnalog);

    runInfo = scanData.createGroup("RunInfo");
    const string movieName= Options::get(Options::OutPrefix);
    a = runInfo.createAttribute("MovieName"
                                  ,StrType(0,movieName.size())
                                  ,H5S_SCALAR);
    a.write(StrType(0,movieName.size()), movieName.c_str());
    unsigned int platformId = 0;
    a = runInfo.createAttribute("PlatformId"
                                  ,PredType::NATIVE_UINT32
                                  ,H5S_SCALAR);
    a.write(PredType::NATIVE_UINT32, &platformId);
    const string platformName= "Springfield";
    a = runInfo.createAttribute("PlatformName"
                                  ,StrType(0,platformName.size())
                                  ,H5S_SCALAR);
    a.write(StrType(0,platformName.size()), platformName.c_str());
    const string runCode= "simulated_runcode";
    a = runInfo.createAttribute("RunCode"
                                  ,StrType(0,runCode.size())
                                  ,H5S_SCALAR);
    a.write(StrType(0,runCode.size()), runCode.c_str());
}

void BaxH5::writePulseData(const string& bases
                          ,const string& delPhred
                          ,const string& delTag
                          ,const string& insPhred
                          ,const string& mergePhreds
                          ,const vector<unsigned short> &perBaseFrame
                          ,const vector<unsigned int> &pulseIndex
                          ,const string& subsPhred
                          ,const string& subsTag
                          ,const string& qualPhred
                          ,const vector<unsigned short> &widthInFrames
                          ,const vector<unsigned int> &reads)
{
    baseCalls = pulseData.createGroup("BaseCalls");
    writeBaseCallAttrbutes(reads);
    writeBaseCalls(bases
                   ,delPhred
                   ,delTag
                   ,insPhred
                   ,mergePhreds
                   ,perBaseFrame
                   ,pulseIndex
                   ,subsPhred
                   ,subsTag
                   ,qualPhred
                   ,widthInFrames
                   ,reads);
    writeRegions(reads);
}

void BaxH5::writeBaseCallAttrbutes(const vector<unsigned int> &reads)
{
    Attribute a;

    time_t now;
    time(&now);
    char buf[20];
    strftime(buf,20,"%FT%TZ", gmtime(&now));
    const string dataCreated= buf;
    a = baseCalls.createAttribute("DateCreated"
                                  ,StrType(0,dataCreated.size())
                                  ,H5S_SCALAR);
    a.write(StrType(0,dataCreated.size()), dataCreated.c_str());
    const string changeListId = "simbas " + version;
    //const string changeListId = "1.3.0.50.104380";
    a = baseCalls.createAttribute("ChangeListID"
                                  ,StrType(0,changeListId.size())
                                  ,H5S_SCALAR);
    a.write(StrType(0,changeListId.size()), changeListId.c_str());
    const string schemaRevision = "1.0";
    a = baseCalls.createAttribute("SchemaRevision"
                                  ,StrType(0,schemaRevision.size())
                                  ,H5S_SCALAR);
    a.write(StrType(0,schemaRevision.size()), schemaRevision.c_str());
    const string qVDeconding ="Standard Phred encoding: QV = -10 * log10(p)"
                              " - where p is the probability of error";
    a = baseCalls.createAttribute("QVDeconding"
                                  ,StrType(0,qVDeconding.size())
                                  ,H5S_SCALAR);
    a.write(StrType(0,qVDeconding.size()), qVDeconding.c_str());

    hsize_t dimsf[2];
    dimsf[0] = 2;
    dimsf[1] = 11;
    DataSpace dsContent(2, dimsf);
    a = baseCalls.createAttribute("Content", StrType(0,20), dsContent);
    char contentData[2][11][20] =
    {
       { "Basecall","DeletionQV","DeletionTag","InsertionQV","MergeQV"
         ,"PreBaseFrames","PulseIndex","QualtiyValue"
         ,"SubstitutionQV","SubstitutionTag","WidthInFrames"},
       { "uint8", "uint8", "uint8", "uint8", "uint8", "uint16", "int32"
         ,"uint8", "uint8", "uint8","uint16"}
    };
    a.write(StrType(0,20), contentData);

    int count = reads.size();
    a = baseCalls.createAttribute("CountStored"
                                  ,PredType::NATIVE_INT32
                                  ,H5S_SCALAR);
    a.write(PredType::NATIVE_INT32, &count);
}

void BaxH5::writeBaseCalls(const string& bases
                           ,const string& delPhred
                           ,const string& delTag
                           ,const string& insPhred
                           ,const string& mergePhreds
                           ,const vector<unsigned short> &perBaseFrame
                           ,const vector<unsigned int> &pulseIndex
                           ,const string& subsPhred
                           ,const string& subsTag
                           ,const string& qualPhred
                           ,const vector<unsigned short> &widthInFrames
                           ,const vector<unsigned int> &reads)
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
    dsWidthInFrame = baseCalls.createDataSet("WidthInFrames",
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

    zmw = baseCalls.createGroup("ZMW");
    writeZmw(reads);
}

void BaxH5::writeZmw(const vector<unsigned int> &reads)
{
    hsize_t numEvent[1];
    numEvent[0] = reads.size();
    DataSpace dsNE(1, numEvent);

    dsHoleChipLook = zmw.createDataSet("HoleChipLook",PredType::NATIVE_UINT16,dsNE);
    dsHoleNumber = zmw.createDataSet("HoldNumber",PredType::NATIVE_INT32,dsNE);
    dsHoleStatus = zmw.createDataSet("HoleStatus",PredType::NATIVE_UINT8,dsNE);
    dsNumEvent = zmw.createDataSet("NumEvent", PredType::NATIVE_INT32, dsNE);

    const vector<unsigned short> holeChipLook = genHoleChipLookDs(reads);
    dsHoleChipLook.write(&(genHoleChipLookDs(reads)[0]), PredType::NATIVE_UINT16);
    dsHoleNumber.write(&(genHoleNumberData(reads)[0]), PredType::NATIVE_INT32);
    dsHoleStatus.write(&(genHoleStatusData(reads)[0]), PredType::NATIVE_UINT8);
    dsNumEvent.write(&reads[0], PredType::NATIVE_INT32);

    hsize_t numHoleXY[2];
    numHoleXY[0] = reads.size();
    numHoleXY[1] = 2;
    DataSpace dsHXY(2, numHoleXY);
    dsHoleXY = zmw.createDataSet("HoleXY", PredType::NATIVE_INT16, dsHXY);
    const vector<array<short,2>> holeXYData = genHoleXYData(reads);
    dsHoleXY.write(&(holeXYData[0]), PredType::NATIVE_INT16);
}

void BaxH5::writeRegions(const vector<unsigned int> &reads)
{
    hsize_t numRegions[2];
    numRegions[0] = 2*reads.size();
    numRegions[1] = 5;
    DataSpace dsReg(2, numRegions);
    dsRegions = pulseData.createDataSet("Regions", PredType::NATIVE_INT32, dsReg);
    const vector<array<int,5>> regionsData = genRegionsData(reads);
    dsRegions.write(&(regionsData[0]), PredType::NATIVE_INT32);

    Attribute a;

    hsize_t dimsf[1];
    dimsf[0] = 5;
    DataSpace dsColumnNames(1, dimsf);
    a = dsRegions.createAttribute("ColumnNames", StrType(0,30), dsColumnNames);
    char columnNames[5][30] =
    {
        "HoleNumber"
        , "Region type index"
        , "Region start in bases"
        , "Region end in bases"
        , "Region score"
    };
    a.write(StrType(0,30), columnNames);

    dimsf[0] = 3;
    DataSpace dsRegionDescriptions(1, dimsf);
    a = dsRegions.createAttribute("RegionDescriptions",
                                  StrType(0,100),
                                  dsRegionDescriptions);
    char regionDescriptions[3][100] =
    {
        "Adapter Hit"
        , "Insert Region, High Quality bases region. Score is 1000 * predicted accuracy"
        , "where predicted accuary is 0 to 1.0"
    };
    a.write(StrType(0,100), regionDescriptions);

    DataSpace dsregionSources(1, dimsf);
    a = dsRegions.createAttribute("RegionSources",
                                  StrType(0,30),
                                  dsregionSources);
    char regionSources[3][30] =
    {
        "AdapterFinding"
        , "AdapterFinding"
        , "PulseToBase Region classifer"
    };
    a.write(StrType(0,30), regionSources);

    DataSpace retionTypes(1, dimsf);
    a = dsRegions.createAttribute("RegionTypes",
                                  StrType(0,20),
                                  retionTypes);
    char regionTypes[3][20] =
    {
        "Adapter"
        , "Insert"
        , "HQRegion"
    };
    a.write(StrType(0,20), regionTypes);
}

const vector<array<int,5>> BaxH5::genRegionsData(
        const vector<unsigned int> &reads)
{
    vector<array<int,5>> regionsData(2*reads.size(), array<int,5>());
    for ( unsigned int i = 0 ; i < reads.size() ; i++ )
    {
        regionsData[2*i]  [0] = i;
        regionsData[2*i+1][0] = i;
        regionsData[2*i]  [1] = 1;
        regionsData[2*i+1][1] = 2;
        regionsData[2*i]  [3] = reads[i];
        regionsData[2*i+1][3] = 0;
        regionsData[2*i]  [4] = 1000;
        regionsData[2*i+1][4] = 1000;
    }
    return regionsData;
}

const vector<array<short,2>> BaxH5::genHoleXYData(
        const vector<unsigned int> &reads)
{
    vector<array<short,2>> holeXYData(reads.size(), array<short,2>());
    for ( unsigned int i = 0 ; i < reads.size() ; i++ )
        holeXYData[i][1] = i;
    return holeXYData;
}

const vector<unsigned short> BaxH5::genHoleChipLookDs(
        const vector<unsigned int> &reads)
{
    vector<unsigned short> holeChipLook(reads.size(), 0);
    return holeChipLook;
}

const vector<int> BaxH5::genHoleNumberData(
        const vector<unsigned int> &reads)
{
    vector<int> holeNumberData(reads.size());
    for ( unsigned int i ; i < reads.size() ; i++ )
        holeNumberData[i] = i;
    return holeNumberData;
}

const vector<unsigned char> BaxH5::genHoleStatusData(
        const vector<unsigned int> &reads)
{
    vector<unsigned char> holeStatusData(reads.size(), 0);
    return holeStatusData;
}











































