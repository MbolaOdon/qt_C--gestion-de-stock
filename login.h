#ifndef LOGIN_H
#define LOGIN_H

#include <QMainWindow>
#include "stockmanager.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Login; }
QT_END_NAMESPACE

class Login : public QMainWindow
{
    Q_OBJECT

public:
    Login(QWidget *parent = nullptr);
    ~Login();

private slots:
    void on_btnIdentifier_clicked();

private:
    Ui::Login *ui;
};
#endif // LOGIN_H
