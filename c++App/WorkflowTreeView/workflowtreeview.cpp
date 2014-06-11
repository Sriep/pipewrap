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
#include <QMimeData>
#include <QDrag>
#include <QContextMenuEvent>
#include <QPixmap>
#include <QStatusBar>
#include <QSqlQuery>
#include <QColor>
#include <QMainWindow>
#include <QTime>
#include <QPainter>
#include <QClipboard>
#include <QApplication>
#include <QDir>
#include <QTextEdit>
#include <QFileInfo>
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
}

void WorkflowTreeView::Init()
{
    setWindowTitle(tr("Workflow"));

    m_treeview_model = new QStandardItemModel(0,2);
    setModel(m_treeview_model);

    connect(this, SIGNAL(doubleClicked(QModelIndex)),
            this, SLOT(doubleClickedEditSlot(QModelIndex)));
    createMenu();
}

void WorkflowTreeView::contextMenuEvent(QContextMenuEvent *event)
{
    QMenu menu(this);
    QModelIndex index = indexAt(event->pos());

    menu.addAction(addAct);
    menu.addAction(cutAct);
    menu.addAction(pasteAct);
    separatorAct = menu.addSeparator();
    menu.addAction(editAct);
    menu.addAction(varyOpAct);
    menu.addAction(inPipeAct);
    menu.addAction(outPipeAct);
    menu.addAction(executeAct);

    QStandardItem* item = m_treeview_model->itemFromIndex(index);
    if (item)
    {
        UsageRoleType u = item->data(UsageRole).toInt();

        editAct->setEnabled(u == ToolUsageRole);
        cutAct->setEnabled(true);
        pasteAct->setEnabled(!item_clipboard.isEmpty());
        inPipeAct->setEnabled(canPipeInto(index));
        outPipeAct->setEnabled(canPipeOutof(index));

        QString datatype = item->data(DataTypeRole).toString();
        varyOpAct->setEnabled(u == DataUsageRole && "int" == datatype);

        addAct->setData(QVariant::fromValue(item));
        cutAct->setData(QVariant::fromValue(item));
        pasteAct->setData(QVariant::fromValue(item));
        editAct->setData(QVariant::fromValue(item));
        varyOpAct->setData(QVariant::fromValue(item));
        inPipeAct->setData(QVariant::fromValue(item));
        outPipeAct->setData(QVariant::fromValue(item));
    }
    else
    {
        cutAct->setEnabled(false);
        pasteAct->setEnabled(false);
        editAct->setEnabled(false);
        varyOpAct->setEnabled(false);
        inPipeAct->setEnabled(false);
        outPipeAct->setEnabled(false);
    }
    menu.exec(event->globalPos());
}

QMainWindow* WorkflowTreeView::GetMainWindow()
{
    QMainWindow* parent_window = qobject_cast<QMainWindow*>(parent());
    return parent_window;
}

void WorkflowTreeView::editOpSlot()
{
    GetMainWindow()->statusBar()->showMessage(tr("Edit Options"));
    doubleClickedEditSlot(selectedIndexes().first());
}

void WorkflowTreeView::executeSlot()
{
    qDebug() << "WorkflowTreeView::executeSlot()";
    GetMainWindow()->statusBar()->showMessage(tr("Execute"));

    shellScript = "#!/bin/bash\n";
    int row_num = 0;
    QStandardItem* item = m_treeview_model->item(row_num);
    while(item)
    {
        QString shellCommand = COMMAND_PATH; //"./Bin/"        
        if (item->data(UsageRole) == StructureUsageRole)
        {
            QStandardItem* sib = m_treeview_model->item(row_num,1);
            shellCommand += sib->text();
        }
        else
        {
            shellCommand += item->data(CommandStrRole).toString();
        }
        int row_relative = 0;
        bool pipe_out = false;
        QStandardItem* child = NULL;
        do
        {
            child = item->child(row_relative++, 1);
            if (child)
            {           
                if (child->data(UsageRole) == OutPipeUsageRole)
                {
                    pipe_out = true;
                }
                else if (child->data(UsageRole) == InPipeUsageRole)
                {
                    // Nothing could check pipe_out = true?
                }
                else
                {
                    shellCommand += " " + child->data(NameRole).toString();
                    shellCommand += child->text();
                }
            }
        }
        while(child);
        shellScript += shellCommand;
        shellScript += pipe_out ? " | " : "\n";
        pipe_out = false;
        item = m_treeview_model->item(++row_num);
    }

    runPipe();
}

