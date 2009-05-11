/***************************************************************************
 *   Copyright (C) 2004 - 2009 by Matthias Reif, Holger Gerth              *
 *   matthias.reif@informatik.tu-chemnitz.de                               *
 *   holger.gerth@informatik.tu-chemnitz.de                                *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the          *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, see http://www.gnu.org/licenses/     *
 ***************************************************************************/

#ifndef DIRSELECTIONBASE_H
#define DIRSELECTIONBASE_H

/**
	@author Holger Gerth <holger.gerth@informatik.tu-chemnitz.de>
*/

#include <QDialog>
#include "ui_dirselectiondialogbase.h"
#include <QFileIconProvider>

class QString;
class XMLModule;

class DirSelectionDialog : public QDialog, public Ui::DirSelectionDialogBase
{
    Q_OBJECT

public:
    typedef enum { WORKSTATION  = 1,
           DRIVE        = 2,
           DISKDRIVE    = 3,
           FOLDER       = 4,
           DESKTOP      = 5
         } DirType;

    DirSelectionDialog(const QString& label, const QString& startDir, bool hideNew, XMLModule* const xml, QWidget* parent, Qt::WFlags fl = 0);
    ~DirSelectionDialog();

    QString getPath() const;
    typedef struct DirStruct{
        DirStruct(const QString name, const QString& path, const QString& type) {
            this->name = name;
            this->path = path;
            this->type = (DirType)type.toInt();
        }
        QString name, path;
        DirType type;
    } Dir;
    static QList<Dir> rootDir;
    int rootDirRequestId;

protected:
    XMLModule * const xml_;
    QString filesystemSeperator_;
    QTreeWidgetItem* expandedItem_;
    QHash<int, QIcon> shareIcons_;
    QList<int> xmlIds;

    void insertDirectory( const QString& dir, const QString& path, DirType type );
    void insertSeperator( const QString& seperator );
    void requestSubdirsFromDir(const QString& dir = QString::Null());
    QString getTreePath( QTreeWidgetItem * const item ) const;
    QString getSelectedPath() const;

protected slots:
    void updateSubDirectoriesSlot( QTreeWidgetItem * item );
    void requestFinished( int id, bool error );

    virtual void reject();
    virtual void accept();

private:
//     DirSelectionDialog( const DirSelectionDialog& rhs );
//     DirSelectionDialog& operator=( const DirSelectionDialog& rhs );

};

#endif
