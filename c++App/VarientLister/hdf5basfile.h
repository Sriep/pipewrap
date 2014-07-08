#ifndef HDF5BASFILE_H
#define HDF5BASFILE_H
#include <string>
#include <vector>
#include "H5Cpp.h"
#include "main.h"

using namespace std;
using namespace H5;

class Hdf5BasFile : H5File
{
public:
    enum ReadFormat
    {
        BlasrRead,
        BwaRead
    };

    Hdf5BasFile(const string& basFilename);
    virtual ~Hdf5BasFile();
    void setReadFromId(const string& readId, ReadFormat format = BlasrRead);
    unsigned char getDeletionQV(short position) const;
    unsigned char getInsertionQV(short position) const;
    unsigned char getSubstitutionQV(short position) const;
    unsigned char getMergeQV(short position) const;
    unsigned int getPhred(const string& readId,
                          int posRead,
                          ReadFormat format = BlasrRead);
    unsigned int getPhred(const string& readId,
                          int posRead,
                          const string& t,
                          const string& read,
                          size_t relPos,
                          ReadFormat format = BlasrRead);

private:
    void init();
    void populateArray(vector<unsigned char>& dataArray, DataSet& dataSet);
    unsigned long getIndexFromId(const string& readId, ReadFormat format) const;
    long double tagFactor(long double pPhred
                                          , long double pBackground
                                          , char tBase
                                          , char tagBase);

    vector<unsigned long> readStarts;    
    vector<unsigned char> deletionsQV;
    vector<unsigned char> insertionsQV;
    vector<unsigned char> subsititutionsQV;
    vector<unsigned char> mergeQV;
    vector<unsigned char>  deletionTag;
    vector<unsigned char>  substitutionTag;
    DataSet readsDS;
    DataSet deletionDS;
    DataSet insertionDS;
    DataSet substitutionDS;
    DataSet deletionTagDS;
    DataSet mergeDS;
    DataSet substitutionTagDS;

    long double delBackground = phred2prob(20);
    long double subBackground = phred2prob(30);
    int readIndex = 0;
};

#endif // HDF5BASFILE_H
