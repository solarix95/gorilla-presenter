#include "transitions/goprrotatetransition.h"
#include "goprshader.h"
#include "goprgeometrybuffer.h"

//-------------------------------------------------------------------------------------------------
GoPrRotateTransition::GoPrRotateTransition(GoPrGeometryBuffer *from, GoPrGeometryBuffer *to, GoPrTransition::Direction d)
 : GoPrTransition(from,to,d)
{
}

//-------------------------------------------------------------------------------------------------
void GoPrRotateTransition::render(GoPrShader *shader, const QMatrix4x4 &modelView)
{
    Q_ASSERT(shader);
    Q_ASSERT(toBuffer());

    QMatrix4x4 toView;
    if (fromBuffer()) {
        toView = modelView;
        toView.rotate(normDirection()*(-180)*animationState(),0,1,0);
        shader->drawBuffer(*fromBuffer(), toView);
    }

    toView = modelView;
    toView.rotate(normDirection()*((180) - (180 * animationState())),0,1,0);
    shader->drawBuffer(*toBuffer(), toView);
}
