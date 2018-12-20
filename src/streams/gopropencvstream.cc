#include "streams/gopropencvstream.h"
#include "streams/gopropencvsource.h"
#include "utils/goprterminator.h"

//-------------------------------------------------------------------------------------------------
GoPrOpenCvStream::GoPrOpenCvStream(const QString &source)
{
    mSource = new GoPrOpenCvSource();
    connect(mSource, SIGNAL(nextFrame(QImage)), this, SIGNAL(frame(QImage)));
    mSource->init(source);
}

//-------------------------------------------------------------------------------------------------
GoPrOpenCvStream::~GoPrOpenCvStream()
{
    Q_ASSERT(!mSource);
}

//-------------------------------------------------------------------------------------------------
void GoPrOpenCvStream::stop()
{
    new GoPrTerminator(mSource, SIGNAL(finished()));
    mSource->quit();
    mSource = NULL;
}

