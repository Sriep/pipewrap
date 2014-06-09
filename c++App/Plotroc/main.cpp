#include <QCoreApplication>
#include <QApplication>
#include "plotwindow.h"
#include "main.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QCommandLineParser clp;
    configCommandLineParser(clp);
    clp.process(a);

    PlotWindow pw(clp);
    pw.show();
    return a.exec();
}

void configCommandLineParser(QCommandLineParser& parser)
{
    QCoreApplication::setApplicationName("VarientLister");
    QCoreApplication::setApplicationVersion("1.0.0.1");

    parser.setApplicationDescription("Create ROC curves from list of pvalues");
    parser.addHelpOption();
    parser.addVersionOption();
    parser.addOption(inPvPath);
    parser.addOption(inpSNPs);
    return;
}
