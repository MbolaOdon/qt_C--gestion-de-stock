#include "stockmanager.h"
#include "ui_stockmanager.h"
#include "databaseconnection.h"
#include "login.h"
#include <QDebug>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QMessageBox>
#include <QPrinter>
#include <QPrinterInfo>
#include <QPrintDialog>
#include <QFile>
#include <QFileInfo>
#include <QFileDialog>
#include <QPdfWriter>
#include <QGraphicsDropShadowEffect>
#include <QtCharts>
#include <QChartView>
#include <QBarSeries>
#include <QBarSet>
#include <QtCharts/QPieSeries>
#include <QtCharts/QPieSlice>
#include <QVector>
#include <vector>
#include <string>
#include <QList>
#include <QTimer>
#include <QDateTime>
//#include <QPrintPreviewDialog>
#include <QSqlTableModel>

StockManager::StockManager(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::StockManager)
{
    ui->setupUi(this);


    affichageDonneUtilisateur();
    affichageDonneProduit();
    affichageDonneBonEntree();
    affichageDonneBonSortie();
    affichageEtatStock();
    rafraichirUser_table();
    rafraichirPoduit_table();
    rafraichirbonEntree_table();
    rafraichirbonSortie_table();
    appendCbBoxDesignation();

    alertProduit();
    on_btnListeProduit_clicked();
    on_btnGestionProduit_clicked();
    on_btnListeBonSortie_clicked();
    on_btnListeSortie_clicked();
   // on_btnEtatStock_clicked();
   on_btnAccueil_clicked();
    barChart();
    barChart();
    donutChart();
    initialiseAffichage();


    ui->stackedWidget->setCurrentIndex(1);
    ui->tabWidget->setCurrentIndex(0);
   /* modelMvmtStock->setHeaderData(0,Qt::Horizontal,"N°BON");
    modelMvmtStock->setHeaderData(1,Qt::Horizontal,"ENTREE");
    modelMvmtStock->setHeaderData(2,Qt::Horizontal,"SORTIE");
    modelMvmtStock->setHeaderData(3,Qt::Horizontal,"DATE");*/

    setWindowFlags(windowFlags() & ~Qt::WindowCloseButtonHint);
    setAttribute(Qt::WA_TranslucentBackground); //enable MainWindow to be transparent

    QGraphicsDropShadowEffect *shadow = new QGraphicsDropShadowEffect;
    shadow->setOffset(2, 2);
    QGraphicsDropShadowEffect *shadow1 = new QGraphicsDropShadowEffect;
    shadow1->setOffset(2, 2);
    QGraphicsDropShadowEffect *shadow2 = new QGraphicsDropShadowEffect;
    shadow2->setOffset(2, 2);
    shadow->setColor(QColor(0, 0, 0, 127));
    shadow1->setColor(QColor(0, 0, 0, 127));
    shadow2->setColor(QColor(0, 0, 0, 127));
    ui->widget_2->setGraphicsEffect(shadow);
    ui->widget_3->setGraphicsEffect(shadow1);
    ui->widget_4->setGraphicsEffect(shadow2);

    QTimer *timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(showTime()));
    timer->start();
    QDateTime dateTime=QDateTime::currentDateTime();
    QString dateTimeText = dateTime.toString();
    ui->datejour->setText(dateTimeText);
}

StockManager::~StockManager()
{
    delete ui;
}

void StockManager::initialiseAffichage(){

}

void StockManager::showTime(){
    QTime time = QTime::currentTime();
    QString timeText = time.toString("hh : mm : ss");

    if((time.second() % 2) == 0){
        timeText[3]= ' ';
        timeText[8]= ' ';
    }
    ui->datejour->setText(timeText);
}


void StockManager::appendCbBoxDesignation(){
    rafraichirPoduit_table();
    DatabaseConnection connection;

    if(connection.Connect()){
        qDebug()<<"mivoha Etat Stock"<< endl;
       }
    QSqlQuery query;
    ui->cbBoxDesignation->clear();
    if(query.exec("SELECT Designation FROM produit_table")){
        while(query.next()){

            ui->cbBoxDesignation->addItems(query.value(0).toStringList());
        }
    }
}

void StockManager::alertProduit(){
    rafraichirPoduit_table();
    ui->labelProduitMin->setText(""  );
    ui->labelProduitMax->setText("");
    DatabaseConnection connection;
    if(connection.Connect()){
        qDebug()<<"Database open";
    }

    QSqlQuery qrMin,qrMax,qrNbProduit;
    QString produitMin,stockMin,produitMax,stockMax,nbrProduit;

    qrMin.exec("SELECT Designation,MIN(Stock) FROM produit_table ");
    while(qrMin.next()){
        produitMin = qrMin.value(0).toString();
        stockMin = qrMin.value(1).toString();
    }
    qrMax.exec("SELECT Designation,MAX(Stock) FROM produit_table ");
    while(qrMax.next()){
        produitMax = qrMax.value(0).toString();
        stockMax = qrMax.value(1).toString();
    }
    qrNbProduit.exec("SELECT COUNT(*) FROM produit_table");
    while(qrNbProduit.next()){
        nbrProduit = qrNbProduit.value(0).toString();
    }

    ui->labelProduitMin->setText("\tProduit à faible quantité""\n" "\t"+produitMin+" : "+stockMin);
    ui->labelProduitMax->setText("\tProduit à grande quantité""\n" "\t"+produitMax+" : "+stockMax);
    ui->labelNbrProduit->setText("\tNombre de type de produit""\n" "\t"+nbrProduit);

    connection.Disconnect();
}

