//
// C++ Interface: qajmodulebase
//
// Description: 
//
//
// Author: Matthias Reif <matthias.reif@informatik.tu-chemnitz.de>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef QAJMODULEBASE_H
#define QAJMODULEBASE_H

#include <QObject>
#include <QTreeWidget>
#include <QMessageBox>
#include <QFileDialog>
#include <QToolBar>
#include <QMainWindow>


class Juicer;
class QXMLModule;

/**
    @author Matthias Reif <matthias.reif@informatik.tu-chemnitz.de>
*/
class QAjModuleBase : public QObject {
Q_OBJECT
public:
    QAjModuleBase(Juicer* juicer, QTreeWidget* treeWidget, QToolBar* toolbar);
    ~QAjModuleBase();
    void sortItemsInitially(QString settingsGroup);
    void saveSortOrder(QString settingsGroup);
protected:
    Juicer* juicer;
    QTreeWidget* treeWidget;
    QToolBar* toolbar;

    QXMLModule* xml;
public slots:
    void adjustSizeOfColumns();
protected slots:
    void linkListSlot();
    virtual void selectionChanged() = 0;
};


class DockMain : public QMainWindow {
Q_OBJECT
public:
    DockMain(QWidget *parent = 0) : QMainWindow(parent) {}
    ~DockMain() {}
//     QSize sizeHint() const {
//         QSize s = QMainWindow::sizeHint();
//         printf("here:   %d   %d\n", s.width(), s.height());
//         s.setHeight(100);
//         return s;
//     }
};

#endif
