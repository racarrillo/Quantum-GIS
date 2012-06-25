#include "qgsmapcanvasproxy.h"

#include <qgsmapcanvas.h>

QgsMapCanvasProxy::QgsMapCanvasProxy(QGraphicsItem *parent) :
    QGraphicsProxyWidget(parent)
{
    mMapCanvas = new QgsMapCanvas( (QWidget *)parent );
    mMapCanvas->enableAntiAliasing(true);
    mMapCanvas->setCanvasColor( Qt::white );

    setWidget(mMapCanvas);
}

QgsMapCanvas *QgsMapCanvasProxy::mapCanvas()
{
  return mMapCanvas;
}
