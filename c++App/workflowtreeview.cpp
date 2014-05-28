#include <QStandardItemModel>
#include <QStandardItem>
#include <QModelIndex>
#include <QListView>
#include <QSqlTableModel>
#include <QMenuBar>
#include <QMenu>
#include <QSqlRecord>
#include <QVector>
#include <QProcess>
#include <qdebug.h>
#include <QContextMenuEvent>
#include <iostream>
#include "workflowtreeview.h"
#include "tooloptionsdialog.h"
#include "fromtodialog.h"

WorkflowTreeView::WorkflowTreeView(QWidget *parent)
    : QTreeView(parent)
{
    Init();
}

WorkflowTreeView::~WorkflowTreeView()
{
    /*QFile file("/media/sdb2/Projects/Source/C++/Qt/WorkflowTreeView/file.dat");
    file.open(QIODevice::WriteOnly);
    QDataStream out(&file);
    writeRecursive(out, m_treeview_model->invisibleRootItem());
    file.close();*/
}

void WorkflowTreeView::Init()
{
    setWindowTitle(tr("Workflow"));

    m_treeview_model = new QStandardItemModel;    
    setModel(m_treeview_model);

    //QStandardItem* tool_items_root = m_treeview_model->invisibleRootItem();
    //QStandardItem* blank_item = new QStandardItem("...");
    //blank_item->setData(DmmyUsageRole, UsageRole);
    //blank_item->setEditable(false);
    //tool_items_root->appendRow(blank_item);
    connect(this, SIGNAL(doubleClicked(QModelIndex)),
            this, SLOT(doubleClickedAddItemSlot(QModelIndex)));
    createMenu();
}

void WorkflowTreeView::contextMenuEvent(QContextMenuEvent *event)
{
    QMenu menu(this);
    menu.addAction(executeAction);

    QModelIndex index = indexAt(event->pos());
    QStandardItem* item = m_treeview_model->itemFromIndex(index);
    if (item)
    {
        menu.addAction(editOpAction);
        varyOpAction->setData(QVariant::fromValue(item));
        menu.addAction(varyOpAction);
    }
    menu.exec(event->globalPos());
}

void WorkflowTreeView::editOpSlot()
{
    //infoLabel->setText(tr("Edit Options"));
}

void WorkflowTreeView::executeSlot()
{
    //infoLabel->setText(tr("Execute));
    QString shell_script = "";
    int row_num = 1;
    QStandardItem* item = m_treeview_model->item(row_num++);
    while(item)
    {

        QString shell_command = COMMAND_PATH;
        shell_command += item->data(CommandStrRole).toString();
        int row_relative = 0;
        QStandardItem* child = NULL;
        do
        {
            child = item->child(row_relative++, item->column());
            if (child)
            {           
                shell_command += " " + child->data(NameRole).toString();
                shell_command += child->data(ValueRole).toString();
            }
        }
        while(child);
        shell_script += "\n" + shell_command;
        item = m_treeview_model->item(row_num++);
    }
    qDebug() << shell_script;
    QProcess process;
    process.execute(shell_script);
}

void WorkflowTreeView::varyOpSlot()
{
    QVariant v = varyOpAction->data();
    QStandardItem* item = v.value<QStandardItem*>();
    if (item->data(DataTypeRole).toString() == "int")
    {
        FromToDialog dialog(item);
        if (dialog.exec())
        {
            QString shell_var = genShellVarName(item->text());
            QString from = item->data(FromValueRole).toString();
            QString to = item->data(ToValueRole).toString();
            QString step = item->data(StepValueRole).toString();
            
            QString for_st = "for ((" + shell_var + "=";
            for_st += from + "; " + shell_var + " < " + to + "; ";
            for_st += shell_var + " + "+ step + ")); do";
            QStandardItem* from_item = new QStandardItem(tr("Start For Loop"));
            from_item->setData(for_st,CommandStrRole);
            from_item->setData(StructureUsageRole, UsageRole);
            QStandardItem* parent = item->parent();
            m_treeview_model->insertRow(parent->row(),from_item);
            //from_item->setChild(0,parent);

            QStandardItem* done_item = new QStandardItem(tr("End for loop"));
            done_item->setData("done",CommandStrRole);
            done_item->setData("done",NameRole);
            done_item->setData(StructureUsageRole, UsageRole);
            m_treeview_model->appendRow(done_item);
        }
    }
 }

QString WorkflowTreeView::genShellVarName(const QString& name)
{
    QString shell_variable = name;
    shell_variable.simplified();
    shell_variable.remove(' ');
    shell_variable.remove('-');
    return shell_variable;
}

void WorkflowTreeView::createMenu()
{
    editOpAction = new QAction(tr("&Edit Options"), this);
    //editOptionsAction->setShortcuts(QKeySequence::Edit);
    editOpAction->setStatusTip(tr("Edit Options"));
    connect(editOpAction, SIGNAL(triggered()), this, SLOT(editOpSlot()));

    executeAction = new QAction(tr("E&xecute"), this);
    //executeAction->setShortcuts(QKeySequence::OK);
    executeAction->setStatusTip(tr("Excecute"));
    connect(executeAction, SIGNAL(triggered()), this, SLOT(executeSlot()));

    varyOpAction = new QAction(tr("&Vary parameter"), this);
    //executeAction->setShortcuts(QKeySequence::OK);
    varyOpAction->setStatusTip(tr("Vary parameter"));
    connect(varyOpAction, SIGNAL(triggered()), this, SLOT(varyOpSlot()));
}

