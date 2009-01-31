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

#include "handlerdialog.h"

HandlerDialog::HandlerDialog( const QString& caption,
                                    const QString & text,
                                    QDialogButtonBox::StandardButtons buttons,
                                    QStyle::StandardPixmap icon,
                                    QWidget* parent /*= 0*/,
                                    Qt::WFlags fl /*= 0*/ )
  : QDialog( parent, fl ), Ui::HandlerDialog()
  , dontAskAgain_( false )
{
    setupUi( this );
    buttonBox->setStandardButtons(buttons);
    setWindowTitle( caption );
    QWindowsStyle qStyle;
    iconLabel->setPixmap(qStyle.standardIcon(icon).pixmap(32,32));
    setText(text);
    if(buttons == QDialogButtonBox::Ok) {
        chkAskAgain->setText("Don't show this message again");
    }
    connect ( chkAskAgain, SIGNAL( stateChanged(int) ), this, SLOT( reaskSlot(int) ) );
}


void HandlerDialog::setText( const QString& text )
{
    textLabel->setText( text );
}

void HandlerDialog::setIcon( const QPixmap& pixmap )
{
    iconLabel->setPixmap( pixmap );
}

bool HandlerDialog::dontAskAgain() const
{
    return dontAskAgain_;
}

void HandlerDialog::reaskSlot( int state )
{
    dontAskAgain_ = ( state == Qt::Checked );
}


/*!
    \fn HandlerDialog::exec(const QString& saveString)
    if the dialog was already answered permanently, return the saved answer,
    otherwise the dialog is shown and the answer will be saved
 */
int HandlerDialog::exec(const QString& saveString)
{
    if(OptionsDialog::hasSetting("accepted", saveString)) {
        return OptionsDialog::getGroupSetting("accepted", saveString).toInt();
    } else {
        int result = QDialog::exec();
        if(chkAskAgain->isChecked()) {
            OptionsDialog::setSetting("accepted", saveString, result);
        }
        return result;
    }
}
