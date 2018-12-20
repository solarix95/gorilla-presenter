
#include "transitions/goprcubetransition.h"
#include "goprshader.h"
#include "goprgeometrybuffer.h"

//-------------------------------------------------------------------------------------------------
GoPrCubeTransition::GoPrCubeTransition(GoPrGeometryBuffer *from, GoPrGeometryBuffer *to, GoPrTransition::Direction d)
 : GoPrTransition(from,to,d), mGradient(0)
{
}

//-------------------------------------------------------------------------------------------------
void GoPrCubeTransition::render(GoPrShader *shader, const QMatrix4x4 &modelView)
{
    Q_ASSERT(shader);
    Q_ASSERT(toBuffer());

    QMatrix4x4 toView;
    if (fromBuffer()) {
        toView = modelView;
        toView.translate(0,0,-fromBuffer()->imageRatio());
        toView.rotate(normDirection()*(-90)*animationState(),0,1,0);
        toView.translate(0,0,fromBuffer()->imageRatio());
        shader->drawBuffer(*fromBuffer(), toView);
    }

    toView = modelView;

    toView.translate(0,0,-toBuffer()->imageRatio());
    toView.rotate(normDirection()*((90) - (90 * animationState())),0,1,0);
    toView.translate(0,0,toBuffer()->imageRatio());
    shader->drawBuffer(*toBuffer(), toView);
}
