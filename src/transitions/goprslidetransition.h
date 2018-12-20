#ifndef GOPRSLIDETRANSITION_H
#define GOPRSLIDETRANSITION_H

#include "goprtransition.h"

class GoPrSlideTransition : public GoPrTransition
{
public:
    GoPrSlideTransition(GoPrGeometryBuffer *from, GoPrGeometryBuffer *to, GoPrTransition::Direction d);

    virtual void render(GoPrShader *shader, const QMatrix4x4 &modelView);
};

#endif // GOPRSLIDETRANSITION_H
