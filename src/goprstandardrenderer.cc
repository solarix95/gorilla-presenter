#include <QDebug>
#include <QFont>
#include <QTextDocument>
#include "goprtextdocument.h"
#include "goprstandardrenderer.h"
#include "goprpresentation.h"

//-------------------------------------------------------------------------------------------------
GoPrStandardRenderer::GoPrStandardRenderer()
{
}

//-------------------------------------------------------------------------------------------------
QImage GoPrStandardRenderer::render(const GoPrPresentation &presentation, int slideIndex)
{
    QImage slide = createBackground(presentation, slideIndex);

    QPainter p(&slide);
    renderSlide(p, slide.rect().size(), presentation, slideIndex);

    prepareImageForShader(slide,presentation, slideIndex);
    return slide;
}

//-------------------------------------------------------------------------------------------------
QImage GoPrStandardRenderer::createBackground(const GoPrPresentation &p, int slideIndex)
{
    QImage slide(p.attribute("size", QSize(800,600)),QImage::Format_ARGB32);

    QString background = p.slideString(slideIndex,"background",QString()).toString();
    if (background.toLower().endsWith(".jpg") || background.toLower().endsWith(".png")) {
        QImage backgroundImage = QImage::fromData(QByteArray::fromBase64(p.resourceContent(background, true).toLatin1()));
        slide = backgroundImage.scaled(slide.size());
    } else {
        slide.fill(backgroundColor("background",p,slideIndex));
    }
    return slide;
}

//-------------------------------------------------------------------------------------------------
QColor GoPrStandardRenderer::backgroundColor(const QString &key, const GoPrPresentation &p, int slideIndex) const
{
    QString background = p.slideString(slideIndex,key,QString()).toString();
    return background.isEmpty() ? QColor(Qt::transparent) : QColor(background);
}

//-------------------------------------------------------------------------------------------------
void GoPrStandardRenderer::renderSlide(QPainter &p, const QSize &size, const GoPrPresentation &presentation, int slideIndex)
{
    renderByTextDocument(p,size,presentation,slideIndex);
}

//-------------------------------------------------------------------------------------------------
void GoPrStandardRenderer::prepareImageForShader(QImage &image, const GoPrPresentation &presentation,int slideIndex)
{
    if (!image.hasAlphaChannel())
        return;

    QColor bg = backgroundColor("clearcolor",presentation, slideIndex);

    for (int y=0; y<image.height(); y++)
        for (int x = 0; x < image.width(); x++) {
            QRgb c = image.pixel(x,y);
            int alpha = qAlpha(c);
            if (alpha == 255)
                continue;
            float mix = alpha/255.0f;
            QColor p = QColor(qRed(c)*mix + bg.red()*(1.0-mix),qGreen(c)*mix + bg.green()*(1-mix),qBlue(c)*mix + bg.blue()*(1-mix),alpha);
            image.setPixel(x,y,p.rgba());
        }
}

//-------------------------------------------------------------------------------------------------
void GoPrStandardRenderer::renderByTextDocument(QPainter &p, const QSize &size, const GoPrPresentation &presentation, int slideIndex)
{
    int scaleUp = presentation.slideString(presentation.currentSlide(),"html-scale","1").toInt();

    // QTextDocument doc;
    GoPrTextDocument doc(presentation);
    doc.setPageSize(QSize(size.width()/scaleUp, size.height()/scaleUp));

    doc.setDefaultStyleSheet(presentation.slideString(slideIndex,"htmlstyle",QString("")).toString());

    QString htmlText = presentation.slideString(slideIndex,QString("html")).toString();
    if (htmlText.toLower().endsWith(".html"))
        htmlText = presentation.resourceContent(htmlText, false);
    doc.setHtml(htmlText);

    float startX = (size.width()  - doc.size().width()*scaleUp)  / 2;
    float startY = (size.height() - doc.size().height()*scaleUp) / 2;

    p.save();
    p.translate(startX,startY);
    p.scale(scaleUp,scaleUp);
    p.setRenderHints(QPainter::TextAntialiasing | QPainter::HighQualityAntialiasing | QPainter::SmoothPixmapTransform	| QPainter::Antialiasing, true);
    doc.drawContents(&p,QRectF(0,0,size.width()*scaleUp,size.height()*scaleUp));
    p.restore();
}

//-------------------------------------------------------------------------------------------------
void GoPrStandardRenderer::renderByWebView(QPainter &p, const QSize &size, const GoPrPresentation &presentation, int slideIndex)
{

}

