#include <qmenu.h>
#include <QContextMenuEvent>
#include "oplineedit.h"

OpLineEdit::OpLineEdit(QWidget *parent) :
    QLineEdit(parent)
{
}

 void OpLineEdit::contextMenuEvent(QContextMenuEvent * event)
 {
     QMenu *menu = createStandardContextMenu();
     menu->addAction(tr("Vary Parameter"));
     //...
     menu->exec(event->globalPos());
     delete menu;
 }
