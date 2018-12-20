#ifndef GOPRCONTROLLER_H
#define GOPRCONTROLLER_H

#include <QWidget>
#include <QImage>
#include "goprpresentation.h"
#include "goprplayer.h"
#include "gopropenglviewer.h"

namespace Ui {
class GoPrEditor;
}

class GoPrAbstractSource;
class GoPrAbstractRenderer;
class GoPrController : public QWidget
{
    Q_OBJECT

public:
    explicit GoPrController(QWidget *parent = 0);
    ~GoPrController();

    GoPrPlayer *player();

signals:
    void requestImage(const QString &filename);
    void requestSlide(const QImage &slide);

public slots:
    void init();

private slots:
    void selectLiveImage();
    void selectLogo();

private:
    Ui::GoPrEditor *ui;

    GoPrAbstractRenderer *mRenderer;
    GoPrPlayer            mPlayer;
    GoPrOpenglViewer     *mOpenGlViewer;
};

#endif // GOPREDITOR_H
