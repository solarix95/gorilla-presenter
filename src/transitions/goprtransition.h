#ifndef GOPRTRANSITION_H
#define GOPRTRANSITION_H

#include <QList>
#include <QMatrix4x4>

class GoPrGeometryBuffer;
class GoPrShader;

class GoPrTransition
{
public:
    enum Direction {
        Left,
        Right
    };

    GoPrTransition(GoPrGeometryBuffer *from, GoPrGeometryBuffer *to, Direction d);
    virtual ~GoPrTransition();

    virtual void render(GoPrShader *shader, const QMatrix4x4 &modelView) = 0;
    virtual bool animate();
    virtual GoPrTransition *destroy();

    GoPrGeometryBuffer *currentBuffer();
    inline float        animationState() const { return mAnimationState/10000.0f; }

protected:
    virtual void process(float state);


    inline GoPrGeometryBuffer *fromBuffer() const { return mFrom; }
    inline GoPrGeometryBuffer *toBuffer()   const { return mTo;   }
    inline Direction           direction()  const { return mDirection; }
    inline float               normDirection()  const { return mDirection == Left ? 1.0f : -1.0f; }

private:
    GoPrGeometryBuffer *mFrom;
    GoPrGeometryBuffer *mTo;
    Direction           mDirection;
    int                 mAnimationState; // 0 .. 1 -> 0 .. 100% animation
    int                 mGradient;

};

typedef QList<GoPrTransition*> GoPrTransitions;

#endif // GOPRTRANSITION_H
