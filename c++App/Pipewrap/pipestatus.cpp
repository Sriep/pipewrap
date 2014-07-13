#include <QVariant>
#include <QPdfWriter>
#include <QTextCursor>
#include <QTextBlock>
#include <QTextBlockFormat>
#include <QByteArray>
#include <QProcess>
#include <QStringList>
#include <QTextStream>

#include "pipestatus.h"
#include "workflowtreeview.h"

PipeStatus::PipeStatus(WorkflowTreeView *parent, QProcess* process)
    :QTextBrowser()
    , pipeName(NULL == parent ? "" : parent->name())
    , process(process)
{
    defaultFontFamily = fontFamily();
    defaultPoitSize = fontPointSize();
    defaultWeight= fontWeight();
    resize(800, 600);
}

void PipeStatus::pipeStarted()
{
    pipeTimer.start();
    commandTimer.start();
    setWindowTitle("Pipe output log");
    setDocumentTitle(pipeName + " log");
    QDateTime time = QDateTime::currentDateTime();
    pipeHead(tr("Start pipe ") + pipeName);
    currentTime(tr("Pipe started at ") + time.toString() + '\n');
}

void PipeStatus::pipeFinished()
{
    int milliseconds = pipeTimer.elapsed();
    QDateTime time = QDateTime::currentDateTime();
    finishedPipe(pipeName + " pipe finished");
    currentTime(tr("Pipe finished at ") + time.toString());
    elapsedTime(tr("Total time taken ") + QVariant(milliseconds).toString()
                + tr(" milliseconds"));
    if (errorLogged) returnError(tr("There were errors logged"));
    else noError(tr("There were no errors logged"));

    QPdfWriter* pdfWriter = new QPdfWriter(pipeName + "_log.pdf");
    pdfWriter->setPageSize(QPageSize(QPageSize::A3));
    pdfWriter->setTitle(pipeName + "22_log.pdf");
    print(pdfWriter);

    QFile file("PipeLog.tsv");
    file.open(QIODevice::WriteOnly | QIODevice::Truncate);
    QTextStream out(&file);
    out << tr("Command\tExit code\tExit status\tTime ms\n");
    for ( int i = 0 ; i < pipeLog.size() ; i++ )
    {
        out << '\"' << pipeLog[i].cmdString << '\"'
            << '\t' << pipeLog[i].exitCode
            << '\t' << pipeLog[i].finishedNormaliy
            << '\t' << pipeLog[i].timeMilliseconds << '\n';
    }
    file.close();
}

void PipeStatus::commandStart(const QString& command)
{
    QDateTime time = QDateTime::currentDateTime();
    commandHead(tr("Start new command"));
    currentTime(tr("Command started at ") + time.toString());
    QString echolessCmd = removeEchos(command);
    shellCommand(echolessCmd);
    append("...");
    currCommand = (echolessCmd == command)
                ? command
                : echolessCmd.left(echolessCmd.indexOf('\n'));
    commandTimer.restart();
}

QString PipeStatus::removeEchos(const QString& echoedString)
{
    QStringList commands = echoedString.split('\n', QString::SkipEmptyParts);
    QStringList noEchos;
    for ( int i = 0 ; i < commands.size() ; i++ )
    {
        if (!commands[i].startsWith("echo "))
        {
            noEchos << commands[i];
        }
    }
    return noEchos.join('\n');
}

void PipeStatus::commandFinish(int exitCode, QProcess::ExitStatus exitStatus)
{
    append("");
    int milliseconds = commandTimer.elapsed();
    QDateTime time = QDateTime::currentDateTime();
    currentTime(tr("Command finshied at ") + time.toString());
    elapsedTime(tr("Time taken ") + QVariant(milliseconds).toString() + " milliseconds");

    if (currCommand.size() > 0)
    {
        commandData newCmd =
        {
            currCommand
            , exitStatus
            , (QProcess::NormalExit == exitStatus)
            , milliseconds
        };
        pipeLog << newCmd;
    }

    if (-1 == exitCode)
    {
        noError(tr("Exit code unkown. Part of compund statment."));
    }
    else
    {
        if (0 == exitCode)
            noError(tr("Exit code was 0"));
        else
            returnError(tr("Exit code was ") + QVariant(exitCode).toString());
        if (QProcess::NormalExit == exitStatus)
            noError(tr("The command exited normally"));
        else
            fatelError(tr("The command crashed"));
        errorLogged = 0 != exitCode  || QProcess::NormalExit != exitStatus;
        append("\n");
    }
}

