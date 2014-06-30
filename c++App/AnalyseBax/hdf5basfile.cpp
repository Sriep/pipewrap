#include <exception>
#include <array>
#include <cstddef>
#include <cmath>
#include "hdf5basfile.h"
#include "main.h"
#include <iomanip>

Hdf5BasFile::Hdf5BasFile(const string& basFilename)
:   H5File(basFilename, H5F_ACC_RDONLY)
    , outPrefix("data_" + basFilename)
    , readsDS(openDataSet("/PulseData/BaseCalls/ZMW/NumEvent"))
    , deletionDS(openDataSet("/PulseData/BaseCalls/DeletionQV"))
    , insertionDS(openDataSet("/PulseData/BaseCalls/InsertionQV"))
    , substitutionDS(openDataSet("/PulseData/BaseCalls/SubstitutionQV"))
    , qualityValueDS(openDataSet("/PulseData/BaseCalls/QualityValue"))
    , mergeDS(openDataSet("/PulseData/BaseCalls/MergeQV"))
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
    populateArray(qualtyValue, qualityValueDS);
    populateArray(mergeQV, mergeDS);
}

void Hdf5BasFile::operator()()
{
    writeStuff();
    qualityDependance();
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

void Hdf5BasFile::writeStuff()
{
    qualityDist(deletionsQV,  "Deletes.csv");
    qualityDist(insertionsQV,  "Inserts.csv");
    qualityDist(subsititutionsQV,  "Subs.csv");
    qualityDist(qualtyValue,  "Quality.csv");
    qualityDist(mergeQV,  "Merges.csv");
    //populateDist(readStarts,  "readsizes.csv");
}

void Hdf5BasFile::qualityDist(vector<unsigned char>& dataArray,
                                const string& outFile)
{
    array<unsigned int,101> dist = {};
    const int numCalls = dataArray.size();
    for ( int i = 0 ; i < numCalls ; i++ )
    {
        if (dataArray[i] < 100)
            dist[dataArray[i]]++;
        else
            dist[100]++;
    }

    ofstream dout(outPrefix + outFile, ios_base::out | ios_base::trunc);
    for (unsigned int i = 0 ; i < 101 ; i++)
    {
        double amount = double (100*(double)dist[i])/(double)numCalls;
        dout << i << '\t' << amount << "\n";
    }
}

void Hdf5BasFile::qualityDependance()
{
    int numCalls = deletionsQV.size();
    if (subsititutionsQV.size() < numCalls) numCalls = subsititutionsQV.size();
    if (insertionsQV.size() < numCalls) numCalls = insertionsQV.size();
    //assert(numCalls == deletionsQV.size());

    unsigned int depend [intervals][intervals][intervals] = {};
    for ( int j=0 ; j<numCalls ; j++ )
    {
        for ( unsigned int del = 0 ; del < intervals ; del++ )
        {
            for ( unsigned int ins = 0 ; ins < intervals ; ins++ )
            {
                for (unsigned int  sub = 0 ; sub < intervals ; sub++ )
                {
                    if (dBound[del] <=  deletionsQV[j]
                            && deletionsQV[j] < dBound[del+1]
                            && iBound[ins] <= insertionsQV[j]
                            && insertionsQV[j] < iBound[ins+1]
                            && sBound[sub] <= subsititutionsQV[j]
                            && subsititutionsQV[j] < sBound[sub+1])
                    {
                        depend[del][ins][sub]++;
                    }
                }
            }
        }
    }

    ofstream dout(outPrefix + "Dependacies.csv", ios_base::out | ios_base::trunc);
    writeBoxSet(dout, depend, numCalls, 0,1,2);
    writeBoxSet(dout, depend, numCalls, 1,2,0);
    writeBoxSet(dout, depend, numCalls, 2,0,1);
    //writeBoxSet(dout, depend, numCalls, 1,0,2);
}

void Hdf5BasFile::writeBoxSet(ofstream& dout
                        , unsigned int depend[intervals][intervals][intervals]
                        , int numCalls, int boxI, int colI, int rowI)
{
    const string boxHead[intervals] = headers[boxI];
    const string colHead[intervals] = headers[colI];
    const string rowHead[intervals] = headers[rowI];

    unsigned int rowTotal[intervals] = {};
    unsigned int colTotal[intervals] = {};

    for ( unsigned int box = 0 ; box < intervals ; box++ )
    {
        for ( unsigned int row = 0 ; row < intervals ; row++ )
        {
            for ( unsigned int col = 0 ; col < intervals ; col ++ )
            {
                int i0 = (boxI == 0) ? box : (colI == 0) ? col : row;
                int i1 = (boxI == 1) ? box : (colI == 1) ? col : row;
                int i2 = (boxI == 2) ? box : (colI == 2) ? col : row;
                rowTotal[row] += depend[i0][i1][i2];
                colTotal[col] += depend[i0][i1][i2];
            }
        }
    }

    for ( unsigned int box = 0 ; box < intervals ; box++ )
    {
        unsigned int boxTotal = 0;
        for ( unsigned int row = 0 ; row < intervals ; row++ )
        {
            for ( unsigned int col = 0 ; col < intervals ; col ++ )
            {
                int i0 = (boxI == 0) ? box : (colI == 0) ? col : row;
                int i1 = (boxI == 1) ? box : (colI == 1) ? col : row;
                int i2 = (boxI == 2) ? box : (colI == 2) ? col : row;
                boxTotal += depend[i0][i1][i2];
            }
        }

        dout << boxHead[box];
        for ( unsigned int colh = 0 ; colh < intervals ; colh ++ )
            dout << '\t' << colHead[colh];
        dout << '\t' << "Observed" << '\t' << "Expected"
             << '\t' << "Total" << '\n';
        int colSum[intervals] = {};
        for ( unsigned int row = 0 ; row < intervals ; row++ )
        {
            dout << rowHead[row];
            int rowSum = 0;
            for ( unsigned int col = 0 ; col < intervals ; col ++ )
            {
                int i0 = (boxI == 0) ? box : (colI == 0) ? col : row;
                int i1 = (boxI == 1) ? box : (colI == 1) ? col : row;
                int i2 = (boxI == 2) ? box : (colI == 2) ? col : row;

                rowSum += depend[i0][i1][i2];
                colSum[col] += depend[i0][i1][i2];
                dout << '\t' << toPct(depend[i0][i1][i2], numCalls);
            }
            dout << '\t' << toPct(rowSum, numCalls);


            double expected = (double(boxTotal) / double(numCalls))
                            * (double(rowTotal[row]) / double(numCalls));
            expected = expected * double(100.0);
            dout << '\t' << expected;

            if( box == intervals-1)
                dout << '\t' << toPct(rowTotal[row], numCalls);
            dout <<'\n';
        }

        dout << "Observed";
        for ( unsigned int cols = 0 ; cols < intervals ; cols++ )
            dout << '\t' << toPct(colSum[cols], numCalls);
        dout << '\t' << toPct(boxTotal, numCalls);
        dout << '\n';

        dout << "Expected";
        for ( unsigned int cols = 0 ; cols < intervals ; cols++ )
        {
            double expected = (double(boxTotal) / double(numCalls))
                            * (double(colTotal[cols]) / double(numCalls));
            expected = expected * double(100.0);
            dout << '\t' << expected;
        }
        // dout << '\t' << toPct(boxTotal, numCalls);
        dout << '\n';
    }
    dout << "Total";
    for ( unsigned int cols = 0 ; cols < intervals ; cols++ )
        dout << '\t' << toPct(colTotal[cols], numCalls);
    dout << '\n' << '\n';
}

inline double toPct(int num, int denom)
{
    return double(100*double(num)/double(denom));
}




































