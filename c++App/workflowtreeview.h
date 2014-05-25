#ifndef WORKFLOWTREEVIEW_H
#define WORKFLOWTREEVIEW_H

#include <QTreeView>
#include <QStandardItem>
#include <QDataStream>

class QStandardItemModel;
class QListView;
class QSqlQueryModel;
class QMenuBar;
//class QStandardItem;

Q_DECLARE_METATYPE(QStandardItem*)
const int ItemUsageRole = Qt::UserRole + 1;
const int DataTypeRole = Qt::UserRole + 2;
const int CommandStrRole = Qt::UserRole + 3;
const int FromValueRole = Qt::UserRole + 4;
const int ToValueRole = Qt::UserRole + 5;
const int StepValueRole = Qt::UserRole + 6;
const int NameRole = Qt::UserRole + 7;
const int ValueRole = Qt::UserRole + 8;
const int LevelRole = Qt::UserRole + 9;
//static QStandardItem* getItemFromName(QStandardItem* parent, const QString& name);

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

private slots:
    void doubleClickedAddItemSlot(const QModelIndex& index);
    void doubleClickedListViewSlot(const QModelIndex& index);

    void editOpSlot();
    void executeSlot();
    void varyOpSlot();



protected:
    virtual void contextMenuEvent(QContextMenuEvent *event);

private:
    void createMenu();
    QString genShellVarName(const QString& name);
    void writeRecursive(QDataStream& out, QStandardItem* parent, int level = 0);

    QSqlQueryModel* m_sql_list_model;
    QListView* m_list_view;
    QStandardItemModel* m_listview_model;
    QStandardItemModel* m_treeview_model;

    QAction *executeAction;
    QAction *editOpAction;
    QAction *varyOpAction;
};

#endif // WORKFLOWTREEVIEW_H
