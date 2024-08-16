#include "formulairebon.h"
#include "ui_formulairebon.h"
#include  "databaseconnection.h"
#include <QDebug>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QMessageBox>
#include<QGraphicsDropShadowEffect>

FormulaireBon::FormulaireBon(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FormulaireBon)
{
    ui->setupUi(this);

    ui->btnValiderAjout->setVisible(true);
    ui->pushButton->setVisible(false);
    getListeProduit();
    setAttribute(Qt::WA_TranslucentBackground); //enable MainWindow to be transparent

    QGraphicsDropShadowEffect* effect = new QGraphicsDropShadowEffect();
    effect->setBlurRadius(5);
    this->setGraphicsEffect(effect);


}

FormulaireBon::~FormulaireBon()
{
    delete ui;
}
void FormulaireBon::getListeProduit(){
    DatabaseConnection connection;

   if(connection.Connect()){
       qDebug()<<"connectée"<< endl;
   }

       QSqlQuery query;
       ui->comboBoxListeProduit->clear();


       if(query.exec("SELECT Designation FROM produit_table")){
           while(query.next()){
               ui->comboBoxListeProduit->addItems(query.value(0).toStringList());
           }
       }
    connection.Disconnect();
}
void FormulaireBon::setDataBon(QString idB, QString NumBon, QString NumProduit, QString Quantite, QString Nature, QString DateEntree){
    ui->lineEditNumBon->setText(NumBon);
    ui->lineEditNumProduit->setText(NumProduit);
    ui->lineEditQuantite->setText(Quantite);
    ui->comboBoxNature->setCurrentText(Nature);
    ui->dateEdit->setDisplayFormat(DateEntree);
    idBon = idB;
    ui->pushButton->setVisible(true);
    ui->btnValiderAjout->setVisible(false);
    DatabaseConnection connection;

   if(connection.Connect()){
       qDebug()<<"connectée"<< endl;
   }


       QSqlQuery query;
       query.exec("SELECT Designation FROM produit_table WHERE  Numero = '"+NumProduit+"'");
       while(query.next()){
           ui->comboBoxListeProduit->setCurrentText(query.value(0).toString());

       }

}

void FormulaireBon::on_btnAnnulerAjout_clicked()
{
    this->close();
}


void FormulaireBon::on_btnValiderAjout_clicked()
{
    QString NumBon,NumProduit,Nature,DateEntree;
    int Quantite;


    NumBon = ui->lineEditNumBon->text();
    NumProduit = ui->lineEditNumProduit->text();
    Quantite = ui->lineEditQuantite->text().toInt();
    Nature = ui->comboBoxNature->currentText();
    DateEntree = ui->dateEdit->text();

    if (NumBon.isEmpty() || NumProduit.isEmpty() || Nature.isEmpty() ||  DateEntree.isEmpty() ) {

      QMessageBox::information(this,"Formulaire incorrect","Veulliez remplir tout les champs");
      return;

    }


     DatabaseConnection connection;

    if(connection.Connect()){
        qDebug()<<"connectée"<< endl;
    }
qDebug()<<"tsy lasa    .................";

        QSqlQuery query,qry,qry2,qr;


    if(Nature == "Bonentrée"){
        qDebug()<<"condition lasa    .................";

        qry2.exec("SELECT * FROM bonEntree_table");

        while (qry2.next()) {

            if (qry2.value(1).toString() == NumBon && qry2.value(2).toString() == NumProduit) {
                QMessageBox::information(this,tr("Ajout bon"),tr("Bon déja saisie"));

             break;
            }
            if(qry2.value(1).toString() != NumBon && qry2.value(2).toString() != NumProduit){

                qry.prepare("INSERT INTO bonEntree_table (NumBon,NumProduit,Quantité,Nature,DateEntrée) VALUES('"+NumBon+"','"+NumProduit+"','"+QString::number(Quantite)+"','"+Nature+"','"+DateEntree+"') ");
                 query.exec("SELECT * FROM produit_table Stock WHERE Numero= '"+NumProduit+"'");
                    QMessageBox::StandardButton reponse=QMessageBox::question(this,"Ajout de bon","Voulez vous ajouter cette bon",QMessageBox::Ok|QMessageBox::No);
                qDebug()<<"ts mandeha lasa";
                     if (reponse==QMessageBox::Ok) {
                         qry.exec();
                         query.exec("UPDATE produit_table SET Stock=Stock+'"+QString::number(Quantite)+"' WHERE Numero ='"+NumProduit+"' ");

                         QMessageBox::information(this,tr("Ajout bon"),tr("Bon ajouter avec succès"));

                         this->close();

                     }

                     if (reponse==QMessageBox::No) {
                         this->close();

                     }


                break;
            }

        }



    }

    if(Nature == "Bonsortie"){

        qry2.exec("SELECT * FROM bonSortie_table");

        while (qry2.next()) {

            if (qry2.value(1).toString() == NumBon && qry2.value(2).toString() == NumProduit) {
                QMessageBox::information(this,tr("Ajout bon"),tr("Bon déja saisie"));
                break;

            }
            if(qry2.value(1).toString() != NumBon && qry2.value(2).toString() == NumProduit){
                qry.prepare("INSERT INTO bonSortie_table (NumBon,NumProduit,Quantité,Nature,DateSortie) VALUES('"+NumBon+"','"+NumProduit+"','"+QString::number(Quantite)+"','"+Nature+"','"+DateEntree+"')");
                query.exec("SELECT Stock FROM produit_table  WHERE Numero= '"+NumProduit+"'");
                while(query.next()){
                    if(query.value(0) < Quantite){
                       QMessageBox::information(this,"Stock insuffisante","Stock insuffisant ");
                    }
                    else{
                        QMessageBox::StandardButton reponse=QMessageBox::question(this,"Ajout de bon","Voulez vous ajouter cette bon",QMessageBox::Ok|QMessageBox::No);

                         if (reponse==QMessageBox::Ok) {
                            qry.exec();
                            query.exec("UPDATE produit_table SET Stock=Stock-'"+QString::number(Quantite)+"' WHERE Numero ='"+NumProduit+"' ");
                            QMessageBox::information(this,tr("Ajout bon"),tr("Bon ajouter avec succès"));
                            ui->lineEditNumBon->setText("");
                            ui->lineEditNumProduit->setText("");
                            ui->lineEditQuantite->setText("");

                            this->close();
                         }
                         if (reponse==QMessageBox::No) {
                             this->close();

                         }
                     }
                }
                break;

            }

        }




        }





  }





