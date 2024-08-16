QT       += core gui sql
QT       += printsupport
QT       += charts

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    databaseconnection.cpp \
    fonction.cpp \
    formulairebon.cpp \
    formulaireproduit.cpp \
    main.cpp \
    login.cpp \
    statistic.cpp \
    stockmanager.cpp

HEADERS += \
    databaseconnection.h \
    fonction.h \
    formulairebon.h \
    formulaireproduit.h \
    login.h \
    statistic.h \
    stockmanager.h

FORMS += \
    formulairebon.ui \
    formulaireproduit.ui \
    login.ui \
    statistic.ui \
    stockmanager.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    image-icon.qrc

DISTFILES += \
    image/home_icon_131771.png
