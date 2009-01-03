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
#include "ui_qajpartlistdialogbase.h"
#include "qconvert.h"
#include "qajpartswidget.h"

class QAjPartListDialog : public QDialog, private Ui::partListDialog
{
  Q_OBJECT

public:
    QAjPartListDialog(QWidget* parent = 0, Qt::WFlags fl = 0 );
    ~QAjPartListDialog();
    void update( qulonglong size, QLinkedList<Part>& partList );
    void setFilename( const QString& filename );
private:
    QAjPartsWidget* paintWidget;
public slots:
    void paintedSlot();
};

#endif

