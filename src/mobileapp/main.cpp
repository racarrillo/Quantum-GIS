/***************************************************************************
                            main.cpp  -  description
                              -------------------
              begin                : Wed Apr 04 10:48:28 CET 2012
              copyright            : (C) 2012 by Marco Bernasocchi
              email                : marco@bernawebdesign.ch
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include <QApplication>
#include <QDeclarativeView>
#include <QDeclarativeEngine>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QDeclarativeView view;
    view.setSource(QUrl("qrc:/qml/main.qml"));
    view.setResizeMode(QDeclarativeView::SizeRootObjectToView);

    QObject::connect((QObject*)view.engine(), SIGNAL(quit()), &app, SLOT(quit()));

    view.setGeometry(100,100, 800, 480);
    view.show();
    return app.exec();
}
