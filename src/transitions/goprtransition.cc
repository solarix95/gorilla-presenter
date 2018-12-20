#include "goprgeometrybuffer.h"
#include "transitions/goprtransition.h"


//-------------------------------------------------------------------------------------------------
GoPrTransition::GoPrTransition(GoPrGeometryBuffer *from, GoPrGeometryBuffer *to, GoPrTransition::Direction d)
 : mFrom(from), mTo(to), mDirection(d), mAnimationState(0), mGradient(0)
{
}

//-------------------------------------------------------------------------------------------------
GoPrTransition::~GoPrTransition()
{
}

//-------------------------------------------------------------------------------------------------
bool GoPrTransition::animate()
{
    bool fromChanged = fromBuffer() ? fromBuffer()->animate() : false;
    bool toChanged   = toBuffer()   ? toBuffer()->animate() : false;

    if (mAnimationState >= 10000) {
        destroy(); // stop stream
        return fromChanged || toChanged;
    }

    mGradient += mAnimationState < 5000 ? 40 : -40;
    mAnimationState += mGradient;

    process(animationState());
    return true;
}

//-------------------------------------------------------------------------------------------------
GoPrTransition *GoPrTransition::destroy()
{
    if (mFrom && (mFrom != mTo)) {
        mFrom->destroy();
        delete mFrom;
        mFrom = NULL;
    }

    /*
    if (mTo) {
        mTo->destroy();
        delete mTo;
    }
    */

    return this;
}

//-------------------------------------------------------------------------------------------------
GoPrGeometryBuffer *GoPrTransition::currentBuffer()
{
    return mTo;
}

//-------------------------------------------------------------------------------------------------
void GoPrTransition::process(float state)
{
}
