#ifndef GOPRTERMINATOR_H
#define GOPRTERMINATOR_H

#include <QObject>

class GoPrTerminator : public QObject
{
    Q_OBJECT
public:
    explicit GoPrTerminator(QObject *target, const char *SIG);

private slots:
    void terminateSender();

};

#endif // GOPRTERMINATOR_H