void StockManager::affichageDonneUtilisateur(){
    QSqlQuery query;
    int ligne= 0;
    query.exec("SELECT COUNT(*) FROM user_table");
    while (query.next()) {
        ligne = query.value(0).toInt();
    }

    modele = new QStandardItemModel(ligne,8);
    int row = 0;
    query.exec("SELECT idUser,Nom,Prenom,Telephone,Adresse,Age,Date,Mot_de_passe FROM user_table");
    while (query.next()) {
        for(int colonne = 0;colonne < 8; colonne++){
            QStandardItem *item = new QStandardItem(query.value(colonne).toString());
            modele->setItem(row,colonne,item);
        }
        row++;
    }
    modele->setHeaderData(0,Qt::Horizontal,"idUser");
    modele->setHeaderData(1,Qt::Horizontal,"Nom");
    modele->setHeaderData(2,Qt::Horizontal,"Prenom");
    modele->setHeaderData(3,Qt::Horizontal,"Age");
    modele->setHeaderData(4,Qt::Horizontal,"Sexe");
    modele->setHeaderData(5,Qt::Horizontal,"Date");
    modele->setHeaderData(6,Qt::Horizontal,"Mot_de_passe");
    modele->setHeaderData(7,Qt::Horizontal,"typeUser");
    ui->tabwListeUtilisateur->setModel(modele);

}

void StockManager::rafraichirUser_table()
{
    DatabaseConnection connection;
    QSqlQueryModel *modele = new QSqlQueryModel();

    if(connection.Connect()){
        qDebug()<<"connectée"<< endl;
    }

    QSqlQuery *query = new QSqlQuery(connection.database);
    query->prepare("SELECT idUser,Nom,Prenom,Age,Sexe,Date,Mot_de_passe,typeUser FROM user_table");
     query->exec();
     modele->setQuery(*query);
     ui->tabwListeUtilisateur->setModel(modele);


}


void StockManager::affichageDonneBonSortie(){
    DatabaseConnection connection;

    if(connection.Connect()){
        qDebug()<<"mivoha"<< endl;
       }
    QSqlQuery query;
    int ligne= 0;
    query.exec("SELECT COUNT(*) FROM bonSortie_table");
    while (query.next()) {
        ligne = query.value(0).toInt();
    }

    modelSortie = new QStandardItemModel(ligne,5);
    int row = 0;
    query.exec("SELECT NumBon,NumProduit,Quantité,Nature,DateSortie FROM bonSortie_table");
    while (query.next()) {
        for(int colonne = 0;colonne < 5; colonne++){
            QStandardItem *item = new QStandardItem(query.value(colonne).toString());
            modelSortie->setItem(row,colonne,item);


        }
        row++;
    }

    modelSortie->setHeaderData(0,Qt::Horizontal,"N°Bon");
    modelSortie->setHeaderData(1,Qt::Horizontal,"N°Produit");
    modelSortie->setHeaderData(2,Qt::Horizontal,"Quantité");
    modelSortie->setHeaderData(3,Qt::Horizontal,"Nature");
    modelSortie->setHeaderData(4,Qt::Horizontal,"DateSortie");


    ui->tbvBonSortie->setModel(modelSortie);
    ui->tbvListeBon->setModel(modelSortie);
    connection.Disconnect();

}

void StockManager::affichageDonneBonEntree(){
    DatabaseConnection connection;

    if(connection.Connect()){
        qDebug()<<"mivoha"<< endl;
        qDebug()<<"bien afficher ssss";
       }
    QSqlQuery query2;
    int ligne1= 0;
    query2.exec("SELECT COUNT(*) FROM bonEntree_table");
    while (query2.next()) {
        ligne1 = query2.value(0).toInt();
    }

    model = new QStandardItemModel(ligne1,5);
    int row1 = 0;
    query2.exec("SELECT NumBon,NumProduit,Quantité,Nature,DateEntrée FROM bonEntree_table");
    while (query2.next()) {
        for(int colonne = 0;colonne < 5; colonne++){
            QStandardItem *item2 = new QStandardItem(query2.value(colonne).toString());
            qDebug()<<query2.value(colonne).toString();

            model->setItem(row1,colonne,item2);

        }
        row1++;
    }
   // modelEntre->setHeaderData(0,Qt::Horizontal,"NumBon");
    model->setHeaderData(1,Qt::Horizontal,"N°Produit");
    model->setHeaderData(2,Qt::Horizontal,"Quantité");
    model->setHeaderData(3,Qt::Horizontal,"Nature");
    model->setHeaderData(4,Qt::Horizontal,"DateEntrée");
    model->setHeaderData(0,Qt::Horizontal,"N°Bon");

    ui->tbvBonSortie->setModel(model);

    ui->tbvListeBon->setModel(model);

}


void StockManager::getTypeUser(QString typeUser,QString nomUtilisateur){
    if(typeUser=="Utilisateur"){
        ui->btnParametreAdministrateur->setDisabled(true);
        ui->profiluser->setText(nomUtilisateur);


    }
    if(typeUser=="Administrateur"){
        ui->btnParametreAdministrateur->setEnabled(true);
        ui->profiluser->setText(nomUtilisateur);
       //ui->tabWidget_2->setTabEnabled(0,true);
    }
}

void StockManager:: affichageEtatStock(){
    DatabaseConnection connection;

    if(connection.Connect()){
        qDebug()<<"mivoha Etat Stock"<< endl;
       }
    QSqlQuery qury;
    int ligne= 0;
    qury.exec("SELECT COUNT(*) FROM produit_table");
    while (qury.next()) {
        ligne = qury.value(0).toInt();
    }

    modelEtatStock = new QStandardItemModel(ligne,3);
    int row = 0;
    qury.exec("SELECT Numero,Designation,Stock FROM produit_table");
    while (qury.next()) {
        for(int colonne = 0;colonne < 3; colonne++){
            QStandardItem *item = new QStandardItem(qury.value(colonne).toString());
            modelEtatStock->setItem(row,colonne,item);

        }
        row++;
    }

    modelEtatStock->setHeaderData(0,Qt::Horizontal,"Numero");
    modelEtatStock->setHeaderData(1,Qt::Horizontal,"Designation");
    modelEtatStock->setHeaderData(2,Qt::Horizontal,"Stock");

    ui->tbvEtatStock->setModel(modelEtatStock);

}


void StockManager::rafraichirPoduit_table(){
    DatabaseConnection connection;
    QSqlQueryModel *modele = new QSqlQueryModel();

    if(connection.Connect()){
        qDebug()<<"connectée";
    }

    QSqlQuery *query = new QSqlQuery(connection.database);
    query->prepare("SELECT Numero, Designation, Stock FROM produit_table");
     query->exec();
     modele->setQuery(*query);
     ui->tabwListeProduit->setModel(modele);
     ui->tbviewListeProduit->setModel(modele);
    barChart();

}

