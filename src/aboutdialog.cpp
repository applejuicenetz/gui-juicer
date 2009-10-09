//
// C++ Implementation: 
//
// Description: 
//
//
// Author: Matthias Reif <matthias.reif@informatik.tu-chemnitz.de>, (C) 2009
//
// Copyright: See COPYING file that comes with this distribution
//
//


#include "aboutdialog.h"

AboutDialog::AboutDialog(QWidget* parent, Qt::WFlags fl) : QDialog( parent, fl ), Ui::AboutDialog() {
    setupUi(this);
    versionLabel->setText(qApp->applicationVersion());
}

AboutDialog::~AboutDialog() {
}
