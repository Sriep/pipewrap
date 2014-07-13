#ifndef PIPESTATUS_H
#define PIPESTATUS_H

#include <QTextBrowser>
#include <QProcess>
#include <QString>
#include <QTime>
#include "workflowtreeview.h"

class WorkflowTreeView;
class QProcess;

class PipeStatus : public QTextBrowser
{
     Q_OBJECT
public:
    PipeStatus(WorkflowTreeView *parent,  QProcess* process);

    void pipeStarted();
    void pipeFinished();
    void commandStart(const QString &command);
    void commandFinish(int exitCode, QProcess::ExitStatus exitStatus);
    void fatelError(QProcess::ProcessError error);
    void processStdErrSlot(const QString& message);
    void processEnded();
public slots:
    void processStdErrSlot();
    void processStdOutSlot();

private:
    struct commandData
    {
        QString cmdString;
        int exitCode;
        bool finishedNormaliy;
        int timeMilliseconds;
    };
    QList<commandData> pipeLog;
    QString currCommand;

    void pipeHead(const QString& text);
    void currentTime(const QString& text);
    void elapsedTime(const QString& text);
    void commandHead(const QString& text);
    void shellCommand(const QString& text);
    void returnError(const QString &text);
    void fatelError(const QString &text);
    void stdError(const QString& text);
    void stdOut(const QString& text);
    void noError(const QString& text);
    void finishedPipe(const QString& text);
    QString removeEchos(const QString& echoedString);

    QTime pipeTimer;
    QTime commandTimer;
    QString pipeName;
    bool errorLogged = false;
    QProcess* process;
    bool pipeIsFinished = false;
    bool processIsTerminated = false;

    QString defaultFontFamily;
    qreal defaultPoitSize;
    int defaultWeight;

};

#endif // PIPESTATUS_H
