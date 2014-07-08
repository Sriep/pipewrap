/*#include <QFile>
#include <QTextStream>
#include <QObject>
#include <qdebug.h>

#include "pipestatus.h"
#include "pipe.h"
#include "workflowtreeview.h"


Pipe::Pipe(const QStringList& commands, WorkflowTreeView* parent)
    : commands(commands)
    , parentView(parent)
{
}

void Pipe::operator()()
{
    //emit executeAvailable(false);
    //timePipeStarted = QDateTime::currentDateTime();
#ifdef QT_DEBUG
    for ( int i = 0 ; i < commands.size() ; i++ )
        qDebug() << commands[i];
#endif
    execute("PATH=PATH:$PWD");

    setReadChannel(QProcess::StandardError);
    pipeStatus = new PipeStatus(parentView);
    //pipeStatus->setWindowTitle("Pipe status");
    connect(qobject_cast<QProcess*>(this), SIGNAL(QProcess::readyReadStandardError()),
            this, SLOT(processStdErrSlot()));

    connect(qobject_cast<QProcess*>(this), SIGNAL(QProcess::error(QProcess::ProcessError error)),
            this, SLOT(errorSlot(QProcess::ProcessError error)));

    connect(qobject_cast<QProcess*>(this), SIGNAL(QProcess::finished(int, QProcess::ExitStatus)),
            this, SLOT(finishedSlot(int, QProcess::ExitStatus)));

    connect(qobject_cast<QProcess*>(this), SIGNAL(QProcess::finished(int, QProcess::ExitStatus)),
            pipeStatus, SLOT(commandFinish(int, QProcess::ExitStatus)));

    //connect(this, SIGNAL(finished(int, QProcess::ExitStatus)),
    //        pipeStatus, SLOT(processFinishedSlot(int, QProcess::ExitStatus)));
    //connect(this, SIGNAL(finished(int, QProcess::ExitStatus)),
    //        parentView, SLOT(processFinishedSlot(int, QProcess::ExitStatus)));

    //connect(this, SIGNAL(error(QProcess::ProcessError)),
    //        pipeStatus, SLOT(processErrorSlot(QProcess::ProcessError)));

    pipeStatus->show();
    pipeStatus->pipeStarted();
    currentCommand = 0;
    nextCommand();

    //start("./shell_script.sh");
}

void Pipe::processStdErrSlot()
{
    QByteArray ba = process->readAllStandardError();
    QString message(ba);
   // emit  processStdErr(message);
    pipeStatus->append(message);
}

void Pipe::finishedSlot(int exitCode, QProcess::ExitStatus exitStatus)
{
    pipeStatus->commandFinish(exitCode, exitStatus);
    nextCommand();
}

void Pipe::nextCommand()
{
    if (commands.size() == currentCommand)
    {
        pipeStatus->pipeFinished();
        emit(finishedAll());
    }
    else
    {
        pipeStatus->commandStart(commands[currentCommand]);
        start(commands[currentCommand]);
        currentCommand++;
    }
}

void Pipe::errorSlot(QProcess::ProcessError error)
{
    pipeStatus->fatelError(error);
    emit(finishedAll());
}
*/


































