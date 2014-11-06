#ifndef VARIENTCALLERHDF5_H
#define VARIENTCALLERHDF5_H
#include "varientcaller.h"

using namespace std;
using namespace BamTools;

class Hdf5BasFile;
class LocusInfo;

class VarientCallerHdf5 : public VarientCaller
{
public:
    VarientCallerHdf5();
    ~VarientCallerHdf5();
    void operator() ();
private:
    virtual bool goodEnoughRead(unsigned short position
                        , const BamAlignment& al
                        , const Hdf5BasFile* baxFile);
    virtual void populateLociInfo();
    string basH5file;

};

#endif // VARIENTCALLERHDF5_H
