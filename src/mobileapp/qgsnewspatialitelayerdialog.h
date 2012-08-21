/***************************************************************************
    qgsnewspatialitelayerdialog.cpp  -  New Spatialite Layer
     --------------------------------------
    Date                 : 25-Jul-2012
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

#ifndef QGSNEWSPATIALITELAYERDIALOG_H
#define QGSNEWSPATIALITELAYERDIALOG_H

#include "qgisgui.h"
#include "qgscontexthelp.h"

#include "qgis.h"

#include <QDeclarativeView>

extern "C"
{
#include <sqlite3.h>
#include <spatialite.h>
}

#include <QVariant>

class QDeclarativeView;

class QgsNewSpatialiteLayerDialog: public QObject
{
    Q_OBJECT

  public:
    QgsNewSpatialiteLayerDialog(QDeclarativeView *view);
    ~QgsNewSpatialiteLayerDialog();

  public slots:
    /** Create a new database */
    bool createDb();

    /** Create the layer  */
    void apply();

    void setDatabase(QString database);
    void setLayerName(QString layerName);
    void setType(QString type);
    void setPrimaryKey(bool primaryKey);
    void setGeometryColumn(QString geometryColumn);
    void setAttributes(QVariant attributes);

  private:

    static QString quotedIdentifier( QString id );
    static QString quotedValue( QString value );

    /**
     * Export the connections to QML.
     *  This should be called each time the connections list changes
     */
    void updateConnections();

    QDeclarativeView *mView;

    // Fields
    QString mDatabase;
    QString mLayerName;
    QString mType;
    bool mPrimaryKey;
    int mCrsId;
    QString mGeometryColumn;
    QVariantList mAttributes;
};

#endif // QGSNEWVECTORLAYERDIALOG_H
