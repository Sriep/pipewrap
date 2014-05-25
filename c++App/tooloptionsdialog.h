#ifndef TOOLOPTIONSDIALOG_H
#define TOOLOPTIONSDIALOG_H

#include <QDialog>
#include <QQueue>
class QSqlQuery;
class QFormLayout;
class QDialogButtonBox;
class QLineEdit;
class QCheckBox;
class QStandardItem;
class QStandardItemModel;

class ToolOptionsDialog : public QDialog
{
    Q_OBJECT
public:
    //explicit ToolOptionsDialog(QWidget *parent = 0);
    explicit ToolOptionsDialog(QStandardItem* item, QWidget *parent = 0);
    ~ToolOptionsDialog();
private:
    QWidget* getNextWidget();
    QWidget* getFilenameWidget();
    QString getOptionLabel(const QString& value);
    void addNewDataItem(int child_row, const QString& value);
    void readControlsDB();
    void Init();    

    QSqlQuery* m_tool_option_table;
    QFormLayout* m_forms_layout;
    QDialogButtonBox *m_buttonBox;
    QVector<QString> m_variable_options;
    QQueue<QLineEdit*> m_line_edits;
    QQueue<QCheckBox*> m_check_boxes;
    QStandardItem* m_tree_item;
    //QStandardItemModel m_model;

signals:

public slots:
    virtual void accept();
    //virtual void reject();
};

#endif // TOOLOPTIONSDIALOG_H
