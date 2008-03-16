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

#include "qajlistwidget.h"
#include "qajshareitem.h"
#include "qajfiledialog.h"

/**
@author Matthias Reif
*/
class QAjShareWidget : public QAjListWidget
{
    Q_OBJECT
public:
    QAjShareWidget( QXMLModule* xml, QWidget *parent = 0 );

    ~QAjShareWidget();

    QAjFileDialog* fileSystem;

    void insertShare( QString path, QString shareMode, QString filesystemSeperator );
    void initToolBar();
    QAction* removeId;
    QAction* copyLinkButton, *prioOkButton;
    QAction* createLinkListButton;
    QCheckBox* prioCheck;
    QSpinBox* prioSpin;
    bool changed;

public slots:
    void commitSlot();

protected:
    void insertDirList( QTreeWidgetItem* parent, QStringList* dirList );
    QString filesystemSeparator;
    QAction *removeButton, *applyButton, *reloadButton, *insertButton;

private slots:
    void insertSlot();
    void removeSlot();
    void reloadSlot();

signals:
    void insert();
    void remove();
    void reload();
    void commit();
private:
    void initPopup();
};

#endif
