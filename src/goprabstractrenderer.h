#ifndef GOPRABSTRACTRENDERER_H
#define GOPRABSTRACTRENDERER_H

#include <QObject>
#include <QImage>
#include <QRect>

class GoPrPresentation;
class GoPrAbstractRenderer : public QObject
{
    Q_OBJECT
public:
    explicit GoPrAbstractRenderer(QObject *parent = 0);

    virtual QImage render(const GoPrPresentation &presentation, int slideIndex) = 0;

};

#endif // GOPRABSTRACTRENDERER_H