void WorkflowTreeView::runPipe()
{
    timePipeStarted = QDateTime::currentDateTime();
    QFile file("shell_script.sh");
    file.remove();
    file.open(QIODevice::WriteOnly);
    QTextStream out(&file);
    out << shellScript;
    file.close();

    qDebug() << shellScript;
    process = new QProcess;
    process->execute("PATH=PATH:$PWD");
    process->setReadChannel(QProcess::StandardError);

    pipeStatus = new QTextEdit();
    pipeStatus->setWindowTitle("Pipe status");

    connect(process, SIGNAL(readyReadStandardError()),
            this, SLOT(processOutSlot()));
    connect(process,
            SIGNAL(finished(int exitCode, QProcess::ExitStatus exitStatus)),
            this,
            SLOT(processFinishedSlot(int exitCode,
                                     QProcess::ExitStatus exitStatus)));
    pipeStatus->show();
    //process->execute("./shell_script.sh");
    process->start("./shell_script.sh");

    //pipeStatus->close();
    //process = NULL;
}

void WorkflowTreeView::processOutSlot()
{
    QByteArray ba = process->readAllStandardError();
    pipeStatus->append(QString(ba));
}

void WorkflowTreeView::processFinishedSlot(int exitCode,
                                           QProcess::ExitStatus exitStatus)
{
    pipeStatus->append(QString("!!!!!Finshed!!!!!"));
    copyResultFiles();
}

void WorkflowTreeView::copyResultFiles()
{
    qDebug() << "WorkflowTreeView::copyResultFiles";
    QDir working = QDir::current();
    QString archive = "./Results/" + timePipeStarted.toString() + "/";
    working.mkdir(archive);

    //QList<QFileInfo>.
    QFileInfoList files = working.entryInfoList(QDir::Files, QDir::Time);
    //for (i = files.begin(); i != files.end(); ++i)
    //    if ((*i).created() < start_time) files.erase(i);

    int i = 0;
    while (i<files.size())
    {
        if (files[i].created() < timePipeStarted)
            files.removeAt(i);
        else
            i++;
    }

    for (int i = 0 ; i < files.size(); ++i)
    {
        QString fn = files[i].fileName();
        qDebug() << fn;
        QFile::rename(fn, archive + fn);
    }

    QFile file(archive + "shell_script.sh");
    file.open(QIODevice::WriteOnly);
    QTextStream out(&file);
    out << shellScript;
    file.close();

}

