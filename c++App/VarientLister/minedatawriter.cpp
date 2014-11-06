#include <cmath>

#include "minedatawriter.h"
#include "pvalues.h"
#include "locusinfo.h"


MineDataWriter::MineDataWriter(const string &MineDataFilename
                               , const vector<unique_ptr<LocusInfo> > &als_info
                               , const string &source
                               , const string& reference
                               , const string &referenceFile)
  : MineDataFilename(MineDataFilename)
  , als_info(als_info)
  , vSource(source)
  , ref(reference)
  , refFilename(referenceFile)
{
}

void MineDataWriter::operator()()
{
    mout.open(MineDataFilename, ios_base::out | ios_base::app);
    for (unsigned int pos = 10 ; pos < ref.length()- 10 ; pos++)
    {
        int logPV = log10(als_info[pos]->getPValue(PValues::KnownFrequency));
        if (logPV < pvThreshold)
        {

        }
    }
}
