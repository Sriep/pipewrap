#include <exception>
#include <array>
#include <cstddef>
#include <iomanip>
#include <cmath>
#include "iostream"
#include "hdf5basfile.h"
#include "main.h"
#include "options.h"


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

Hdf5BasFile::Hdf5BasFile()
    : H5File(Options::get(Options::InBax), H5F_ACC_RDONLY)
    , outPrefix(Options::get(Options::OutPrefix))
    , readsDS(openDataSet("/PulseData/BaseCalls/ZMW/NumEvent"))
    , deletionDS(openDataSet("/PulseData/BaseCalls/DeletionQV"))
    , insertionDS(openDataSet("/PulseData/BaseCalls/InsertionQV"))
    , substitutionDS(openDataSet("/PulseData/BaseCalls/SubstitutionQV"))
    , qualityValueDS(openDataSet("/PulseData/BaseCalls/QualityValue"))
    , mergeDS(openDataSet("/PulseData/BaseCalls/MergeQV"))
{
    if (outPrefix=="") outPrefix = "data_" + Options::get(Options::InBax);
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
    cout << "Reading quality arrays\n";
    populateArray(deletionsQV, deletionDS);
    populateArray(insertionsQV, insertionDS);
    populateArray(subsititutionsQV, substitutionDS);    
    populateArray(qualtyValue, qualityValueDS);
    populateArray(mergeQV, mergeDS);
}

void Hdf5BasFile::operator()()
{
    if (Options::flag(Options::Distribution))
        writeDistribution();
    if (Options::flag(Options::ConfusionTable))
        writeQualityDependance();
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

void Hdf5BasFile::writeDistribution()
{
    cout << "About to caclulate deletion distribution\n";
    const array<unsigned int,101>
            delDist(qualityDist(deletionsQV));
    cout << "About to caclulate insertion distribution\n";
    const array<unsigned int,101>
            insDist(qualityDist(insertionsQV));
    cout << "About to caclulate substitution distribution\n";
    const array<unsigned int,101>
            subDist(qualityDist(subsititutionsQV));
    cout << "About to caclulate merge distribution\n";
    const array<unsigned int,101>
            mergeDist(qualityDist(mergeQV));
    const array<unsigned int,101>
            aggreDist(qualityDist(qualtyValue));

    cout << "Done calculating, writing distributinos to file\n";
    ofstream dout(outPrefix + "_dists.csv", ios_base::out | ios_base::trunc);
    dout << "phred\t" << "DeletionQV\t" <<  "InsertionQV\t" << "MergeQV\t"
         << "SubstitutionQV\t" << "QualityValue\n";
    for (unsigned int i = 0 ; i < 101 ; i++)
    {
        dout << i << '\t' << delDist[i] << '\t' << insDist[i]
             << '\t' <<  mergeDist[i] << '\t' << subDist[i]
             << '\t' << aggreDist[i] << '\n';
    }

}

const array<unsigned int,101> Hdf5BasFile::qualityDist(
                                            vector<unsigned char>& dataArray)
{
    array<unsigned int,101> dist = {{}};
    const int numCalls = dataArray.size();
    for ( int i = 0 ; i < numCalls ; i++ )
    {
        if (dataArray[i] < 100)
            dist[dataArray[i]]++;
        else
            dist[100]++;
    }

    //ofstream dout(outPrefix + outFile, ios_base::out | ios_base::trunc);
    for (unsigned int i = 0 ; i < 101 ; i++)
    {
        double amount = double (100*(double)dist[i])/(double)numCalls;
        //dout << i << '\t' << amount << "\n";
        dist[i] = (int) amount;
    }
    return dist;
}

void Hdf5BasFile::writeQualityDependance()
{
    cout << "Calculating dependancies\n";
    unsigned int numCalls = deletionsQV.size();
    if (subsititutionsQV.size() < numCalls) numCalls = subsititutionsQV.size();
    if (insertionsQV.size() < numCalls) numCalls = insertionsQV.size();
    //assert(numCalls == deletionsQV.size());

    unsigned int depend [intervals][intervals][intervals] = {};
    for ( unsigned int j=0 ; j<numCalls ; j++ )
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
    cout << "Writing contignecy table to file\n";
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




































