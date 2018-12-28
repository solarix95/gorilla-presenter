#ifndef GOPROPENCVSTREAM_H
#define GOPROPENCVSTREAM_H

#include "goprimagestream.h"
#include <QThread>
#include <QString>

class GoPrOpenCvSource;
class GoPrOpenCvStream : public GoPrImageStream
{
    Q_OBJECT
public:
    GoPrOpenCvStream(const QString &source);
    ~GoPrOpenCvStream();

    virtual void stop();

private:
    GoPrOpenCvSource *mSource;
};

#endif // GOPROPENCVSTREAM_H
