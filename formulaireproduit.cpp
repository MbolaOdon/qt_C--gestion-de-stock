#include "formulaireproduit.h"
#include "ui_formulaireproduit.h"
#include  "databaseconnection.h"
#include <QDebug>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QMessageBox>
#include<QGraphicsDropShadowEffect>

FormulaireProduit::FormulaireProduit(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FormulaireProduit)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_TranslucentBackground); //enable MainWindow to be transparent

    QGraphicsDropShadowEffect* effect = new QGraphicsDropShadowEffect();
    effect->setBlurRadius(5);
    this->setGraphicsEffect(effect);




}

FormulaireProduit::~FormulaireProduit()
{
    delete ui;
}


void FormulaireProduit::setDataProduit(QString design,QString num, QString stock){

     ui->Designation->setText(design);
     ui->NumProduit->setText(num);
     ui->Stock->setText(stock);
     ui->btnValiderAjout->setVisible(false);
     ui->pushButton->setVisible(true);

}

void FormulaireProduit::on_btnAnnulerAjout_clicked()
{
    this->close();
}


void FormulaireProduit::on_btnValiderAjout_clicked()
{

    QString designation = ui->Designation->text();
    QString numProduit = ui->NumProduit->text();
    QString stock = ui->Stock->text();

    if (designation.isEmpty() || numProduit.isEmpty() || stock.isEmpty() ) {

      QMessageBox::information(this,"Formulaire incorrect","Veulliez remplir tout les champs");
      return;

    }

    DatabaseConnection connection;

    if(connection.Connect()){
        qDebug()<<"connectée"<< endl;
    }

    QSqlQuery qry;

    qry.exec("SELECT * FROM produit_table");

    while (qry.next()) {

        if (qry.value(0)== numProduit && qry.value(1) == designation) {

            QMessageBox::information(this,"Ajout de Produit","Produit déja saisie");
            return;
        }
    }
    QMessageBox::StandardButton reponse=QMessageBox::question(this,"Ajout de Produit","Voulez vous ajouter cette produit",QMessageBox::Ok|QMessageBox::No);

    if (reponse==QMessageBox::Ok) {

    if(qry.exec("INSERT INTO produit_table ( Numero, Designation, Stock) VALUES( '"+numProduit+"', '"+designation+"', '"+stock+"')")){
          qDebug()<<"qeury executer"<<endl;

           ui->Designation->setText("");
           ui->NumProduit->setText("");
            ui->Stock->setText("");

        }

      }
    this->close();


}

void FormulaireProduit::on_pushButton_clicked()
{
    QString designation = ui->Designation->text();
    QString numProduit = ui->NumProduit->text();
    QString stock = ui->Stock->text();




    DatabaseConnection connection;

    if(connection.Connect()){
        qDebug()<<"connectée"<< endl;
    }

    QSqlQuery qry;

    if(qry.exec("UPDATE produit_table SET   Designation='"+designation+"', Stock='"+stock+"' WHERE Numero='"+numProduit+"'")){
        qDebug()<<"qeury executer"<<endl;

         ui->Designation->setText("");
         ui->NumProduit->setText("");
          ui->Stock->setText("");

    }
      else qDebug()<<"non executer"<<endl;

    connection.Disconnect();
    this->close();
}


