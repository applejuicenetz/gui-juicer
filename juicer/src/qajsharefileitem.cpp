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

QAjShareFileItem::QAjShareFileItem(const QString& id, QAjShareItem *parent) 
  : QAjItem((QTreeWidgetItem*)parent, id)
{
    parent->insertSharedFile(this);
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

    info.setFile(fileName);
    size_ = size.toDouble();

    this->setText( QAjShareItem::PATH_COL, info.fileName() );
    this->setText( QAjShareItem::SIZE_COL, QConvert::bytesExtra(size) );
    this->setText( QAjShareItem::PRIORITY_COL, priority );

    ((QAjShareItem*)QTreeWidgetItem::parent())->update();
}

void QAjShareFileItem::updatePrio( int prio )
{
    setText( QAjShareItem::PRIORITY_COL, QString::number(prio) );
}

bool QAjShareFileItem::operator<( const QTreeWidgetItem & other ) const
{
    int sortIndex = treeWidget()->header()->sortIndicatorSection();
    QAjShareFileItem* shareFileItem = (QAjShareFileItem*)&other;
    switch ( sortIndex )
    {
    case QAjShareItem::PATH_COL:
        return this->text( QAjShareItem::PATH_COL ) < other.text( QAjShareItem::PATH_COL );
    case QAjShareItem::SIZE_COL:
        return size_ < shareFileItem->getSize();
    default:
        return this->text( sortIndex ) < other.text( sortIndex );
    }
}

QString QAjShareFileItem::getLinkAJFSP() {
    QString ajfspLink;
    ajfspLink.append("ajfsp://file|");
    ajfspLink.append(info.fileName());
    ajfspLink.append("|");
    ajfspLink.append(hash_);
    ajfspLink.append("|");
    ajfspLink.append(QString::number( (unsigned long int)size_ ));
    ajfspLink.append("/");
    return ajfspLink;
}


