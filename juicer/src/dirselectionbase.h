//
// C++ Interface: dirselectionbase
//
// Description: 
//
//
// Author: Holger Gerth <holger.gerth@informatik.tu-chemnitz.de>, (C) 2009
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef DIRSELECTIONBASE_H
#define DIRSELECTIONBASE_H

/**
	@author Holger Gerth <holger.gerth@informatik.tu-chemnitz.de>
*/

#include <QDialog>
#include "ui_targetfolderdialogbase.h"

class QString;
class XMLModule;

class DirSelectionBase : public QDialog, public Ui::targetFolderDialogBase
{
    Q_OBJECT

public:
    enum { WORKSTATION  = 1,
           DRIVE        = 2,
           DISKDRIVE    = 3,
           FOLDER       = 4,
           DESKTOP      = 5
         };

    DirSelectionBase( XMLModule * const xml, QWidget* parent, Qt::WFlags fl );

    QString getPath() const;

protected:
    XMLModule * const xml_;
    QString filesystemSeperator_;
    QTreeWidgetItem* expandedItem_;
    QHash<int, QIcon> shareIcons_;

    virtual ~DirSelectionBase();

    void insertDirectory( const QString& dir, const QString& path, int type );
    void insertSeperator( const QString& seperator );
    void requestSubdirsFromDir( const QString& dir = QString::Null() );
    QString getTreePath( QTreeWidgetItem * const item ) const;
    QString getSelectedPath() const;

protected slots:
    void updateSubDirectoriesSlot( QTreeWidgetItem * item );
    void requestFinished( int id, bool error );

    virtual void reject();
    virtual void accept();

private:
    DirSelectionBase( const DirSelectionBase& rhs );
    DirSelectionBase& operator=( const DirSelectionBase& rhs );

};

#endif
