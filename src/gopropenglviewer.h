#ifndef GOPROPENGLPLAYER_H
#define GOPROPENGLPLAYER_H

#include <QOpenGLWidget>
#include <QWheelEvent>
#include <QKeyEvent>
#include <QTimer>
#include <QColor>
#include "gopropenglviewer.h"
#include "goprshader.h"
#include "goprgeometrybuffer.h"
#include "goprlogoanimation.h"
#include "transitions/goprtransition.h"

class GoPrImageStream;
class GoPrOpenglViewer : public QOpenGLWidget
{
    Q_OBJECT
public:
    GoPrOpenglViewer();

    enum Transition {
        Slide,
        Cube,
        Rotate
    };

protected:
    virtual void initializeGL();
    virtual void paintGL();
    virtual void wheelEvent(QWheelEvent *);
    virtual void keyPressEvent(QKeyEvent *e);

signals:
    void viewerReady();
    void requestNextSlide();
    void requestPreviousSlide();
    void requestNextPresentation();
    void requestPreviousPresentation();
    void requestAbort();

public slots:
    void toSlide(const QImage &image);
    void fromSlide(const QImage &image);
    void toStream(GoPrImageStream *stream);
    void fromStream(GoPrImageStream *stream);

    void showLiveImage(const QString &fname);
    void toColor(QColor background);
    void setShader(int index);
    void setTransition(int transitionType);
    void loadLogoByJson(const QString &jsonFile);

private slots:
    void animate();

private:
    void createTransition(const QImage &image, GoPrTransition::Direction d);
    void createTransition(GoPrImageStream *stream, GoPrTransition::Direction d);
    void createTransition(GoPrGeometryBuffer *fromBuffer, GoPrGeometryBuffer *toBuffer, GoPrTransition::Direction d);
    void createTransition();
    void cleanupTransitions();

    GoPrShader         *mShaderFlat;
    GoPrShader         *mShaderPhong;
    GoPrShader         *mShaderCurrent;
    GoPrTransitions     mTransitions;
    GoPrLogoAnimation   mLogoAnimation;

    QColor              mCurrentColor;
    QColor              mTargetColor;

    float               mPosZ[2]; // 0: Custom, 1: park/break
    int                 mPosIndex;
    float               mCurrentZ;
    Transition          mTransitionType;
};

#endif // GOPROPENGLPLAYER_H
