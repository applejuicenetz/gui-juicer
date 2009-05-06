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

#ifndef QAJSHARESELECTIONDIALOG_H
#define QAJSHARESELECTIONDIALOG_H

#include "dirselectionbase.h"

class ShareSelectionDialog : public DirSelectionBase
{
public:
    ShareSelectionDialog( XMLModule * const xml, QWidget* parent = 0, Qt::WFlags fl = 0 );
    ~ShareSelectionDialog();
};

#endif

