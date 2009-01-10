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

/**
	@author Holger Gerth <holger.gerth@informatik.tu-chemnitz.de>
*/
class QAjShareFileItem : public QAjItem
{
public:
    QAjShareFileItem( const QString& id, QTreeWidgetItem *parent = 0 );
    virtual ~QAjShareFileItem();

    void update( const QString& hash, const QString& fileName, const QString& size, const QString& priority, const QString& filesystemSeperator  );

    QString getLinkAJFSP();

    bool operator<( const QTreeWidgetItem & other ) const;

    void setPath ( const QString& theValue )
    {
      path_ = theValue;
    }

    QString getPath() const
    {
      return path_;
    }

    void updatePrio( int prio );

    enum {FILENAME_COL, SIZE_COL, PRIORITY_COL, NUM_SHARED_FILES_OVERVIEW_COL};

protected:
    QString path_;
};

#endif
