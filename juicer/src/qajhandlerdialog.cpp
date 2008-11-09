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

QAjHandlerDialog::QAjHandlerDialog(QWidget* parent, Qt::WFlags fl)
: QDialog( parent, fl ), Ui::HandlerDialog()
{
	setupUi(this);
    QWindowsStyle qStyle;
    iconLabel->setPixmap(qStyle.standardIcon(QStyle::SP_MessageBoxQuestion).pixmap(32,32));
}

QAjHandlerDialog::~QAjHandlerDialog()
{
}

/*$SPECIALIZATION$*/
void QAjHandlerDialog::reject()
{
    if(againCheckBox->isChecked()) {
        QAjOptionsDialog::setSetting("handler", false);
    } else {
        QAjOptionsDialog::removeSetting("handler");
    }
    QDialog::reject();
}

void QAjHandlerDialog::accept()
{
    if(againCheckBox->isChecked()) {
        QAjOptionsDialog::setSetting("handler", true);
    } else {
        QAjOptionsDialog::removeSetting("handler");
    }
    QDialog::accept();
}



