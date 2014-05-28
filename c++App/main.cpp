#include <QApplication>
#include <QtSql>
#include "workflowtreeview.h"
#include "mainwindow.h"


int main(int argc, char *argv[])
{    
    QApplication a(argc, argv);

    //QSqlDatabase* db = connectPsql();
 /*    QSqlDatabase db = QSqlDatabase::addDatabase("QPSQL");
    db.setHostName( "localhost" );
    db.setDatabaseName( "It1" );
    db.setUserName( "piers" );
    db.setPassword( "piers" );

    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL","It1");
    db.setHostName( "localhost" );
    db.setDatabaseName( "It1" );
    db.setUserName( "root" );
    db.setPassword( "" );*/

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setHostName( "main" );
    db.setDatabaseName( "/media/sdb2/Projects/Source/Dissertation/SQLData/SqliteData" );
    db.setUserName( "" );
    db.setPassword( "" );

    if( !db.open() )
    {
        qDebug() << db.lastError();
        qFatal( "Failed to connect." );
    }    
    qDebug( "Connected!" );

    MainWindow mw;
    mw.show();
    //WorkflowTreeView w;
    //w.show();
    return a.exec();
}


/*
QSqlDatabase* connectPsql()
{
    QSqlDatabase* db = NULL;
    *db = QSqlDatabase::addDatabase("QPSQL");
    qDebug("QSqlDatabase::addDatabase");
    db->setHostName( "localhost" );
    db->setDatabaseName( "test" );
    db->setUserName( "piers" );
    db->setPassword( "piers" );
    return db;
}

QSqlDatabase* connectMysql()
{
    QSqlDatabase* db = NULL;
    *db = QSqlDatabase::addDatabase("QMYSQL","It1");
    db->setHostName( "localhost" );
    db->setDatabaseName( "test" );
    db->setUserName( "root" );
    db->setPassword( "" );
    return db;
}*/
