#ifndef TOOLOPTIONSDIALOG_H
#define TOOLOPTIONSDIALOG_H

#include <QDialog>
#include <QQueue>
#include <QVector>

class QSqlQuery;
class QFormLayout;
class QGridLayout;
class QDialogButtonBox;
class QLineEdit;
class QCheckBox;
class QStandardItem;
class QStandardItemModel;
class QSignalMapper;

class ToolOptionsDialog : public QDialog
{
    Q_OBJECT
public:
    //explicit ToolOptionsDialog(QWidget *parent = 0);
    explicit ToolOptionsDialog(QStandardItem* item, QWidget *parent = 0);
    ~ToolOptionsDialog();
private:
    const int col_size = 30;

    QVector<QWidget*>  getNextWidget();
    QVector<QWidget*>  getFilenameWidget();
    QString getOptionLabel(const QString& value);
    void addNewDataItem(int child_row, const QString& value);
    void readControlsDB();
    void Init();
    void SetFromExistingItem(QStandardItem* parent,
                             const QString& name,
                             QLineEdit* line_edit);

    QSqlQuery* m_tool_option_table;
    QFormLayout* m_forms_layout;
    QGridLayout* m_grid_layout;
    QDialogButtonBox *m_buttonBox;
    //QVector<QString> m_variable_options;
    QQueue<QLineEdit*> m_line_edits;
    QQueue<QCheckBox*> m_check_boxes;
    QStandardItem* m_tree_item;
    QSignalMapper* m_fileEditMapper;

    //QStandardItemModel m_model;

signals:

public slots:
    virtual void accept();
    //void setFilenameSlot(QLineEdit* line_edit);
    void setFilenameSlot(QWidget* widget);
    //virtual void reject();
};

#endif // TOOLOPTIONSDIALOG_H
