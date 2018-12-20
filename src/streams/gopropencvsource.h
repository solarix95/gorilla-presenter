#ifndef GOPROPENCVSOURCE_H
#define GOPROPENCVSOURCE_H

#include <QThread>
#include "opencv2/opencv.hpp"

class GoPrOpenCvSource : public QThread
{
    Q_OBJECT
public:
    explicit GoPrOpenCvSource();

    void init(const QString &source);

signals:
    void nextFrame(const QImage &image);

protected:
    virtual void run();

private slots:
    bool initCapture();
    void emitStartImage();
    void grab();

private:
    QString mSource;
    cv::VideoCapture *mCap;
};

#endif // GOPROPENCVSOURCE_H
