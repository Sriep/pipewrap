#ifndef BASECALLER_H
#define BASECALLER_H
#include <string>
#include <random>

using namespace std;

class BaseCaller
{
public:

    BaseCaller(const string& t, unsigned int locus = 0);

    virtual void next();
    void setLocus(unsigned int value);

    unsigned int getLocus() const;

    unsigned char getBaseCall() const;
    unsigned char getDeletionQV() const;
    unsigned char getDeletionTag() const;
    unsigned char getInsrtionQV() const;
    unsigned char getMergeQV() const;
    unsigned short getPerBaseFrame() const;
    unsigned int getPulseIndex() const;
    unsigned char getQualityValue() const;
    unsigned char getSubstitutionQV() const;
    unsigned char getSubstitutionTag() const;
    unsigned short getWidthInFrames() const;

protected:
    enum BaseErrorType {
        Deletion,
        Insertion,
        Merge,
        Substitution
    };

    virtual void simulateData() = 0;
    bool mergePosible(unsigned int tpos) const;
    char randomBase(char notThis = '-');
    void nextLocus();


    const string& t;
    unsigned int locus = 0;
    bool moveLocus = true;

    unsigned char baseCall = 0;
    unsigned char deletionQV = 5;
    unsigned char deletionTag = 0;
    unsigned char insrtionQV = 5;
    unsigned char mergeQV = 100;
    unsigned short perBaseFrame = 0;
    unsigned int  pulseIndex = 0;
    unsigned char qualityValue = 3;
    unsigned char substitutionQV = 20;
    unsigned char substitutionTag = 0;
    unsigned short widthInFrames = 0;

    std::random_device rd;
    std::mt19937 gen;

    uniform_int_distribution<short> rndBase;
    uniform_real_distribution<long double> rndProb;

};

#endif // BASECALLER_H






















