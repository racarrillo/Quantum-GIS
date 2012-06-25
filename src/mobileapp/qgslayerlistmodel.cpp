#include "qgslayerlistmodel.h"

#include <qgsmapcanvasproxy.h>
#include <qgsmaplayer.h>

QgsLayerListModel::QgsLayerListModel(QObject *parent) :
    QAbstractListModel(parent)
{
    // roles are accessed like properties on QML delegates
    QHash<int, QByteArray> roles;
    roles[NameRole] = "name";
    setRoleNames(roles);
}

int QgsLayerListModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);

    if ( mMapCanvasProxy ) {
        qDebug("list model row count: %d", mMapCanvasProxy->mapCanvas()->layerCount());
        return mMapCanvasProxy->mapCanvas()->layerCount();
    } else {
        qDebug("no canvas proxy on layer list model");
        return 0;
    }
}

QVariant QgsLayerListModel::data(const QModelIndex &index, int role) const
{
    qDebug("accesing model data");

    if (!index.isValid())
        return QVariant();

    QgsMapCanvas *mapCanvas = mMapCanvasProxy->mapCanvas();

    if ( index.row() < 0 || index.row() >= mapCanvas->layerCount() )
        return QVariant();

    // TODO Is model index row equal to layer set row?
    QgsMapLayer *layer = mapCanvas->layers().at(index.row());

    if (role == NameRole) {
        qDebug("name role");
        return layer->name();
    } else {
        qDebug("no role");
        return QVariant();
    }
}

QgsMapCanvasProxy *QgsLayerListModel::mapCanvas() const
{
    return mMapCanvasProxy;
}

void QgsLayerListModel::setMapCanvas(QgsMapCanvasProxy *m)
{
    qDebug("setting map canvas to model");

    mMapCanvasProxy = m;

    connect( mMapCanvasProxy->mapCanvas(), SIGNAL(layersChanged()), this, SLOT(notifyLayerChange()) );
}

void QgsLayerListModel::notifyLayerChange()
{
    qDebug("map canvas' layer set have changed");

    // if layers have been added
    // TODO use layercount to know how many layers were added
    beginInsertRows(QModelIndex(), rowCount() - 1, rowCount() - 1);
    endInsertRows();

    // emit changes for all the elements of the list
    int layerCount = mMapCanvasProxy->mapCanvas()->layerCount();
    emit dataChanged( index(0, 0, QModelIndex()), index(layerCount - 1, 0, QModelIndex()) );
}
