//
// C++ Interface: qajsharefileitem
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

#include "qajuseritem.h"

#include "qajitem.h"

class QAjShareItem;

/**
	@author Holger Gerth <holger.gerth@informatik.tu-chemnitz.de>
*/
class QAjShareFileItem : public QAjItem
{
public:
    QAjShareFileItem( const QString& id, QAjShareItem *parent = 0 );
    virtual ~QAjShareFileItem();

    void update( const QString& hash, const QString& fileName, const QString& size, const QString& priority, const QString& filesystemSeperator  );

    QString getLinkAJFSP();

//    bool operator<( const QTreeWidgetItem & other ) const;

    void updatePrio( int prio );

protected:
    QFileInfo info;
};

#endif
