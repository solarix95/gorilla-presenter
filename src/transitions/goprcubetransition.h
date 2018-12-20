#ifndef GOPRCUBETRANSITION_H
#define GOPRCUBETRANSITION_H

#include "goprtransition.h"

class GoPrCubeTransition : public GoPrTransition
{
public:
    GoPrCubeTransition(GoPrGeometryBuffer *from, GoPrGeometryBuffer *to, GoPrTransition::Direction d);

    virtual void render(GoPrShader *shader, const QMatrix4x4 &modelView);

private:
    float mGradient;
};

#endif // GOPRCUBETRANSITION_H
