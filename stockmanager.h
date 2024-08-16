#ifndef STOCKMANAGER_H
#define STOCKMANAGER_H

#include <QMainWindow>
#include "formulairebon.h"
#include "formulaireproduit.h"
#include "statistic.h"
#include "QStandardItemModel"

namespace Ui {
class StockManager;
}

class StockManager : public QMainWindow
{
    Q_OBJECT

public:
    explicit StockManager(QWidget *parent = nullptr);
    ~StockManager();

    QString id,design,num,stock,unite;
    QString idB,NumBon,NumProduit,Quantite,Nature;
    QString DateEntree;
    int bonAfficher = 0;
    int listeBon = 0;
    void rafraichirPoduit_table();
    void getTypeUser(QString typeUser,QString nomUtilisateur);
    void showTime();

public slots:
    void barChart();
    void donutChart();

private slots:

    void affichageDonneUtilisateur();
    void affichageDonneProduit();
   void affichageDonneBonEntree();
   void affichageDonneBonSortie();
   void affichageEtatStock();
   void rafraichirUser_table();
   void initialiseAffichage();
   //void print(QPrinter *printer);
   void alertProduit();

   void rafraichirbonEntree_table();
   void rafraichirbonSortie_table();
   void appendCbBoxDesignation();


    void on_btnAccueil_clicked();

    void on_btnGestionStock_clicked();

    void on_btnParametreAdministrateur_clicked();

    void on_btnQuitter_clicked();

    void on_btnListeProduit_clicked();

    void on_btnListeBon_clicked();

    void on_searchListeProduit_textChanged(const QString &arg1);

    void on_searchListeBon_textChanged(const QString &arg1);

    void on_btnListeEntree_clicked();

    void on_btnListeSortie_clicked();

    void on_btnGestionProduit_clicked();

    void on_btnAjout_clicked();

    void on_btnModifierProduit_clicked();

    void on_btnSupprimerProduit_clicked();

    void on_btnActualiser_clicked();

    void on_lineEditSearchProduit_textChanged(const QString &arg1);

    void on_tabwListeProduit_clicked(const QModelIndex &index);

    void on_btnGestionBon_clicked();

    void on_btnAjoutBon_clicked();

    void on_btnModifierBon_clicked();

    void on_btnSupprimerBon_clicked();

    void on_btnActualiserBon_clicked();

    void on_lineEditSearchBon_textChanged(const QString &arg1);

    void on_tbvBonSortie_clicked(const QModelIndex &index);

    void on_btnlisteBonEntree_clicked();

    void on_btnListeBonSortie_clicked();

    void on_btnEtatStock_clicked();

    void on_btnMouvementStock_clicked();


    void on_pushButton_clicked();

    void on_btnAjouterUtilisateur_clicked();

    void on_btnModifierUtilisateur_clicked();

    void on_btnSupprimerUtilisateur_clicked();

    void on_btnAnnuler_clicked();



    void on_btnVoir_clicked();

private:
    Ui::StockManager *ui;

    FormulaireBon *formulaireBon;
    FormulaireProduit *formulaireProduit;
    Statistic * statistic;



    QStandardItemModel *modele;
    QStandardItemModel *modele1;
    QStandardItemModel *modele2;
    QStandardItemModel *modele3;
    QStandardItemModel *modele4;
    QStandardItemModel *modele6;
    QStandardItemModel *modele7;
    QStandardItemModel *modele5;
    QStandardItemModel *model;
    QStandardItemModel *modelSortie;
    QStandardItemModel *modelEtatStock;
    QStandardItemModel *modelMvmtStockEntree;
    QStandardItemModel *modelMvmtStockSortie;
    QStandardItemModel *modelMvmtStock;
};

#endif // STOCKMANAGER_H
