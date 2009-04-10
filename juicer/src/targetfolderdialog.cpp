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


#include "targetfolderdialog.h"

TargetFolderDialog::TargetFolderDialog(QWidget* parent, Qt::WFlags fl)
: QDialog( parent, fl ), Ui::targetFolderDialogBase() {
    setupUi(this);
    
}

TargetFolderDialog::~TargetFolderDialog() {
}

void TargetFolderDialog::reject() {
    QDialog::reject();
}

void TargetFolderDialog::accept() {
    QDialog::accept();
}