void WorkflowTreeView::addSlot()
{
    m_sql_list_model = new QSqlQueryModel;
    m_sql_list_model->setQuery("SELECT tool_name, tool_shell_string FROM BioTools");
    m_sql_list_model->setHeaderData(0, Qt::Horizontal, tr("tool_name"));

    m_list_view  = new QListView();
    m_list_view->setModel(m_sql_list_model);
    connect(m_list_view, SIGNAL(doubleClicked(QModelIndex)),
            this, SLOT(doubleClickedListViewSlot(QModelIndex)));
    m_list_view->show();
}

void WorkflowTreeView::doubleClickedAddItemSlot(const QModelIndex& index)
{
    //qDebug("start\tWorkflowTreeView::doubleClickedAddItemSlot");
    QStandardItem* 	item = m_treeview_model->itemFromIndex(index);
    if (item->text() == "...")
    {
        addSlot();
    }
    else
    {
        {
            ToolOptionsDialog *options_dialog = new ToolOptionsDialog(item);
            options_dialog->exec();
        }
    }
    //qDebug("end\tWorkflowTreeView::doubleClickedAddItemSlot");
}

// This is a handler for an action from a pop up list view, not the tree view.
// Where we are selecting a new shell command to add to the list
void WorkflowTreeView::doubleClickedListViewSlot(const QModelIndex& index)
{
    QString text = m_sql_list_model->data(index,Qt::DisplayRole).toString();
    QStandardItem* tree_item = new QStandardItem(text);
    tree_item->setData(ToolUsageRole, UsageRole);

    QSqlRecord sql_record = m_sql_list_model->record(index.row());
    QString comd = sql_record.value("tool_shell_string").toString();
    tree_item->setData(comd, CommandStrRole);

    ToolOptionsDialog* options_dialog = new ToolOptionsDialog(tree_item);
    int rtv = options_dialog->exec();
    if (rtv)
    {        
        m_treeview_model->appendRow(tree_item);
        m_list_view->close();
    }
}

void WorkflowTreeView::cutSlot()
{
    QModelIndexList index_list = selectedIndexes();
    QModelIndex index = index_list.first();
    QStandardItem* first_item = m_treeview_model->itemFromIndex(index);
    int UsageType = first_item->data(UsageRole).toInt();
    if (UsageType == StructureUsageRole ||  UsageType == ToolUsageRole)
    {
        item_clipboard = m_treeview_model->takeRow(index_list.first().row());
    }
    emit pasteAvailable(true);
}

void WorkflowTreeView::copySlot()
{
    //QModelIndexList index_list = selectedIndexes();
    //QModelIndex* first_item = index_list.first();
    //emit pasteAvailiable(true);
}

void WorkflowTreeView::pasteSlot()
{
    QModelIndexList index_list = selectedIndexes();
    QModelIndex index = index_list.first();
    QStandardItem* item = m_treeview_model->itemFromIndex(index);
    int UsageType = item->data(UsageRole).toInt();
    if (UsageType == DataUsageRole)
    {
        item = item->parent();
    }
    m_treeview_model->insertRow(item->row(), item_clipboard);
    emit pasteAvailable(false);
}

void WorkflowTreeView::upSlot()
{

}

void WorkflowTreeView::downSlot()
{
    cutSlot();
    pasteSlot();
}

void WorkflowTreeView::deleteSlot()
{
    cutSlot();
}

QStandardItem* WorkflowTreeView::getItemFromName(QStandardItem* parent,
                                                 const QString& name)
{
    for (int r=0 ; r < parent->rowCount() ; r++)
    {
        for (int c=0 ; c < parent->columnCount() ; c++)
        {
            QStandardItem* child = parent->child(r,c);
            if (name == child->data(NameRole).toString())
                return child;
        }
    }
    return NULL;
}

bool WorkflowTreeView::isModified()
{
    return isWindowModified();
}

void WorkflowTreeView::selectionChanged(const QItemSelection & selected,
                                        const QItemSelection & deselected)
{
    emit copyAvailable(!selected.isEmpty());
    emit cutAvailable(!selected.isEmpty());
    return QAbstractItemView::selectionChanged(selected, deselected);
}


void WorkflowTreeView::write(QDataStream& out)
{
    writeRecursive(out, m_treeview_model->invisibleRootItem());
}

void WorkflowTreeView::writeRecursive(QDataStream& out,
                                      QStandardItem* parent,
                                      int level)
{
    for (int r=0 ; r < parent->rowCount() ; r++)
    {
        for (int c=0 ; c < parent->columnCount() ; c++)
        {
            QStandardItem* child = parent->child(r,c);
            if (child)
            {
                child->setData(level, LevelRole);
                child->write(out);
                writeRecursive(out, child, level+1);
            }
        }
    }
}

void WorkflowTreeView::read(QDataStream& in)
{
    m_treeview_model = new QStandardItemModel;
    setModel(m_treeview_model);
    QStandardItem* root = m_treeview_model->invisibleRootItem();
    QVector<QStandardItem*> ancestsors(1);
    ancestsors[0] = root;
    while (!in.atEnd())
    {
        QStandardItem* read_item = new QStandardItem;
        read_item->read(in);
        int level = read_item->data(LevelRole).toInt();
        if (ancestsors.size() >= level)
        {
            ancestsors[level]->appendRow(read_item);
        }
        else
        {
            QStandardItem* parent = ancestsors.last();
            for (int i = ancestsors.size() ; i <= level ; i++)
            {
                ancestsors.append(parent);
            }
        }
        if (ancestsors.size() > level+1)
            ancestsors.replace(level+1, read_item);
        else
            ancestsors.append(read_item);
    }
}





