void WorkflowTreeView::varyOpSlot()
{
    GetMainWindow()->statusBar()->showMessage(tr("Varay parametr"));
    QStandardItem* item = varyOpAct->data().value<QStandardItem*>();

    if (item->data(DataTypeRole).toString() == "int")
    {
        FromToDialog dialog(item); //NameRole
        if (dialog.exec())
        {
            //QString shell_var = genShellVarName(item->text());
            QString name = item->data(NameRole).toString();
            QString shell_var = genShellVarName(name);
            QString from = item->data(FromValueRole).toString();
            QString to = item->data(ToValueRole).toString();
            QString step = item->data(StepValueRole).toString();
            
            //QString for_st = "for ((" + shell_var + "=";
            //for_st += from + "; " + shell_var + " < " + to + "; ";
            //for_st += shell_var + " + "+ step + ")); do";

            QString for_st = "for " + shell_var + " in {" + from
                    + ".." + to + ".." + step + "}; do";
            item->setText("${" + shell_var + "}");

            QStandardItem* from_item = new QStandardItem(tr("Start For Loop"));
            QStandardItem* parent = item->parent();
            from_item->setData(for_st,CommandStrRole);
            from_item->setData(StructureUsageRole, UsageRole);
            from_item->setEditable(false);

            QStandardItem* from_command = new QStandardItem(for_st);
            from_command->setData(ShellStrUsageRole, UsageRole);
            from_command->setData(for_st, CommandStrRole);
            from_command->setEditable(true);
            QList<QStandardItem*> from_row;
            from_row.append(from_item);
            from_row.append(from_command);
            m_treeview_model->insertRow(parent->row(),from_row);

            QString done_text = tr("End for loop : ");
            done_text += shell_var;
            QString done_commandstr = "done";
            QStandardItem* done_item = new QStandardItem(done_text);
            done_item->setData(done_commandstr,CommandStrRole);
            done_item->setData(StructureUsageRole, UsageRole);
            done_item->setEditable(false);

            QStandardItem* done_command = new QStandardItem(done_commandstr);
            done_command->setData(ShellStrUsageRole, UsageRole);
            done_command->setEditable(true);

            QList<QStandardItem*> done_row;
            done_row.append(done_item);
            done_row.append(done_command);
            QStandardItem* root =  m_treeview_model->invisibleRootItem();
            root->appendRow(done_row);

        }
    }
 }

QString WorkflowTreeView::genShellVarName(const QString& name)
{
    QString shell_variable = "Var" + name;
    shell_variable.simplified();
    shell_variable.remove(' ');
    shell_variable.remove('-');
    return shell_variable;
}

void WorkflowTreeView::createMenu()
{
    editAct = new QAction(tr("&Edit Options"), this);
    //editOptionsAction->setShortcuts(QKeySequence::Edit);
    editAct->setStatusTip(tr("Edit Options"));
    connect(editAct, SIGNAL(triggered()), this, SLOT(editOpSlot()));

    executeAct = new QAction(tr("E&xecute"), this);
    //executeAction->setShortcuts(QKeySequence::OK);
    executeAct->setStatusTip(tr("Excecute"));
    connect(executeAct, SIGNAL(triggered()), this, SLOT(executeSlot()));

    varyOpAct = new QAction(tr("&Vary parameter"), this);
    //executeAction->setShortcuts(QKeySequence::OK);
    varyOpAct->setStatusTip(tr("Vary parameter"));
    connect(varyOpAct, SIGNAL(triggered()), this, SLOT(varyOpSlot()));

    inPipeAct = new QAction(tr("Add &input pipe"), this);
    //executeAction->setShortcuts(QKeySequence::OK);
    inPipeAct->setStatusTip(tr("Pipe input from last function"));
    connect(inPipeAct, SIGNAL(triggered()), this, SLOT(inPipeSlot()));

    outPipeAct = new QAction(tr("Add &output pipe"), this);
    //executeAction->setShortcuts(QKeySequence::OK);
    outPipeAct->setStatusTip(tr("Pipe output to next function"));
    connect(outPipeAct, SIGNAL(triggered()), this, SLOT(outPipeSlot()));

    cutAct = new QAction(QIcon(":/images/cut.png"), tr("Cu&t"), this);
    cutAct->setShortcuts(QKeySequence::Cut);
    cutAct->setStatusTip(tr("Cut the current selection's contents to the "
                            "clipboard"));
    connect(cutAct, SIGNAL(triggered()), this, SLOT(cutSlot()));

    pasteAct = new QAction(QIcon(":/images/paste.png"), tr("&Paste"), this);
    pasteAct->setShortcuts(QKeySequence::Paste);
    pasteAct->setStatusTip(tr("Paste the clipboard's contents into the current "
                              "selection"));
    connect(pasteAct, SIGNAL(triggered()), this, SLOT(pasteSlot()));

    addAct = new QAction(QIcon(":/images/edit_add.png"), tr("&Add"), this);
    //addAct->setShortcuts(QKeySequence::Paste);
    addAct->setStatusTip(tr("Add new tool to pipline"));
    connect(addAct, SIGNAL(triggered()), this, SLOT(addSlot()));
}

