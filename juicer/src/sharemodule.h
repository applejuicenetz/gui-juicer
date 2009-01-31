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
#include <QPushButton>

#include "modulebase.h"
#include "shareitem.h"
#include "sharefileitem.h"
#include "shareselectiondialog.h"

/**
@author Matthias Reif
*/
class ShareModule : public ModuleBase
{
    Q_OBJECT

    QHash<QString, ShareFileItem*> sharedFiles;
    QString tmpDir;

public:
    QLabel*        prioLabel;
    QSpinBox*      prioSpin;
    QPushButton*   prioButton;

    ShareModule(Juicer* juicer);
    ~ShareModule();

    ShareFileItem* findFile( const QString& id );
    ShareFileItem* findFile( const QString& size, const QString& hash );

    void insertShare( const QString& path, const QString& shareMode);
    void insertFile( const QString& id, const QString& hash, const QString& fileName, const QString& size, const QString& priority, const QString& filesystemSeperator );
    void updateSharedFilesList();

    void setTmpDir ( const QString& theValue ) { tmpDir = theValue; }
    QString getTmpDir() const { return tmpDir; }
    bool isChanged() const { return changed_; }

    ShareSelectionDialog* shareSelectionDialog;
    
    void reset()
    {
        sharedFiles.clear();
    }

public slots:
    void commitSlot();
    void selectionChanged();

protected:
    ShareItem* findShare(const QString& fileName);
    QString filesystemSeparator;
    int prio_;
    bool changed_;

private slots:
    void insertSlot();
    void removeSlot();
    void reloadSlot();
    void linkSlot();
    void setTmpPriority(int prio);
    void setPriority();
};

#endif
