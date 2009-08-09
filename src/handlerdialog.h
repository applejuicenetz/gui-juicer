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

#ifndef QAJHANDLERDIALOG_H
#define QAJHANDLERDIALOG_H

#include <QDialog>
#include <QWindowsStyle>

#include "ui_handlerdialogbase.h"
#include "optionsdialog.h"

class HandlerDialog : public QDialog, private Ui::HandlerDialog
{
  Q_OBJECT

    bool dontAskAgain_;

public:
    HandlerDialog(const QString& caption, const QString & text,
        QDialogButtonBox::StandardButtons buttons = QDialogButtonBox::Yes | QDialogButtonBox::No,
        QStyle::StandardPixmap icon = QStyle::SP_MessageBoxQuestion,
        QWidget* parent = 0, Qt::WFlags fl = 0);

    void setText(const QString& text);
    void setIcon(const QPixmap& pixmap);

    bool dontAskAgain() const;

    static const int DONT_ASK = -1;

protected slots:
    void reaskSlot(int);

public slots:
    int exec(const QString& saveString);
};

#endif

