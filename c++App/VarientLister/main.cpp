#include <QCommandLineParser>
#include <QCommandLineOption>
#include <QCoreApplication>
#include <QString>
#include <fstream>
#include <string>
#include <cerrno>
#include "varientcaller.h"
#include "main.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QCommandLineParser clp;
    configCommandLineParser(clp);

    //stampy_sorted.bam -t Sc8915bp_cs.fasta -o varients.csv
    //string out_file = "varients.csv";
    string out_file = clp.value("o").toStdString();
    if (out_file == "stdout")
    {
        runProgram(clp.value("i").toStdString(),
                   clp.value("t").toStdString(),
                   clp.value("e").toInt(),
                   cout);
    }
    else
    {
        ofstream out_fstream;
        try
        {
            out_fstream.open(out_file.c_str(), ios_base::out | ios_base::trunc);
            runProgram(clp.value("i").toStdString(),
                       clp.value("t").toStdString(),
                       clp.value("e").toInt(),
                       out_fstream);
            out_fstream.close();
        }
        catch (...)
        {
            out_fstream.close();
            throw;
        }
    }

    return 0;// a.exec();
}

void configCommandLineParser(QCommandLineParser& parser)
{
    QCoreApplication::setApplicationName("VarientLister");
    QCoreApplication::setApplicationVersion("1.0.0.0");

    //QCommandLineParser parser;
    parser.setApplicationDescription("Generate pSNPs from BAM format file");
    parser.addHelpOption();
    parser.addVersionOption();

    //-i stampy_sorted.bam -t Sc8915bp_cs.fasta -o varients.csv
    //input Bam file
    const QCommandLineOption in_bam_op(QStringList() << "i" << "in-Bam",
                                   "Input BAM file to be searched for pSNPs",
                                   "filename", "stampy_sorted.bam");
    parser.addOption(in_bam_op);
    //input template fasta file
    const QCommandLineOption template_op(QStringList() << "t" << "template",
                                   "Input template fasta file",
                                   "filename", "Sc8915bp_cs.fasta");
    parser.addOption(template_op);
    //output filename
    const QCommandLineOption output_csv_op(QStringList() << "o" << "output",
                                   "Output csv filename",
                                   "filename",
//                                    "stdout");
                                    "varients.csv");
    parser.addOption(output_csv_op);
    //error threshold
    const QCommandLineOption err_th_op(QStringList() << "e" << "err-threshold",
                                   "Ignore reads with errors abvove threhold",
                                   "percentage", "10");
    parser.addOption(err_th_op);

    return;
}

void runProgram(const string& in_file,
                const string& tp,
                int err_t,
                ostream& out_stream)
{

    VarientCaller vc(in_file, tp, err_t, out_stream);
    vc.callVarants();
}


// http://insanecoding.blogspot.co.uk/2011/11/how-to-read-in-file-in-c.html
std::string get_file_contents(const char *filename)
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



















