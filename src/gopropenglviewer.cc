
#include <limits>
#include <math.h>
#include <QOpenGLFunctions>
#include <QIcon>
#include "gopropenglviewer.h"
#include "transitions/goprslidetransition.h"
#include "transitions/goprcubetransition.h"
#include "transitions/goprrotatetransition.h"

//-------------------------------------------------------------------------------------------------
GoPrOpenglViewer::GoPrOpenglViewer()
    : mTransitionType(Cube)
{
    setWindowIcon(QIcon(":resources/icons8-gorilla-50.png"));
    setWindowTitle("Gorilla Projector");
    mPosZ[0] = mCurrentZ = -7;
    mPosZ[1] = 1;
    mPosIndex = 0;

    mCurrentColor = mTargetColor = Qt::white;
    QTimer *t = new QTimer(this);
    connect(t, SIGNAL(timeout()), this, SLOT(animate()));
    t->start(10);
}

//-------------------------------------------------------------------------------------------------
void GoPrOpenglViewer::initializeGL()
{
    QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();

    f->initializeOpenGLFunctions();

    f->glEnable(GL_DEPTH_TEST);
    f->glDepthFunc(GL_LEQUAL) ;
    f->glEnable(GL_CULL_FACE) ;
    f->glCullFace(GL_BACK);

    mShaderFlat  = new GoPrShader("flat");
    mShaderPhong = new GoPrShader("phong");
    mShaderCurrent = mShaderFlat;

    emit viewerReady();
}

//-------------------------------------------------------------------------------------------------
void GoPrOpenglViewer::paintGL()
{
    QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();

    f->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    f->glClearColor(mCurrentColor.redF() ,  mCurrentColor.greenF() ,  mCurrentColor.blueF() ,  1.0f ) ;

    // setup perspective
    QMatrix4x4 perspectiveMatrix;
    perspectiveMatrix.perspective(30,(float)width()/(float)height(),1,-1);

    // setup modelView (Rotation + Position)
    QMatrix4x4 rot;
    // rot.rotate(mAngle*50,QVector3D(1,0,0));
    // rot.rotate(mAngle*50/2.0,QVector3D(0,0,1));

    QMatrix4x4 pos;
    pos.translate(0,0,mCurrentZ);

    QMatrix4x4 modelview = pos; // * rot;

    mShaderCurrent->bind(perspectiveMatrix);

    mLogoAnimation.render(mShaderCurrent,modelview);
    if (mTransitions.isEmpty())
        return;
    mTransitions.first()->render(mShaderCurrent, modelview);
}

//-------------------------------------------------------------------------------------------------
void GoPrOpenglViewer::wheelEvent(QWheelEvent *e)
{
    mPosZ[0] += e->delta()/120.0;
}

//-------------------------------------------------------------------------------------------------
void GoPrOpenglViewer::keyPressEvent(QKeyEvent *e)
{
    /*
      PowerPoint ( + Logitech Presenter)
         F5:       Run
         Esc:      Stop
         Period:   black out
         PageDown: next
         PageUp  : previous
    */
    switch (e->key()) {
    case Qt::Key_PageDown:
    case Qt::Key_Right:  emit requestNextSlide();     break;
    case Qt::Key_PageUp:
    case Qt::Key_Left:   emit requestPreviousSlide(); break;
    case Qt::Key_Up:     emit requestNextPresentation();     break;
    case Qt::Key_Down:   emit requestPreviousPresentation(); break;
    case Qt::Key_Period: mPosIndex = mPosIndex == 0 ? 1  : 0; update(); break;
    case Qt::Key_F5:     mTransitionType = Cube;   createTransition();   break;
    case Qt::Key_F6:     mTransitionType = Rotate; createTransition();   break;
    case Qt::Key_F7:     mTransitionType = Slide;  createTransition();   break;
    case Qt::Key_F11:    setWindowState(windowState() ^ Qt::WindowFullScreen); break;
    case Qt::Key_S:      mShaderCurrent = mShaderCurrent == mShaderFlat ? mShaderPhong : mShaderFlat; update(); break;
    }
}

//-------------------------------------------------------------------------------------------------
void GoPrOpenglViewer::toSlide(const QImage &image)
{
    createTransition(image,GoPrTransition::Left);
}

//-------------------------------------------------------------------------------------------------
void GoPrOpenglViewer::fromSlide(const QImage &image)
{
    createTransition(image,GoPrTransition::Right);
}

//-------------------------------------------------------------------------------------------------
void GoPrOpenglViewer::toStream(GoPrImageStream *stream)
{
    createTransition(stream,GoPrTransition::Left);
}

//-------------------------------------------------------------------------------------------------
void GoPrOpenglViewer::fromStream(GoPrImageStream *stream)
{
    createTransition(stream,GoPrTransition::Right);
}

