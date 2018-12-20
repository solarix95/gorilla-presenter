#include "utils/goprterminator.h"

//-------------------------------------------------------------------------------------------------
GoPrTerminator::GoPrTerminator(QObject *target, const char *SIG)
    : QObject()
{
    connect(target, SIG, this, SLOT(terminateSender()));
}

//-------------------------------------------------------------------------------------------------
void GoPrTerminator::terminateSender()
{
    Q_ASSERT(sender());
    delete sender();
    deleteLater();
}