void WorkflowTreeView::inPipeSlot()
{
    GetMainWindow()->statusBar()->showMessage(tr("Add pipeline into"));
    QModelIndex piped_to = selectedIndexes().first();
    while(piped_to.parent().isValid()) piped_to = piped_to.parent();
    QModelIndex piped_from = piped_to.sibling(piped_to.row()-1,0);
    pipe(piped_from, piped_to);
}

void WorkflowTreeView::outPipeSlot()
{
    GetMainWindow()->statusBar()->showMessage(tr("Add pipeline from"));
    QModelIndex piped_from = selectedIndexes().first();
    while(piped_from.parent().isValid()) piped_from = piped_from.parent();
    QModelIndex piped_to = piped_from.sibling(piped_from.row()+1,0);
    pipe(piped_from, piped_to);
}

void WorkflowTreeView::pipe(QModelIndex out, QModelIndex in)
{
    QStandardItem* out_pipe_label = new QStandardItem("> OUT PIPE:");
    QStandardItem* out_pipe = new QStandardItem("|");
    QList<QStandardItem*> out_pipe_row;
    out_pipe_row.append(out_pipe_label);
    out_pipe_row.append(out_pipe);
    out_pipe_label->setData(OutPipeUsageRole,UsageRole);
    out_pipe->setData(OutPipeUsageRole,UsageRole);

    QStandardItem* in_pipe_lable = new QStandardItem("< IN PIPE:");
    QStandardItem* in_pipe = new QStandardItem("");
    QList<QStandardItem*> in_pipe_row;
    in_pipe_row.append(in_pipe_lable);
    in_pipe_row.append(in_pipe);
    in_pipe_lable->setData(InPipeUsageRole,UsageRole);
    in_pipe->setData(InPipeUsageRole,UsageRole);

    QStandardItem* this_end = m_treeview_model->itemFromIndex(out);
    QStandardItem* that_end = m_treeview_model->itemFromIndex(in);

    QList<QStandardItem*> stdout_item
            = getItemFromRole(this_end, NameRole, ">",0);
    if (!stdout_item.isEmpty())
        this_end->removeRow(stdout_item.first()->row());
    QList<QStandardItem*> stdin_item
            = getItemFromRole(that_end, NameRole, "<",0);
    if (!stdin_item.isEmpty())
        this_end->removeRow(stdin_item.first()->row());

    this_end->appendRow(out_pipe_row);
    that_end->appendRow(in_pipe_row);
}

void WorkflowTreeView::doubleClickedEditSlot(const QModelIndex& index)
{
    QStandardItem* 	item = m_treeview_model->itemFromIndex(index);
    if (item->data(UsageRole).toInt() == ToolUsageRole)
    {
        ToolOptionsDialog *options_dialog = new ToolOptionsDialog(item);
        options_dialog->exec();
    }
}

void WorkflowTreeView::addSlot()
{
    current_row_cludge = 0;
    if (!selectedIndexes().isEmpty())
    {
        QModelIndex sel_item = selectedIndexes().first();
        current_row_cludge = m_treeview_model->itemFromIndex(sel_item)->row()+1;
    }

    m_sql_list_model = new QSqlQueryModel;
    m_sql_list_model->setQuery("SELECT tool_name, tool_shell_string FROM BioTools");
    m_sql_list_model->setHeaderData(0, Qt::Horizontal, tr("tool_name"));

    m_list_view  = new QListView();
    m_list_view->setModel(m_sql_list_model);
    connect(m_list_view, SIGNAL(doubleClicked(QModelIndex)),
            this, SLOT(doubleClickedListViewSlot(QModelIndex)));
    m_list_view->show();
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
        tree_item->setEditable(false);
        //m_treeview_model->appendRow(tree_item);
        m_treeview_model->insertRow(current_row_cludge, tree_item);
        m_list_view->close();
    }
}