void StockManager::rafraichirbonEntree_table(){
    DatabaseConnection connection;
    QSqlQueryModel *modele = new QSqlQueryModel();

    if(connection.Connect()){
        qDebug()<<"connectée";
    }

    QSqlQuery *query = new QSqlQuery(connection.database);
    query->prepare("SELECT NumBon,NumProduit,Quantité,Nature,DateSortie FROM bonEntree_table");
     query->exec();
     modele->setQuery(*query);
    // ui->tbvBonEntree->setModel(modele);

}
void StockManager::rafraichirbonSortie_table(){
    DatabaseConnection connection;
    QSqlQueryModel *modele = new QSqlQueryModel();

    if(connection.Connect()){
        qDebug()<<"connectée";
    }

    QSqlQuery *query = new QSqlQuery(connection.database);
    query->prepare("SELECT NumBon,NumProduit,Quantité,Nature,DateSortie FROM bonSortie_table");
     query->exec();
     modele->setQuery(*query);
     ui->tbvBonSortie->setModel(modele);

}


void StockManager::donutChart(){
    DatabaseConnection connection;

    if(connection.Connect()){
        qDebug()<<"connectée"<< endl;
    }

    QSqlQuery query;

    int ligne= 0;
    query.exec("SELECT COUNT(*) FROM produit_table");
    while (query.next()) {
        ligne = query.value(0).toInt();
    }
    QList<QString> designation;
    int stock;
   QPieSeries *series = new QPieSeries();
     QChartView *chartView = new QChartView();
     QPieSlice *slice ;
    query.exec("SELECT  Designation, Stock FROM produit_table");
    int x=0;
    while (query.next()) {
             stock = query.value(1).toInt();
              designation.append(query.value(0).toString());
             slice = series->append(designation[x],stock);
            slice->percentage();
             slice->setLabelVisible();
             series->setHoleSize(0.35);
             chartView->setRenderHint(QPainter::Antialiasing);
         chartView->chart()->addSeries(series);
        chartView->chart()->setTitle("Donut Camembert");
        chartView->chart()->setAnimationOptions(QChart::SeriesAnimations);
        chartView->chart()->legend()->setVisible(true);
        chartView->chart()->legend()->setAlignment(Qt::AlignBottom);
            chartView->chart()->setTheme(QChart::ChartThemeLight);
            chartView->chart()->legend()->setFont(QFont("Arial", 8));
     // chartView->setParent(ui->horizontalFrameDonutChart);

        chartView->update();

        x++;
        }

}


/*void StockManager::on_btnPdfChart_clicked()
{

}*/
void StockManager::barChart(){
    DatabaseConnection connection;

    if(connection.Connect()){
        qDebug()<<"connectée"<< endl;
    }

    QSqlQuery query;

    int ligne= 0;
    query.exec("SELECT COUNT(*) FROM produit_table");
    while (query.next()) {
        ligne = query.value(0).toInt();
    }
    QList<QString> designation;
    int stock;
   QBarSet *set[ligne];

    QBarSeries *series = new QBarSeries();
    QChart *chart = new QChart();
     QChartView *chartView;
     QValueAxis *axisY = new QValueAxis();


     query.exec("SELECT  Designation FROM produit_table");
         int x=0;
         while (query.next()) {

                   designation.append(query.value(0).toString());
                    //designation= query.value()
                 set[x] = new QBarSet(designation[x]);
                 x++;
         }
         query.exec("SELECT  Stock FROM produit_table");
             int i=0;
             while (query.next()) {
                 stock = query.value(0).toInt();

                *set[i]<< stock;
                i++;
             }

                for(int i= 0;i<ligne;i++){

                    series->append(set[i]);

                }

          chart->addSeries(series);

         chart->setTitle("Histogramme de l'état de stock");
         chart->setAnimationOptions(QChart::SeriesAnimations);
         chart->animationDuration();


         chart->addAxis(axisY, Qt::AlignLeft);
            series->attachAxis(axisY);
         chartView = new QChartView(chart);

      chartView->setParent(ui->horizontalFrameBarchart);

         chart->legend()->show();
         chartView->chart()->legend()->setAlignment(Qt::AlignBottom);
         chartView->chart()->setTheme(QChart::ChartThemeLight);
             chartView->chart()->legend()->setFont(QFont("Arial", 8));
             chartView->chart()->axes();
             chart->legend()->setVisible(true);

    }




void StockManager::affichageDonneProduit(){
    QSqlQuery query;
    int ligne= 0;
    query.exec("SELECT COUNT(*) FROM produit_table");
    while (query.next()) {
        ligne = query.value(0).toInt();
    }

    modele = new QStandardItemModel(ligne,3);

    int row = 0;
    query.exec("SELECT Numero, Designation, Stock FROM produit_table");
    while (query.next()) {
        for(int colonne = 0;colonne < 3; colonne++){
            QStandardItem *item = new QStandardItem(query.value(colonne).toString());
            modele->setItem(row,colonne,item);
        }
        row++;

    }

    modele->setHeaderData(0,Qt::Horizontal,"Numero");
    modele->setHeaderData(1,Qt::Horizontal,"Designation");
    modele->setHeaderData(2,Qt::Horizontal,"Stock");


    ui->tabwListeProduit->setModel(modele);


}

/* ============================================  Accueil ==============*/

void StockManager::on_btnAccueil_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);

    ui->btnGestionStock->setStyleSheet("#btnGestionStock{background-color:#ff80475b;color: white;}");
    ui->btnAccueil->setStyleSheet("#btnAccueil{background-color:#8E5454;color: #ff80475b;}");
    ui->btnParametreAdministrateur->setStyleSheet("#btnParametreAdministrateur{background-color:#ff80475b;color: white;}");
    ui->pushButton->setStyleSheet("#pushButton{background-color:#ff80475b;color: white;}");
}


void StockManager::on_btnGestionStock_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
    ui->btnGestionStock->setStyleSheet("#btnGestionStock{background-color:#8E5454;color: #ff80475b;}");
    ui->btnAccueil->setStyleSheet("#btnAccueil{background-color:#ff80475b;color: white;}");
    ui->btnParametreAdministrateur->setStyleSheet("#btnParametreAdministrateur{background-color:#ff80475b;color: white;}");
    ui->pushButton->setStyleSheet("#pushButton{background-color:#ff80475b;color: white;}");
}


