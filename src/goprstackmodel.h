#ifndef GOPRSTACKMODEL_H
#define GOPRSTACKMODEL_H

#include <QAbstractTableModel>

class GoPrPlayer;
class GoPrStackModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    GoPrStackModel(QObject *parent, GoPrPlayer *player);
    virtual ~GoPrStackModel();

    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
    int columnCount(const QModelIndex & parent = QModelIndex()) const;
    int rowCount(const QModelIndex & parent = QModelIndex()) const;
    bool setData(const QModelIndex & index, const QVariant & value, int role = Qt::EditRole);
    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;
    Qt::ItemFlags flags(const QModelIndex & index) const;

private slots:
    void aboutToInsertPresentation(int row);
    void presentationInserted();
    void refreshData();

private:
    GoPrPlayer *mPlayer;
};

#endif // GOPRSTACKMODEL_H
