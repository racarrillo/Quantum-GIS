/***************************************************************************
    qgsmaptooldeletefeature.cpp  -  map tool for deleting features touching them
     --------------------------------------
    Date                 : 24-Jul-2012
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

#ifndef QGSMAPTOOLDELETEFEATURE_H
#define QGSMAPTOOLDELETEFEATURE_H

#include "qgsmaptooledit.h"

/**This tool delele features from already existing vector layers*/
class QgsMapToolDeleteFeature: public QgsMapToolEdit
{
    Q_OBJECT
  public:
    QgsMapToolDeleteFeature( QgsMapCanvas* canvas );
    virtual ~QgsMapToolDeleteFeature();

    virtual void canvasReleaseEvent( QMouseEvent * e );
};

#endif
