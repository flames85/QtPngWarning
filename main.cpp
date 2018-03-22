#include "mainwindow.h"
#include <QApplication>
#include <QTranslator>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    // 翻译
    QTranslator translator;
    translator.load("zh.qm");
    app.installTranslator(&translator);

    MainWindow w;
    w.show();

    return app.exec();
}
