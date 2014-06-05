#include <QFont>
#include "plotwindow.h"
#include "qcpdocumentobject.h"

//PlotWindow::PlotWindow(QWidget *parent) :
//    QMainWindow(parent)
//{
//    Init();
//}

PlotWindow::PlotWindow(QCommandLineParser& clp, QWidget *parent)
    : QMainWindow(parent), clp(clp), rocData(clp)
{
    Init();
}

PlotWindow::~PlotWindow()
{
}

void PlotWindow::Init()
{
    double width = 900;
    double height = 700;
    resize(width, height);
    textEdit = new QTextEdit;
    setCentralWidget(textEdit);
    QCustomPlot* customPlot = new QCustomPlot();
    customPlot->legend->setVisible(true);
    QFont legendFont = font();
    legendFont.setPointSize(9); // and make a bit smaller for legend
    customPlot->legend->setFont(legendFont);
    customPlot->legend->setBrush(QBrush(QColor(255,255,255,230)));

    for (int p = 0 ; p < rocData.getPVFilenames().size() ; p++)
    {
        // create graph and assign data to it:
        customPlot->addGraph();
        customPlot->graph(p)->setPen(QPen((Qt::GlobalColor)(Qt::lightGray+p+1)));
        customPlot->graph(p)->setScatterStyle(QCPScatterStyle::ssCross);
        customPlot->graph(p)->setName(rocData.getPVFilenames().at(p));
        customPlot->graph(p)->setData(rocData.getFprs(p), rocData.getTprs(p));
    }
    // give the axes some labels:
    customPlot->xAxis->setLabel("FPR");
    customPlot->yAxis->setLabel("TPR");
    // set axes ranges, so we see all data:
    customPlot->xAxis->setRange(0, 1);
    customPlot->yAxis->setRange(0, 1);


    // Register the plot document object (only needed once, no matter how many
    // plots will be in the QTextDocument):
    QCPDocumentObject *interface = new QCPDocumentObject(this);
    textEdit->document()->documentLayout()
            ->registerHandler(QCPDocumentObject::PlotTextFormat, interface);

    QTextCursor cursor = textEdit->textCursor();
    // insert the current plot at the cursor position. QCPDocumentObject::generatePlotFormat creates a
    // vectorized snapshot of the passed plot (with the specified width and height) which gets inserted
    // into the text document.
    cursor.insertText(QString(QChar::ObjectReplacementCharacter),
                      QCPDocumentObject::generatePlotFormat(customPlot,
                                                            width,
                                                            height));

    textEdit->setTextCursor(cursor);
}
