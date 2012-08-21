/***************************************************************************
    qgslayerlistmodel.h  -  Model data for store the loaded layers
     --------------------------------------
    Date                 : 22-Jun-2012
    Copyright            : (C) 2012 by Ramon Carrillo
    Email                : racarrillo91 at gmail.com
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef QGSLAYERLISTMODEL_H
#define QGSLAYERLISTMODEL_H

#include <QAbstractListModel>

class QgsMapCanvas;
class QgsMapCanvasProxy;
class QgsMapLayer;
class QgsMapCanvasLayer;

class QgsLayerListModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(QgsMapCanvasProxy *mapCanvas READ mapCanvas WRITE setMapCanvas)
public:
    enum LayerRoles {
        UnknownLayerRole = Qt::UserRole + 1,
        Name,
        Visible,
        Editable
    };

    explicit QgsLayerListModel(QObject *parent = 0);

    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);
    Qt::ItemFlags flags(const QModelIndex &index) const;

    /* TODO It's not posible to change the date from QML using roles
     * view QTBUG-7932 */
    Q_INVOKABLE void setData(int row, const QString &, QVariant value);

    QgsMapCanvasProxy *mapCanvas() const;
    void setMapCanvas(QgsMapCanvasProxy *m);

    void updateCanvasLayerSet();

signals:
    void startEditingLayer(QgsMapLayer *);
    void currentLayerChanged( QgsMapLayer *);

public slots:
    //void removeLayers( QStringList theLayers );
    void removeAll();
    void addLayers( QList<QgsMapLayer *> );
    void removeLayer( int index );

    void setCurrentLayer( int row );

private:
    QList<QgsMapCanvasLayer *> mLayerSet;
    QgsMapCanvasProxy *mMapCanvasProxy;
};

#endif // QGSLAYERLISTMODEL_H
