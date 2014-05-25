#ifndef OPLINEEDIT_H
#define OPLINEEDIT_H

#include <QLineEdit>

class OpLineEdit : public QLineEdit
{
    Q_OBJECT
public:
    explicit OpLineEdit(QWidget *parent = 0);

protected:
    virtual void contextMenuEvent(QContextMenuEvent * event);

signals:

public slots:

};

#endif // OPLINEEDIT_H
