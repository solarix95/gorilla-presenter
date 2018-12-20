#include <QDebug>
#include "goprpresentation.h"
#include "goprtextdocument.h"

GoPrTextDocument::GoPrTextDocument(const GoPrPresentation &p)
 : mPresentation(p)
{
}


QVariant GoPrTextDocument::loadResource(int type, const QUrl &name)
{
    qDebug() << type << name;
    switch (type) {
    case QTextDocument::HtmlResource:
    case QTextDocument::StyleSheetResource:
        return mPresentation.resourceContent(name.toString(),false);
    case QTextDocument::ImageResource:
        return QByteArray::fromBase64(mPresentation.resourceContent(name.toString(),true).toLatin1());
    }

    return QTextDocument::loadResource(type,name);
}
