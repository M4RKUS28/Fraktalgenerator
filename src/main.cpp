#include <QApplication>
#include "mainwindow.h"


#include <QApplication>
#include <QMutex>
#include <iostream>


//#define redirect_debug


#ifdef redirect_debug
void redirectedOutput(QtMsgType, const QMessageLogContext &, const QString &);
QMutex debugOutMutex;


void redirectedOutput(QtMsgType type, const QMessageLogContext &, const QString &msg)
{
    debugOutMutex.lock();
    std::cout << QDateTime::currentDateTime().toString("hh.mm.ss.zzz  ").toStdString() <<  msg.toStdString() << std::endl;
    if (type == QtFatalMsg) {
        abort();
    }
    debugOutMutex.unlock();
}
#endif

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

#ifdef redirect_debug
    qInstallMessageHandler(redirectedOutput);
#endif
    MainWindow w;
    w.show();
    return a.exec();
}
