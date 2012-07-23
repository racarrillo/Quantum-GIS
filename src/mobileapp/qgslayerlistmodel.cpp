#include "qgslayerlistmodel.h"

#include "qgismobileapp.h"

#include <qgsmapcanvasproxy.h>
#include <qgsmaplayer.h>
#include <qgsmaprenderer.h>
#include <qgsmaplayerregistry.h>



QgsLayerListModel::QgsLayerListModel(QObject *parent) :
    QAbstractListModel(parent)
{
    // roles are accessed like properties on QML delegates
    QHash<int, QByteArray> roles;
    roles[Name] = "name";
    roles[Visible] = "visibility";
    roles[Editable] = "editable";
    setRoleNames(roles);

    // connect map layer registry signal to legend
    connect( QgsMapLayerRegistry::instance(),
             SIGNAL( layersWillBeRemoved( QStringList ) ),
             this, SLOT( removeLayers( QStringList ) ) );
    connect( QgsMapLayerRegistry::instance(), SIGNAL( removedAll() ),
             this, SLOT( removeAll() ) );
    connect( QgsMapLayerRegistry::instance(),
             SIGNAL( layersAdded( QList<QgsMapLayer*> ) ),
             this, SLOT( addLayers( QList<QgsMapLayer *> ) ) );

//    connect( mMapCanvas, SIGNAL( layersChanged() ),
//             this, SLOT( refreshCheckStates() ) );
}

int QgsLayerListModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);

    qDebug("list model row count: %d", mLayerSet.size());

    return mLayerSet.size();
}

QVariant QgsLayerListModel::data(const QModelIndex &index, int role) const
{
    qDebug("accesing model data");

    if (!index.isValid())
        return QVariant();

    if ( index.row() < 0 || index.row() >= mLayerSet.size() )
        return QVariant();

    // TODO Is model index row equal to layer set row?
    QgsMapCanvasLayer *canvasLayer = mLayerSet.at(index.row());

    if (role == Name) {
        return canvasLayer->layer()->name();
    } else if (role == Visible)
    {
        return canvasLayer->isVisible();
    } else if (role == Editable)
    {
        return canvasLayer->layer()->isEditable();
    } else {
        return QVariant();
    }
}


bool QgsLayerListModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
  qDebug("setting model data");

   if (!index.isValid())
        return false;

    if ( index.row() < 0 || index.row() >= mLayerSet.size() )
        return false;

    //QgsMapLayer *layer = mapCanvas->layers().at(index.row());
    QgsMapCanvasLayer *canvasLayer = mLayerSet.at(index.row());

    if (role == Name) {
        canvasLayer->layer()->setLayerName(value.toString());
        emit dataChanged(index, index);
        return true;
    } else if (role == Visible)
    {
        canvasLayer->setVisible(value.toBool());
        updateCanvasLayerSet();
        emit dataChanged(index, index);
        return true;
    } else if (role == Editable)
    {
        emit startEditingLayer(canvasLayer->layer());
        emit dataChanged(index, index);
        return true;
    } else {
        return false;
    }
}

Qt::ItemFlags QgsLayerListModel::flags(const QModelIndex &index) const
{
    Q_UNUSED( index );
    return Qt::ItemIsSelectable | Qt::ItemIsEditable | Qt::ItemIsEnabled;
}

void QgsLayerListModel::setData(int row, const QString &field, QVariant value)
{
    LayerRoles role = UnknownLayerRole;

    if (field == "name")
    {
        role = Name;
    } else if (field == "visible")
    {
        role = Visible;
    } else if (field == "editable")
    {
        role = Editable;
    }

    setData(index(row), value, role);
}

QgsMapCanvasProxy *QgsLayerListModel::mapCanvas() const
{
    return mMapCanvasProxy;
}

void QgsLayerListModel::setMapCanvas(QgsMapCanvasProxy *m)
{
    qDebug("setting map canvas to model");

    mMapCanvasProxy = m;

//    connect( mMapCanvasProxy->mapCanvas(), SIGNAL(layersChanged()), this, SLOT(notifyLayerChange()) );
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

#if 0
void QgsLayerListModel::removeLayers( QStringList theLayers )
{
    Q_UNUSED( theLayers );
    qDebug("list model REMOVE LAYERS!");
}
#endif

void QgsLayerListModel::removeAll()
{
    qDebug("list model REMOVE ALL!");
}

void QgsLayerListModel::addLayers( QList<QgsMapLayer *> theLayerList )
{
    qDebug("list model ADD LAYERS!");

    bool myFirstLayerFlag = false;
    if ( mLayerSet.size() < 1 )
    {
      myFirstLayerFlag = true;
    }

    QgsMapCanvas *mapCanvas = mMapCanvasProxy->mapCanvas();
    if ( !mapCanvas || mapCanvas->isDrawing() )
    {
      return;
    }

    beginInsertRows(QModelIndex(), rowCount(), rowCount() + theLayerList.size() - 1);
    for(int i = 0; i < theLayerList.size(); i++)
    {
        mLayerSet << new QgsMapCanvasLayer(theLayerList.at(i));
    }
    endInsertRows();

    updateCanvasLayerSet();

  // set the canvas to the extent of our layer
  // TODO do this only if it's the first layer

  // first layer?
  if ( myFirstLayerFlag )
  {
    QgsMapCanvas *mapCanvas = mMapCanvasProxy->mapCanvas();

    QgsMapLayer *myFirstLayer = theLayerList.at( 0 );
    if ( !mapCanvas->mapRenderer()->hasCrsTransformEnabled() )
    {
      mapCanvas->mapRenderer()->setDestinationCrs( myFirstLayer->crs() );
      mapCanvas->mapRenderer()->setMapUnits( myFirstLayer->crs().mapUnits() );
    }
    mapCanvas->zoomToFullExtent();
    mapCanvas->clearExtentHistory();
  }

}

void QgsLayerListModel::removeLayer(int row)
{
    if (row < 0 || row >= mLayerSet.size())
    {
        return;
    }

    beginRemoveRows(QModelIndex(), row, row);
    mLayerSet.removeAt(row);
    endRemoveRows();

    updateCanvasLayerSet();
}

void QgsLayerListModel::updateCanvasLayerSet()
{
  QList<QgsMapCanvasLayer> canvasLayerSet;
  for(int i = 0; i < mLayerSet.size(); i++)
  {
    canvasLayerSet << *mLayerSet.at(i);
  }

  mMapCanvasProxy->mapCanvas()->setLayerSet(canvasLayerSet);

  mMapCanvasProxy->mapCanvas()->update();
}

void QgsLayerListModel::setCurrentLayer(int row)
{
  if (row < 0 || row >= mLayerSet.size())
  {
    return;
  }

  QgsMapLayer *layer = mLayerSet.at(row)->layer();

  if ( !layer )
  {
    return;
  }

  mMapCanvasProxy->mapCanvas()->setCurrentLayer(layer);

  emit currentLayerChanged( layer );

  qDebug() << "current layer:" << layer->name();
}