void StockManager::on_btnParametreAdministrateur_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
    ui->btnGestionStock->setStyleSheet("#btnGestionStock{background-color:#ff80475b;color: white;}");
    ui->btnAccueil->setStyleSheet("#btnAccueil{background-color:#ff80475b;color: white;}");
    ui->btnParametreAdministrateur->setStyleSheet("#btnParametreAdministrateur{background-color:#8E5454;color: #ff80475b;}");
    ui->pushButton->setStyleSheet("#pushButton{background-color:#ff80475b;color: white;}");
}


void StockManager::on_btnQuitter_clicked()
{
    this->close();
    Login *fenetreLogin;
    fenetreLogin = new Login();
    fenetreLogin->show();
}

/*================================== Listage produit et bon ==========================*/




void StockManager::on_btnListeProduit_clicked()
{
    ui->stackedWidget_2->setCurrentIndex(1);
    ui->btnListeProduit->setStyleSheet("#btnListeProduit{font: 75 italic 12pt TeX Gyre Bonum Math;background-color:rgb(104, 10, 10);color: white;}");
    ui->btnListeBon->setStyleSheet("#btnListeBon{font: 75 italic 12pt TeX Gyre Bonum Math;background-color:#ff80475b;color: white;}");
}


void StockManager::on_btnListeBon_clicked()
{
    ui->stackedWidget_2->setCurrentIndex(0);
    ui->btnListeBon->setStyleSheet("#btnListeBon{font: 75 italic 12pt TeX Gyre Bonum Math;background-color:rgb(104, 10, 10);color: white;}");
    ui->btnListeProduit->setStyleSheet("#btnListeProduit{font: 75 italic 12pt TeX Gyre Bonum Math;background-color:#ff80475b;color: white;}");
}


void StockManager::on_searchListeProduit_textChanged(const QString &arg1)
{
    QString searchValue = ui->searchListeProduit->text();

    DatabaseConnection connection;

    if(connection.Connect()){
        qDebug()<<"mivoha Etat mouvement Stock"<< endl;
       }
    QSqlQuery query;
    int ligne= 0;
    query.exec("SELECT COUNT(*) FROM produit_table");
    while (query.next()) {
        ligne = query.value(0).toInt();
    }
    if(searchValue.isEmpty() || searchValue ==""){
        affichageDonneProduit();
        return;
    }

    else{
        modele3 = new QStandardItemModel(ligne,3);

        int row = 0;
        query.exec("SELECT * FROM produit_table WHERE Numero LIKE '%"+searchValue+"%' OR Designation LIKE '%"+searchValue+"%' OR Stock LIKE '%"+searchValue+"%'");
        while(query.next()){
            for(int colonne = 0;colonne < 3; colonne++){
                QStandardItem *item = new QStandardItem(query.value(colonne).toString());
                modele3->setItem(row,colonne,item);
        }
             row++;
    }


        modele3->setHeaderData(0,Qt::Horizontal,"Numero");
        modele3->setHeaderData(1,Qt::Horizontal,"Designation");
        modele3->setHeaderData(2,Qt::Horizontal,"Stock");

        ui->tbviewListeProduit->setModel(modele3);
   }
}


void StockManager::on_searchListeBon_textChanged(const QString &arg1)
{
    if(listeBon == 1){
         QString searchValue = ui->searchListeBon->text();
        DatabaseConnection connection;

        if(connection.Connect()){
            qDebug()<<"mivoha Etat mouvement Stock"<< endl;
           }
        QSqlQuery queryy2;
        int ligne= 0;
        queryy2.exec("SELECT COUNT(*) FROM bonEntree_table");
        while (queryy2.next()) {
            ligne = queryy2.value(0).toInt();
        }
        if(searchValue.isEmpty()){
            affichageDonneBonEntree();
            return;
        }


            modele5 = new QStandardItemModel(ligne,5);

            int rowss = 0;
            queryy2.exec("SELECT * FROM bonEntree_table WHERE  NumProduit LIKE '%"+searchValue+"%' OR Quantité LIKE '%"+searchValue+"%' OR Nature LIKE '%"+searchValue+"%' DateEntrée LIKE '%"+searchValue+"%' ");
            while(queryy2.next()){
                for(int colonne = 0;colonne < 5; colonne++){
                    QStandardItem *items = new QStandardItem(queryy2.value(colonne).toString());
                    modele5->setItem(rowss,colonne,items);
            }
            rowss++;
        }

            modele5->setHeaderData(0,Qt::Horizontal,"N° Bon");
            modele5->setHeaderData(1,Qt::Horizontal,"N° Produit");
            modele5->setHeaderData(2,Qt::Horizontal,"Quantité");
            modele5->setHeaderData(3,Qt::Horizontal,"Nature");
            modele5->setHeaderData(4,Qt::Horizontal,"Date d'Entree");
            ui->tbvListeBon->setModel(modele5);

    }

    if(listeBon == -1 || listeBon== 0){
         QString searchValue = ui->searchListeBon->text();
        DatabaseConnection connection;

        if(connection.Connect()){
            qDebug()<<"mivoha Etat mouvement Stock"<< endl;
           }
        QSqlQuery queryy;
        int ligne= 0;
        queryy.exec("SELECT COUNT(*) FROM bonSortie_table");
        while (queryy.next()) {
            ligne = queryy.value(0).toInt();
        }
        if(searchValue.isEmpty()){
            affichageDonneBonSortie();
            return;
        }


            modele7 = new QStandardItemModel(ligne,5);

            int rowss = 0;
            queryy.exec("SELECT * FROM bonSortie_table WHERE  NumProduit LIKE '%"+searchValue+"%' OR Quantité LIKE '%"+searchValue+"%' OR Nature LIKE '%"+searchValue+"%' DateSortie LIKE '%"+searchValue+"%' ");
            while(queryy.next()){
                for(int colonne = 0;colonne < 5; colonne++){
                    QStandardItem *items = new QStandardItem(queryy.value(colonne).toString());
                    modele7->setItem(rowss,colonne,items);
            }
            rowss++;
        }

            modele7->setHeaderData(0,Qt::Horizontal,"N°Bon");
            modele7->setHeaderData(1,Qt::Horizontal,"N°Produit");
            modele7->setHeaderData(2,Qt::Horizontal,"Quantité");
            modele7->setHeaderData(3,Qt::Horizontal,"Nature");
            modele7->setHeaderData(4,Qt::Horizontal,"Date de sortie");
            ui->tbvListeBon->setModel(modele7);
            connection.Disconnect();
    }
}


