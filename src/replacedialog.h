/***************************************************************************
 *   Copyright (C) 2004 - 2009 by Matthias Reif, Holger Gerth              *
 *   matthias.reif@informatik.tu-chemnitz.de                               *
 *   holger.gerth@informatik.tu-chemnitz.de                                *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the          *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, see http://www.gnu.org/licenses/     *
 ***************************************************************************/

#ifndef REPLACEDIALOG_H
#define REPLACEDIALOG_H

#include <QDialog>
#include <QRegExp>
#include <QPushButton>
#include <QMessageBox>
#include <QCompleter>

#include "ui_replacedialogbase.h"
#include "convert.h"
#include "optionsdialog.h"

class ReplaceDialog : public QDialog, public Ui::ReplaceDialog {
  Q_OBJECT

public:
    ReplaceDialog(QItemList& items, int itemCol, QWidget* parent = 0, Qt::WFlags fl = 0 );
    ~ReplaceDialog();
    QString replace(const QString& src);

protected:
    QRegExp::PatternSyntax getPatternSyntax();
    enum {ORIGINAL, ARROW, PREVIEW};
    QString errorMsg;
    QRegExp re;
    QCompleter *beforeCompleter, *afterCompleter;
    static const int HIST_SIZE = 100;
protected slots:
    virtual void accept();
    void createPreview();
};

#endif

