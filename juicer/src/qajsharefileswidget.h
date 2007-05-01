//
// C++ Interface: qajsharefileswidget
//
// Description: 
//
//
// Author: Holger Gerth <holger.gerth@informatik.tu-chemnitz.de>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef QAJSHAREFILESWIDGET_H
#define QAJSHAREFILESWIDGET_H

#include <QWidget>
#include <QTreeWidget>
#include <QPainter>
#include <QMenu>
#include <QHash>
#include <QList>
#include <QIcon>
#include <QCheckBox>
#include <QInputDialog>
#include <QApplication>
#include <QClipboard>

#include "qajlistwidget.h"
#include "qxmlmodule.h"

#include "qajsharefileitem.h"

#include "types.h"

/**
	@author Holger Gerth <holger.gerth@informatik.tu-chemnitz.de>
*/
class QAjShareFilesWidget : public QAjListWidget
{
Q_OBJECT

public:
    QAjShareFilesWidget( QXMLModule* xml, QWidget *parent = 0 );

    ~QAjShareFilesWidget();

    void insertFile( QString id, QString hash, QString fileName, QString size, QString priority, QString filesystemSeperator );
    QAjShareFileItem* findFile( QString id );
    void updateSharedFilesList();
    void updateVisibleFiles( QString path );
    void setPriority( int prio );

    void setTmpDir ( const QString& theValue )
    {
      tmpDir = theValue;
    }

    QString getTmpDir() const
    {
      return tmpDir;
    }

private:
    QHash<QString, QAjShareFileItem*> sharedFiles;
    QString tmpDir;

private slots:
    void linkSlot();

};

#endif