void WorkflowTreeView::cutSlot()
{
    //QModelIndexList index_list = selectedIndexes();
    QModelIndex index = selectedIndexes().first();
    QStandardItem* first_item = m_treeview_model->itemFromIndex(index);
    int UsageType = first_item->data(UsageRole).toInt();    
    if (UsageType == StructureUsageRole ||  UsageType == ToolUsageRole)
    {                
        item_clipboard = m_treeview_model->takeRow(index.row());
        emit pasteAvailable(true);
    }
    else
    {
        //m_treeview_model->removeRow()
        m_treeview_model->removeRow(index.row(), index.parent());
        //m_treeview_model->takeRow(index.row());
        emit pasteAvailable(false);
        item_clipboard.clear();
    }
    /*
    QMimeData* mime_data = new QMimeData;
    QByteArray itemData;
    QDataStream dataStream(&itemData, QIODevice::WriteOnly);
    cutRow(dataStream, first_item);
    mime_data->setData("cut row", itemData);
    QClipboard *clipboard = QApplication::clipboard();
    clipboard->setMimeData(mime_data, QClipboard::Clipboard);*/
}

void WorkflowTreeView::copySlot()
{
    //QModelIndex* first_item = selectedIndexes().first();
    //emit pasteAvailiable(true);
}

void WorkflowTreeView::pasteSlot()
{
    //QModelIndexList index_list = selectedIndexes();
    QModelIndex index = selectedIndexes().first();
    QStandardItem* item = m_treeview_model->itemFromIndex(index);
    int UsageType = item->data(UsageRole).toInt();
    if (UsageType == DataUsageRole || UsageType == LabelUsageRole )
    {
        item = item->parent();
    }
    m_treeview_model->insertRow(item->row(), item_clipboard);
/*
    QClipboard* clipboard = QApplication::clipboard();
    //const QMimeData* mimeData = clipboard->mimeData();
    const QMimeData* mime_data = clipboard->mimeData(QClipboard::Clipboard);
    QByteArray itemData = mime_data->data("cut row");
    QDataStream dataStream(&itemData, QIODevice::ReadOnly);
    read(dataStream, item);
*/
    emit pasteAvailable(false);
    item_clipboard.clear();
}

void WorkflowTreeView::upSlot()
{

}

void WorkflowTreeView::downSlot()
{
    //cutSlot();
    //pasteSlot();
}

void WorkflowTreeView::deleteSlot()
{
    cutSlot();
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

    QModelIndex index = selected.indexes().first();
    emit upAvailable(!isFirstItem(index));
    emit downAvailable(!isLastItem(index));
    emit pipeIntoAvailable(canPipeInto(index));
    emit pipeOutofAvailable(canPipeOutof(index));

    return QAbstractItemView::selectionChanged(selected, deselected);
}


void WorkflowTreeView::write(QDataStream& out)
{
    writeRow(out, m_treeview_model->invisibleRootItem());
}

void WorkflowTreeView::cutRow(QDataStream& out, QStandardItem* parent,int level)
{
    parent->setData(level, LevelRole);
    parent->write(out);
    QQueue<QStandardItem*> children;
    for (int r=0 ; r < parent->rowCount() ; r++)
    {
        for (int c=0 ; c < parent->columnCount() ; c++)
        {
            if (parent->child(r,c))
                children.enqueue(parent->child(r,c));
        }        
    }
    while (!children.isEmpty())
        cutRow(out, children.dequeue(), level+1);
    m_treeview_model->takeItem(parent->row(), parent->column());
}

