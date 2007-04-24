//
// C++ Interface: qajfileitem
//
// Description: 
//
//
// Author: Holger Gerth <holger.gerth@informatik.tu-chemnitz.de>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef QAJFILEITEM_H
#define QAJFILEITEM_H

#include <QList>

#include "qajitem.h"



/**
	@author Holger Gerth <holger.gerth@informatik.tu-chemnitz.de>
*/
class QAjFileItem : public QAjItem
{
public:
    QAjFileItem( QTreeWidget *parent );
    QAjFileItem( QAjFileItem *parent );

    ~QAjFileItem();

    QString path, filesystemSeperator;

    void insertDirectory( QString dir );

private:
    QList<QAjFileItem*> nextFolder;

private slots:
//     void getSubDirectories();

};

#endif
