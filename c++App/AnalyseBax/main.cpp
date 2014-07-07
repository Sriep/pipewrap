#include <QCoreApplication>
#include <QString>
#include "main.h"
#include "H5Cpp.h"
#include "hdf5basfile.h"
#include "options.h"

using namespace std;
using namespace H5;

int main(int argc, char *argv[])
{
     Options::readOptions(argc, argv);
    //QCoreApplication a(argc, argv);
    //QCommandLineParser clp;
    //configCommandLineParser(clp);
    //clp.process(a);

    try
    {
        runProgram();
    }
    catch (...)
    {
        return 1;
    }
    return 0;
}
/*
void configCommandLineParser(QCommandLineParser& parser)
{
    QCoreApplication::setApplicationName("AnalyseBax");
    QCoreApplication::setApplicationVersion("1.0.0.0");
    parser.setApplicationDescription("Genearate some basic stats on h5 files");
    parser.addHelpOption();
    parser.addVersionOption();
    parser.addOption(inBaxh5);
}*/

void runProgram()
{
    //string filename = "m130427_030153_42141_c100519052550000001823079909281322_s1_p0.1.bax.h5";
    //string filename = "m130427_030153_42141_c100519052550000001823079909281322_s1_p0.2.bax.h5";
    //string filename = "m130427_030153_42141_c100519052550000001823079909281322_s1_p0.3.bax.h5";
    //string filename = "m140423_202613_42175_c100636102550000001823121909121410_s1_p0.1.bax.h5";
    //string filename = "m140424_014428_42175_c100636102550000001823121909121411_s1_p0.1.bax.h5";
    //string filename = "new2.bax.h5";
    //string filename = "ss.bas.h5";
    //Hdf5BasFile baxh5(filename);

    Hdf5BasFile baxh5;
    baxh5();
}


