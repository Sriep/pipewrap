#include <QApplication>
#include <QtSql>
#include "workflowtreeview.h"
#include "mainwindow.h"
#include "databasedialog.h"

int main(int argc, char *argv[])
{    
    QApplication a(argc, argv);
/*
    //QSqlDatabase* db = connectPsql();
    QSqlDatabase db = QSqlDatabase::addDatabase("QPSQL");
    db.setHostName( "localhost" );
    db.setDatabaseName( "It1" );
    db.setUserName( "piers" );
    db.setPassword( "piers" );

    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL","It1");
    db.setHostName( "localhost" );
    db.setDatabaseName( "It1" );
    db.setUserName( "root" );
    db.setPassword( "" );

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setHostName( "main" );
    db.setDatabaseName( "SqliteData" );
    db.setUserName( "" );
    db.setPassword( "" );

    if( !db.open() )
    {
        qDebug() << db.lastError();
        qFatal( "Failed to connect." );
    }    
    qDebug( "Connected!" );
*/
    int portnumber = 5432;
    DatabaseConnectionDialog* dialog = new DatabaseConnectionDialog(0);
    // optional: set the data that will be presented to the user as auto-filled form
    dialog->setDatabaseName("It1");
    //dialog->setDatabaseName("SqliteData.sqlite3");
    dialog->setDatabasePortNumber(portnumber);
    dialog->setDatabaseHostName("localhost");
    dialog->setDatabaseUsername("pipewrap");
    dialog->setDatabaseDriverName("QPSQL");
    //dialog->setDatabaseDriverName("QSQLITE");
    dialog->setDatabasePassword("pipewrap");

    // enable the connect button if all the data is correct
    dialog->checkFormData();
    // connect the dialog signal to a slot where I will use the connection
    /*connect( dialog,
             SIGNAL(databaseConnect(QSqlDatabase&)),
             &a,
             SLOT(slotHandleNewDatabaseConnection(QSqlDatabase&)));
*/
     // show the dialog (without auto-connection)
    dialog->run(false);


    MainWindow mw;
    mw.show();
    //WorkflowTreeView w;
    //w.show();
    return a.exec();
}


