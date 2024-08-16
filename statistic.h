#ifndef STATISTIC_H
#define STATISTIC_H

#include <QDialog>

namespace Ui {
class Statistic;
}

class Statistic : public QDialog
{
    Q_OBJECT

public:
    explicit Statistic(QWidget *parent = nullptr);
    ~Statistic();
public slots:
    void barChart();
    void donutChart();
private slots:
    void on_btnPdfChart_clicked();

private:
    Ui::Statistic *ui;
};

#endif // STATISTIC_H
