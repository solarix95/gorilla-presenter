#ifndef GOPRROTATETRANSITION_H
#define GOPRROTATETRANSITION_H

#include "goprtransition.h"

class GoPrRotateTransition : public GoPrTransition
{
public:
    GoPrRotateTransition(GoPrGeometryBuffer *from, GoPrGeometryBuffer *to, GoPrTransition::Direction d);

    virtual void render(GoPrShader *shader, const QMatrix4x4 &modelView);
};

#endif // GOPRROTATETRANSITION_H
