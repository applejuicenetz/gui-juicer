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

QAjShareFileItem::QAjShareFileItem(const QString& id, QTreeWidgetItem *parent) 
  : QAjItem(parent, id)
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

    QFileInfo f(fileName);

    path_ = f.absolutePath();
    size_ = size.toDouble();

    if ( !path_.endsWith( filesystemSeperator.data()[0] ) )
        path_ += filesystemSeperator;

    this->setText( QAjShareFileItem::FILENAME_COL, f.fileName() );
    this->setText( QAjShareFileItem::SIZE_COL, QConvert::bytesExtra(size) );
    this->setText( QAjShareFileItem::PRIORITY_COL, priority );
}

void QAjShareFileItem::updatePrio( int prio )
{
    setText( QAjShareFileItem::PRIORITY_COL, QString::number(prio) );
}

bool QAjShareFileItem::operator<( const QTreeWidgetItem & other ) const
{
    int sortIndex = treeWidget()->header()->sortIndicatorSection();
    QAjShareFileItem* shareFileItem = (QAjShareFileItem*)&other;
    switch ( sortIndex )
    {
    case FILENAME_COL:
        return this->text( FILENAME_COL ) < other.text( FILENAME_COL );
    case SIZE_COL:
        return size_ < shareFileItem->getSize();
    default:
        return this->text( sortIndex ) < other.text( sortIndex );
    }
}

QString QAjShareFileItem::getLinkAJFSP() 
{
    QString ajfspLink;

    ajfspLink.append("ajfsp://file|");
    ajfspLink.append(text(FILENAME_COL));
    ajfspLink.append("|");
    ajfspLink.append(hash_);
    ajfspLink.append("|");
    ajfspLink.append(QString::number( (int)size_ ));
    ajfspLink.append("/");

    return ajfspLink;
}


