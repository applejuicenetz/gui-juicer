//
// C++ Implementation: sharefileitem
//
// Description: 
//
//
// Author: Holger Gerth <holger.gerth@informatik.tu-chemnitz.de>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "sharefileitem.h"
#include "shareitem.h"

QString ShareFileItem::filesystemSeperator = "";

ShareFileItem::ShareFileItem(const QString& id, ShareItem *parent) 
  : Item( /*dynamic_cast<QTreeWidgetItem*>*/parent, id)
{
}


ShareFileItem::~ShareFileItem()
{
}


void ShareFileItem::update( const QString& hash,
                               const QString& fileName,
                               const QString& size,
                               const QString& priority,
                               const QString& filesystemSeperator )
{
    setText( ShareItem::PATH_COL, fileName.split(filesystemSeperator).last() );
    setText( ShareItem::SIZE_COL, Convert::bytesExtra(size) );
    setText( ShareItem::PRIORITY_COL, priority );

    if ( hash_.isEmpty() && !hash.isEmpty()) {
        hash_ = hash;
    }
    if ( filename_.isEmpty() && !fileName.isEmpty() ) {
        filename_ = fileName;
        setFileIcon(ShareItem::PATH_COL);
    }
    if ( this->filesystemSeperator.isEmpty() && !filesystemSeperator.isEmpty()) {
        this->filesystemSeperator = filesystemSeperator;
    }

    size_ = size.toDouble();


    ShareItem* parentItem = dynamic_cast<ShareItem*>(QTreeWidgetItem::parent());
    if ( parentItem != NULL ) parentItem->update();
}

void ShareFileItem::updatePrio( int prio )
{
    setText( ShareItem::PRIORITY_COL, QString::number(prio) );
}

/*
bool ShareFileItem::operator<( const QTreeWidgetItem & other ) const
{
    int sortIndex = treeWidget()->header()->sortIndicatorSection();
    ShareFileItem* shareFileItem = dynamic_cast<ShareFileItem*>(&other);
    if ( shareFileItem == NULL ) return false;
    switch ( sortIndex )
    {
    case ShareItem::PATH_COL:
        return text( ShareItem::PATH_COL ) < other.text( ShareItem::PATH_COL );
    case ShareItem::SIZE_COL:
        return size_ < shareFileItem->getSize();
    default:
        return text( sortIndex ) < other.text( sortIndex );
    }
}
*/

QString ShareFileItem::getLinkAJFSP() {
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

