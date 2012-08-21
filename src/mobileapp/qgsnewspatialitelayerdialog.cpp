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

#include "qgsnewspatialitelayerdialog.h"

#include "qgis.h"
#include "qgsapplication.h"
#include "qgsproviderregistry.h"
#include <qgsvectorlayer.h>
#include <qgsmaplayerregistry.h>
#include "qgscoordinatereferencesystem.h"
#include "qgsgenericprojectionselector.h"

#include <QtDeclarative> // for qmlRegisterType
#include <QDeclarativeView>
#include <QDeclarativeEngine>
#include <QFileDialog>

#include "qgslogger.h"

#include <QPushButton>
#include <QSettings>
#include <QLineEdit>
#include <QMessageBox>
#include <QFileDialog>
#include <QLibrary>

#include <spatialite.h>

QgsNewSpatialiteLayerDialog::QgsNewSpatialiteLayerDialog(QDeclarativeView *view)
:mView(view)
{
  updateConnections();

  // Set the SRID box to a default of WGS84
  QgsCoordinateReferenceSystem srs;
  srs.createFromOgcWmsCrs( GEO_EPSG_CRS_AUTHID );
  srs.validate();
  mCrsId = srs.srsid();

  //  Connect signal from QML
  QObject *object;
  object = (QObject*)mView->rootObject();
  object = object->findChild<QObject *>("theNewSpatialiteLayerDialog");
  if (!object) {
    qDebug() << "new spatialite layer not founded";
    abort();
  }
  QObject::connect( object, SIGNAL( createDb() ), this, SLOT( createDb() ) );
  QObject::connect( object, SIGNAL( apply() ), this, SLOT( apply() ) );
  QObject::connect( object, SIGNAL( databaseChanged(QString) ), this, SLOT( setDatabase(QString) ) );
  QObject::connect( object, SIGNAL( layerNameChanged(QString) ), this, SLOT( setLayerName(QString) ) );
  QObject::connect( object, SIGNAL( typeChanged(QString) ), this, SLOT( setType(QString) ) );
  QObject::connect( object, SIGNAL( primaryKeyChanged(bool) ), this, SLOT( setPrimaryKey(bool) ) );
  QObject::connect( object, SIGNAL( geometryColumnChanged(QString) ), this, SLOT( setGeometryColumn(QString) ) );
  QObject::connect( object, SIGNAL( attributesChanged(QVariant) ), this, SLOT( setAttributes(QVariant) ) );
}

QgsNewSpatialiteLayerDialog::~QgsNewSpatialiteLayerDialog()
{
}

void QgsNewSpatialiteLayerDialog::updateConnections()
{
  QSettings settings;

  // Populate the database list from the stored connections
  QStringList connections;
  settings.beginGroup( "/SpatiaLite/connections" );
  QStringList keys = settings.childGroups();
  QStringList::Iterator it = keys.begin();
  while ( it != keys.end() )
  {
    // retrieving the SQLite DB name and full path
    connections << settings.value( *it + "/sqlitepath", "###unknown###" ).toString();
    ++it;
  }
  settings.endGroup();

  // Export the connections
  mView->rootContext()->setContextProperty( "spatialitedatabases", QVariant::fromValue(connections) );
}

void QgsNewSpatialiteLayerDialog::setDatabase(QString database)
{
  mDatabase = database;
}

void QgsNewSpatialiteLayerDialog::setLayerName(QString layerName)
{
  mLayerName = layerName;
}

void QgsNewSpatialiteLayerDialog::setType(QString type)
{
  mType = type;
}

void QgsNewSpatialiteLayerDialog::setPrimaryKey(bool primaryKey)
{
  mPrimaryKey = primaryKey;
}

void QgsNewSpatialiteLayerDialog::setGeometryColumn(QString geometryColumn)
{
  mGeometryColumn = geometryColumn;
}

void QgsNewSpatialiteLayerDialog::setAttributes(QVariant attributes)
{
  mAttributes = attributes.toList();
}

bool QgsNewSpatialiteLayerDialog::createDb()
{
  QString dbPath = QFileDialog::getSaveFileName( 0, tr( "New SpatiaLite Database File" ),
                     ".",
                     tr( "SpatiaLite" ) + " (*.sqlite *.db)" );

  if ( dbPath.isEmpty() )
    return false;

  if ( !dbPath.toLower().endsWith( ".sqlite" ) && !dbPath.toLower().endsWith( ".db" ) )
  {
    dbPath += ".sqlite";
  }

  if ( dbPath.isEmpty() )
    return false;

  QFile newDb( dbPath );
  if ( !newDb.exists() )
  {
    QString errCause;
    bool res = false;

    QString spatialite_lib = QgsProviderRegistry::instance()->library( "spatialite" );
    QLibrary* myLib = new QLibrary( spatialite_lib );
    bool loaded = myLib->load();
    if ( loaded )
    {
      QgsDebugMsg( "spatialite provider loaded" );

      typedef bool ( *createDbProc )( const QString&, QString& );
      createDbProc createDbPtr = ( createDbProc ) cast_to_fptr( myLib->resolve( "createDb" ) );
      if ( createDbPtr )
      {
        res = createDbPtr( dbPath, errCause );
      }
      else
      {
        errCause = "Resolving createDb(...) failed";
      }
    }
    delete myLib;

    if ( !res )
    {
      QMessageBox::warning( 0, tr( "SpatiaLite Database" ), errCause );
      //pbnFindSRID->setEnabled( false );
    }
  }

  QFileInfo fi( newDb );
  if ( !fi.exists() )
  {
    //pbnFindSRID->setEnabled( false );
    return false;
  }

  QString key = "/SpatiaLite/connections/" + fi.fileName() + "/sqlitepath";

  QSettings settings;
  if ( !settings.contains( key ) )
  {
    settings.setValue( "/SpatiaLite/connections/selected", fi.fileName() + tr( "@" ) + fi.canonicalFilePath() );
    settings.setValue( key, fi.canonicalFilePath() );

    QMessageBox::information( 0, tr( "SpatiaLite Database" ), tr( "Registered new database!" ) );
  }

  updateConnections();

  return true;
}

