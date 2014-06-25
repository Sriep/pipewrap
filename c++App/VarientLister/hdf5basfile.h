#ifndef HDF5BASFILE_H
#define HDF5BASFILE_H
#include <string>
#include <vector>
#include "H5Cpp.h"

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
    unsigned long getIndexFromId(const string& readId, ReadFormat format);

    vector<unsigned long> readStarts;    
    vector<unsigned char> deletionsQV;
    vector<unsigned char> insertionsQV;
    vector<unsigned char> subsititutionsQV;
    DataSet readsDS;
    DataSet deletionDS;
    DataSet insertionDS;
    DataSet substitutionDS;
};

#endif // HDF5BASFILE_H
