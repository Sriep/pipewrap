#ifndef PIPESTATUS_H
#define PIPESTATUS_H

#include <QTextBrowser>
#include <QProcess>
#include <QString>
#include <QTime>

class WorkflowTreeView;
class QProcess;

class PipeStatus : public QTextBrowser
{
     Q_OBJECT
public:
    PipeStatus(WorkflowTreeView *parent,  QProcess* process);

    void pipeStarted();
    void pipeFinished();
    void commandStart(QString command);
    void commandFinish(int exitCode, QProcess::ExitStatus exitStatus);
    void fatelError(QProcess::ProcessError error);
    void processStdErrSlot(const QString& message);
    void processEnded();
public slots:
    void processStdErrSlot();
    void processStdOutSlot();

private:
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