void FormulaireBon::on_comboBoxListeProduit_currentTextChanged(const QString &arg1)
{

}


void FormulaireBon::on_comboBoxListeProduit_activated(const QString &arg1)
{
    QString designation = ui->comboBoxListeProduit->currentText();

    DatabaseConnection connection;

   if(connection.Connect()){
       qDebug()<<"connectée"<< endl;
   }

       QSqlQuery query;
       if(query.exec("SELECT Numero FROM produit_table WHERE Designation ='"+designation+"'")){
           while(query.next()){
               ui->lineEditNumProduit->setText(query.value(0).toString());
           }

       }

}


void FormulaireBon::on_pushButton_clicked()
{
    QString NumBon,NumProduit,Nature,DateEntree;
    int Quantite,totalEntree;


    NumBon = ui->lineEditNumBon->text();
    NumProduit = ui->lineEditNumProduit->text();
    Quantite = ui->lineEditQuantite->text().toInt();
    Nature = ui->comboBoxNature->currentText();
    DateEntree = ui->dateEdit->text();

     DatabaseConnection connection;

    if(connection.Connect()){
        qDebug()<<"connectée"<< endl;
    }


        QSqlQuery query,qry,qry2,qr;


    if(Nature == "Bonentrée"){

        qry2.exec("SELECT * FROM bonEntree_table");

        while (qry2.next()) {

            if (qry2.value(1).toString() == NumBon && qry2.value(2).toString() == NumProduit) {

                QMessageBox::StandardButton reponse=QMessageBox::question(this,"Modification de bon","Voulez vous modifier cette bon",QMessageBox::Ok|QMessageBox::No);

                    if (reponse==QMessageBox::Ok) {
                        qr.exec("SELECT Quantité FROM bonEntree_table WHERE NumBon='"+NumBon+"' AND NumProduit='"+NumProduit+"'");
                       while (qr.next()){
                           totalEntree = qr.value(0).toInt();
                       }
                            qr.prepare("UPDATE bonEntree_table SET Quantité='"+QString::number(Quantite)+"' WHERE  NumBon='"+NumBon+"' AND NumProduit='"+NumProduit+"' ");

                             if(query.exec("UPDATE produit_table SET Stock=Stock-'"+QString::number(totalEntree)+"'+'"+QString::number(Quantite)+"' WHERE Numero ='"+NumProduit+"'")){
                                 qr.exec();
                            ui->lineEditNumBon->setText("");
                            ui->lineEditNumProduit->setText("");
                            ui->lineEditQuantite->setText("");
                             QMessageBox::information(this,tr("Enregistrer"),tr("Bon modifier avec succès........................................."));

                            }

                    }
                    if (reponse==QMessageBox::No) {
                        return;

                    }
                    break;

                }


            }


    }
    if(Nature == "Bonsortie"){
        query.exec("SELECT Stock FROM produit_table  WHERE Numero= '"+NumProduit+"'");
        while(query.next()){
            if(query.value(0) < Quantite){
               QMessageBox::information(this,"Stock insuffisante","Stock insuffisant");
            }
        }
        qry2.exec("SELECT * FROM bonSortie_table");

        while (qry2.next()) {

            if (qry2.value(1).toString() == NumBon && qry2.value(2).toString() == NumProduit) {

                QMessageBox::StandardButton reponse=QMessageBox::question(this,"Modification de bon","Voulez vous modifier cette bon",QMessageBox::Ok|QMessageBox::No);

                    if (reponse==QMessageBox::Ok) {
                        qr.exec("SELECT Quantité FROM bonSortie_table WHERE NumBon='"+NumBon+"' AND NumProduit='"+NumProduit+"'");
                       while (qr.next()){
                           totalEntree = qr.value(0).toInt();
                       }
                            qr.prepare("UPDATE bonSortie_table SET Quantité='"+QString::number(Quantite)+"' WHERE  NumBon='"+NumBon+"' AND NumProduit='"+NumProduit+"' ");

                             if(query.exec("UPDATE produit_table SET Stock=Stock+'"+QString::number(totalEntree)+"'-'"+QString::number(Quantite)+"' WHERE Numero ='"+NumProduit+"'")){
                                 qr.exec();
                            ui->lineEditNumBon->setText("");
                            ui->lineEditNumProduit->setText("");
                            ui->lineEditQuantite->setText("");
                             QMessageBox::information(this,tr("Enregistrer"),tr("Bon modifier avec succès........................................."));

                            }

                    }
                    if (reponse==QMessageBox::No) {
                        return;

                    }
                    break;

                }


            }


       /* qry.prepare("insert into bonSortie_table (NumBon,NumProduit,Quantité,Nature,DateSortie) values('"+NumBon+"','"+NumProduit+"','"+QString::number(Quantité)+"','"+Nature+"','"+DateEntrée+"')");
        query.exec("SELECT * FROM produit_table Stock WHERE Numero= '"+NumProduit+"'");
        while(query.next()){
            if(query.value(0) < Quantité){
               QMessageBox::information(this,"Stock insuffisante","Il ne reste que ");
            }
            else{
                qry.exec();
                query.exec("UPDATE produit_table SET Stock=Stock-'"+QString::number(Quantité)+"' WHERE Numero ='"+NumProduit+"' ");
            }
        }

        //if(qry.exec("insert into bonSortie_table (NumBon,NumProduit,Quantité,Nature,DateSortie) values('"+NumBon+"','"+NumProduit+"','"+QString::number(Quantité)+"','"+Nature+"','"+DateEntrée+"')"))
       // {

            // query.exec("UPDATE produit_table SET Stock=Stock-'"+QString::number(Quantité)+"' WHERE Numero ='"+NumProduit+"' ");
           // QMessageBox::information(this,tr("save"),tr("saved"));
        //}

         if(qry.exec("UPDATE bonSortie_table SET NumBon='"+NumBon+"',NumProduit='"+NumProduit+"',Quantité='"+Quantité+"',Nature='"+Nature+"',DateSortie='"+DateEntrée+"' WHERE NumBon='"+NumBon+"' AND NumProduit='"+NumProduit+"'")){

             qry.exec("SELECT SUM(Quantité) FROM bonSortie_table WHERE  NumProduit='"+NumProduit+"'");
             while (qry.next()){
                 totalSortie = qry.value(0).toInt();
             }
             qry2.exec("SELECT SUM(Quantité) FROM bonEntree_table WHERE NumProduit='"+NumProduit+"'");
            while (qry2.next()){
                totalEntree = qry2.value(0).toInt();
            }
              query.exec("UPDATE produit_table SET Stock=Stock+'"+QString::number(totalEntree)+"'-'"+QString::number(totalSortie)+"' WHERE Numero ='"+NumProduit+"'");
             ui->lineEditNumBon->setText("");
             ui->lineEditNumProduit->setText("");
             ui->lineEditQuantite->setText("");


         }*/

    }

    //if(qry.exec("UPDATE bonSortie_table SET NumBon='"+NumBon+"',NumProduit='"+NumProduit+"',Quantité='"+QString::number(Quantité)+"',Nature='"+Nature+"',DateSortie='"+DateEntrée+"' WHERE NumBon='"+NumBon+"'"))

    this->close();
}

