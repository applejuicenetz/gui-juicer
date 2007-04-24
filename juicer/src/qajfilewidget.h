//
// C++ Interface: qajfilewidget
//
// Description: 
//
//
// Author: Holger Gerth <holger.gerth@informatik.tu-chemnitz.de>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef QAJFILEWIDGET_H
#define QAJFILEWIDGET_H

#include "qajlistwidget.h"
#include "qxmlmodule.h"
#include "qajfileitem.h"

/**
	@author Holger Gerth <holger.gerth@informatik.tu-chemnitz.de>
*/
class QAjFileWidget : public QAjListWidget
{
    Q_OBJECT
public:
    QAjFileWidget( QXMLModule *xml, QWidget *parent = 0 );

    ~QAjFileWidget();

    void insertDirectory( QString dir, int type );
    void insertSeperator( QString seperator );

private:
    QAjFileItem* rootPath, *currentPath;
    QString fileSystemSeperator;

private slots:
    void getSubDirectoriesSlot( QTreeWidgetItem* item );

};

#endif
