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

#ifndef QAJHANDLERDIALOG_H
#define QAJHANDLERDIALOG_H

#include <QDialog>
#include <QWindowsStyle>

#include "ui_qajhandlerdialogbase.h"
#include "qajoptionsdialog.h"

class QAjHandlerDialog : public QDialog, private Ui::HandlerDialog
{
  Q_OBJECT

public:
  QAjHandlerDialog(QWidget* parent = 0, Qt::WFlags fl = 0 );
  ~QAjHandlerDialog();

protected slots:
  virtual void reject();
  virtual void accept();

};

#endif