//-------------------------------------------------------------------------------------------------
void GoPrOpenglViewer::showLiveImage(const QString &fname)
{
    createTransition(QImage(fname),GoPrTransition::Right);
}

//-------------------------------------------------------------------------------------------------
void GoPrOpenglViewer::toColor(QColor background)
{
    mTargetColor = background;
}

//-------------------------------------------------------------------------------------------------
void GoPrOpenglViewer::setShader(int index)
{
    if (index == 0)
        mShaderCurrent = mShaderFlat;
    else
        mShaderCurrent = mShaderPhong;
}

//-------------------------------------------------------------------------------------------------
void GoPrOpenglViewer::setTransition(int transitionType)
{
    mTransitionType = (Transition)transitionType;
}

//-------------------------------------------------------------------------------------------------
void GoPrOpenglViewer::loadLogoByJson(const QString &jsonFile)
{
    makeCurrent();
    mLogoAnimation.fromJsonFile(jsonFile);
}

//-------------------------------------------------------------------------------------------------
void GoPrOpenglViewer::animate()
{
    makeCurrent();
    if (mCurrentZ != mPosZ[mPosIndex]) {
        mCurrentZ += (mPosZ[mPosIndex]-mCurrentZ)/10.0;
        update();
    }

    if (mLogoAnimation.animate())
        update();

    if (mCurrentColor != mTargetColor) {
        mCurrentColor.setRedF(mCurrentColor.redF() + (mTargetColor.redF()-mCurrentColor.redF())*mTransitions.first()->animationState());
        mCurrentColor.setGreenF(mCurrentColor.greenF() + (mTargetColor.greenF()-mCurrentColor.greenF())*mTransitions.first()->animationState());
        mCurrentColor.setBlueF(mCurrentColor.blueF() + (mTargetColor.blueF()-mCurrentColor.blueF())*mTransitions.first()->animationState());

        // stop animation
        if ( (abs(mCurrentColor.red()-mTargetColor.red()) <= 1) &&
             (abs(mCurrentColor.green()-mTargetColor.green()) <= 1) &&
             (abs(mCurrentColor.blue()-mTargetColor.blue()) <= 1))
            mCurrentColor = mTargetColor;
        update();
    }

    if (mTransitions.isEmpty())
        return;

    if (mTransitions.first()->animate())
        update();
    else {
        cleanupTransitions();
    }
}

//-------------------------------------------------------------------------------------------------
void GoPrOpenglViewer::createTransition(const QImage &image, GoPrTransition::Direction d)
{
    makeCurrent();

    GoPrGeometryBuffer *currentBuffer = mTransitions.isEmpty() ? NULL : mTransitions.last()->currentBuffer();
    GoPrGeometryBuffer *nextBuffer    = new GoPrGeometryBuffer();
    nextBuffer->init(image);

    createTransition(currentBuffer,nextBuffer, d);
    cleanupTransitions();

    update();
}

//-------------------------------------------------------------------------------------------------
void GoPrOpenglViewer::createTransition(GoPrImageStream *stream, GoPrTransition::Direction d)
{
    makeCurrent();

    GoPrGeometryBuffer *currentBuffer = mTransitions.isEmpty() ? NULL : mTransitions.last()->currentBuffer();
    GoPrGeometryBuffer *nextBuffer    = new GoPrGeometryBuffer();
    nextBuffer->init(stream);

    createTransition(currentBuffer,nextBuffer, d);
    cleanupTransitions();

    update();
}

//-------------------------------------------------------------------------------------------------
void GoPrOpenglViewer::createTransition(GoPrGeometryBuffer *fromBuffer, GoPrGeometryBuffer *toBuffer, GoPrTransition::Direction d)
{
    GoPrTransition *t = NULL;
    switch (mTransitionType) {
    case Cube:   t = new GoPrCubeTransition(fromBuffer,toBuffer,d); break;
    case Rotate: t = new GoPrRotateTransition(fromBuffer,toBuffer,d); break;
    case Slide:  t = new GoPrSlideTransition(fromBuffer,toBuffer,d); break;
    }

    Q_ASSERT(t);
    mTransitions << t;
    cleanupTransitions();
}

//-------------------------------------------------------------------------------------------------
void GoPrOpenglViewer::createTransition()
{
    if (mTransitions.isEmpty())
        return;
    createTransition(mTransitions.last()->currentBuffer(),mTransitions.last()->currentBuffer(),GoPrTransition::Left);
}

//-------------------------------------------------------------------------------------------------
void GoPrOpenglViewer::cleanupTransitions()
{
    while (mTransitions.count() >= 2 && (mTransitions.first()->animationState() >= 1.0f)) {
        makeCurrent();
        delete mTransitions.takeFirst()->destroy();
    }
}

