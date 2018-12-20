#include "goprpresentation.h"
#include "goprabstractsource.h"
#include <QJsonDocument>
#include <QSize>
#include <QStringList>

//-------------------------------------------------------------------------------------------------
GoPrPresentation::GoPrPresentation(GoPrAbstractSource *source)
    : mSource(source), mCurrentSlide(-1)
{
}

//-------------------------------------------------------------------------------------------------
GoPrPresentation::~GoPrPresentation()
{
    if (mSource)
        delete mSource;
}

//-------------------------------------------------------------------------------------------------
void GoPrPresentation::init()
{
    if (mSource) {
        mSource->setPresentation(*this);
        mSource->init();
    }
}

//-------------------------------------------------------------------------------------------------
bool GoPrPresentation::fromJson(const QByteArray &buffer)
{
    QJsonDocument d = QJsonDocument::fromJson(buffer);
    return fromVariant(d.toVariant());
}

//-------------------------------------------------------------------------------------------------
bool GoPrPresentation::fromVariant(const QVariant &variant)
{
    if (variant.type() != QVariant::Map)
        return false;
    QVariantMap map = variant.toMap();
    if (!map.contains("settings"))
        return false;
    if (!map.contains("slides"))
        return false;

    mAttributs = map.value("settings").toMap();
    mSlides    = map.value("slides").toList();
    mResources.clear();

    QVariantList resources = map.value("resources").toList();
    foreach( QVariant resource, resources) {
        QVariantMap resInfo = resource.toMap();
        mResources[resInfo.value("name").toString()] = resInfo.value("content").toString();
    }

    emit changed(-1);
    return true;
}

//-------------------------------------------------------------------------------------------------
QString GoPrPresentation::lastError() const
{
    return mLastError;
}

//-------------------------------------------------------------------------------------------------
bool GoPrPresentation::setCurrentSlide(int index)
{
    if (mCurrentSlide == index || index < 0 || index >= mSlides.count())
        return false;

    int old = mCurrentSlide;
    mCurrentSlide = index;
    emit currentChanged(this, old, mCurrentSlide);
    return true;
}

//-------------------------------------------------------------------------------------------------
bool GoPrPresentation::nextSlide()
{
    return setCurrentSlide(mCurrentSlide+1);;
}

//-------------------------------------------------------------------------------------------------
bool GoPrPresentation::prevSlide()
{
    return setCurrentSlide(mCurrentSlide-1);;
}

//-------------------------------------------------------------------------------------------------
int GoPrPresentation::currentSlide() const
{
    return mCurrentSlide;
}

//-------------------------------------------------------------------------------------------------
QString GoPrPresentation::title() const
{
    return (mSource ? mSource->name() : QString());
}

//-------------------------------------------------------------------------------------------------
QString GoPrPresentation::url() const
{
    return (mSource ? mSource->url() : QString());
}

//-------------------------------------------------------------------------------------------------
QVariant GoPrPresentation::attribute(const QString &key, const QVariant &defaultValue) const
{
    return mAttributs.value(key,defaultValue);
}

//-------------------------------------------------------------------------------------------------
QString GoPrPresentation::attribute(const QString &key, const QString &defaultValue) const
{
    if (!mAttributs.contains(key))
        return defaultValue;
    return mAttributs.value(key).toString();
}

//-------------------------------------------------------------------------------------------------
QSize GoPrPresentation::attribute(const QString &key, const QSize &defaultValue) const
{
    if (!mAttributs.contains(key))
        return defaultValue;

    QString     sizeString = mAttributs.value(key).toString();
    QStringList parts      = sizeString.split("x");
    if (parts.count() != 2)
        return defaultValue;
    return QSize(parts[0].toInt(),parts[1].toInt());
}

//-------------------------------------------------------------------------------------------------
QColor GoPrPresentation::attribute(const QString &key, const QColor &defaultValue) const
{
    if (!mAttributs.contains(key))
        return defaultValue;
    QString colorString = mAttributs.value(key).toString();
    return QColor(colorString);
}

//-------------------------------------------------------------------------------------------------
int GoPrPresentation::slideCount() const
{
    return mSlides.count();
}

//-------------------------------------------------------------------------------------------------
QVariant GoPrPresentation::slideString(int index, const QString &key, const QString &defaultValue) const
{
    QString retDefault = attribute(key,defaultValue);
    if (index < 0 || index >= mSlides.count())
        return defaultValue;
    if (!mSlides[index].toMap().contains(key))
        return retDefault;
    return mSlides[index].toMap().value(key);
}

//-------------------------------------------------------------------------------------------------
QColor GoPrPresentation::slideColor(int index, const QString &key, const QColor &defaultValue) const
{
    QColor retDefault = attribute(key,defaultValue);
    if (index < 0 || index >= mSlides.count())
        return retDefault;
    if (!mSlides[index].toMap().contains(key))
        return retDefault;
    return QColor(mSlides[index].toMap().value(key).toString());
}

//-------------------------------------------------------------------------------------------------
QString GoPrPresentation::resourceContent(const QString &name, bool binaryData) const
{
    if (mResources.contains(name))
        return mResources.value(name,QString());

    if (mSource)
        return mSource->content(name, binaryData);
    return "";
}

