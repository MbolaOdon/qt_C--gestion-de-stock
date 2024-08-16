#include "statistic.h"
#include "ui_statistic.h"
#include "databaseconnection.h"
#include <QDebug>
#include <QSqlQuery>
#include <QtCharts>
#include <QChartView>
#include <QBarSeries>
#include <QBarSet>
#include <QtCharts/QPieSeries>
#include <QValueAxis>
#include <QtCharts/QPieSlice>
#include <QVector>
#include <vector>
#include <string>
#include <QList>
#include <QPrinter>
#include <QPrinterInfo>
#include <QPrintDialog>
#include <QFile>
#include <QFileInfo>
#include <QFileDialog>
#include <QPdfWriter>
#include <QPrintPreviewDialog>
#include <QFile>
#include <QPainter>


Statistic::Statistic(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Statistic)
{
    ui->setupUi(this);
    barChart();
    donutChart();
}

Statistic::~Statistic()
{
    delete ui;
}





void Statistic::donutChart(){
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
        chartView->chart()->setTheme(QChart::ChartThemeBlueCerulean);
            chartView->chart()->legend()->setFont(QFont("Arial", 8));
      chartView->setParent(ui->horizontalFrameDonutChart);

        chartView->update();

        x++;
        }

}


void Statistic::on_btnPdfChart_clicked()
{
    QPrinter printer;
    printer.setOrientation(QPrinter::Landscape);
           QPrintDialog dialog(&printer, this);
           dialog.setWindowTitle(tr("Print Document"));
               dialog.addEnabledOption(QAbstractPrintDialog::PrintSelection);
           if (dialog.exec() != QDialog::Accepted) {
               return;
           }
           QPainter painter;
                  painter.begin(&printer);
                  double xscale = printer.pageRect().width() / double(ui->widget->width());
                  double yscale = printer.pageRect().height() / double(ui->widget->height());
                  double scale = qMin(xscale, yscale);
                  painter.translate(printer.paperRect().x() + printer.pageRect().width()/2,
                                     printer.paperRect().y() + printer.pageRect().height()/2);
                  painter.scale(scale, scale);
                  painter.translate(-width()/2, -height()/2);

                  ui->widget->render(&painter);

}
void Statistic::barChart(){
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

         chart->setTitle("Histogramme");
         chart->setAnimationOptions(QChart::SeriesAnimations);
         chart->animationDuration();

         chart->addAxis(axisY, Qt::AlignLeft);
            series->attachAxis(axisY);
         chartView = new QChartView(chart);

      chartView->setParent(ui->horizontalFrameBarChart);

         chart->legend()->show();
         chartView->chart()->legend()->setAlignment(Qt::AlignBottom);
         chartView->chart()->setTheme(QChart::ChartThemeBlueCerulean);
             chartView->chart()->legend()->setFont(QFont("Arial", 8));
             chartView->chart()->axes();
             chart->legend()->setVisible(true);

    }


