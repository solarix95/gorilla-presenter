#ifndef GOPRTEXTDOCUMENT_H
#define GOPRTEXTDOCUMENT_H

#include <QTextDocument>

class GoPrPresentation;
class GoPrTextDocument : public QTextDocument
{
public:
    GoPrTextDocument(const GoPrPresentation &p);

protected:
    virtual QVariant loadResource(int type, const QUrl & name);

private:
    const GoPrPresentation &mPresentation;
};

#endif // GOPRTEXTDOCUMENT_H
