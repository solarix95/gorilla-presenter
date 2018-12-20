#ifndef GOPRABSTRACTSOURCE_H
#define GOPRABSTRACTSOURCE_H

#include <QObject>

class GoPrPresentation;
class GoPrAbstractSource : public QObject
{
    Q_OBJECT
public:

    GoPrAbstractSource();

    void setPresentation(GoPrPresentation &prs);

    virtual QString name() const = 0;
    virtual QString url() const;
    virtual QString content(const QString &resourceName, bool binary) const;
    virtual void init();

signals:
    void presentationChanged();

protected:

    GoPrPresentation *presentation();

private:
    GoPrPresentation *mPresensation;
};

#endif // GOPRABSTRACTSOURCE_H
