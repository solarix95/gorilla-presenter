#ifndef GOPRLOGO_H
#define GOPRLOGO_H

#include <QImage>
#include <QVector3D>
#include <QString>

struct GoPrLogo
{
    QString   front;
    QString   back;
    QVector3D pos;
    float     scale;
    QVector3D spin;

    void posByString(const QString &positionString);
    void spinByString(const QString &positionString);
};


#endif // GOPRLOGO_H
