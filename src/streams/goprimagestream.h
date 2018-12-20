#ifndef GOPRIMAGESTREAM_H
#define GOPRIMAGESTREAM_H

#include <QObject>

class GoPrImageStream : public QObject
{
    Q_OBJECT
public:
    GoPrImageStream();

    virtual void init();
    virtual void stop();

    static QString source(const QString &url);

signals:
    void frame(const QImage &nextFrame);
};

#endif // GOPRIMAGESTREAM_H
