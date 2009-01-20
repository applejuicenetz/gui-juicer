//
// C++ Implementation: 
//
// Description: 
//
//
// Author: Matthias Reif <matthias.reif@informatik.tu-chemnitz.de>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//

#include "qajhandlerdialog.h"

QAjHandlerDialog::QAjHandlerDialog( const QString& caption,
                                    QWidget* parent /*= 0*/,
                                    Qt::WFlags fl /*= 0*/ )
  : QDialog( parent, fl ), Ui::HandlerDialog()
  , dontAskAgain_( false )
{
    setupUi( this );
    setWindowTitle( caption );
    QWindowsStyle qStyle;
    iconLabel->setPixmap(qStyle.standardIcon(QStyle::SP_MessageBoxQuestion).pixmap(32,32));

    pb3->hide();

    connect ( chkAskAgain, SIGNAL( stateChanged(int) ), this, SLOT( reaskSlot(int) ) );
    connect ( pb1, SIGNAL( clicked() ), this, SLOT( accept() ) );
    connect ( pb2, SIGNAL( clicked() ), this, SLOT( reject() ) );
}

QAjHandlerDialog::QAjHandlerDialog( const QString& caption,
                                    QWidget* parent /*= 0*/,
                                    const QString& button1 /*= ""*/,
                                    const QString& button2 /*= ""*/,
                                    const QString& button3 /*= ""*/,
                                    Qt::WFlags fl /*= 0*/ )
  : QDialog( parent, fl ), Ui::HandlerDialog()
  , dontAskAgain_( false )
{
    setupUi( this );
    setWindowTitle( caption );
    QWindowsStyle qStyle;
    iconLabel->setPixmap( qStyle.standardIcon(QStyle::SP_MessageBoxQuestion).pixmap(32,32) );

    if ( button1.isEmpty() ) pb1->hide();
    else pb1->setText( button1 );
    if ( button2.isEmpty() ) pb2->hide();
    else pb2->setText( button2 );
    if ( button3.isEmpty() ) pb3->hide();
    else pb3->setText( button3 );

    connect ( chkAskAgain, SIGNAL( stateChanged(int) ), this, SLOT( reaskSlot(int) ) );
    connect ( pb1, SIGNAL( clicked() ), this, SLOT( accept() ) );
    connect ( pb2, SIGNAL( clicked() ), this, SLOT( reject() ) );
}

void QAjHandlerDialog::setText( const QString& text )
{
    textLabel->setText( text );
}

void QAjHandlerDialog::setIcon( const QPixmap& pixmap )
{
    iconLabel->setPixmap( pixmap );
}

bool QAjHandlerDialog::dontAskAgain() const
{
    return dontAskAgain_;
}

void QAjHandlerDialog::reaskSlot( int state )
{
    dontAskAgain_ = ( state == Qt::Checked );
}

void QAjHandlerDialog::setTextButton1( const QString& text )
{
    pb1->setText( text );
}

void QAjHandlerDialog::setTextButton2( const QString& text )
{
    pb2->setText( text );
}

void QAjHandlerDialog::setTextButton3( const QString& text )
{
    pb3->setText( text );
}

QPushButton* QAjHandlerDialog::getPointerButton1()
{
    return pb1;
}

QPushButton* QAjHandlerDialog::getPointerButton2()
{
    return pb2;
}

QPushButton* QAjHandlerDialog::getPointerButton3()
{
    return pb3;
}


/*
QAjHandlerDialog::~QAjHandlerDialog()
{
}

void QAjHandlerDialog::reject() {
    if(againCheckBox->isChecked()) {
        QAjOptionsDialog::setSetting("handler", false);
    } else {
        QAjOptionsDialog::removeSetting("handler");
    }
    QDialog::reject();
}

void QAjHandlerDialog::accept() {
    if(againCheckBox->isChecked()) {
        QAjOptionsDialog::setSetting("handler", true);
    } else {
        QAjOptionsDialog::removeSetting("handler");
    }
    QDialog::accept();
}
*/


