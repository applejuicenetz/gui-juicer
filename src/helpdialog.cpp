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


#include "helpdialog.h"

HelpDialog::HelpDialog(QWidget* parent, Qt::WFlags fl) : QDialog( parent, fl ), Ui::HelpDialog() {
    setupUi(this);
}

HelpDialog::~HelpDialog() {

}
