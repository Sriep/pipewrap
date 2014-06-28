#include <QCoreApplication>
#include <QCommandLineParser>
#include <cmath>
#include <fstream>
#include <cerrno>

#include "main.h"
#include "simbax.h"


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QCommandLineParser clp;
    configCommandLineParser(clp);
    clp.process(a);

    try
    {
        runProgram(clp);
    }
    catch (...)
    {
        return 1;
    }
    return 0;
}

void configCommandLineParser(QCommandLineParser& parser)
{
    QCoreApplication::setApplicationName("VarientLister");
    QCoreApplication::setApplicationVersion("1.1.0.3");

    parser.setApplicationDescription("Generate pSNPs from BAM format file");
    parser.addHelpOption();
    parser.addVersionOption();
    parser.addOption(inFastaTemplate);
    parser.addOption(depth);
    parser.addOption(outFilePrefix);

    return;
}

void runProgram(QCommandLineParser& clp)
{
    SimBax simBax(clp.value(inFastaTemplate).toStdString(),
                  clp.value(outFilePrefix).toStdString(),
                  clp.value(depth).toInt());
    simBax();
}

long double phred2prob(int phred)
{
    return pow(Ten, -((long double) phred)/Ten);
}

int prob2phred(long double prob)
{
    return -Ten*log10(prob);
}

// http://insanecoding.blogspot.co.uk/2011/11/how-to-read-in-file-in-c.html
std::string getFileContents(const char *filename)
{
  std::ifstream in(filename, std::ios::in | std::ios::binary);
  if (in)
  {
    std::string contents;
    in.seekg(0, std::ios::end);
    contents.resize(in.tellg());
    in.seekg(0, std::ios::beg);
    in.read(&contents[0], contents.size());
    in.close();
    return(contents);
  }
  throw(errno);
}


void basesFromFasta(string& t)
{
    if ('>' == t[0])
        t.erase(0, t.find_first_of('\n'));

    //http://en.cppreference.com/w/cpp/algorithm/remove
    t.erase(std::remove_if(t.begin(),
                              t.end(),
                              [](char x){return std::isspace(x);}),
               t.end());
}










