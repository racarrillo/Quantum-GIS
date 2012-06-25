#ifndef QGSMAPCANVASPROXY_H
#define QGSMAPCANVASPROXY_H

#include <QGraphicsProxyWidget>
#include <qgsmapcanvas.h>

class QgsMapCanvasProxy : public QGraphicsProxyWidget
{
    Q_OBJECT
public:
    explicit QgsMapCanvasProxy(QGraphicsItem *parent = 0);
    QgsMapCanvas* mapCanvas();

signals:
    
public slots:

private:
  QgsMapCanvas *mMapCanvas;
};

#endif // QGSMAPCANVASPROXY_H