//void WorkflowTreeView::writeRow(QDataStream& out, int row)
//{
//    QList<QStandardItem*> row_items = m_treeview_model->takeRow(row);
//    for (int i = 0; i < row_items.size(); ++i)
//    {
//        row_items.at(i)->write(out);
//    }
//}

void WorkflowTreeView::writeRow(QDataStream& out,
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
                qDebug() << (child->data(UsageRole).toInt() - Qt::UserRole)
                         << "out"
                         << child->text();
                child->write(out);
                writeRow(out, child, level+1);
            }
        }
    }
}
/*
void WorkflowTreeView::pasteRow(QDataStream& in, QStandardItem* item)
{
    QList<QStandardItem*> row_items;
    while (!in.atEnd())
    {
        QStandardItem* read_item = new QStandardItem;
        read_item->read(in);
        row_items.append(read_item);
    }
    m_treeview_model->insertRow(item->row(), row_items);
}
*/

void WorkflowTreeView::newSlot()
{ 
    m_treeview_model = new QStandardItemModel(0,2);
    setModel(m_treeview_model);
}

void WorkflowTreeView::read(QDataStream& in, QStandardItem* root)
{
    if (NULL == root)
        root = m_treeview_model->invisibleRootItem();
    QStandardItem* ancestsor;
    QList<QStandardItem*> list;
    while (!in.atEnd())
    {
        QStandardItem* read_item = new QStandardItem;
        read_item->read(in);
        int usage = read_item->data(UsageRole).toInt();
        switch (usage)
        {        
        case ToolUsageRole:
            root->appendRow(read_item);
            qDebug() << "ToolUsageRole"<< read_item->text();
            ancestsor = read_item;
            break;
        case StructureUsageRole:
        case LabelUsageRole:
        case InPipeUsageRole:
        case OutPipeUsageRole:
            list.append(read_item);
            qDebug() << "LabelUsageRole"<< read_item->text();
            break;
        case ShellStrUsageRole:
            list.append(read_item);
            qDebug() << "DataUsageRole"<< read_item->text();
            root->appendRow(list);
            list.clear();
            break;
        case DataUsageRole:
            list.append(read_item);
            qDebug() << "DataUsageRole"<< read_item->text();
            ancestsor->appendRow(list);
            list.clear();
            break;
        default:
            break;
        }
    }
    emit copyAvailable(true);
    emit cutAvailable(true);
}
/*
void WorkflowTreeView::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->mimeData()->text() == dragdrop_clipboard->text())
    {
        if (event->source() == this)
        {
            event->setDropAction(Qt::MoveAction);
            event->accept();
        }
        else
        {
            event->acceptProposedAction();
        }
    }
    else
    {
        event->ignore();
    }
    QAbstractItemView::dragEnterEvent(event);
}

void WorkflowTreeView::dragMoveEvent(QDragMoveEvent *event)
{
    if (event->mimeData()->text() == dragdrop_clipboard->text())
    {
        if (event->source() == this)
        {
            event->setDropAction(Qt::MoveAction);
            event->accept();
        }
        else
        {
            event->acceptProposedAction();
        }
    }
    else
    {
        event->ignore();
    }
    QAbstractItemView::dragMoveEvent(event);
}

void WorkflowTreeView::dropEvent(QDropEvent *event)
{
    if (event->mimeData()->text() == dragdrop_clipboard->text())
    {
        QList<QStandardItem*>  dragged;
        dragged = m_treeview_model->takeRow(dragdrop_clipboard->row());

        QModelIndex index = selectedIndexes().first();
        QStandardItem* item = m_treeview_model->itemFromIndex(index);
        int UsageType = item->data(UsageRole).toInt();
        if (UsageType == DataUsageRole)
        {
            item = item->parent();
        }
        m_treeview_model->insertRow(item->row(), dragged);

        if (event->source() == this)
        {
            event->setDropAction(Qt::MoveAction);
            event->accept();
        }
        else
        {
            event->acceptProposedAction();
        }
    }
    else
    {
        event->ignore();
    }
    QAbstractItemView::dropEvent(event);
}

void WorkflowTreeView::mousePressEvent(QMouseEvent *event)
{
    QModelIndex index = indexAt(event->pos());
    QStandardItem* selcted_item = m_treeview_model->itemFromIndex(index);
    dragdrop_clipboard = selcted_item;

    //QPixmap pixmap = *child->pixmap();

    //QByteArray itemData;
    //QDataStream dataStream(&itemData, QIODevice::WriteOnly);
    //writeRecursive(dataStream, item);

    QMimeData *mimeData = new QMimeData;
    if (dragdrop_clipboard)
        mimeData->setText(dragdrop_clipboard->text());

    QDrag *drag = new QDrag(this);
    drag->setMimeData(mimeData);

    //drag->setPixmap(pixmap);
    //drag->setHotSpot(event->pos() - item->pos());

    //QPixmap tempPixmap = pixmap;
    //QPainter painter;
    //painter.begin(&tempPixmap);
    //painter.fillRect(pixmap.rect(), QColor(127, 127, 127, 127));
    //painter.end();

    //item->setPixmap(tempPixmap);

    //if (drag->exec(Qt::CopyAction | Qt::MoveAction, Qt::CopyAction) == Qt::MoveAction)
    //{
    //    item->close();
    //} else
    //{
    //    item->show();
        //item->setPixmap(pixmap);
    //}
    QAbstractItemView::mousePressEvent(event);
}
*/