void StockManager::on_btnListeEntree_clicked()
{
    affichageDonneBonEntree();
    listeBon = 1;
    ui->btnListeEntree->setStyleSheet("#btnListeEntree{background-color:rgb(104, 10, 10);font: 75 italic 12pt TeX Gyre Bonum Math;color: white;}");
    ui->btnListeSortie->setStyleSheet("#btnListeSortie{font: 75 italic 12pt TeX Gyre Bonum Math;background-color:#ff80475b;color: white;}");
}


void StockManager::on_btnListeSortie_clicked()
{
    affichageDonneBonSortie();
    listeBon = -1;
    ui->btnListeSortie->setStyleSheet("#btnListeSortie{font: 75 italic 12pt TeX Gyre Bonum Math;background-color:rgb(104, 10, 10);color: white;}");
    ui->btnListeEntree->setStyleSheet("#btnListeEntree{font: 75 italic 12pt TeX Gyre Bonum Math;background-color:#ff80475b;color: white;}");
}

/*============================ Gestion de produit et bon =====================*/

void StockManager::on_btnGestionProduit_clicked()
{
    ui->stackedWidget_3->setCurrentIndex(0);
    ui->btnGestionProduit->setStyleSheet("#btnGestionProduit{background-color:rgb(104, 10, 10);font: 75 italic 12pt TeX Gyre Bonum Math;color: white;}");
    ui->btnGestionBon->setStyleSheet("#btnGestionBon{font: 75 italic 12pt TeX Gyre Bonum Math;background-color:#ff80475b;color: white;}");
}


void StockManager::on_btnAjout_clicked()
{
    formulaireProduit = new FormulaireProduit(this);
    formulaireProduit->show();

}


void StockManager::on_btnModifierProduit_clicked()
{
    formulaireProduit = new FormulaireProduit(this);
    formulaireProduit->formulaire =-1;
    formulaireProduit->show();

}


void StockManager::on_btnSupprimerProduit_clicked()
{
    DatabaseConnection connection;

    if(connection.Connect()){
        qDebug()<<"connectée"<< endl;
    }

    QSqlQuery query,qry,requete;
      if(query.exec("DELETE FROM produit_table WHERE  Numero='"+this->num+"' ")){
          qry.exec("DELETE  FROM bonEntree_table WHERE NumProduit='"+this->num+"'");
          requete.exec("DELETE  FROM bonSortie_table WHERE NumProduit='"+this->num+"'");
          qDebug()<<"qeury executer"<<endl;
          QMessageBox::critical(this,tr("Suppression du Produit"),tr("Supprimer"));
          rafraichirPoduit_table();
          rafraichirbonEntree_table();
          rafraichirbonSortie_table();
      }
      else qDebug()<<"non executer"<<endl;

    connection.Disconnect();
}


void StockManager::on_btnActualiser_clicked()
{
    rafraichirPoduit_table();
    appendCbBoxDesignation();
    alertProduit();
}


void StockManager::on_lineEditSearchProduit_textChanged(const QString &arg1)
{
    QString searchValue = ui->lineEditSearchProduit->text();

    DatabaseConnection connection;

    if(connection.Connect()){
        qDebug()<<"mivoha Etat mouvement Stock"<< endl;
       }
    QSqlQuery query;
    int ligne= 0;
    query.exec("SELECT COUNT(*) FROM produit_table");
    while (query.next()) {
        ligne = query.value(0).toInt();
    }
    if(searchValue.isEmpty()){
        affichageDonneProduit();
        return;
    }

    else{
        modele2 = new QStandardItemModel(ligne,3);

        int row = 0;
        query.exec("SELECT * FROM produit_table WHERE Numero LIKE '%"+searchValue+"%' OR Designation LIKE '%"+searchValue+"%' OR Stock LIKE '%"+searchValue+"%' ");
        while(query.next()){
            for(int colonne = 0;colonne < 3; colonne++){
                QStandardItem *item = new QStandardItem(query.value(colonne).toString());
                modele2->setItem(row,colonne,item);
        }
             row++;
    }


        modele2->setHeaderData(0,Qt::Horizontal,"Numero");
        modele2->setHeaderData(1,Qt::Horizontal,"Designation");
        modele2->setHeaderData(2,Qt::Horizontal,"Stock");

        ui->tabwListeProduit->setModel(modele2);
   }
}


void StockManager::on_tabwListeProduit_clicked(const QModelIndex &index)
{
    QString val = ui->tabwListeProduit->model()->data(index).toString();
    qDebug()<<index.data().toString();

    DatabaseConnection connection;

    if(connection.Connect()){
        qDebug()<<"connectée"<< endl;
    }

    QSqlQuery query;
      if(query.exec("SELECT * FROM produit_table WHERE  Numero='"+val+"' OR Designation='"+val+"' OR Stock='"+val+"'  ")){
          qDebug()<<"qeury executer"<<endl;

          while (query.next()) {

              this->num = query.value(0).toString();
              this->design =query.value(1).toString();
             this->stock = query.value(2).toString();

          }

          connection.Disconnect();

      }
      else qDebug()<<"non executer"<<endl;

    connection.Disconnect();
}


void StockManager::on_btnGestionBon_clicked()
{
    ui->stackedWidget_3->setCurrentIndex(1);
    ui->btnGestionBon->setStyleSheet("#btnGestionBon{background-color:rgb(104, 10, 10);font: 75 italic 12pt TeX Gyre Bonum Math;color: white;}");
    ui->btnGestionProduit->setStyleSheet("#btnGestionProduit{background-color:#ff80475b;font: 75 italic 12pt TeX Gyre Bonum Math;color: white;}");
}


void StockManager::on_btnAjoutBon_clicked()
{
    formulaireBon = new FormulaireBon(this);
    formulaireBon->show();
}


