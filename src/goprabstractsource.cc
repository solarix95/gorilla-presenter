#include "goprabstractsource.h"

//-------------------------------------------------------------------------------------------------
GoPrAbstractSource::GoPrAbstractSource()
 : mPresensation(NULL)
{

}

//-------------------------------------------------------------------------------------------------
void GoPrAbstractSource::setPresentation(GoPrPresentation &prs)
{
    if (mPresensation == &prs)
        return;
    mPresensation = &prs;
    emit presentationChanged();
}

//-------------------------------------------------------------------------------------------------
QString GoPrAbstractSource::url() const
{
    return "";
}

//-------------------------------------------------------------------------------------------------
QString GoPrAbstractSource::content(const QString &, bool binary) const
{
    return "";
}

//-------------------------------------------------------------------------------------------------
void GoPrAbstractSource::init()
{

}

//-------------------------------------------------------------------------------------------------
GoPrPresentation *GoPrAbstractSource::presentation()
{
    return mPresensation;
}