bool WorkflowTreeView::isFirstItem(const QModelIndex &index) const
{
    QModelIndex sib = index.sibling(index.row()-1,0);
    return !sib.isValid();

}
bool WorkflowTreeView::isLastItem(const QModelIndex &index) const
{
    QModelIndex sib = index.sibling(index.row()+1,0);
    return !sib.isValid();
}
bool WorkflowTreeView::isInFirstTool(const QModelIndex &index) const
{
    QModelIndex top = index;
    while(top.parent().isValid())
        top = top.parent();
    if (isFirstItem(top)) return true;
    do
    {
        top = top.sibling(top.row()-1,0);
    }
    while (top.isValid() && (ToolUsageRole != top.data(UsageRole)));
    return !top.isValid();
}
bool WorkflowTreeView::isInLastTool(const QModelIndex &index) const
{
    QModelIndex top = index;
    while(top.parent().isValid())
        top = top.parent();
    if (isLastItem(top)) return true;
    do
    {
        top = top.sibling(top.row()+1,0);
    }
    while (top.isValid() && (ToolUsageRole != top.data(UsageRole)));
    return !top.isValid();
}

bool WorkflowTreeView::canPipeInto(const QModelIndex &index) const
{
    if (isInFirstTool(index)) return false;
    return true;
/*
    QString select_query = "SELECT count(*)"
        "FROM ToolOptions WHERE op_string like '%> %'";
    QSqlQuery query = new QSqlQuery(select_query);
    query->next();
    return query->value(0).toInt();
*/
}

bool WorkflowTreeView::canPipeOutof(const QModelIndex &index) const
{
    if (isInLastTool(index)) return false;
    return true;
}

QList<QStandardItem*> WorkflowTreeView::getItemFromRole(QStandardItem* parent,
                                                   UsageRoleType usage,
                                                   const QString &match_text,
                                                   int column)
{
    QList<QStandardItem*> list;
    for (int r=0 ; r < parent->row() ; r++)
    {
        QStandardItem* child = parent->child(r,column);
        if (child && (child->data(usage).toString().startsWith(match_text)))
        {
            list.append(child);
        }
    }
    return list;
}
