void StockManager::on_btnModifierBon_clicked()
{
    formulaireBon = new FormulaireBon(this);
    formulaireBon->show();
    formulaireBon->setDataBon(this->idB,this->NumBon,this->NumProduit,this->Quantite,this->Nature,this->DateEntree);
}


void StockManager::on_btnSupprimerBon_clicked()
{
    DatabaseConnection connection;

    if(connection.Connect()){
        qDebug()<<"connectée"<< endl;
    }
     QSqlQuery query,query2,qry;
     int Quantite;

    if(this->Nature=="Bonentrée" && bonAfficher == 1){

        if(query.exec("SELECT * FROM bonEntree_table Quantité WHERE NumBon='"+this->NumBon+"' AND NumProduit='"+this->NumProduit+"' ")){
           while(query.next()){
                Quantite = query.value(3).toInt();
           }

            qry.exec("UPDATE produit_table SET Stock=Stock-'"+QString::number(Quantite)+"' WHERE Numero ='"+this->NumProduit+"' ");
            query.exec("DELETE  FROM bonEntree_table WHERE NumBon='"+this->NumBon+"' AND NumProduit='"+this->NumProduit+"' AND idBE='"+this->idB+"'");
       qDebug()<<"qeury executer"<<endl;
       QMessageBox::critical(this,tr("Suppression du bon"),tr("Supprimer"));
       rafraichirPoduit_table();
       rafraichirbonEntree_table();
       affichageDonneBonEntree();

   }
        else qDebug()<<"non executer"<<endl;
    }

    if(this->Nature=="Bonsortie" && (bonAfficher == 0 || bonAfficher == -1)){

        //if(query.exec("DELETE  FROM bonSortie_table WHERE NumBon='"+this->NumBon+"' AND NumProduit='"+this->NumProduit+"' ")){
             if(query2.exec("SELECT * FROM bonSortie_table Quantité WHERE NumBon='"+this->NumBon+"' AND NumProduit='"+this->NumProduit+"' ")){
                while(query2.next()){
                     Quantite = query2.value(3).toInt();
                }

                 qry.exec("UPDATE produit_table SET Stock=Stock+'"+QString::number(Quantite)+"' WHERE Numero ='"+this->NumProduit+"' ");
                 query2.exec("DELETE  FROM bonSortie_table WHERE NumBon='"+this->NumBon+"' AND NumProduit='"+this->NumProduit+"' AND idBS='"+this->idB+"' ");
            qDebug()<<"qeury executer"<<endl;
            QMessageBox::critical(this,tr("Suppression du bon"),tr("Supprimer"));
            rafraichirPoduit_table();
            rafraichirbonSortie_table();
            affichageDonneBonSortie();

        }
        else qDebug()<<"non executer"<<endl;
    }

    connection.Disconnect();
}


void StockManager::on_btnActualiserBon_clicked()
{

        rafraichirbonEntree_table();

      affichageDonneProduit();

        rafraichirbonSortie_table();

        affichageEtatStock();


}


void StockManager::on_lineEditSearchBon_textChanged(const QString &arg1)
{
    if(bonAfficher == 1){
         QString searchValue = ui->lineEditSearchBon->text();
        DatabaseConnection connection;

        if(connection.Connect()){
            qDebug()<<"mivoha Etat mouvement Stock"<< endl;
           }
        QSqlQuery queryy;
        int ligne= 0;
        queryy.exec("SELECT COUNT(*) FROM bonEntree_table");
        while (queryy.next()) {
            ligne = queryy.value(0).toInt();
        }
        if(searchValue.isEmpty() ||searchValue=="" ){
            affichageDonneBonEntree();
            rafraichirbonEntree_table();
            return;
        }


            modele4 = new QStandardItemModel(ligne,5);

            int rowss = 0;
            queryy.exec("SELECT * FROM bonEntree_table WHERE  NumProduit LIKE '%"+searchValue+"%' OR Quantité LIKE '%"+searchValue+"%' OR Nature LIKE '%"+searchValue+"%' DateEntrée LIKE '%"+searchValue+"%' ");
            while(queryy.next()){
                for(int colonne = 0;colonne < 5; colonne++){
                    QStandardItem *items = new QStandardItem(queryy.value(colonne).toString());
                    modele4->setItem(rowss,colonne,items);
            }
            rowss++;
        }

            modele4->setHeaderData(0,Qt::Horizontal,"N°Bon");
            modele4->setHeaderData(1,Qt::Horizontal,"N°Produit");
            modele4->setHeaderData(2,Qt::Horizontal,"Quantité");
            modele4->setHeaderData(3,Qt::Horizontal,"Nature");
            modele4->setHeaderData(4,Qt::Horizontal,"Date d'entrée");
            ui->tbvBonSortie->setModel(modele4);
    connection.Disconnect();
    }

    if(bonAfficher == -1 || bonAfficher== 0){
         QString searchValue = ui->lineEditSearchBon->text();
        DatabaseConnection connection;

        if(connection.Connect()){
            qDebug()<<"mivoha Etat mouvement Stock"<< endl;
           }
        QSqlQuery queryy;
        int ligne= 0;
        queryy.exec("SELECT COUNT(*) FROM bonSortie_table");
        while (queryy.next()) {
            ligne = queryy.value(0).toInt();
        }
        if(searchValue.isEmpty() || searchValue==""){
            affichageDonneBonSortie();
            rafraichirbonSortie_table();
            return;
        }


            modele6 = new QStandardItemModel(ligne,5);

            int rowss = 0;
            queryy.exec("SELECT * FROM bonSortie_table WHERE NumBon LIKE '%"+searchValue+"%' OR NumProduit LIKE '%"+searchValue+"%' OR Quantité LIKE '%"+searchValue+"%' OR Nature LIKE '%"+searchValue+"%' DateSortie LIKE '%"+searchValue+"%' ");
            while(queryy.next()){
                for(int colonne = 0;colonne < 5; colonne++){
                    QStandardItem *items = new QStandardItem(queryy.value(colonne).toString());
                    modele6->setItem(rowss,colonne,items);
            }
            rowss++;
        }

            modele6->setHeaderData(0,Qt::Horizontal,"N°Bon");
            modele6->setHeaderData(1,Qt::Horizontal,"N°Produit");
            modele6->setHeaderData(2,Qt::Horizontal,"Quantité");
            modele6->setHeaderData(3,Qt::Horizontal,"Nature");
            modele6->setHeaderData(4,Qt::Horizontal,"Date de sortie");
            ui->tbvBonSortie->setModel(modele6);
            connection.Disconnect();
    }
}


