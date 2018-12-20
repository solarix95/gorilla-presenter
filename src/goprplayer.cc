#include "goprabstractrenderer.h"
#include "goprpresentation.h"
#include "goprplayer.h"
#include "streams/gopropencvstream.h"
#include "goprfilesource.h"

//-------------------------------------------------------------------------------------------------
GoPrPlayer::GoPrPlayer()
    : mCurrentPresentation(-1),mfromSlide(-1), mRenderer(NULL)
{
}

//-------------------------------------------------------------------------------------------------
void GoPrPlayer::setRenderer(GoPrAbstractRenderer *renderer)
{
    Q_ASSERT(!mRenderer);
    mRenderer = renderer;
}

//-------------------------------------------------------------------------------------------------
void GoPrPlayer::addPresentation(GoPrPresentation *prs)
{
    emit aboutToInsertPresentation(mPresentationStack.count());
    mPresentationStack << prs;
    connect(prs, SIGNAL(currentChanged(GoPrPresentation*,int,int)), this, SLOT(currentSlideChanged(GoPrPresentation*,int,int)));
    connect(prs, SIGNAL(currentChanged(GoPrPresentation*,int,int)), this, SIGNAL(currentSlideChanged()));
    emit presentationInserted();

    if (mPresentationStack.count() == 1) {
        mCurrentPresentation = mPresentationStack.count()-1;
        init();
    }
}

//-------------------------------------------------------------------------------------------------
bool GoPrPlayer::setCurrent(int index)
{
    if (index < 0 || index >= mPresentationStack.count())
        return false;
    if (mCurrentPresentation == index)
        return false;
    mCurrentPresentation = index;
    emit currentPresentationChanged();
    applyCurrentSlide(0);
    return true;
}

//-------------------------------------------------------------------------------------------------
int GoPrPlayer::current() const
{
    return mCurrentPresentation;
}

//-------------------------------------------------------------------------------------------------
int GoPrPlayer::count() const
{
    return mPresentationStack.count();
}

//-------------------------------------------------------------------------------------------------
GoPrPresentation *GoPrPlayer::presentation(int index) const
{
    if (index < 0)
        index = mCurrentPresentation;
    if (index < 0)
        return NULL;
    if (mPresentationStack.isEmpty())
        return NULL;
    Q_ASSERT(index < mPresentationStack.count());
    return mPresentationStack.at(index);
}

//-------------------------------------------------------------------------------------------------
void GoPrPlayer::init()
{
    if (mPresentationStack.isEmpty())
        return;

    mCurrentPresentation = 0;
    applyCurrentSlide(1);
}

//-------------------------------------------------------------------------------------------------
void GoPrPlayer::nextPresentation()
{
    if (mCurrentPresentation >= (mPresentationStack.count()-1))
        return;
    mCurrentPresentation++;
    emit currentPresentationChanged();
    applyCurrentSlide(0);
}

//-------------------------------------------------------------------------------------------------
void GoPrPlayer::prevPresentation()
{
    if (mCurrentPresentation <= 0)
        return;
    mCurrentPresentation--;
    emit currentPresentationChanged();
    applyCurrentSlide(0);
}

//-------------------------------------------------------------------------------------------------
void GoPrPlayer::nextSlide()
{
    applyCurrentSlide(1);
}

//-------------------------------------------------------------------------------------------------
void GoPrPlayer::prevSlide()
{
    applyCurrentSlide(-1);
}

//-------------------------------------------------------------------------------------------------
void GoPrPlayer::currentSlideChanged(GoPrPresentation *p, int fromIndex, int toIndex)
{
    if (p != presentation())
        return;
    renderCurrentSlide(toIndex > fromIndex ? 1 : -1);
}

//-------------------------------------------------------------------------------------------------
void GoPrPlayer::renderCurrentSlide(int direction)
{
    QString stream = currentStream();

    if (direction >= 0) {
        if (!stream.isEmpty())
            emit slideTo(new GoPrOpenCvStream(stream));
        else
            emit slideTo(mRenderer->render(*presentation(),presentation()->currentSlide()));
    } else {
        if (!stream.isEmpty())
            emit slideFrom(new GoPrOpenCvStream(stream));
        else
            emit slideFrom(mRenderer->render(*presentation(),presentation()->currentSlide()));
    }
    QColor defaultColor = presentation()->attribute("clearcolor", QColor(Qt::white));
    QColor slideColor   = presentation()->slideColor(presentation()->currentSlide(), "clearcolor", defaultColor);
    emit requestBackground(slideColor);
}

//-------------------------------------------------------------------------------------------------
void GoPrPlayer::applyCurrentSlide(int direction)
{
    if (!mRenderer || !presentation())
        return;

    if (direction == 0)
        renderCurrentSlide(0);
    else
        direction > 0 ? presentation()->nextSlide() : presentation()->prevSlide();
}

//-------------------------------------------------------------------------------------------------
QString GoPrPlayer::currentStream() const
{
    QString stream = GoPrImageStream::source(presentation()->slideString(presentation()->currentSlide(),"background","").toString());

    // maybe a local file?
    QString localFile = GoPrFileSource::findFile(stream,presentation()->url());

    return localFile.isEmpty() ? stream : localFile;
}


