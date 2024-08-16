#include "databaseconnection.h"
#include <QDir>
#include <QDebug>
DatabaseConnection::DatabaseConnection()
{
    database = QSqlDatabase::addDatabase("QSQLITE");
    QString db_path = QDir::currentPath();
        qDebug() <<db_path;    //current path
      db_path =  db_path + QString("/login.db");
        database.setDatabaseName(db_path);
   // database.setDatabaseName("D:\mesprogramme\stockManager\build-G-stock-Desktop_Qt_5_9_0_MinGW_32bit-Debug\login.db");

}

bool DatabaseConnection::Connect()
{
    if(database.open()){
        return true;
    }
    else return false;
}

void DatabaseConnection::Disconnect()
{
    database.close();
}

