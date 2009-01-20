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

    bool dontAskAgain_;

public:
    QAjHandlerDialog( const QString& caption, QWidget* parent = 0, Qt::WFlags fl = 0 );
    QAjHandlerDialog( const QString& caption, QWidget* parent = 0,
                      const QString& button1 = "", const QString& button2 = "", const QString& button3 = "",
                      Qt::WFlags fl = 0 );
//  ~QAjHandlerDialog();

    void setText( const QString& text );
    void setIcon( const QPixmap& pixmap );

    bool dontAskAgain() const;

    void setTextButton1( const QString& text );
    void setTextButton2( const QString& text );
    void setTextButton3( const QString& text );

    QPushButton* getPointerButton1();
    QPushButton* getPointerButton2();
    QPushButton* getPointerButton3();

protected slots:
//  virtual void reject();
//  virtual void accept();

    void reaskSlot( int );

};

#endif

