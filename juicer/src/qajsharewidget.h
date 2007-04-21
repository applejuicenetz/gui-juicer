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
#include "qajlistwidget.h"
#include "qajshareitem.h"

/**
@author Matthias Reif
*/
class QAjShareWidget : public QAjListWidget
{
    Q_OBJECT
public:
    QAjShareWidget( QString filesystemSeparator, QXMLModule* xml, QWidget *parent = 0 );

    ~QAjShareWidget();

    void insertShare( QString path, QString shareMode );
    void initToolBar();
    QAction* removeId;
    bool changed;

public slots:
    void commitSlot();

protected:
    void insertDirList( QTreeWidgetItem* parent, QStringList* dirList );
    QString filesystemSeparator;
    QAction *removeButton, *applyButton, *reloadButton;

private slots:
    void selectionChanged( bool oneSelected);
    void insertSlot();
    void removeSlot();
    void reloadSlot();

signals:
    void insert();
    void remove();
    void reload();
    void commit();
};

#endif
