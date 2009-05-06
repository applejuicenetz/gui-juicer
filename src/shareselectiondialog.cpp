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
#include "xmlmodule.h"

#include "shareselectiondialog.h"

ShareSelectionDialog::ShareSelectionDialog( XMLModule* xml, QWidget* parent, Qt::WFlags fl )
  : DirSelectionBase( xml, parent, fl )
  {
    textLabel->setText( tr("Select a directory you want to share:") );

    requestSubdirsFromDir();
}

ShareSelectionDialog::~ShareSelectionDialog()
{
}

