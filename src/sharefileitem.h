//
// C++ Interface: sharefileitem
//
// Description: 
//
//
// Author: Holger Gerth <holger.gerth@informatik.tu-chemnitz.de>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef QAJSHAREFILEITEM_H
#define QAJSHAREFILEITEM_H

#include <QString>
#include <QObject>

#include "useritem.h"

#include "item.h"

class ShareItem;

/**
	@author Holger Gerth <holger.gerth@informatik.tu-chemnitz.de>
*/
class ShareFileItem : public Item
{
public:
    ShareFileItem( const QString& id, ShareItem *parent = 0 );
    virtual ~ShareFileItem();

    void update( const QString& hash, const QString& fileName, const QString& size, const QString& priority, const QString& filesystemSeperator );

    QString getLinkAJFSP();

//    bool operator<( const QTreeWidgetItem & other ) const;

    void updatePrio( int prio );

protected:
    static QString filesystemSeperator;
};

#endif
