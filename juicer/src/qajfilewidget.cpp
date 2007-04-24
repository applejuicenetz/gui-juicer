//
// C++ Implementation: qajfilewidget
//
// Description: 
//
//
// Author: Holger Gerth <holger.gerth@informatik.tu-chemnitz.de>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "qajfilewidget.h"

QAjFileWidget::QAjFileWidget( QXMLModule *xml, QWidget *parent )
 : QAjListWidget( xml, parent )
{
    rootPath = currentPath = NULL;

    QStringList headers;
    headers.append( tr("Choose directory") );
    setHeaderLabels( headers );

    connect( this, SIGNAL( itemDoubleClicked ( QTreeWidgetItem*, int ) ),
             this, SLOT( getSubDirectoriesSlot ( QTreeWidgetItem* ) ) );
}


QAjFileWidget::~QAjFileWidget()
{
}

void QAjFileWidget::insertDirectory( QString dir, int type ) {
    if ( rootPath == NULL ) {
        rootPath = new QAjFileItem( this );
        rootPath->setText(0, dir);
        rootPath->setFlags( Qt::ItemIsEnabled );

        currentPath = rootPath;
    }
    else {
        currentPath->insertDirectory( dir );
    }

    expandItem( currentPath );
}

void QAjFileWidget::insertSeperator( QString seperator ) {
    fileSystemSeperator = seperator;
    if ( (rootPath != NULL) && (rootPath != currentPath) ) {
        currentPath->path +=seperator;
    }
}

void QAjFileWidget::getSubDirectoriesSlot( QTreeWidgetItem* item ) {
    currentPath = (QAjFileItem*)item;

    xml->get( "directory", "&directory="+currentPath->path+fileSystemSeperator+currentPath->text(0) );
}


