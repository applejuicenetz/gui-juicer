//
// C++ Implementation: qajfilewidget
//
// Description: 
//
//
// Author: Holger Gerth <holger.gerth@informatik.tu-chemnitz.de>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "qajfilewidget.h"

QAjFileWidget::QAjFileWidget( QXMLModule *xml, QWidget *parent )
 : QAjListWidget( xml, parent )
{
    rootPath = currentPath = NULL;

    QStringList headers;
    headers.append( tr("Choose directory") );
    setHeaderLabels( headers );

    connect( this, SIGNAL( itemExpanded ( QTreeWidgetItem* ) ),
             this, SLOT( updateSubDirectoriesSlot ( QTreeWidgetItem* ) ) );
}


QAjFileWidget::~QAjFileWidget()
{
}

void QAjFileWidget::insertDirectory( QString dir, QString path, FileSystem type ) {

    if ( type != DISKDRIVE ) {
        if ( rootPath == NULL ) {
            rootPath = new QAjFileItem( this );
            rootPath->setText(0, dir);
            currentPath = rootPath;

        }
        else {
            if ( path.isEmpty() ) {
                if ( rootPath == currentPath )
                    path = currentPath->text(0);
                else
                    path = currentPath->path + currentPath->text(0) + fileSystemSeperator;
            }
            currentPath->insertDirectory( dir, path );
        }
    }

}

void QAjFileWidget::insertSeperator( QString seperator ) {
    fileSystemSeperator = seperator;
}


void QAjFileWidget::updateSubDirectoriesSlot ( QTreeWidgetItem* item ) {

    currentPath = (QAjFileItem*)item;

    if ( currentPath->child(0)->text(0) == "progressing" ) {
        QString dir = currentPath->path+currentPath->text(0);
        fprintf(stderr, "get directory: %s\n", dir.toLatin1().data());
        xml->get( "directory", "&directory="+currentPath->path+currentPath->text(0) );
        emptyDirectory();
    }
}


void QAjFileWidget::emptyDirectory() {
    if (currentPath->child(0)->text(0) == "progressing") {
        currentPath->takeChild(0);
    }
}

QString QAjFileWidget::getPathFromSelectedItem() {
      QList<QTreeWidgetItem *> selection = selectedItems();
      QAjFileItem* firstSelectedItem = (QAjFileItem*)selection[0];

      return firstSelectedItem->path+firstSelectedItem->text(0);
}