void StockManager::on_tbvBonSortie_clicked(const QModelIndex &index)
{
    QString val = ui->tbvBonSortie->model()->data(index).toString();
    qDebug()<<index.data().toString();

    DatabaseConnection connection;

    if(connection.Connect()){
        qDebug()<<"connectée"<< endl;
    }

    QSqlQuery query,query2;
    if(bonAfficher == 0 || bonAfficher == -1){
        if(query.exec("SELECT * FROM bonSortie_table WHERE  NumBon='"+val+"' OR NumProduit='"+val+"' OR Quantité='"+val+"' OR DateSortie='"+val+"' ")){
            qDebug()<<"qeury executer"<<endl;

            while (query.next()) {

                this->idB = query.value(0).toString();
               this->NumBon =query.value(1).toString();
                this->NumProduit = query.value(2).toString();
               this->Quantite= query.value(3).toString();
                this->Nature = query.value(4).toString();
               this->DateEntree = query.value(5).toString();
            }

            connection.Disconnect();

        }
    }
    if(bonAfficher == 1){
        if(query2.exec("SELECT * FROM bonEntree_table WHERE  NumBon='"+val+"' OR NumProduit='"+val+"' OR Quantité='"+val+"' OR Nature='"+val+"' OR DateEntrée='"+val+"' ")){
            qDebug()<<"qeury executer"<<endl;

            while (query2.next()) {

                this->idB = query2.value(0).toString();
               this->NumBon =query2.value(1).toString();
                this->NumProduit = query2.value(2).toString();
               this->Quantite= query2.value(3).toString();
                this->Nature = query2.value(4).toString();
               this->DateEntree = query2.value(5).toString();
            }

            connection.Disconnect();

        }
    }

      else qDebug()<<"non executer"<<endl;

    connection.Disconnect();
}


void StockManager::on_btnlisteBonEntree_clicked()
{
    affichageDonneBonEntree();
    bonAfficher =1;
    ui->btnlisteBonEntree->setStyleSheet("#btnlisteBonEntree{font: 75 italic 12pt TeX Gyre Bonum Math;background-color:rgb(104, 10, 10);color: white;}");
    ui->btnListeBonSortie->setStyleSheet("#btnListeBonSortie{font: 75 italic 12pt TeX Gyre Bonum Math;background-color:#ff80475b;color: white;}");
}


void StockManager::on_btnListeBonSortie_clicked()
{
    affichageDonneBonSortie();
     bonAfficher =-1;
     ui->btnListeBonSortie->setStyleSheet("#btnListeBonSortie{font: 75 italic 12pt TeX Gyre Bonum Math;background-color:rgb(104, 10, 10);color: white;}");
     ui->btnlisteBonEntree->setStyleSheet("#btnlisteBonEntree{font: 75 italic 12pt TeX Gyre Bonum Math;background-color:#ff80475b;color: white;}");
}

/*======================== etat et mouvement de stock =======================*/
void StockManager::on_btnEtatStock_clicked()
{
    ui->stackedWidget_4->setCurrentIndex(0);
    ui->btnEtatStock->setStyleSheet("#btnEtatStock{font: 75 italic 12pt TeX Gyre Bonum Math;background-color:rgb(104, 10, 10);color: white;}");
    ui->btnMouvementStock->setStyleSheet("#btnMouvementStock{font: 75 italic 12pt TeX Gyre Bonum Math;background-color:#ff80475b;color: white;}");
}


void StockManager::on_btnMouvementStock_clicked()
{
    ui->stackedWidget_4->setCurrentIndex(1);
    ui->btnMouvementStock->setStyleSheet("#btnMouvementStock{font: 75 italic 12pt TeX Gyre Bonum Math;background-color:rgb(104, 10, 10);color: white;}");
    ui->btnEtatStock->setStyleSheet("#btnEtatStock{font: 75 italic 12pt TeX Gyre Bonum Math;background-color:#ff80475b;color: white;}");
}





/*================================ gestion Unité =============================*/




void StockManager::on_pushButton_clicked()
{
    statistic = new Statistic(this);
    statistic->show();
}


void StockManager::on_btnAjouterUtilisateur_clicked()
{


    QString nomUtilisateur = ui->lineEditNomUtilisateur->text();
     QString prenomUtilisateur = ui->lineEditPrenomUtilisateur->text();
     QString AgeUtilisateur = ui->lineEditAgeUtilisateur->text();
     QString sexeUtilisateur = ui->comboBoxSexeUtilisateur->currentText();
     QString dateUtilisateur = ui->dateEditUtilisateur->text();
     QString motDePassUtilisateur = ui->lineEditPsswrdUtilisateur->text();
     QString typeUtilisateur = ui->comboBoxType->currentText();

     DatabaseConnection connection;

     if(connection.Connect()){
         qDebug()<<"connectée"<< endl;
     }

     QSqlQuery query;
       if(query.exec("INSERT INTO user_table ( Nom, Prenom, Age, Sexe, Date, Mot_de_passe, typeUser) VALUES('"+nomUtilisateur+"', '"+prenomUtilisateur+"', '"+AgeUtilisateur+"', '"+sexeUtilisateur+"', '"+dateUtilisateur+"', '"+motDePassUtilisateur+"', '"+typeUtilisateur+"')")){
           qDebug()<<"qeury executer"<<endl;


           ui->lineEditNomUtilisateur->setText("");
           ui->lineEditPrenomUtilisateur->setText("");
             ui->lineEditAgeUtilisateur->setText("");
              ui->lineEditPsswrdUtilisateur->setText("");
              rafraichirUser_table();

       }
       else qDebug()<<"non executer"<<endl;

     connection.Disconnect();
}


