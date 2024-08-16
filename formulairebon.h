#ifndef FORMULAIREBON_H
#define FORMULAIREBON_H

#include <QDialog>

namespace Ui {
class FormulaireBon;
}

class FormulaireBon : public QDialog
{
    Q_OBJECT

public:
    explicit FormulaireBon(QWidget *parent = nullptr);
    ~FormulaireBon();
    QString idBon;

     void setDataBon(QString idB, QString NumBon,QString NumProduit, QString Quantite, QString Nature, QString DateEntree);

private slots:
    void on_btnAnnulerAjout_clicked();

    void on_btnValiderAjout_clicked();
    void getListeProduit();

    void on_comboBoxListeProduit_currentTextChanged(const QString &arg1);

    void on_comboBoxListeProduit_activated(const QString &arg1);

    void on_pushButton_clicked();

private:
    Ui::FormulaireBon *ui;
};

#endif // FORMULAIREBON_H
