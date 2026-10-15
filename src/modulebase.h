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

#ifndef QAJMODULEBASE_H
#define QAJMODULEBASE_H

#include <QObject>
#include <QTreeWidget>
#include <QMessageBox>
#include <QFileDialog>
#include <QToolBar>
#include <QMainWindow>
#include <xmlmodule.h>

#include "convert.h"

class Juicer;

/**
    @author Matthias Reif <matthias.reif@informatik.tu-chemnitz.de>
*/
class ModuleBase : public QObject {
Q_OBJECT
public:
    ModuleBase(Juicer* juicer, QTreeWidget* treeWidget, QToolBar* toolbar, QWidget* tabWidget = NULL);
    ModuleBase(Juicer* juicer, QWidget* contentWidget, QToolBar* toolbar, QWidget* tabWidget = NULL);
    ~ModuleBase();
    void init(Juicer* juicer, QWidget* contentWidget, QToolBar* toolbar, QWidget* tabWidget);
    void sortItemsInitially(const QString& settingsGroup);
    void saveSortOrder(const QString& settingsGroup);
    void updateAlternatingRowColors();
    void process(QItemList& items, XMLModule::Type type, const QString& request, const QString& para = "");
    void processSelected(XMLModule::Type type, const QString& request, const QString& para = "");
    void processIdX(QItemList& items, const QString& request, const QString& para = "");
    void processSelectedIdX(const QString& request, const QString& para = "");
protected:
    Juicer* juicer;
    QTreeWidget* treeWidget;
    QWidget* contentWidget;
    QToolBar* toolbar;
    QString tabText;
    int tabIndex;
    XMLModule* xml;
public slots:
    void adjustSizeOfColumns();
protected slots:
    void linkListSlot();
    virtual void selectionChanged() = 0;
};

/// needed because of a Qt Designer issue
class DockMain : public QMainWindow {
Q_OBJECT
public:
    DockMain(QWidget *parent = 0) : QMainWindow(parent) {}
    ~DockMain() {}
};

#endif
