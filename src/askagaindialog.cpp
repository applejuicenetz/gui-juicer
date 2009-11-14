/***************************************************************************
 *   Copyright (C) 2004 - 2009 by Matthias Reif, Holger Gerth              *
 *   matthias.reif@informatik.tu-chemnitz.de                               *
 *   holger.gerth@informatik.tu-chemnitz.de                                *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the          *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, see http://www.gnu.org/licenses/     *
 ***************************************************************************/

#include "askagaindialog.h"

AskAgainDialog::AskAgainDialog( const QString& caption,
                                    const QString & text,
                                    QDialogButtonBox::StandardButtons buttons,
                                    QStyle::StandardPixmap icon,
                                    QWidget* parent /*= 0*/,
                                    Qt::WFlags fl /*= 0*/ )
  : QDialog( parent, fl ), Ui::AskAgainDialog()
  , dontAskAgain_( false )
{
    setupUi( this );
    buttonBox->setStandardButtons(buttons);
    setWindowTitle( caption );
    QWindowsStyle qStyle;
    iconLabel->setPixmap(qStyle.standardIcon(icon).pixmap(32,32));
    setText(text);
    if(buttons == QDialogButtonBox::Ok) {
        chkAskAgain->setText(tr("Don't show this message again."));
    }
    connect ( chkAskAgain, SIGNAL( stateChanged(int) ), this, SLOT( reaskSlot(int) ) );
}


void AskAgainDialog::setText( const QString& text )
{
    textLabel->setText( text );
}

void AskAgainDialog::setIcon( const QPixmap& pixmap )
{
    iconLabel->setPixmap( pixmap );
}

bool AskAgainDialog::dontAskAgain() const
{
    return dontAskAgain_;
}

void AskAgainDialog::reaskSlot( int state )
{
    dontAskAgain_ = ( state == Qt::Checked );
}


/*!
    \fn AskAgainDialog::exec(const QString& saveString)
    if the dialog was already answered permanently, return the saved answer,
    otherwise the dialog is shown and the answer will be saved
 */
int AskAgainDialog::exec(const QString& saveString)
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
