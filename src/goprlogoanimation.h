#ifndef GOPRLOGOANIMATION_H
#define GOPRLOGOANIMATION_H

#include <QMatrix4x4>
#include <QList>
#include <QVector3D>
#include "goprlogo.h"
#include "goprgeometrybuffer.h"

class GoPrShader;
class GoPrLogoAnimation
{
public:
    GoPrLogoAnimation();

    bool fromJsonFile(const QString &filename);
    bool fromJson(const QByteArray &buffer);
    bool fromVariant(const QVariant &variant);
    void addLogo(const GoPrLogo &logo);
    void render(GoPrShader *shader, const QMatrix4x4 &modelView);
    bool animate();

private:
    QList<GoPrLogo>            mLogos;
    QList<QVector3D>           mTurnStates;
    QList<GoPrGeometryBuffers> mBuffers;
    QString                    mSourceFileName;
};

#endif // GOPRLOGOANIMATION_H
