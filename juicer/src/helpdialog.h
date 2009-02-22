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

#ifndef HELPDIALOG_H
#define HELPDIALOG_H

#include <QDialog>

#include "ui_helpdialogbase.h"

class HelpDialog : public QDialog, private Ui::HelpDialog
{
  Q_OBJECT

public:
  HelpDialog(QWidget* parent = 0, Qt::WFlags fl = 0 );
  ~HelpDialog();
};

#endif

