#ifndef PIPESTATUS_H
#define PIPESTATUS_H

#include <QTextBrowser>
#include <QProcess>
#include <QString>

class PipeStatus : public QTextBrowser
{
     Q_OBJECT
public:
    PipeStatus(QWidget* parent = 0);

public slots:
    void stdErrOutSlot(const QString& message);
    void processFinishedSlot(int,QProcess::ExitStatus);
    void processErrorSlot(QProcess::ProcessError error);
};

#endif // PIPESTATUS_H
