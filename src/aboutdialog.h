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

#ifndef ABOUTDIALOG_H
#define ABOUTDIALOG_H

#include <QDialog>
#include "ui_aboutdialogbase.h"

class AboutDialog : public QDialog, private Ui::AboutDialog {
  Q_OBJECT

public:
  AboutDialog(QWidget* parent = 0, Qt::WFlags fl = 0 );
  ~AboutDialog();
};

#endif