void QgsNewSpatialiteLayerDialog::apply()
{
  // Build up the sql statement for creating the table
  QString sql = QString( "create table %1(" ).arg( quotedIdentifier( mLayerName ) );
  QString delim = "";

  if ( mPrimaryKey )
  {
    sql += "pkuid integer primary key autoincrement,";
  }


  /*
   * The attributes comes from QML runtime in an QStringList where each element
   * is an attribute composed as "attributeName attributeType"
   */
  QStringList attribute;
  for (int i = 0; i < mAttributes.size(); i++)
  {
    attribute = mAttributes.at(i).toString().split(" "); // Separate attribute name y attribute type

    sql += delim + QString( "%1 %2" ).arg( quotedIdentifier( attribute.at(0) ) ).arg( attribute.at(1) );

    delim = ",";
  }

  // complete the create table statement
  sql += ")";

  QgsDebugMsg( QString( "Creating table in database %1" ).arg( mDatabase ) );

  QgsDebugMsg( sql ); // OK

  QString sqlAddGeom = QString( "select AddGeometryColumn(%1,%2,%3,%4,2)" )
                       .arg( quotedValue( mLayerName ) )
                       .arg( quotedValue( mGeometryColumn ) )
                       .arg( mCrsId )
                       .arg( quotedValue( mType ) );
  QgsDebugMsg( sqlAddGeom ); // OK

  QString sqlCreateIndex = QString( "select CreateSpatialIndex(%1,%2)" )
                           .arg( quotedValue( mLayerName ) )
                           .arg( quotedValue( mGeometryColumn ) );
  QgsDebugMsg( sqlCreateIndex ); // OK

  spatialite_init( 0 );

  sqlite3 *db;
  int rc = sqlite3_open( mDatabase.toUtf8(), &db );
  if ( rc != SQLITE_OK )
  {
    QMessageBox::warning( 0,
                          tr( "SpatiaLite Database" ),
                          tr( "Unable to open the database: %1" ).arg( mDatabase ) );
  }
  else
  {
    char * errmsg;
    rc = sqlite3_exec( db, sql.toUtf8(), NULL, NULL, &errmsg );
    if ( rc != SQLITE_OK )
    {
      QMessageBox::warning( 0,
                            tr( "Error Creating SpatiaLite Table" ),
                            tr( "Failed to create the SpatiaLite table %1. The database returned:\n%2" ).arg( mLayerName ).arg( errmsg ) );
      sqlite3_free( errmsg );
    }
    else
    {
      // create the geometry column and the spatial index
      rc = sqlite3_exec( db, sqlAddGeom.toUtf8(), NULL, NULL, &errmsg );
      if ( rc != SQLITE_OK )
      {
        QMessageBox::warning( 0,
                              tr( "Error Creating Geometry Column" ),
                              tr( "Failed to create the geometry column. The database returned:\n%1" ).arg( errmsg ) );
        sqlite3_free( errmsg );
      }
      else
      {
        // create the spatial index
        rc = sqlite3_exec( db, sqlCreateIndex.toUtf8(), NULL, NULL, &errmsg );
        if ( rc != SQLITE_OK )
        {
          QMessageBox::warning( 0,
                                tr( "Error Creating Spatial Index" ),
                                tr( "Failed to create the spatial index. The database returned:\n%1" ).arg( errmsg ) );
          sqlite3_free( errmsg );
        }

        QgsVectorLayer *layer = new QgsVectorLayer( QString( "dbname='%1' table='%2'(%3) sql=" )
            .arg( mDatabase )
            .arg( mLayerName )
            .arg( mGeometryColumn ), mLayerName, "spatialite" );
        if ( layer->isValid() )
        {
          // register this layer with the central layers registry
          QList<QgsMapLayer *> myList;
          myList << layer;
          QgsMapLayerRegistry::instance()->addMapLayers( myList );
        }
        else
        {
          QgsDebugMsg( mLayerName + " is an invalid layer - not loaded" );
          QMessageBox::critical( 0, tr( "Invalid Layer" ), tr( "%1 is an invalid layer and cannot be loaded." ).arg( mLayerName ) );
          delete layer;
        }
      }
    }
  }

}

QString QgsNewSpatialiteLayerDialog::quotedIdentifier( QString id )
{
  id.replace( "\"", "\"\"" );
  return id.prepend( "\"" ).append( "\"" );
}

QString QgsNewSpatialiteLayerDialog::quotedValue( QString value )
{
  value.replace( "'", "''" );
  return value.prepend( "'" ).append( "'" );
}
