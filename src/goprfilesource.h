#ifndef GOPRFILESOURCE_H
#define GOPRFILESOURCE_H

#include "goprabstractsource.h"

class GoPrFileSource : public GoPrAbstractSource
{
public:
    GoPrFileSource(const QString &filename);

    void loadFile(const QString &filename);

    virtual QString name() const;
    virtual QString url() const;
    virtual QString content(const QString &resourceName, bool binary) const;
    virtual void init();

    static QString findFile(const QString &resourceName, const QString &parent);

private:
    QString mSourceFileName;
};

#endif // GOPRFILESOURCE_H
