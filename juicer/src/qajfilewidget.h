//
// C++ Interface: qajfilewidget
//
// Description: 
//
//
// Author: Holger Gerth <holger.gerth@informatik.tu-chemnitz.de>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef QAJFILEWIDGET_H
#define QAJFILEWIDGET_H

#include "qajlistwidget.h"
#include "qxmlmodule.h"
#include "qajfileitem.h"

/**
	@author Holger Gerth <holger.gerth@informatik.tu-chemnitz.de>
*/
class QAjFileWidget : public QAjListWidget
{
    Q_OBJECT
public:
    QAjFileWidget( QXMLModule *xml, QWidget *parent = 0 );

    ~QAjFileWidget();

    void insertDirectory( QString dir, QString path, int type );
    void insertSeperator( QString seperator );
    void emptyDirectory();
    QString getPathFromSelectedItem();

private:
    QAjFileItem* rootPath, *currentPath;
    QString fileSystemSeperator;

    void getSubDirectories( QAjFileItem* item );


private slots:
    void updateSubDirectoriesSlot ( QTreeWidgetItem* item );

};

#endif