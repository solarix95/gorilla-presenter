#include "goprplayer.h"
#include "goprpresentation.h"
#include "goprstackmodel.h"

//-------------------------------------------------------------------------------------------------
GoPrStackModel::GoPrStackModel(QObject *parent, GoPrPlayer *player)
    : QAbstractTableModel(parent), mPlayer(player)
{
    Q_ASSERT(mPlayer);
    connect(mPlayer, SIGNAL(aboutToInsertPresentation(int)), this, SLOT(aboutToInsertPresentation(int)));
    connect(mPlayer, SIGNAL(presentationInserted()), this, SLOT(presentationInserted()));
    connect(mPlayer, SIGNAL(currentPresentationChanged()), this, SLOT(refreshData()));
    connect(mPlayer, SIGNAL(currentSlideChanged()), this, SLOT(refreshData()));
}

//-------------------------------------------------------------------------------------------------
GoPrStackModel::~GoPrStackModel()
{
}

//-------------------------------------------------------------------------------------------------
QVariant GoPrStackModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation != Qt::Horizontal || role != Qt::DisplayRole)
        return QAbstractTableModel::headerData(section, orientation, role);

    switch (section) {
    case 0 : return "Current";
    case 1: return "Title";
    case 2: return "Slide#";
    case 3: return "Count";
    }

    return "";
}

//-------------------------------------------------------------------------------------------------
int GoPrStackModel::columnCount(const QModelIndex &parent) const
{
    return 4;
}

//-------------------------------------------------------------------------------------------------
int GoPrStackModel::rowCount(const QModelIndex &parent) const
{
    return mPlayer->count();
}

//-------------------------------------------------------------------------------------------------
bool GoPrStackModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!index.isValid())
        return false;

    Q_ASSERT(index.row() >= 0 && index.row() < mPlayer->count());

    if (index.column() == 0 && role == Qt::CheckStateRole && value.toInt() == Qt::Checked)
        return mPlayer->setCurrent(index.row());

    if (index.column() == 2 && role == Qt::EditRole)
        return mPlayer->presentation(index.row())->setCurrentSlide(value.toInt()-1);

    return false;
}

//-------------------------------------------------------------------------------------------------
QVariant GoPrStackModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    auto c = index.column();
    auto r = index.row();
    switch (role) {
    case Qt::DisplayRole: {
        switch (c) {
        case 1 : return mPlayer->presentation(r)->title();
        case 2: return mPlayer->presentation(r)->currentSlide() + 1;
        case 3: return mPlayer->presentation(r)->slideCount();
        }
    } break;
    case Qt::EditRole: {
        switch (c) {
        case 0 : return r == mPlayer->current() ? Qt::Checked : Qt::Unchecked;
        case 2: return mPlayer->presentation(r)->currentSlide() + 1;
        }
    } break;
    case Qt::CheckStateRole: {
        switch (c) {
        case 0 : return r == mPlayer->current() ? Qt::Checked : Qt::Unchecked;
        }
    } break;

    }

    return QVariant();
}

//-------------------------------------------------------------------------------------------------
Qt::ItemFlags GoPrStackModel::flags(const QModelIndex &index) const
{
    Qt::ItemFlags f = QAbstractTableModel::flags(index);
    if (index.column() == 0)
        f |= Qt::ItemIsUserCheckable;
    if (index.column() == 2)
        f |= Qt::ItemIsEditable;

    return f;
}

//-------------------------------------------------------------------------------------------------
void GoPrStackModel::aboutToInsertPresentation(int row)
{
    beginInsertRows(QModelIndex(),row,row);
}

//-------------------------------------------------------------------------------------------------
void GoPrStackModel::presentationInserted()
{
    connect(mPlayer->presentation(mPlayer->count()-1), SIGNAL(currentChanged(GoPrPresentation*,int,int)), this, SLOT(refreshData()));
    endInsertRows();
}

//-------------------------------------------------------------------------------------------------
void GoPrStackModel::refreshData()
{
    Q_ASSERT(rowCount() > 0);
    emit dataChanged(index(0,0), index(rowCount()-1,columnCount()-1));
}
