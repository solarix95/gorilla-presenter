#include "goprfilesource.h"
#include "goprpresentation.h"
#include <QFile>
#include <QDir>

//-------------------------------------------------------------------------------------------------
GoPrFileSource::GoPrFileSource(const QString &filename)
 : mSourceFileName(filename)
{
}

//-------------------------------------------------------------------------------------------------
void GoPrFileSource::loadFile(const QString &filename)
{
    if (!presentation())
        return;
    QFile f(filename);
    f.open(QIODevice::ReadOnly);
    presentation()->fromJson(f.readAll());
}

//-------------------------------------------------------------------------------------------------
QString GoPrFileSource::name() const
{
    QDir f(mSourceFileName);
    return f.dirName();
}

//-------------------------------------------------------------------------------------------------
QString GoPrFileSource::url() const
{
    return mSourceFileName;
}

//-------------------------------------------------------------------------------------------------
QString GoPrFileSource::content(const QString &resourceName, bool binary) const
{
    QString fileName = findFile(resourceName, mSourceFileName);

    if (fileName.isEmpty())
        return "";

   QFile f(fileName);
   if (!f.open(QIODevice::ReadOnly))
       return "";

   return binary ? f.readAll().toBase64() : QString::fromUtf8(f.readAll());
}

//-------------------------------------------------------------------------------------------------
void GoPrFileSource::init()
{
    loadFile(mSourceFileName);
}

//-------------------------------------------------------------------------------------------------
QString GoPrFileSource::findFile(const QString &resourceName, const QString &parent)
{
    if (resourceName.isEmpty())
        return "";

    QString fileName;
    if (QFile::exists(resourceName)) {
        fileName = resourceName;
    } else {
        fileName = QFileInfo(parent).dir().path() + QDir::separator() + resourceName;
        fileName = QFile::exists(fileName) ? fileName : QString();
    }

    return fileName;
}

