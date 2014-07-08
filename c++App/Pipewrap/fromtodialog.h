#ifndef FROMTODIALOG_H
#define FROMTODIALOG_H

#include <QDialog>

class QStandardItem;
class QDialogButtonBox;
class QFormLayout;
class QLineEdit;

class FromToDialog : public QDialog
{
    Q_OBJECT
public:
    explicit FromToDialog(QStandardItem* item, QWidget *parent = 0);
private:
    void Init();

    QStandardItem* m_item;
    QDialogButtonBox* m_button_box;
    QFormLayout* m_forms_layout;
    QLineEdit* m_from_edit;
    QLineEdit* m_to_edit;
    QLineEdit* m_step_edit;

signals:

public slots:
    virtual void accept();

};

#endif // FROMTODIALOG_H
