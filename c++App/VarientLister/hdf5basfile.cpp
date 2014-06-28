#include <exception>
#include <array>
#include <cstddef>
#include <cmath>
#include "hdf5basfile.h"
#include "main.h"

Hdf5BasFile::Hdf5BasFile(const string& basFilename)
:   H5File(basFilename, H5F_ACC_RDONLY)
    ,readsDS(openDataSet("/PulseData/BaseCalls/ZMW/NumEvent"))
    ,deletionDS(openDataSet("/PulseData/BaseCalls/DeletionQV"))
    ,insertionDS(openDataSet("/PulseData/BaseCalls/InsertionQV"))
    ,substitutionDS(openDataSet("/PulseData/BaseCalls/SubstitutionQV"))
{
    init();
}

Hdf5BasFile::~Hdf5BasFile()
{
    readsDS.close();
    deletionDS.close();
    insertionDS.close();
    substitutionDS.close();
}

void Hdf5BasFile::init()
{

    size_t dtSize = readsDS.getDataType().getSize();
    size_t memorySize = readsDS.getInMemDataSize();
    readStarts.clear();
    readStarts.resize(memorySize/dtSize+1);
    readStarts[0]=0;
    readsDS.read(&readStarts[1], PredType::NATIVE_INT64);
    for ( unsigned int i=1 ; i<readStarts.size() ; i++ )
    {
        readStarts[i] += readStarts[i-1];
    }
    readStarts.pop_back();

    populateArray(deletionsQV, deletionDS);
    populateArray(insertionsQV, insertionDS);
    populateArray(subsititutionsQV, substitutionDS);
}

void Hdf5BasFile::populateArray(vector<unsigned char>& dataArray,
                                DataSet &dataSet)
{
    //size_t dtSize = dataSet.getDataType().getSize();
    size_t memorySize = dataSet.getInMemDataSize();
    dataArray.clear();
    dataArray.resize(memorySize);
    dataSet.read(&dataArray[0], PredType::NATIVE_UCHAR);
}


unsigned int Hdf5BasFile::getPhred(const string &readId,
                                   int posRead,
                                   ReadFormat format)
{
    unsigned long readIndex = getIndexFromId(readId, format);
    unsigned long baseIndex = readStarts[readIndex] + posRead;
    long double insertQV = (long double) insertionsQV[baseIndex];
    long double deleteQV = (long double) deletionsQV[baseIndex];
    long double substQV = (long double) subsititutionsQV[baseIndex];
    long double prob = 2*pow(Ten, ((long double) (-insertQV - deleteQV))/Ten)
                        + pow(Ten, ((long double) -substQV)/Ten);
    long double phred = -10*log10(prob);
    return (unsigned int) phred;
}


unsigned long Hdf5BasFile::getIndexFromId(const string &readId,
                                          ReadFormat format)
{
    switch (format)
    {
    case BlasrRead:
    {
        size_t lastFS = readId.find_last_of('/');
        size_t startReadNum = readId.find_last_not_of("0123456789", lastFS-1);
        string readNum = readId.substr(startReadNum+1, lastFS - startReadNum-1);
        return stoul(readNum);
    }
    case BwaRead:
        throw(invalid_argument("No support for Bwa yet"));
    default:
        throw(invalid_argument("Unknown alignment sofware"));
    }
}

unsigned int Hdf5BasFile::getPhred(const string& readId,
                                   int posRead,
                                   const string& t,
                                   const string& read,
                                   size_t relPos,
                                   ReadFormat format)
{
    const char base = t[relPos];
    size_t end = relPos < t.size()-1 ? relPos
                                     : t.find_first_not_of(base, relPos+1);
    if (string::npos == end) end = relPos;
    size_t start = 0 == relPos ? 0 : t.find_last_not_of(base, relPos-1);
    if (string::npos == start) start = relPos;
    const size_t runSize = end-start + 1;

    unsigned long readIndex = getIndexFromId(readId, format);
    unsigned long baseIndex = readStarts[readIndex] + posRead;
    long double insertQV = (long double) insertionsQV[baseIndex];
    long double deleteQV = (long double) deletionsQV[baseIndex];
    long double substQV = (long double) subsititutionsQV[baseIndex];
    long double prob = pow(Ten, ((long double) -substQV)/Ten)
              + runSize * pow(Ten, ((long double) (-insertQV - deleteQV))/Ten);
    long double phred = -10*log10(prob);
    return (unsigned int) phred;
}




































