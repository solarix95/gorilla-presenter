#include <QStringList>
#include "goprlogo.h"


void GoPrLogo::posByString(const QString &positionString)
{
    QStringList parts = positionString.split(",");
    if (parts.count() != 3)
        return;
    pos.setX(parts[0].toFloat());
    pos.setY(parts[1].toFloat());
    pos.setZ(parts[2].toFloat());
}

void GoPrLogo::spinByString(const QString &positionString)
{
    QStringList parts = positionString.split(",");
    if (parts.count() < 1)
        return;
    spin.setX(parts[0].toFloat());
    spin.setY(parts.count() >= 2 ? parts[1].toFloat() : 0.0f);
    spin.setZ(parts.count() >= 3 ? parts[2].toFloat() : 0.0f);
}
