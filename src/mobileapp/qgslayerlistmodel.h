#ifndef QGSLAYERLISTMODEL_H
#define QGSLAYERLISTMODEL_H

#include <QAbstractListModel>

class QgsMapCanvas;
class QgsMapCanvasProxy;

class QgsLayerListModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(QgsMapCanvasProxy *mapCanvas READ mapCanvas WRITE setMapCanvas)
public:
    enum LayerRoles {
        NameRole = Qt::UserRole + 1
    };

    explicit QgsLayerListModel(QObject *parent = 0);

    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

    QgsMapCanvasProxy *mapCanvas() const;
    void setMapCanvas(QgsMapCanvasProxy *m);

signals:
    
public slots:
    void notifyLayerChange();

private:
    QgsMapCanvasProxy *mMapCanvasProxy;
};

#endif // QGSLAYERLISTMODEL_H
