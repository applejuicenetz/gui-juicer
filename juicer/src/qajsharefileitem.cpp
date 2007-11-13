//
// C++ Implementation: qajsharefileitem
//
// Description: 
//
//
// Author: Holger Gerth <holger.gerth@informatik.tu-chemnitz.de>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "qajsharefileitem.h"

QAjShareFileItem::QAjShareFileItem( QString id, QAjListWidget *parent ) : QAjItem( parent, id )
{
    parentWidget = parent;
}


QAjShareFileItem::~QAjShareFileItem()
{
}


void QAjShareFileItem::update( QString hash, QString fileName, QString size, QString priority, QString filesystemSeperator ) {
    this->hash = hash;
    this->filename = fileName;

    QFileInfo f(fileName);

    this->path = f.absolutePath();
    this->size = size;

    if ( !path.endsWith( filesystemSeperator.data()[0] ) )
        path += filesystemSeperator;

    this->setText( FILENAME_SHARED_FILE_INDEX, f.fileName() );
    this->setText( SIZE_SHARED_FILE_INDEX, QConvert::bytesExtra(size) );
    this->setText( PRIORITY_SHARED_FILE_INDEX, priority );

    setHidden( true );
}


bool QAjShareFileItem::operator<( const QTreeWidgetItem & other ) const
{
    int sortIndex = treeWidget()->header()->sortIndicatorSection();
    QAjShareFileItem* shareFileItem = (QAjShareFileItem*)&other;
    switch ( sortIndex )
    {
    case FILENAME_DOWN_INDEX:
        return this->text( FILENAME_SHARED_FILE_INDEX ) < other.text( FILENAME_SHARED_FILE_INDEX );
    case SIZE_DOWN_INDEX:
        return this->size.toInt() < shareFileItem->getSize().toInt();
    default:
        return this->text( sortIndex ) < other.text( sortIndex );
    }
}

QString QAjShareFileItem::getLinkAJFSP() {
    QString ajfspLink;

    ajfspLink.append("ajfsp://file|");
    ajfspLink.append(this->text(FILENAME_SHARED_FILE_INDEX));
    ajfspLink.append("|");
    ajfspLink.append(this->hash);
    ajfspLink.append("|");
    ajfspLink.append(this->size);
    ajfspLink.append("/");

    return ajfspLink;
}


