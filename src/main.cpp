#include "goprcontroller.h"
#include "gopropenglviewer.h"

#include <QApplication>
#include <QColor>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    GoPrController w;

    w.show();

    /*
    GoPrOpenglViewer p;

    QObject::connect(&w, SIGNAL(requestImage(QString)), &p, SLOT(showLiveImage(QString)));
    QObject::connect(&p, SIGNAL(viewerReady()), &w, SLOT(init()));

    QObject::connect(w.player(), SIGNAL(slideTo(QImage)), &p, SLOT(toSlide(QImage)));
    QObject::connect(w.player(), SIGNAL(slideFrom(QImage)), &p, SLOT(fromSlide(QImage)));
    QObject::connect(w.player(), SIGNAL(requestBackground(QColor)), &p, SLOT(toColor(QColor)));
    QObject::connect(&p, SIGNAL(requestNextSlide()), w.player(), SLOT(nextSlide()));
    QObject::connect(&p, SIGNAL(requestPreviousSlide()), w.player(), SLOT(prevSlide()));
    QObject::connect(&p, SIGNAL(requestNextPresentation()), w.player(), SLOT(nextPresentation()));
    QObject::connect(&p, SIGNAL(requestPreviousPresentation()), w.player(), SLOT(prevPresentation()));


    w.show();
    p.show();

    */
    return a.exec();
}
