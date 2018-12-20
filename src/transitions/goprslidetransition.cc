#include <QDebug>
#include "goprshader.h"
#include "transitions/goprslidetransition.h"
#include "goprgeometrybuffer.h"

//-------------------------------------------------------------------------------------------------
GoPrSlideTransition::GoPrSlideTransition(GoPrGeometryBuffer *from, GoPrGeometryBuffer *to, GoPrTransition::Direction d)
 : GoPrTransition(from,to,d)
{

}

//-------------------------------------------------------------------------------------------------
void GoPrSlideTransition::render(GoPrShader *shader, const QMatrix4x4 &modelView)
{
    Q_ASSERT(shader);
    Q_ASSERT(toBuffer());

    int distance = 4;
    QMatrix4x4 toView;
    if (fromBuffer()) {
        toView = modelView;
        toView.translate(normDirection()*-1*distance*animationState(),0,0);
        shader->drawBuffer(*fromBuffer(), toView);
    }

    toView = modelView;
    toView.translate(normDirection()*(distance - (distance*animationState())),0,0);
    shader->drawBuffer(*toBuffer(), toView);
}
