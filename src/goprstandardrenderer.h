#ifndef GOPRSTANDARDRENDERER_H
#define GOPRSTANDARDRENDERER_H

#include "goprabstractrenderer.h"
#include <QPainter>

class GoPrStandardRenderer : public GoPrAbstractRenderer
{
public:
    GoPrStandardRenderer();

    virtual QImage render(const GoPrPresentation &presentation, int slideIndex);

private:
    QImage createBackground(const GoPrPresentation &presentation, int slideIndex);
    QColor backgroundColor(const QString &key, const GoPrPresentation &presentation, int slideIndex) const;
    void   renderSlide(QPainter &p, const QSize &size, const GoPrPresentation &presentation,int slideIndex);
    void   prepareImageForShader(QImage &image, const GoPrPresentation &presentation,int slideIndex);

    void   renderByTextDocument(QPainter &p, const QSize &size, const GoPrPresentation &presentation,int slideIndex);
    void   renderByWebView(QPainter &p, const QSize &size, const GoPrPresentation &presentation,int slideIndex);

};

#endif // GOPRSTANDARDRENDERER_H
