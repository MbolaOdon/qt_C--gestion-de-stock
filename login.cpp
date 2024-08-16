#include "login.h"
#include "ui_login.h"
#include <QStyle>
#include <QDebug>
#include <QSqlQuery>
#include "databaseconnection.h"
#include <QMessageBox>
Login::Login(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Login)
{
    ui->setupUi(this);
    ui->lineEditNomUtilisateur->setFocus();
}

Login::~Login()
{
    delete ui;
}


void Login::on_btnIdentifier_clicked()
{
    QString nomUtilisateur = ui->lineEditNomUtilisateur->text();
   QString motDePasse = ui->lineEditPasswdUtilisateur->text();
   QString typeUser = ui->comboBox->currentText();

   DatabaseConnection connection;

   if(connection.Connect()){
       qDebug()<<"connectée";
   }

   QSqlQuery requete;
     if(requete.exec("SELECT * FROM user_table WHERE Nom='"+nomUtilisateur+"' and Mot_de_passe='"+motDePasse+"' and typeUser='"+typeUser+"'")){
         qDebug()<<"qeury executer";
         int condition = 0;

         while (requete.next()) {
             condition++;
         }
         if(condition == 1){

           StockManager *FenetreGestionStock = new StockManager();
          FenetreGestionStock->getTypeUser(typeUser,nomUtilisateur);
             FenetreGestionStock->show();
              this->close();

         }
         else QMessageBox::information(this,"Information","Verifier votre identité et Réessayer");
     }
     else {
         qDebug()<<"non executer";

     }

   connection.Disconnect();
}

