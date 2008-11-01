/***************************************************************************
 *   Copyright (C) 2005 by Matthias Reif   *
 *   matthias.reif@informatik.tu-chemnitz.de   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#ifndef QAJOPTIONSDIALOG_H
#define QAJOPTIONSDIALOG_H

#include <QDialog>
#include <QFileDialog>
#include <QSettings>
#include <QFtp>
#include <QFontDialog>
#include "ui_qajoptionsdialogbase.h"

#include "qajiconwidget.h"

#include "types.h"


class QAjOptionsDialog : public QDialog, public Ui::QAjOptionsDialogBase
{
    Q_OBJECT

public:
    QAjOptionsDialog( QWidget* parent = 0 );
    ~QAjOptionsDialog();

    AjSettings getAjSettings();
    void setAjSettings( AjSettings settings );
    void setSettings();
    void writeSettings();
    QFont getFont();

    QStringList getDefaultStatusbarComponents();
    QString defaultLauncher;

    static QVariant getSetting( QString key, QVariant defaultValue );
    static void setSetting( QString key, QVariant value );
    static QVariant getSetting( QString group, QString key, QVariant defaultValue );
    static void setSetting( QString group, QString key, QVariant value );
    void setConnected(bool connected);
    static bool hasSetting(QString group, QString key);
    static bool hasSetting(QString key);

private:
    QFontDatabase fontDatabase;
//     QFont currentFont;

protected slots:
    void selectIncomingDir();
    void selectTempDir();
    void selectIncomingDirSpecific();
    void selectTempDirSpecific();
    void selectLauncher();
    void specificRadioToggled( bool checked );
private slots:
    void jumpToFtpSlot();
    void acceptedSlot();
    void setFontSizes( const QFont& font );
};

#endif

