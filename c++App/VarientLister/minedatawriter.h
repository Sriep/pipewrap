#ifndef MINEDATAWRITER_H
#define MINEDATAWRITER_H
#include <string>
#include <vector>
#include <memory>
#include <fstream>

using namespace std;
class LocusInfo;

class MineDataWriter
{
public:
    MineDataWriter(const string& MineDataFilename
              ,const vector<unique_ptr<LocusInfo>>&als_info
              , const string& source
              , const string& ref
              , const string& refFilename);
    void operator() ();

private:
    const string MineDataFilename;
    const vector<unique_ptr<LocusInfo>>& als_info;
    string vSource;
    string ref;
    string refFilename;
    ofstream mout;
    int pvThreshold = -15;
};

#endif // MINEDATAWRITER_H
