//
// C++ Interface: 
//
// Description: 
//
//
// Author: Matthias Reif <matthias.reif@informatik.tu-chemnitz.de>, (C) 2009
//
// Copyright: See COPYING file that comes with this distribution
//
//

#ifndef QAJSHARESELECTIONDIALOG_H
#define QAJSHARESELECTIONDIALOG_H

#include <QDialog>
#include <QHash>
#include <QTreeWidgetItem>
#include "xmlmodule.h"
#include "ui_shareselectiondialogbase.h"

class ShareSelectionDialog : public QDialog, protected Ui::Dialog
{
  Q_OBJECT

public:
    ShareSelectionDialog(XMLModule* xml, QWidget* parent = 0, Qt::WFlags fl = 0 );
    ~ShareSelectionDialog();

    void insertDirectory(const QString& dir, const QString& path, int type );
    void insertSeperator(const QString& seperator );
    QString getPath(QTreeWidgetItem* item);
    QString selectedPath();
    enum {WORKSTATION=1, DRIVE=2, DISKDRIVE=3, FOLDER=4, DESKTOP=5};

protected:
    XMLModule* xml;
    QString filesystemSeperator;
    QTreeWidgetItem* expandedItem;
    QHash<int, QIcon> shareIcons;

protected slots:
    void updateSubDirectoriesSlot(QTreeWidgetItem* item);
};

#endif

