//
// C++ Interface: 
//
// Description: 
//
//
// Author: Matthias Reif <matthias.reif@informatik.tu-chemnitz.de>, (C) 2009
//
// Copyright: See COPYING file that comes with this distribution
//
//

#ifndef TARGETFOLDERDIALOG_H
#define TARGETFOLDERDIALOG_H

#include "dirselectionbase.h"

class TargetFolderDialog : public DirSelectionBase
{
public:
    TargetFolderDialog( const QString& incomingDir,
                        XMLModule * const xml,
                        QWidget* parent = 0,
                        Qt::WFlags fl = 0 );
    ~TargetFolderDialog();
};

#endif