void StockManager::on_btnModifierUtilisateur_clicked()
{
    QString nomUtilisateur = ui->lineEditNomUtilisateur->text();

     DatabaseConnection connection;

     if(connection.Connect()){
         qDebug()<<"connectée"<< endl;
     }

     QSqlQuery query;
       if(query.exec("DELETE FROM user_table WHERE Nom='"+nomUtilisateur+"'")){
           qDebug()<<"qeury executer"<<endl;
           QMessageBox::critical(this,tr("Suppression l'utilisateur"),tr("Supprimer"));
           ui->lineEditNomUtilisateur->setText("");
           rafraichirUser_table();
       }
       else qDebug()<<"non executer"<<endl;

     connection.Disconnect();
}


void StockManager::on_btnSupprimerUtilisateur_clicked()
{
    QString nomUtilisateur = ui->lineEditNomUtilisateur->text();
     QString prenomUtilisateur = ui->lineEditPrenomUtilisateur->text();
     QString AgeUtilisateur = ui->lineEditAgeUtilisateur->text();
     QString sexeUtilisateur = ui->comboBoxSexeUtilisateur->currentText();
     QString dateUtilisateur = ui->dateEditUtilisateur->text();
     QString motDePassUtilisateur = ui->lineEditPsswrdUtilisateur->text();
     QString typeUtilisateur = ui->comboBoxType->currentText();

     DatabaseConnection connection;

     if(connection.Connect()){
         qDebug()<<"connectée"<< endl;
     }

     QSqlQuery query;
       if(query.exec("UPDATE user_table SET Nom='"+nomUtilisateur+"', Prenom='"+prenomUtilisateur+"', Age='"+AgeUtilisateur+"',Sexe='"+sexeUtilisateur+"', Date='"+dateUtilisateur+"', Mot_de_passe='"+motDePassUtilisateur+"', typeUser='"+typeUtilisateur+"' WHERE Nom='"+nomUtilisateur+"' ")){
           qDebug()<<"qeury executer"<<endl;

           ui->lineEditNomUtilisateur->setText("");
           ui->lineEditPrenomUtilisateur->setText("");
           ui->lineEditAgeUtilisateur->setText("");
           ui->lineEditPsswrdUtilisateur->setText("");

              rafraichirUser_table();

       }
       else qDebug()<<"non executer"<<endl;

     connection.Disconnect();
}


void StockManager::on_btnAnnuler_clicked()
{
    ui->lineEditNomUtilisateur->setText("");
    ui->lineEditPrenomUtilisateur->setText("");
    ui->lineEditAgeUtilisateur->setText("");
    ui->lineEditPsswrdUtilisateur->setText("");
}




void StockManager::on_btnVoir_clicked()
{
    QString desingnation = ui->cbBoxDesignation->currentText();
    QString date1 = ui->date1->text();
    QString date2 = ui->date2->text();

    DatabaseConnection connection;

    if(connection.Connect()){
        qDebug()<<"connectée"<< endl;
    }
    QSqlQuery qry;
    QString numproduit,stockActuel;
    qry.exec("SELECT Numero,Stock FROM produit_table WHERE Designation='"+desingnation+"'");
    while(qry.next()){
        numproduit = qry.value(0).toString();
        stockActuel = qry.value(1).toString();

    }

    QSqlQuery query,qry3;
    int ligne =0;
    query.exec("SELECT COUNT(*) FROM (SELECT NumBon,Quantité,'',DateEntrée FROM bonEntree_table be WHERE bs.NumProduit=? UNION  SELECT NumBon,'' ,Quantité,DateSortie FROM bonSortie_table bs WHERE bs.NumProduit=? ORDER BY be.DateEntrée,bs.DateSortie) WHERE DateEntrée BETWEEN ? AND ?");
    query.addBindValue(numproduit);
    query.addBindValue(numproduit);
    query.addBindValue(date1);
    query.addBindValue(date2);


    while(query.next()){
        ligne = query.value(0).toInt();
    }
     int row= 0;
     modelMvmtStock = new QStandardItemModel(ligne,4);
    qry3.prepare("SELECT * FROM (SELECT NumBon,Quantité,'',DateEntrée FROM bonEntree_table be WHERE be.NumProduit=? UNION  SELECT NumBon,'' ,Quantité,DateSortie FROM bonSortie_table bs WHERE bs.NumProduit=? ORDER BY be.DateEntrée,bs.DateSortie) WHERE DateEntrée BETWEEN ? AND ? ");
    qry3.addBindValue(numproduit);
    qry3.addBindValue(numproduit);
    qry3.addBindValue(date1);
     qry3.addBindValue(date2);

    qry3.exec();
         while(qry3.next()){
             for(int colonne = 0;colonne < 4; colonne++){
                 QStandardItem *item = new QStandardItem(qry3.value(colonne).toString());
                 modelMvmtStock->setItem(row,colonne,item);

             }

             row++;
         }

         modelMvmtStock->setHeaderData(0,Qt::Horizontal,"N°BON");
         modelMvmtStock->setHeaderData(1,Qt::Horizontal,"ENTREE");
         modelMvmtStock->setHeaderData(2,Qt::Horizontal,"SORTIE");
         modelMvmtStock->setHeaderData(3,Qt::Horizontal,"DATE");
         //ui->tbvMouvementStockEntree->setModel(modelMvmtStockEntree);
         ui->tbviewMvmtStock->setModel(modelMvmtStock);
         QString totalEntree,totalSortie;
         QSqlQuery qryE, qryS;
         qryE.prepare("SELECT SUM(Quantité) FROM bonEntree_table WHERE NumProduit=? AND DateEntrée BETWEEN ? AND ? ");
         qryE.addBindValue(numproduit);
         qryE.addBindValue(date1);
          qryE.addBindValue(date2);
         qryE.exec();
          while(qryE.next()){
              totalEntree = qryE.value(0).toString();
          }

          qryS.prepare("SELECT SUM(Quantité) FROM bonSortie_table WHERE NumProduit=? AND DateSortie BETWEEN ? AND ? ");
          qryS.addBindValue(numproduit);
          qryS.addBindValue(date1);
           qryS.addBindValue(date2);
          qryS.exec();
           while(qryS.next()){
               totalSortie = qryS.value(0).toString();
           }

          ui->labelTotalEntre->setText(totalEntree);
          ui->labelTotalSortie->setText(totalSortie);
          ui->labelStockActuel->setText(stockActuel);



}

