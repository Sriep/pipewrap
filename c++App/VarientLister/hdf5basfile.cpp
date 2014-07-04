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
    ,deletionTagDS(openDataSet("/PulseData/BaseCalls/DeletionTag"))
    ,substitutionTagDS(openDataSet("/PulseData/BaseCalls/SubstitutionTag"))
{
    init();
}

Hdf5BasFile::~Hdf5BasFile()
{
    readsDS.close();
    deletionDS.close();
    insertionDS.close();
    substitutionDS.close();
    deletionTagDS.close();
    substitutionTagDS.close();
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
    populateArray(deletionTag, deletionTagDS);
    populateArray(substitutionTag, substitutionTagDS);

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
                                          ReadFormat format) const
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

void Hdf5BasFile::setReadFromId(const string &readId, ReadFormat format)
{
    readIndex = readStarts[getIndexFromId(readId, format)];
}

unsigned char Hdf5BasFile::getDeletionQV(short position) const
{
    return deletionsQV[readIndex + position];
}

unsigned char Hdf5BasFile::getInsertionQV(short position) const
{
    return insertionsQV[readIndex + position];
}

unsigned char Hdf5BasFile::getSubstitutionQV(short position) const
{
    return subsititutionsQV[readIndex + position];
}

unsigned char Hdf5BasFile::getMergeQV(short position) const
{
    return mergeQV[readIndex + position];
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

    //unsigned long readIndex = getIndexFromId(readId, format);
    //unsigned long baseIndex = readStarts[readIndex] + posRead;
    unsigned long baseIndex = readIndex + posRead;
    unsigned char insertQV = insertionsQV[baseIndex];
    unsigned char deleteQV = deletionsQV[baseIndex];
    unsigned char substQV = subsititutionsQV[baseIndex];
    char delTag = deletionTag[baseIndex];
    char subTag = substitutionTag[baseIndex];
    long double subTagF = tagFactor(substQV, subBackground, base, subTag);
    long double delTagF = tagFactor(deleteQV, delBackground, base, delTag);

    long double prob = subTagF + delTagF * runSize * phred2prob(insertQV);
    unsigned int phred = prob2phred(prob);


    //long double prob = subTagF * phred2prob(substQV)
    //                 + delTagF * runSize * phred2prob(insertQV + deleteQV);
    //unsigned int phred = prob2phred(prob);
    //long double prob = pow(Ten, ((long double) -substQV)/Ten)
    //          + runSize * pow(Ten, ((long double) (-insertQV - deleteQV))/Ten);
    //long double phred = -10*log10(prob);
    return phred;
}

long double Hdf5BasFile::tagFactor(long double pPhred
                                      , long double pBackground
                                      , char tBase
                                      , char tagBase)
{
    if (tBase == tagBase)
    {
        return pPhred / ( pPhred + 2*pBackground);
    }
    else
    {
        return pBackground / ( pPhred + 2*pBackground);
    }
}




































