#include "login.h"
#include "stockmanager.h"
#include <QSplashScreen>
#include <QTimer>
#include <QApplication>
#include <QPixmap>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QSplashScreen *splash = new QSplashScreen;
          splash->setPixmap(QPixmap(":/image/spalshs.png"));

         splash->setMaximumWidth(1000);splash->setMaximumHeight(9999);
          splash->show();
          Qt::Alignment topRight = Qt::AlignRight | Qt::AlignTop;
        splash->showMessage(QObject::tr(" chargement de l'application..."),topRight, Qt::white);
       Login w;

       QTimer::singleShot(5000,splash,SLOT(close()));
       QTimer::singleShot(5000,&w,SLOT(show()));


    return a.exec();
}
