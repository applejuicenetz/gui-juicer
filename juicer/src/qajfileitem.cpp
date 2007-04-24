//
// C++ Implementation: qajfileitem
//
// Description: 
//
//
// Author: Holger Gerth <holger.gerth@informatik.tu-chemnitz.de>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "qajfileitem.h"

QAjFileItem::QAjFileItem( QTreeWidget *parent )
 : QAjItem( parent )
{
//     QObject::connect( this, SIGNAL( itemDoubleClicked ( QTreeWidgetItem*, int ) ), this, SLOT( getSubDirectories() ) );

}

QAjFileItem::QAjFileItem( QAjFileItem *parent )
 : QAjItem( parent )
{
//     QObject::connect( this, SIGNAL( itemDoubleClicked ( QTreeWidgetItem*, int ) ), this, SLOT( getSubDirectories() ) );

}


QAjFileItem::~QAjFileItem()
{
}

void QAjFileItem::insertDirectory( QString dir ) {
    QAjFileItem* newDirectory = new QAjFileItem( this );

    newDirectory->setText(0, dir);
    newDirectory->path = this->text(0);

    this->nextFolder.append( newDirectory );
    this->addChild( newDirectory );
}


// void QAjFileItem::getSubDirectories() {
// 
// }

