#include <QCoreApplication>
#include "main.h"
#include "plotwindow.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QCommandLineParser clp;
    configCommandLineParser(clp);
    clp.process(a);

    PlotWindow mw;
    //mw.show();
    return a.exec();
}

void configCommandLineParser(QCommandLineParser& parser)
{
    QCoreApplication::setApplicationName("VarientLister");
    QCoreApplication::setApplicationVersion("1.0.0.0");

    //QCommandLineParser parser;
    parser.setApplicationDescription("Create ROC curves from list of pvalues");
    parser.addHelpOption();
    parser.addVersionOption();
    parser.addOption(inSuffix);
    parser.addOption(inpSNPs);
    return;
}


