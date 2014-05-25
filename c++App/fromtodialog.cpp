#include <QStandardItem>
#include <QFormLayout>
#include <QDialogButtonBox>
#include <QDialogButtonBox>
#include <QVariant>
#include <QLineEdit>
#include <QStandardItem>
#include <QIntValidator>
#include "fromtodialog.h"
#include "workflowtreeview.h"

FromToDialog::FromToDialog(QStandardItem* item, QWidget *parent)
  :  QDialog(parent), m_item(item)
{
    Init();
}

void FromToDialog::Init()
{
    setWindowTitle(tr("Define for loop for doing repeating pipeline"));

    m_button_box = new QDialogButtonBox(QDialogButtonBox::Save
                                       | QDialogButtonBox::Cancel);
    connect(m_button_box, SIGNAL(accepted()), this, SLOT(accept()));
    connect(m_button_box, SIGNAL(rejected()), this, SLOT(reject()));

    m_forms_layout = new QFormLayout;


    QVariant v_from = m_item->data(FromValueRole);
    QVariant v_to = m_item->data(ToValueRole);
    QVariant v_stop = m_item->data(StepValueRole);

    QIntValidator* validator = new QIntValidator;

    m_from_edit = new QLineEdit;
    m_from_edit->setValidator(validator);
    if (v_from.isValid())
    {
        m_from_edit->insert(v_from.toString());
    }

    m_to_edit = new QLineEdit;
    m_to_edit->setValidator(validator);
    if (v_to.isValid())
    {
        m_to_edit->insert(v_to.toString());
    }

    m_step_edit = new QLineEdit;
    m_step_edit->setValidator(validator);
    if (v_stop.isValid())
    {
        m_step_edit->insert(v_stop.toString());
    }

    m_forms_layout->addRow(tr("Value from"), m_from_edit);
    m_forms_layout->addRow(tr("End value"), m_to_edit);
    m_forms_layout->addRow(tr("Step size"), m_step_edit);
    m_forms_layout->addRow(m_button_box);
    setLayout(m_forms_layout);
}

void FromToDialog::accept()
{
    QString from = m_from_edit->text();
    QString to = m_to_edit->text();
    QString step = m_step_edit->text();

    if (from.length()>1 && to.length()> 1)
    {
        if (step.length()==0)
            step = "1";
        m_item->setData(from, FromValueRole);
        m_item->setData(to, ToValueRole);
        m_item->setData(step, StepValueRole);
        setResult(true);
    }
    else
    {
        m_item->setData(QVariant(), FromValueRole);
        m_item->setData(QVariant(), ToValueRole);
        m_item->setData(QVariant(), StepValueRole);
        setResult(false);
    }
    QDialog::accept();
}














