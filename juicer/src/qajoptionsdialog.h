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

static const QString CONNECTED_SINCE = "0";
static const QString CORE_VERSION = "1";
static const QString DOWNSTREAM = "2";
static const QString UPSTREAM = "3";
static const QString DOWNLOADED = "4";
static const QString UPLOADED = "5";
static const QString CREDITS = "6";

static const QString WIN_LAUNCHER = "Windows default";
static const QString MAC_LAUNCHER = "MacOS default";
static const QString KDE_LAUNCHER = "kfmclient (KDE)";
static const QString GNOME_LAUNCHER = "gnome-open (Gnome)";

#ifdef Q_WS_WIN
    static const QString DEFAULT_LAUNCHER = WIN_LAUNCHER;
#else
#ifdef Q_WS_MAC
    static const QString DEFAULT_LAUNCHER = MAC_LAUNCHER;
#else
    static const QString DEFAULT_LAUNCHER = KDE_LAUNCHER;
#endif
#endif

class AjSettings {
    public:
    enum LOCATION{SAME=0,SPECIFIC=1,FTP=2};
    QString nick, tcpPort, xmlPort, incomingDir, tempDir, autoconnect;
    QString maxDown, maxUp, maxCon, maxSlot, maxSources, maxNewCon;
};


class QAjOptionsDialog : public QDialog, public Ui::QAjOptionsDialogBase
{
    Q_OBJECT

public:
    QAjOptionsDialog( QWidget* parent = 0 );
    ~QAjOptionsDialog();

    AjSettings getAjSettings();
    void setAjSettings( const AjSettings& settings );
    void setSettings();
    void writeSettings();
    QFont getFont();

    QStringList getDefaultStatusbarComponents();
    QString defaultLauncher;

    static QVariant getSetting( const QString& key, QVariant defaultValue );
    static void setSetting( const QString& key, QVariant value );
    static QVariant getSetting( const QString& group, const QString& key, QVariant defaultValue );
    static void setSetting( const QString& group, const QString& key, QVariant value );
    void setConnected(bool connected);
    static bool hasSetting(const QString& group, const QString& key);
    static bool hasSetting(const QString& key);
    static void removeSetting(const QString& group, const QString& key);
    static void removeSetting(const QString& key);
private:
    QFontDatabase fontDatabase;

protected slots:
    void selectIncomingDir();
    void selectTempDir();
    void selectIncomingDirSpecific();
    void selectTempDirSpecific();
    void selectLauncher();
    void specificRadioToggled( bool checked );
    void jumpToFtpSlot();
    void acceptedSlot();
    void setFontSizes( const QFont& font );
    void setAjfspHandler();
};

#endif

