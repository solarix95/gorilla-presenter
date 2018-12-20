#include <QCoreApplication>
#include <QByteArray>
#include <QImage>
#include <QFile>
#include <iostream>


int main(int argc, char *argv[])
{
    QCoreApplication app(argc,argv);

    if (app.arguments().count() < 2)
        return 0;

    for (int i=1; i<app.arguments().count(); i++) {
         QImage img(app.arguments()[i]);
         if (!img.isNull()) {
            QFile f(app.arguments()[i]);
                f.open(QIODevice::ReadOnly);
                std::cout << QString(f.readAll().toBase64()).toUtf8().constData() << std::endl;
            }
    }
    return 0;
}
