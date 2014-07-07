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

    Hdf5BasFile(const string& basFilename);
    Hdf5BasFile();
    virtual ~Hdf5BasFile();
    void operator() ();

private:
    static const int intervals = 3;
    static const int fields = 3;
    void writeDistribution();
    void init();
    void populateArray(vector<unsigned char>& dataArray, DataSet& dataSet);
    //unsigned long getIndexFromId(const string& readId, ReadFormat format);
    const array<unsigned int,101>  qualityDist(vector<unsigned char> &dataArray);
    void writeQualityDependance();
    void writeBoxSet(ofstream& dout
                    , unsigned int depend[intervals][intervals][intervals]
                    , int numCalls, int boxI, int colI, int rowI);

    string outPrefix;
    vector<unsigned long> readStarts;
    vector<unsigned char> deletionsQV;
    vector<unsigned char> insertionsQV;
    vector<unsigned char> subsititutionsQV;
    vector<unsigned char> qualtyValue;
    vector<unsigned char> mergeQV;
    DataSet readsDS;
    DataSet deletionDS;
    DataSet insertionDS;
    DataSet substitutionDS;
    DataSet qualityValueDS;
    DataSet mergeDS;


    const int dBound[intervals+1] = {0,8,16,100};
    const int iBound[intervals+1] = {0,6,13,100};
    const int sBound[intervals+1] = {0,9,23,100};
    const string headers[fields][intervals] =
                {
                    {"Del 0-7" , "Del 8-15" , "Del 16+"},
                    {"Ins 0-5","Ins 6-12","Ins 13+"},
                    {"Sub 0-8", "Sub 9-22", "Sub 23+"}
                };
};
inline double toPct(int num, int denom);

#endif // HDF5BASFILE_H
