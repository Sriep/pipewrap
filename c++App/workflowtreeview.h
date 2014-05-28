#ifndef WORKFLOWTREEVIEW_H
#define WORKFLOWTREEVIEW_H

#include <QTreeView>
#include <QStandardItem>
#include <QDataStream>
#include <QString>

class QStandardItemModel;
class QListView;
class QSqlQueryModel;
class QMenuBar;


Q_DECLARE_METATYPE(QStandardItem*)
const int UsageRole = Qt::UserRole + 1;
const int DataTypeRole = Qt::UserRole + 2;
const int CommandStrRole = Qt::UserRole + 3;
const int FromValueRole = Qt::UserRole + 4;
const int ToValueRole = Qt::UserRole + 5;
const int StepValueRole = Qt::UserRole + 6;
const int NameRole = Qt::UserRole + 7;
const int ValueRole = Qt::UserRole + 8;
const int LevelRole = Qt::UserRole + 9;

const int DmmyUsageRole = 0;
const int ToolUsageRole = 1;
const int StructureUsageRole = 2;
const int DataUsageRole = 3;

const QString COMMAND_PATH = "./Bin/";

class WorkflowTreeView : public QTreeView
{
    Q_OBJECT

public:
    WorkflowTreeView(QWidget *parent = 0);
    ~WorkflowTreeView();
    void Init();
    static QStandardItem* getItemFromName(QStandardItem* parent,
                                          const QString& name);

    void read(QDataStream& in);
    void write(QDataStream& out);
    bool isModified();

public slots:
    void cutSlot();
    void copySlot();
    void pasteSlot();
    void executeSlot();
    void editOpSlot();
    void varyOpSlot();
    void addSlot();
    void upSlot();;
    void downSlot();
    void deleteSlot();

private slots:
    void doubleClickedAddItemSlot(const QModelIndex& index);
    void doubleClickedListViewSlot(const QModelIndex& index);

signals:
    void copyAvailable(bool);
    void pasteAvailable(bool);
    void cutAvailable(bool);
    void contentsChanged();

protected:
    virtual void contextMenuEvent(QContextMenuEvent *event);
    virtual void selectionChanged(const QItemSelection & selected,
                                  const QItemSelection & deselected);


private:
    void createMenu();
    QString genShellVarName(const QString& name);
    void writeRecursive(QDataStream& out, QStandardItem* parent, int level = 0);

    QSqlQueryModel* m_sql_list_model;
    QListView* m_list_view;
    QStandardItemModel* m_listview_model;
    QStandardItemModel* m_treeview_model;
    QList<QStandardItem*> item_clipboard;

    QAction *executeAction;
    QAction *editOpAction;
    QAction *varyOpAction;
};

#endif // WORKFLOWTREEVIEW_H
