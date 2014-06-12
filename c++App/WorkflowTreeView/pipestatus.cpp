#include "pipestatus.h"

PipeStatus::PipeStatus(QWidget *parent)
    :QTextBrowser(parent)
{
}

void PipeStatus::stdErrOutSlot(const QString &message)
{
    //QByteArray ba = process->readAllStandardError();
    append(message);
}

void PipeStatus::processFinishedSlot(int, QProcess::ExitStatus)
{
    append(QString("!!!!!Finshed OK!!!!!"));
}

void PipeStatus::processErrorSlot(QProcess::ProcessError error)
{
    append(QString("!!!!!Finished Error!!!!!"));
}
