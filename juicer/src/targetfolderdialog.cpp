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

TargetFolderDialog::TargetFolderDialog( const QString& incomingDir,
                                        XMLModule * const xml,
                                        QWidget * parent,
                                        Qt::WFlags fl )
  : DirSelectionBase( xml, parent, fl )
{
    leNewFolder->setHidden( false );
    labelNewFolder->setHidden( false );

    textLabel->setText( tr("Select a folder below ") + incomingDir );

    requestSubdirsFromDir( incomingDir );
}

TargetFolderDialog::~TargetFolderDialog()
{
}

