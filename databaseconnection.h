#ifndef DATABASECONNECTION_H
#define DATABASECONNECTION_H
#include<QSqlDatabase>

class DatabaseConnection
{
 private:

public:
    DatabaseConnection();
    bool Connect();
    void Disconnect();
    QSqlDatabase database;
};

#endif // DATABASECONNECTION_H
