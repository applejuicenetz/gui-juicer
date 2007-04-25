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
    QAjFileItem* progressingItem = new QAjFileItem( this );
    progressingItem->setText(0, "progressing");
}

QAjFileItem::QAjFileItem( QAjFileItem *parent )
 : QAjItem( parent )
{

}


QAjFileItem::~QAjFileItem()
{
}

void QAjFileItem::insertDirectory( QString dir, QString path ) {

    QAjFileItem* newDirectory = new QAjFileItem( this );

    newDirectory->setText(0, dir);
    newDirectory->path = path;

    QAjFileItem* progressingItem = new QAjFileItem( newDirectory );
    progressingItem->setText(0, "progressing");

}




