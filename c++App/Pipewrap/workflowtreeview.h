#ifndef WORKFLOWTREEVIEW_H
#define WORKFLOWTREEVIEW_H

#include <QTreeView>
#include <QStandardItem>
#include <QDataStream>
#include <QString>
#include <QProcess>
#include <QDateTime>
#include <QThread>

class QStandardItemModel;
class QListView;
class QSqlQueryModel;
class QMenuBar;
class QMainWindow;
class MainWindow;
class QTime;
class QTextEdit;
class QProcess;
class PipeStatus;

Q_DECLARE_METATYPE(QStandardItem*)
const int UsageRole = Qt::UserRole + 1;
const int DataTypeRole = Qt::UserRole + 2;
const int CommandStrRole = Qt::UserRole + 3;
const int FromValueRole = Qt::UserRole + 4;
const int ToValueRole = Qt::UserRole + 5;
const int StepValueRole = Qt::UserRole + 6;
const int NameRole = Qt::UserRole + 7;
//const int ValueRole = Qt::UserRole + 8;
const int LevelRole = Qt::UserRole + 9;

#define UsageRoleType int
//const int DmmyUsageRole = 0;
const int ToolUsageRole = 1;
const int StructureUsageRole = 2;
const int DataUsageRole = 3;
const int LabelUsageRole = 4;
const int CommandUsageRole = 5;
const int InPipeUsageRole = 6;
const int OutPipeUsageRole = 7;
const int ShellStrUsageRole = 8;

const QString COMMAND_PATH = "";
const QString CmdEchoTag = "Echoed_command ";

class WorkflowTreeView : public QTreeView
{
    Q_OBJECT

public:
    WorkflowTreeView(QWidget *parent = 0);
    ~WorkflowTreeView();
    void init();
    static QStandardItem* getItemFromName(QStandardItem* parent,
                                          const QString& name);

    void read(QDataStream& in, QStandardItem *root = NULL);
    void write(QDataStream& out);
    bool isModified();
    QString name() const;

public slots:
    void newSlot();
    void cutSlot();
    void copySlot();
    void pasteSlot();
    void executeSlot();
    void editOpSlot();
    void varyOpSlot();
    void addSlot();
    void upSlot();
    void downSlot();
    void deleteSlot();
    void inPipeSlot();
    void outPipeSlot();

    //void processStdErrSlot();
    //void processFinishedSlot();
    //void processErrorSlot(QProcess::ProcessError error);
private slots:
    void doubleClickedEditSlot(const QModelIndex& index);
    void doubleClickedListViewSlot(const QModelIndex& index);

    //void processStdErrSlot();
    void finishedSlot(int, QProcess::ExitStatus);
    void errorSlot(QProcess::ProcessError error);
    void stateChangedSlot(int, QProcess::ProcessState);
signals:
    void copyAvailable(bool);
    void pasteAvailable(bool);
    void cutAvailable(bool);
    void upAvailable(bool);
    void downAvailable(bool);
    void contentsChanged();
    void pipeIntoAvailable(bool);
    void pipeOutofAvailable(bool);
    void executeAvailable(bool);

    void processStdErr(const QString& message);
    void startPipeStatus();
    void finishedAll();
protected:
    virtual void contextMenuEvent(QContextMenuEvent *event);
    virtual void selectionChanged(const QItemSelection & selected,
                                  const QItemSelection & deselected);

    //virtual void dragEnterEvent(QDragEnterEvent *event);
    //virtual void dragMoveEvent(QDragMoveEvent *event);
    //virtual void dropEvent(QDropEvent *event);
    //virtual void mousePressEvent(QMouseEvent *event);

private:
    void nextCommand();

    QStringList commandList;
    int currentCommand = 0;
    QDateTime timePipeStarted;
    QProcess* process;
    PipeStatus* pipeStatus;
private:
    void createMenu();
    QString genShellVarName(const QString& name);
    void writeRow(QDataStream& out, QStandardItem* parent, int level = 0);
    //void writeRow(QDataStream& out, int row);
    void cutRow(QDataStream& out, QStandardItem* parent, int level = 0);
    void readRow(QDataStream& in, QStandardItem* item);
    QList<QStandardItem*> readRow(QDataStream& in);
    MainWindow* getMainWindow() const;
    bool isFirstItem(const QModelIndex &index) const;
    bool isLastItem(const QModelIndex &index) const;
    bool isInFirstTool(const QModelIndex &index) const;
    bool isInLastTool(const QModelIndex &index) const;
    bool canPipeInto(const QModelIndex &index) const;
    bool canPipeOutof(const QModelIndex &index) const;
    void pipe(QModelIndex out, QModelIndex in);
    QList<QStandardItem*> getItemFromRole(QStandardItem* parent,
                                           UsageRoleType usage,
                                           const QString& match_text,
                                           int column);
    void copyResultFiles();
    void runPipe();
    void pipeFinshed();
    bool moveItems(QModelIndexList indexs, int amount);
    void toolsToPaths(QSet<QString> tools);

    QSqlQueryModel* m_sql_list_model;
    QListView* m_list_view;
    QStandardItemModel* m_listview_model;
    QStandardItemModel* m_treeview_model;
    QSet<QString> paths;

    QString shellScript;

    QList<QStandardItem*> item_clipboard;
    QModelIndex indexClipboard;

    QStandardItem* dragdrop_clipboard;
    int current_row_cludge;

    QAction *executeAct;
    QAction *editAct;
    QAction *varyOpAct;
    QAction *inPipeAct;
    QAction *outPipeAct;

    QAction *separatorAct;
    QAction *addAct;
    QAction *cutAct;
    QAction *pasteAct;
    QAction *copyAct;
    QAction *upAct;
    QAction *downAct;

    QList<QStandardItem*> snipRows(QModelIndexList indexList);
    //QList<QStandardItem*> getCopyOfRows(QModelIndexList indexList);
    void insertRows(QModelIndex index, QList<QStandardItem*> insertItems);
    QList<QStandardItem*> cloneItems(const QModelIndexList &indexs);

   QPoint dragStartPosition;
   virtual void dragEnterEvent(QDragEnterEvent *event);
   virtual void dragMoveEvent(QDragMoveEvent *event);
   virtual void dropEvent(QDropEvent *event);
   virtual void mousePressEvent(QMouseEvent *event);
};

#endif // WORKFLOWTREEVIEW_H
