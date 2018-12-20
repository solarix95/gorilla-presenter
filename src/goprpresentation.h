#ifndef GOPRPRESENTATION_H
#define GOPRPRESENTATION_H

#include <QVariant>
#include <QString>
#include <QObject>
#include <QMap>
#include <QByteArray>
#include <QSize>
#include <QColor>

class GoPrAbstractSource;
class GoPrPresentation : public QObject
{
    Q_OBJECT

public:
    GoPrPresentation(GoPrAbstractSource *source = NULL);
    virtual ~GoPrPresentation();

    // Loading
    void     init();
    bool     fromJson(const QByteArray &buffer);
    bool     fromVariant(const QVariant &variant);
    QString  lastError() const;

    // Player
    bool     setCurrentSlide(int index);
    bool     nextSlide();
    bool     prevSlide();
    int      currentSlide() const;

    // Presentation Description
    QString  title() const;
    QString  url() const;
    QVariant attribute(const QString &key, const QVariant &defaultValue = QVariant()) const;
    QString  attribute(const QString &key, const QString  &defaultValue = "") const;
    QSize    attribute(const QString &key, const QSize    &defaultValue = QSize()) const;
    QColor   attribute(const QString &key, const QColor   &defaultValue = QColor()) const;

    int      slideCount() const;
    QVariant slideString(int index, const QString &key, const QString  &defaultValue = "") const;
    QColor   slideColor(int index, const QString &key, const QColor   &defaultValue = QColor()) const;

    QString    resourceContent(const QString &name, bool binaryData) const;

signals:
    void currentChanged(GoPrPresentation *p, int fromIndex, int toIndex);
    void changed(int slide);

private:
    QVariantMap           mAttributs;
    QVariantList          mSlides;
    QMap<QString,QString> mResources;
    QString               mLastError;
    GoPrAbstractSource   *mSource;

    int                   mCurrentSlide;
};

typedef QList<GoPrPresentation*> GoPrPresentations;

#endif // GOPRPRESENTATION_H
