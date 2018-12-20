#include <QFile>
#include <QJsonDocument>
#include "goprlogoanimation.h"
#include "streams/goprimagestream.h"
#include "streams/gopropencvstream.h"

#include "goprfilesource.h"
#include "goprshader.h"
//-------------------------------------------------------------------------------------------------
GoPrLogoAnimation::GoPrLogoAnimation()
{
}

//-------------------------------------------------------------------------------------------------
bool GoPrLogoAnimation::fromJsonFile(const QString &filename)
{
    QFile f(filename);
    if (!f.open(QIODevice::ReadOnly))
        return false;
    mSourceFileName = filename;
    return fromJson(f.readAll());
}

//-------------------------------------------------------------------------------------------------
bool GoPrLogoAnimation::fromJson(const QByteArray &buffer)
{
    QJsonDocument d = QJsonDocument::fromJson(buffer);
    return fromVariant(d.toVariant());
}

//-------------------------------------------------------------------------------------------------
bool GoPrLogoAnimation::fromVariant(const QVariant &variant)
{
    if (variant.type() != QVariant::Map)
        return false;
    QVariantMap map = variant.toMap();
    if (!map.contains("logos"))
        return false;


    QVariantList logos = map.value("logos").toList();
    for (int i=0; i<logos.count(); i++) {
        QVariantMap logoData = logos[i].toMap();

        // Minimal
        QString frontName      = logoData.value("front","").toString();
        QString positionString = logoData.value("pos","").toString();
        float scale            = logoData.value("scale","1.0").toFloat();

        if (frontName.isEmpty() || positionString.isEmpty() || !scale)
            continue;

        // Optional

        QString backName   = logoData.value("back","").toString();
        QString spinString = logoData.value("spin","").toString();

        GoPrLogo logo;
        logo.front = frontName;
        logo.back  = backName;
        logo.scale = scale;
        logo.posByString(positionString);
        logo.spinByString(spinString);
        addLogo(logo);
    }

    return true;
}

//-------------------------------------------------------------------------------------------------
void GoPrLogoAnimation::addLogo(const GoPrLogo &logo)
{
    if (logo.front.isNull() || logo.scale == 0.0f)
        return;
    mLogos      << logo;
    mTurnStates << QVector3D(0,0,0);

    GoPrGeometryBuffers buffers;

    // Front
    QString streamName = GoPrImageStream::source(mLogos.last().front);
    QString frontFile   = GoPrFileSource::findFile(mLogos.last().front, mSourceFileName);
    buffers << new GoPrGeometryBuffer();
    if (streamName.isEmpty()) {
        buffers.last()->init(QImage(frontFile));
    } else {
        QString localFile = GoPrFileSource::findFile(streamName,mSourceFileName);
        buffers.last()->init(new GoPrOpenCvStream(localFile.isEmpty() ? streamName : localFile));
    }

    // Back
    streamName       = GoPrImageStream::source(mLogos.last().back);
    QString backFile = GoPrFileSource::findFile(mLogos.last().back, mSourceFileName);
    buffers << new GoPrGeometryBuffer();
    if (streamName.isEmpty()) {
        buffers.last()->init(backFile.isEmpty() ? QImage(frontFile).mirrored(true,false) : QImage(backFile));
    } else {
        QString localFile = GoPrFileSource::findFile(streamName,mSourceFileName);
        buffers << new GoPrGeometryBuffer();
        buffers.last()->init(new GoPrOpenCvStream(localFile.isEmpty() ? streamName : localFile));
    }

    mBuffers << buffers;
}

//-------------------------------------------------------------------------------------------------
void GoPrLogoAnimation::render(GoPrShader *shader, const QMatrix4x4 &modelView)
{
    Q_ASSERT(mLogos.count() == mBuffers.count());
    for (int i=0; i<mLogos.count(); i++) {
        QMatrix4x4 view = modelView;
        view.translate(mLogos[i].pos);
        view.rotate(mTurnStates[i].x(),1,0,0);
        view.rotate(mTurnStates[i].y(),0,1,0);
        view.rotate(mTurnStates[i].z(),0,0,1);
        view.scale(mLogos[i].scale);
        shader->drawBuffer(*mBuffers[i].first(),view);
        if (mBuffers[i].count() > 1) {
            view.rotate(180,0,1,0);
            shader->drawBuffer(*mBuffers[i][1],view);
        }
    }
}

//-------------------------------------------------------------------------------------------------
bool GoPrLogoAnimation::animate()
{
    bool changed = false;
    Q_ASSERT(mTurnStates.count() == mLogos.count());
    for (int i=0; i<mTurnStates.count(); i++) {
        QVector3D state = mTurnStates[i];
        state.setX(state.x() + mLogos[i].spin[0]);
        state.setY(state.y() + mLogos[i].spin[1]);
        state.setZ(state.z() + mLogos[i].spin[2]);
        if (state != mTurnStates[i]) {
            mTurnStates[i] = state;
            changed = true;
        }
    }

    for (int i=0; i<mBuffers.count(); i++) {
        Q_ASSERT(mBuffers[i].count() == 2);
        changed = mBuffers[i][0]->animate() || changed;
        changed = mBuffers[i][1]->animate() || changed;
    }

    return changed;
}



