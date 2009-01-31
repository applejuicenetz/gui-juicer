//
// C++ Interface: 
//
// Description: 
//
//
// Author: Matthias Reif <matthias.reif@informatik.tu-chemnitz.de>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//

#ifndef QAJPARTLISTDIALOG_H
#define QAJPARTLISTDIALOG_H

#include <QDialog>
#include <QTimer>
#include "ui_partlistdialogbase.h"
#include "qconvert.h"
#include "partswidget.h"

class PartListDialog : public QDialog, private Ui::partListDialog
{
  Q_OBJECT

public:
    PartListDialog(QWidget* parent = 0, Qt::WFlags fl = 0 );
    ~PartListDialog();
    void update( qulonglong size, QLinkedList<PartsWidget::Part>& partList );
    void setFilename( const QString& filename );
public slots:
    void paintedSlot();
};

#endif

