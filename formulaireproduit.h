#ifndef FORMULAIREPRODUIT_H
#define FORMULAIREPRODUIT_H

#include <QDialog>

namespace Ui {
class FormulaireProduit;
}

class FormulaireProduit : public QDialog
{
    Q_OBJECT

public:
    explicit FormulaireProduit(QWidget *parent = nullptr);
    ~FormulaireProduit();
    int formulaire;
    void setDataProduit( QString design,QString num, QString stock);


private slots:
    void on_btnAnnulerAjout_clicked();

    void on_btnValiderAjout_clicked();

    void on_pushButton_clicked();

private:
    Ui::FormulaireProduit *ui;
};

#endif // FORMULAIREPRODUIT_H
