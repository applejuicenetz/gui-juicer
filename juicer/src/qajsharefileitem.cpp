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
#include "qajshareitem.h"

QString QAjShareFileItem::filesystemSeperator = "";

QAjShareFileItem::QAjShareFileItem(const QString& id, QAjShareItem *parent) 
  : QAjItem( /*dynamic_cast<QTreeWidgetItem*>*/parent, id)
{
}


QAjShareFileItem::~QAjShareFileItem()
{
}


void QAjShareFileItem::update( const QString& hash,
                               const QString& fileName,
                               const QString& size,
                               const QString& priority,
                               const QString& filesystemSeperator )
{
    if ( hash_.isEmpty() ) {
        hash_ = hash;
    }
    if ( filename_.isEmpty() ) {
        filename_ = fileName;
    }
    if ( this->filesystemSeperator.isEmpty() ) {
        this->filesystemSeperator = filesystemSeperator;
    }

    size_ = size.toDouble();

    setText( QAjShareItem::PATH_COL, filename_.split(filesystemSeperator).last() );
    setText( QAjShareItem::SIZE_COL, QConvert::bytesExtra(size) );
    setText( QAjShareItem::PRIORITY_COL, priority );

    QAjShareItem* parentItem = dynamic_cast<QAjShareItem*>(QTreeWidgetItem::parent());
    if ( parentItem != NULL ) parentItem->update();
}

void QAjShareFileItem::updatePrio( int prio )
{
    setText( QAjShareItem::PRIORITY_COL, QString::number(prio) );
}

/*
bool QAjShareFileItem::operator<( const QTreeWidgetItem & other ) const
{
    int sortIndex = treeWidget()->header()->sortIndicatorSection();
    QAjShareFileItem* shareFileItem = dynamic_cast<QAjShareFileItem*>(&other);
    if ( shareFileItem == NULL ) return false;
    switch ( sortIndex )
    {
    case QAjShareItem::PATH_COL:
        return text( QAjShareItem::PATH_COL ) < other.text( QAjShareItem::PATH_COL );
    case QAjShareItem::SIZE_COL:
        return size_ < shareFileItem->getSize();
    default:
        return text( sortIndex ) < other.text( sortIndex );
    }
}
*/

QString QAjShareFileItem::getLinkAJFSP() {
    QString ajfspLink;
    ajfspLink.append("ajfsp://file|");
    ajfspLink.append(filename_.split(filesystemSeperator).last());
    ajfspLink.append("|");
    ajfspLink.append(hash_);
    ajfspLink.append("|");
    ajfspLink.append(QString::number( (unsigned long int)size_ ));
    ajfspLink.append("/");
    return ajfspLink;
}

