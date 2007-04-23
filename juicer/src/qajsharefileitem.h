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

#include "qajlistwidget.h"
#include "qajitem.h"

#include "types.h"

/**
	@author Holger Gerth <holger.gerth@informatik.tu-chemnitz.de>
*/
class QAjShareFileItem : public QAjItem
{
public:
    QAjShareFileItem( QString id, QAjListWidget *parent = 0 );

    ~QAjShareFileItem();

    void update( QString hash, QString fileName, QString size );


    QString getLinkAJFSP();

    QString getSize() const
    {
      return size;
    }

    bool operator<( const QTreeWidgetItem & other ) const;

protected:
    QString hash, size;

    QAjListWidget *parentWidget;

};

#endif
