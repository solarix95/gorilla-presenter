#ifndef GOPRPLAYER_H
#define GOPRPLAYER_H

#include <QObject>
#include <QColor>
#include "goprpresentation.h"

class GoPrImageStream;
class GoPrAbstractRenderer;
class GoPrPlayer : public QObject
{
    Q_OBJECT

public:
    GoPrPlayer();

    void setRenderer(GoPrAbstractRenderer *renderer);
    void addPresentation(GoPrPresentation *prs);

    bool              setCurrent(int index);
    int               current() const;
    int               count() const;
    GoPrPresentation *presentation(int index = -1) const;

signals:
    void aboutToInsertPresentation(int row);
    void presentationInserted();
    void currentPresentationChanged();
    void currentSlideChanged();

    void slideTo(const QImage &slide);
    void slideFrom(const QImage &slide);
    void slideTo(GoPrImageStream *stream);
    void slideFrom(GoPrImageStream *stream);
    void requestBackground(QColor c);

public slots:
    void init();
    void nextPresentation();
    void prevPresentation();
    void nextSlide();
    void prevSlide();

private slots:
    void currentSlideChanged(GoPrPresentation *p, int fromIndex, int toIndex);
    void renderCurrentSlide(int direction);

private:
    void    applyCurrentSlide(int direction);
    QString currentStream() const;

    int                     mCurrentPresentation;
    int                     mfromSlide;

    GoPrPresentations       mPresentationStack;
    GoPrAbstractRenderer   *mRenderer;
};

#endif // GOPRPLAYER_H
