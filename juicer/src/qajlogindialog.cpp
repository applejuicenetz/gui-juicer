//
// C++ Implementation: 
//
// Description: 
//
//
// Author: Matthias Reif <matthias.reif@informatik.tu-chemnitz.de>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//


#include "qajlogindialog.h"

QAjLoginDialog::QAjLoginDialog(QWidget* parent, Qt::WFlags fl): QDialog( parent, fl ), Ui::QAjLoginDialogBase()
{
	setupUi(this);
	connect(buttonBox, SIGNAL(clicked(QAbstractButton*)), this, SLOT(clicked(QAbstractButton*)));
}

QAjLoginDialog::~QAjLoginDialog()
{
}


/*!
    \fn QAjLoginDialog::clicked(QAbstractButton* button)
 */
void QAjLoginDialog::clicked(QAbstractButton* button)
{
	ignore = (buttonBox->standardButton(button) == QDialogButtonBox::Ignore);
}
