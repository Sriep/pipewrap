#include <QFormLayout>
#include <QSqlQuery>
#include <QLabel>
#include <QLineEdit>
#include <QCheckBox>
#include <QDialogButtonBox>
#include <QFormLayout>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QStandardItem>
#include <QDoubleValidator>
#include <QIntValidator>
#include <QPushButton>
#include <QFileDialog>
#include <QSignalMapper>
#include <QFlag>
#include "workflowtreeview.h"
#include "tooloptionsdialog.h"

ToolOptionsDialog::ToolOptionsDialog(QStandardItem* tree_item, QWidget *parent)
   : QDialog(parent), m_tree_item(tree_item)
{
    Init();
}

ToolOptionsDialog::~ToolOptionsDialog()
{
}

void ToolOptionsDialog::Init()
{
    setWindowTitle(m_tree_item->text());
    m_fileEditMapper = new QSignalMapper(this);

    m_buttonBox = new QDialogButtonBox(QDialogButtonBox::Save
                                       | QDialogButtonBox::Cancel);
    connect(m_buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
    connect(m_buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
    readControlsDB();
}

void ToolOptionsDialog::accept()
{
    int num_children = 0;
    if (m_tool_option_table->first())
    {
        //QString shell_command = m_tool_option_table->value(0).toString();
        //shell_command = shell_command + " ";
        do
        {            
            QString data_type = m_tool_option_table->value(2).toString();
            if (data_type == "")
            {
                QCheckBox* check_box = m_check_boxes.dequeue();
                if (check_box->isChecked())
                {
                    addNewDataItem(num_children++, "");
                }
            }
            else
            {
                QLineEdit* line_edit = m_line_edits.dequeue();
                QString text = line_edit->text();
                if (text.length() > 0)
                {
                    addNewDataItem(num_children++, text);
                }
            }

        }
        while(m_tool_option_table->next());
        //m_command = new QString(shell_command);
    }

    QDialog::accept();
}

void ToolOptionsDialog::addNewDataItem(int child_row, const QString& value)
{
    QString key = m_tool_option_table->value(1).toString();
    QString datatype = m_tool_option_table->value(2).toString();
    QString label = m_tool_option_table->value(3).toString();
    QString tooltip = m_tool_option_table->value(4).toString();
    QString text = key +  ": " + label;
    QString shell_text = key + value;    
    QStandardItem* item = new QStandardItem(text);
    item->setToolTip(tooltip);
    item->setData(datatype,DataTypeRole);
    item->setData(shell_text,CommandStrRole);
    item->setData(key,NameRole);
    item->setData(LabelUsageRole, UsageRole);
    item->setEditable(false);
    m_tree_item->setChild(child_row,item);

    QStandardItem* value_item = new QStandardItem(value);
    value_item->setToolTip(tooltip);
    value_item->setData(datatype,DataTypeRole);
    value_item->setData(shell_text,CommandStrRole);
    value_item->setData(key,NameRole);
    value_item->setData(DataUsageRole, UsageRole);
    m_tree_item->setChild(child_row,1,value_item);
}
/*
QString ToolOptionsDialog::getOptionLabel(const QString& value)
{
    QString key = m_tool_option_table->value(1).toString();
    QString label = m_tool_option_table->value(3).toString();
    QString datatype = m_tool_option_table->value(2).toString();
    if ("" == datatype)
    {
        return key +  ": " + label;
    }
    else
    {
        return key +  ": " + label + " = \t" + value;
    }
}*/

void ToolOptionsDialog::readControlsDB()
{
    QString tool_name = m_tree_item->text();

    m_forms_layout = new QFormLayout;
    m_grid_layout = new QGridLayout;
    m_line_edits.clear();

    QString command = m_tree_item->data(CommandStrRole).toString();
    //QString select_query = "SELECT op_command, op_string, op_data_type,"
    //    "op_short_description, op_long_description, op_optional, op_in_out,"
    //    "op_group, op_postion, op_default_text,op_default_int "
    //    "FROM ToolOptions WHERE op_command='";

    QString select_query = "SELECT op_command, op_string, op_data_type,"
        "op_short_description, op_long_description, op_optional, op_in_out,"
        "op_group, op_postion, op_default_text,op_default_int "
        "FROM ToolOptions WHERE op_command like '%";
    select_query += command + "%' ORDER BY op_postion";
    m_tool_option_table = new QSqlQuery(select_query);

    int row = 0;
    while (m_tool_option_table->next())
    {
        //Q_ASSERT(command == (m_tool_option_table->value(0)).toString());

        QString option = m_tool_option_table->value(1).toString();
        QString description = m_tool_option_table->value(3).toString();
        QString tool_tip = m_tool_option_table->value(4).toString();
        QString label_text = option;
        label_text += "\t" + description;

        QVector<QWidget*> next_widgets(getNextWidget());
        for (int i = 0; i < next_widgets.size(); ++i)
        {
            next_widgets.at(i)->setToolTip(tool_tip);
            next_widgets.at(i)->setWhatsThis(tool_tip);
            //m_forms_layout->addRow(new QLabel(label_text), next_widgets.at(i));
            m_grid_layout->addWidget(new QLabel(label_text)
                                     , row % col_size
                                     , 2*(row / col_size));
            m_grid_layout->addWidget(next_widgets.at(i)
                                     , row % col_size
                                     , 2*(row / col_size) +1);
            row++;
        }
    }
    //m_forms_layout->setRowWrapPolicy(QFormLayout::WrapAllRows);
    //m_forms_layout->addRow(m_buttonBox);
    //setLayout(m_forms_layout);

    m_grid_layout->addWidget(m_buttonBox, col_size+1,0);
    setLayout(m_grid_layout);

}

void ToolOptionsDialog::setFilenameSlot(QWidget* widget)
{
    //const QFileDialog::Options options = QFlag();
    QString fileName = QFileDialog::getOpenFileName(this,tr("Select filename"));
    //QString fileName = QFileDialog::getOpenFileName();
    if (!fileName.isEmpty())
    {
        QLineEdit* line_edit = qobject_cast<QLineEdit*>(widget);
        line_edit->setText(fileName);
    }
}

QVector<QWidget*> ToolOptionsDialog::getFilenameWidget()
{
    QLineEdit* line_edit = new QLineEdit;
    m_line_edits.enqueue(line_edit);
    QPushButton *setFileBut = new QPushButton(tr("..."));

    m_fileEditMapper->setMapping(setFileBut, line_edit);
    connect(setFileBut, SIGNAL(clicked()),m_fileEditMapper, SLOT(map()));
    //connect(setFileBut, SIGNAL(released()),m_fileEditMapper, SLOT(map()));
    connect(m_fileEditMapper, SIGNAL(mapped(QWidget*)),
            this, SLOT(setFilenameSlot(QWidget*)));

    QString option = m_tool_option_table->value(1).toString();
    SetEditFromExistingItem(m_tree_item, option, line_edit);
    QVector<QWidget*> rtv;
    rtv.append(line_edit);
    //rtv.append(setFileBut);
    return rtv;
}

void ToolOptionsDialog::SetEditFromExistingItem(QStandardItem* parent,
                                             const QString& name,
                                             QLineEdit* line_edit)
{
    for (int r=0 ; r < parent->rowCount() ; r++)
    {
        QStandardItem* child = parent->child(r,1);
        if (child && (name == child->data(NameRole).toString()))
        {
            line_edit->setText(child->text());
            return;
        }
    }
}

void ToolOptionsDialog::SetCheckFromExistingItem(QStandardItem* parent,
                                             const QString& name,
                                             QCheckBox* checkBox)
{
    for (int r=0 ; r < parent->rowCount() ; r++)
    {
        QStandardItem* child = parent->child(r,1);
        if (child && (name == child->data(NameRole).toString()))
        {
            checkBox->setChecked(true);
            return;
        }
    }
}

QVector<QWidget*> ToolOptionsDialog::getNextWidget()
{    
    QString data_type = m_tool_option_table->value(2).toString();
    if (data_type == "filename")
    {
        return getFilenameWidget();
    }
    else if (data_type == "")
    {
        QCheckBox* check_box = new QCheckBox;
        QString option = m_tool_option_table->value(1).toString();
        SetCheckFromExistingItem(m_tree_item, option, check_box);

        m_check_boxes.enqueue(check_box);
        QVector<QWidget*> rtv;
        rtv.append(check_box);
        return rtv;
    }    
    else
    {
        QLineEdit* line_edit = new QLineEdit;
        //Might want to add shell varable for numeric field
        /*if (data_type == "double")
        {
            QDoubleValidator* validator = new QDoubleValidator;
            line_edit->setValidator(validator);
        }
        else if (data_type == "int")
        {
            QIntValidator* validator = new QIntValidator;
            line_edit->setValidator(validator);
        }*/

        QString option = m_tool_option_table->value(1).toString(); //op_string
        SetEditFromExistingItem(m_tree_item, option, line_edit);

        m_line_edits.enqueue(line_edit);
        QVector<QWidget*> rtv;
        rtv.append(line_edit);
        return rtv;
    }
}







