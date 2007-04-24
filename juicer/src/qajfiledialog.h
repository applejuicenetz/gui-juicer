//
// C++ Interface: qajfiledialog
//
// Description: 
//
//
// Author: Holger Gerth <holger.gerth@informatik.tu-chemnitz.de>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef QAJFILEDIALOG_H
#define QAJFILEDIALOG_H

#include <QDialog>
#include <QTreeWidget>
#include <QTreeWidgetItem>

#include "qxmlmodule.h"
#include "qajfileitem.h"
#include "qajfilewidget.h"

/**
	@author Holger Gerth <holger.gerth@informatik.tu-chemnitz.de>
*/
class QAjFileDialog : public QDialog
{
public:
    QAjFileDialog( QXMLModule *xml, QWidget *parent = 0 );

    ~QAjFileDialog();

    void insertDirectory( QString dir, int type );
    void insertSeperator( QString seperator );

private:
    QAjFileWidget* fileSystem;
//     QXMLModule* xml;

};

#endif