void PipeStatus::fatelError(QProcess::ProcessError error)
{
    errorLogged = true;
    append(tr("Pipe finshed with fatel error"));
    switch (error)
    {
    case QProcess::FailedToStart:
        fatelError(tr("The process failed to start. Either the invoked program "
               "is missing, or you may have insufficient permissions to invoke "
               "the program."));
        break;
    case QProcess::Crashed:
        fatelError(tr("The process crashed some time after starting "
                      "successfully."));
        break;
    case QProcess::Timedout:
        fatelError(tr("The last waitFor...() function timed out. The state of "
               "QProcess is unchanged, and you can try calling "
               "waitFor...() again."));
        break;
    case QProcess::WriteError:
        fatelError(tr("An error occurred when attempting to write to the "
               "process. For example, the process may not be running, or it "
               "may have closed its input channel."));
        break;
    case QProcess::ReadError:
        fatelError(tr("An error occurred when attempting to read from the "
               "process. For example, the process may not be running."));
        break;
    case QProcess::UnknownError:
    default:
        fatelError(tr("An unknown error occurred. This is the default return "
               "value of error()."));
    }
    pipeFinished();
}

void PipeStatus::processStdErrSlot(const QString &message)
{
    stdError(message);
}

void PipeStatus::processEnded()
{
    finishedPipe(tr("Process ended."));
}

void PipeStatus::processStdErrSlot()
{
    //QByteArray ba = process->readAllStandardError();
    //stdError(ba);
    QDateTime time = QDateTime::currentDateTime();
    process->setReadChannel(QProcess::StandardError);
    qint64 bytesAvailable = process->bytesAvailable();
    QByteArray ba = process->read(bytesAvailable);    
    stdError(ba);
    currentTime(tr("Std error above recieved at ") + time.toString());
}

void PipeStatus::processStdOutSlot()
{
    //QByteArray ba = process->readAllStandardOutput();
    //stdOut(ba);
    QDateTime time = QDateTime::currentDateTime();
    process->setReadChannel(QProcess::StandardOutput);
    qint64 bytesAvailable = process->bytesAvailable();
    QByteArray ba = process->read(bytesAvailable);
    stdOut(ba);
    currentTime(tr("Std output above recieved at ") + time.toString());
}

void PipeStatus::pipeHead(const QString &text)
{
    setTextColor(Qt::black);
    setFontPointSize(15);
    setFontWeight(100);
    append(text);
    setFontPointSize(0);
    setFontWeight(0);
}

void PipeStatus::currentTime(const QString &text)
{
    setTextColor(Qt::gray);
    setAlignment(Qt::AlignLeft);
    append(text);
}

void PipeStatus::elapsedTime(const QString &text)
{
    setTextColor(Qt::darkGray);
    setAlignment(Qt::AlignLeft);
    append(text);
}

void PipeStatus::commandHead(const QString &text)
{
    setTextColor(Qt::black);
    setAlignment(Qt::AlignLeft);
    append(text);
}

void PipeStatus::shellCommand(const QString &text)
{
    setTextColor(Qt::blue);
    setAlignment(Qt::AlignLeft);
    append(text);
}

void PipeStatus::returnError(const QString &text)
{
    setTextColor(Qt::red);
    setAlignment(Qt::AlignLeft);
    append(text);
}

void PipeStatus::fatelError(const QString &text)
{
    setTextColor(Qt::darkRed);
    setAlignment(Qt::AlignLeft);
    append(text);
}

void PipeStatus::stdError(const QString &text)
{
    setAlignment(Qt::AlignHCenter);
    setTextColor(Qt::magenta);
    append(text);
}

void PipeStatus::stdOut(const QString &text)
{
    QStringList texts = text.split('\n');
    for ( int i = 0 ; i < texts.size() ; i++ )
    {
        if (texts[i].startsWith(CmdEchoTag))
        {
            QString nextCommand = texts[i].right(
                                    texts[i].size()-CmdEchoTag.size());
            commandFinish(-1, QProcess::NormalExit);
            commandStart(nextCommand);
        }
        else
        {
            setAlignment(Qt::AlignHCenter);
            setTextColor(Qt::cyan);
            append(texts[i]);
        }
    }
}

void PipeStatus::noError(const QString &text)
{
    setTextColor(Qt::green);
    setAlignment(Qt::AlignLeft);
    append(text);
}

void PipeStatus::finishedPipe(const QString &text)
{
    setTextColor(Qt::black);
    setAlignment(Qt::AlignLeft);
    append(text);
}


