#ifndef PIPE_H
#define PIPE_H
/*
#include <QObject>
#include <QDateTime>
#include <QStringList>
#include <QProcess>
#include <QString>

class WorkflowTreeView;
class PipeStatus;

class Pipe : public QProcess
{
    Q_OBJECT
public:
    Pipe(const QStringList& commands, WorkflowTreeView* parent);
    void operator() ();

signals:
    void finishedAll();

private slots:
    void processStdErrSlot();
    void finishedSlot(int, ExitStatus);
    void errorSlot(QProcess::ProcessError error);

private:
    void nextCommand();

    const QStringList commands;
    int currentCommand = 0;
    QDateTime timePipeStarted;
    WorkflowTreeView* parentView;
    QProcess* process;
    PipeStatus* pipeStatus;
};*/

#endif // PIPE_H
