/***************************************************************************
 *   Copyright (C) 2004 by Matthias Reif                                   *
 *   matthias.reif@informatik.tu-chemnitz.de                               *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#ifndef QAJSHAREWIDGET_H
#define QAJSHAREWIDGET_H

#include <QStringList>
#include <QFileDialog>
#include <QMessageBox>
#include <QCheckBox>
#include <QSpinBox>

#include "qajmodulebase.h"
#include "qajshareitem.h"
#include "qajsharefileitem.h"
#include "qajfiledialog.h"

/**
@author Matthias Reif
*/
class QAjShareModule : public QAjModuleBase
{
    Q_OBJECT
public:
    QAjShareModule(Juicer* juicer);

    ~QAjShareModule();

    QAjFileDialog* fileSystem;

    void insertShare( QString path, QString shareMode, QString filesystemSeperator );

    QAction* prioOkButton;
    QCheckBox* prioCheck;
    QSpinBox* prioSpin;
    bool changed;

public slots:
    void commitSlot();
    void selectionChanged();

protected:
    QAjShareItem* findShare(const QString& fileName);
    void insertDirList( QTreeWidgetItem* parent, QStringList* dirList );
    QString filesystemSeparator;

private slots:
    void insertSlot();
    void removeSlot();
    void reloadSlot();

public:
    void insertFile( QString id, QString hash, QString fileName, QString size, QString priority, QString filesystemSeperator );
    
    void updateSharedFilesList();
    void setPriority( int prio );

    void setTmpDir ( const QString& theValue ) { tmpDir = theValue; }
    QString getTmpDir() const { return tmpDir; }

    QAjShareFileItem* findFile( QString id );
    QAjShareFileItem* findFile( QString size, QString hash );
    
    QSize sizeHint() const {
        QSettings localSettings;
        localSettings.beginGroup("FilesDock");
        return localSettings.value("size", true).toSize();
    }

private:
    QHash<QString, QAjShareFileItem*> sharedFiles;
    QString tmpDir;

private slots:
    void linkSlot();


};

#endif
