#include "streams/gopropencvsource.h"
#include <QDebug>
#include <QImage>
#include <QColor>
#include <QTimer>
#include <QPainter>

//-------------------------------------------------------------------------------------------------
GoPrOpenCvSource::GoPrOpenCvSource()
    : QThread(), mCap(NULL)
{
    moveToThread(this);
}

//-------------------------------------------------------------------------------------------------
void GoPrOpenCvSource::init(const QString &source)
{
    mSource = source;
    start();
}

//-------------------------------------------------------------------------------------------------
void GoPrOpenCvSource::run()
{
    emitStartImage();
    initCapture();

    QTimer t;
    connect(&t, SIGNAL(timeout()), this, SLOT(grab()));
    t.start(1000/30.0);
    exec();

    if (mCap)
        delete mCap;
    mCap = NULL;
}

//-------------------------------------------------------------------------------------------------
bool GoPrOpenCvSource::initCapture()
{
    Q_ASSERT(!mCap);

    if (mSource.startsWith(":"))
        mCap = new cv::VideoCapture(mSource.mid(1).toInt()); // open the default camera
    else
        mCap = new cv::VideoCapture(mSource.toStdString()); // open local file

    if(mCap->isOpened())  // check if we succeeded
        return true;

    delete mCap;
    mCap = NULL;
    return false;
}

//-------------------------------------------------------------------------------------------------
void GoPrOpenCvSource::emitStartImage()
{
    QImage img(640,480, QImage::Format_RGB32);
    img.fill(Qt::black);

    QPainter p(&img);
    p.setPen(Qt::red);
    p.drawText(img.rect(),Qt::AlignCenter, QString("Stream '%1'").arg(mSource));

    emit nextFrame(img);
}
//-------------------------------------------------------------------------------------------------
void GoPrOpenCvSource::grab()
{
    if (!mCap && !initCapture()) {
        emitStartImage();
        return;
    }

    Q_ASSERT(mCap);
    if (!mCap->isOpened()) {
        delete mCap;
        return;
    }

    cv::Mat frame;

    if (!mCap->grab() || !mCap->retrieve(frame)) {
        delete mCap;
        mCap = NULL;
        return;
    }

    if (frame.type() != CV_8UC3)
        return;

    QImage img(frame.cols,frame.rows, QImage::Format_RGB32);
    for (int y=0; y<frame.rows; y++) {
        for (int x=0; x<frame.cols; x++) {
            cv::Vec3b bgrPixel = frame.at<cv::Vec3b>(y,x);
            img.setPixel(x,y,QColor(bgrPixel[2],bgrPixel[1],bgrPixel[0]).rgb());
        }
    }

    emit nextFrame(img);
}
