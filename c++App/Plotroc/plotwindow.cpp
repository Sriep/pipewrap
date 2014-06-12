#include <QFont>
#include <QPainter>
#include <QPageLayout>
#include <QMarginsF>
#include <QMargins>
#include <QPageSize>
#include <QVariant>
#include <QMap>
#include <algorithm>
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
    init();
}

PlotWindow::~PlotWindow()
{
}

void PlotWindow::init()
{
    double width = 800;
    double height = 600;
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

    setSecondAxis(rocData.getFrequencies());
    customPlot->yAxis2->setAutoTicks(false);
    customPlot->yAxis2->setAutoTickLabels(false);
    customPlot->yAxis2->setRange(0, 5);
    customPlot->yAxis2->setTickVector(ticPositions);
    customPlot->yAxis2->setTickVectorLabels(ticLabels);
    customPlot->yAxis2->setLabel("Percentage pSNP frequency");
    customPlot->yAxis2->setVisible(true);

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
    writeToPdf("ROCcurve.pdf");
}

void PlotWindow::writeToPdf(const QString& fileName)
{
    QPdfWriter* pdfWriter = new QPdfWriter(fileName);
    //pdfWriter->setCreator();
    //pdfWriter->setPageLayout();
    QMargins margin = textEdit->contentsMargins();
    QMarginsF marginsF(margin);
    pdfWriter->setPageMargins(marginsF);
    pdfWriter->setPageSize(QPageSize(QPageSize::A3));
    //pdfWriter->setResolution();
    pdfWriter->setTitle("ROC curve");

    //pdfWriter->setFullPage(true);
    //pdfWriter.setPaperSize(QPrinter::A4);
    //pdfWriter.setOrientation(QPrinter::Portrait);
    textEdit->print(pdfWriter);

    //QString fileName2 = QFileDialog::getSaveFileName(this, "Save document...", qApp->applicationDirPath(), "*.pdf");
    QString fileName2 = "RocCurve2.pdf";
 /*
    QPrinter printer;
    printer.setFullPage(true);
    printer.setPaperSize(QPrinter::A4);
    printer.setOrientation(QPrinter::Portrait);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(fileName2);
    textEdit->document()->print(&printer);
    QPdfWriter* pdfWriter2 = new QPdfWriter(fileName2);
    QPageLayout* newPageLayout = new QPageLayout;
    pdfWriter2->setPageLayout(*newPageLayout)  ;

    QPainter painter(pdfWriter2);
    painter.begin(pdfWriter2);
    double xscale = pdfWriter2->pageRect().width()/double(textEdit->width());
    double yscale = pdfWriter2->pageRect().height()/double(textEdit->height());
    double scale = qMin(xscale, yscale);
    painter.translate(pdfWriter2->paperRect().x() + printer.pageRect().width()/2,
                       pdfWriter2->paperRect().y() + printer.pageRect().height()/2);
    painter.scale(scale, scale);
    painter.translate(-width()/2, -height()/2);
    textEdit->render(&painter);

    textEdit->print(pdfWriter2);*/
}

void PlotWindow::setSecondAxis(QList<double> frequencies)
{
    //QList<int> bins;
    //QList<int> binCount;
    //QMap<double,int> ;
    //QVector<int> bins(101);
    //for (int i = 1 ; i < frequencies.size() ; i++ )
    //    if (0 <= frequencies[i] && frequencies[i] <=100)
    //       bins[frequencies[i]]++;
    ticPositions.clear();
    //ticPositions << 0.1 << 0.2 << 0.3 << 0.4 << 0.5
    //             << 0.6 << 0.7 << 0.8 << 0.9 << 1.0;
    ticPositions << 0.0 << 0.5 << 1 << 1.5 << 2 << 2.5
                 << 3 << 3.5 << 4 << 4.5 << 5;
    ticLabels.clear();
    std::sort(frequencies.begin(), frequencies.end());
    for ( int j = 10 ; j>= 1 ; j-- )
    {
        int index = j * frequencies.size()/10;
        if (frequencies.size() == index) index--;
        QVariant label(frequencies[index]);
        ticLabels.append(label.toString());
    }
}






































