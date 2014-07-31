#include <cassert>
#include <fstream>
#include "H5Cpp.h"
#include <H5IdComponent.h>
#include <H5Location.h>
#include <H5Object.h>
#include <H5DataType.h>
#include <H5AtomType.h>
#include <H5PredType.h>
#include "simbax.h"
#include "main.h"
#include "baxh5.h"
#include "constantcaller.h"
#include "bipolercaller.h"
#include "options.h"
#include "chrono"
#include "ctime"
#include "iostream"
#include <qdebug.h>

SimBax::SimBax(const string& t,
               const string &prefix,
               const int depth,
               unique_ptr<BaseCaller> baseCaller)
    : t(t)//Filename(tFilename)
    , baxFilename(prefix + ".bax.h5")
    , fastqFilename(prefix + ".fastq")
    , depth(depth)
    , baseCaller(move(baseCaller))
    , gen(rd())
{
    //baseCaller.reset(new BiPolerCaller(t));
    //baseCaller = move(baseCaller);
    //t = getFileContents(tFilename.c_str());
    //basesFromFasta(t);
}

SimBax::SimBax(const string &t, unique_ptr<BaseCaller> baseCaller)
    : t(t)
    , baxFilename(Options::get(Options::OutPrefix) + ".bax.h5")
    , fastqFilename(Options::get(Options::OutPrefix) + ".fastq")    
    , depth(stoul(Options::get(Options::Depth)))
    , readLen(stoul(Options::get(Options::Length)))
    , baseCaller(move(baseCaller))
    , gen(rd())
{
}

void SimBax::operator()()
{
    uniform_int_distribution<int> readStart(0, t.size());
    ofstream fastq(fastqFilename);

    resetStrings();
    const int numReads = t.size() * depth / readLen;

    int nextOut = numReads/10;
    int pct = 0;
    chrono::time_point<chrono::system_clock> start;
    start = chrono::system_clock::now();
    cout << "Inilisation finished, simulating reads.\n";
    
    for (int readNum = 0 ; readNum < numReads ; readNum++ )
    {
        long addedSoFar = baseCall.size();
        string fastqPhreds;
        makeRead(readLen, readStart(gen), fastqPhreds);
        const int lengthRead = baseCall.size() - addedSoFar;
        reads.push_back(lengthRead);
        writeFastq(baseCall.substr(addedSoFar, lengthRead)
                   ,fastqPhreds
                   ,fastq
                   ,readNum);

        if (readNum == nextOut)
        {
            chrono::time_point<chrono::system_clock> current;
            current = chrono::system_clock::now();
            chrono::duration<double> elapsed = start - current;
            nextOut += numReads/10;
            pct += 10;
            cout << pct << "% done. Time ellapsed :"
                 << elapsed.count() << " seconds\n";
        }

    }
    
    cout << "Wrinting data to file.\n";
    BaxH5 baxh5(baxFilename);
    baxh5.writeReads(baseCall
                    ,deletionQV
                    ,deletionTag
                    ,insertionQV
                    ,mergeQV
                    ,preBaseFrame
                    ,pulseIndex
                    ,substitutionQV
                    ,subsititutionTag
                    ,qualityValue
                    ,widthInFrame
                    ,reads);
}

void SimBax::makeRead(unsigned int readLen
                      , unsigned int startLocus
                      , string& fastqPhreds)
{
    baseCaller->setLocus(startLocus);
    for ( unsigned int i = 0 ; i < readLen ; i++ )
    {
        baseCall.push_back(baseCaller->getBaseCall());
        deletionQV.push_back(baseCaller->getDeletionQV());
        deletionTag.push_back(baseCaller->getDeletionTag());
        insertionQV.push_back(baseCaller->getInsrtionQV());
        qualityValue.push_back(baseCaller->getQualityValue());
        fastqPhreds.push_back(baseCaller->getQualityValue() + 33);
        mergeQV.push_back(baseCaller->getMergeQV());
        preBaseFrame.push_back(baseCaller->getPreBaseFrame());
        pulseIndex.push_back(baseCaller->getPulseIndex());
        substitutionQV.push_back(baseCaller->getSubstitutionQV());
        subsititutionTag.push_back(baseCaller->getSubstitutionTag());
        widthInFrame.push_back(baseCaller->getWidthInFrames());
        baseCaller->next();
    }

}

inline void SimBax::resetStrings()
{
    baseCall.clear(); baseCall.resize(0);
    deletionQV.clear(); deletionQV.resize(0);
    deletionTag.clear(); deletionTag.resize(0);
    insertionQV.clear(); insertionQV.resize(0);
    qualityValue.clear(); qualityValue.resize(0);
    mergeQV.clear(); mergeQV.resize(0);
    preBaseFrame.clear(); preBaseFrame.resize(0);
    pulseIndex.clear(); pulseIndex.resize(0);
    substitutionQV.clear(); substitutionQV.resize(0);
    subsititutionTag.clear(); subsititutionTag.resize(0);
    widthInFrame.clear(); widthInFrame.resize(0);
    reads.clear(); reads.resize(0);
}

void SimBax::writeFastq(const string &r
                        , const string &q
                        , ofstream& fastq
                        , int readNum)
{
    fastq << "@S1_" << std::to_string(readNum)  << '\n';
    fastq << r << '\n';
    fastq << "+S1_" << std::to_string(readNum) << '\n';
    fastq << q << '\n';
}



































