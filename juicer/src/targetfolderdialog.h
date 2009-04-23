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

#include <QDialog>
#include "ui_targetfolderdialogbase.h"

class TargetFolderDialog : public QDialog, public Ui::targetFolderDialogBase
{
  Q_OBJECT

public:
    TargetFolderDialog(QWidget* parent = 0, Qt::WFlags fl = 0 );
    ~TargetFolderDialog();

protected slots:
    virtual void reject();
    virtual void accept();
};

#endif

