#ifndef GENERATEDPSNPS_H
#define GENERATEDPSNPS_H
#include <string>
#include <vector>
#include <set>
using namespace std;

class GeneratedPsnps
{
public:
    GeneratedPsnps(const string& psnpFilename);
    bool hasPosition(int pos) const;

private:
    vector<int> getPosition() const;
    vector<int> getFrequency() const;
    string getRefBase() const;
    string getPsnp() const;
    uint length() const;

    void init();

    set<int> positions;
    vector<int> position;
    vector<int> frequency;
    string refBase;
    string psnp;
    const string& inFilemane;
};

#endif // GENERATEDPSNPS_H
