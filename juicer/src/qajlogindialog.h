//
// C++ Interface: 
//
// Description: 
//
//
// Author: Matthias Reif <matthias.reif@informatik.tu-chemnitz.de>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//

#ifndef QAJLOGINDIALOG_H
#define QAJLOGINDIALOG_H

#include <QDialog>
#include "ui_qajlogindialogbase.h"

class QAjLoginDialog : public QDialog, private Ui::QAjLoginDialogBase
{
  Q_OBJECT

public:
  QAjLoginDialog(QWidget* parent = 0, Qt::WFlags fl = 0 );
  ~QAjLoginDialog();

  QString getHost() { return hostEdit->text(); }
  int getPort() { return portSpinBox->value(); }
  QString getPassword() { return passwordEdit->text(); }
  void setHost( QString host ) { hostEdit->setText( host ); }
  void setPort( int port ) { portSpinBox->setValue( port ); }
  void setPassword( QString password ) { passwordEdit->setText( password ); }
  void setHeader( QString text ) { headerLabel->setText( text ); }
  bool ignore;
public slots:

protected:

protected slots:

public slots:
    void clicked(QAbstractButton* button);
};

#endif

