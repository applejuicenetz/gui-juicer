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
    QAjShareFileItem( QString id, QTreeWidgetItem *parent = 0 );

    ~QAjShareFileItem();

    void update( QString hash, QString fileName, QString size, QString priority, QString filesystemSeperator  );


    QString getLinkAJFSP();

    bool operator<( const QTreeWidgetItem & other ) const;

    void setPath ( const QString& theValue )
    {
      path = theValue;
    }

    QString getPath() const
    {
      return path;
    }

    enum {FILENAME_COL, SIZE_COL, PRIORITY_COL, NUM_SHARED_FILES_OVERVIEW_COL};
protected:
    QString path;
};

#endif
