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

#ifndef QAJOPTIONSDIALOG_H
#define QAJOPTIONSDIALOG_H

#include <QDialog>
#include <QFileDialog>
#include <QSettings>
#include <QFtp>
#include <QFontDialog>
#include "ui_optionsdialogbase.h"
#include "iconwidget.h"

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

class Juicer;

class OptionsDialog : public QDialog, public Ui::OptionsDialogBase
{
    Q_OBJECT

public:
    OptionsDialog( Juicer* juicer );
    ~OptionsDialog();

    AjSettings getAjSettings();
    void setAjSettings( const AjSettings& settings );
    void setSettings();
    void writeSettings();
    QFont getFont();

    QStringList getDefaultStatusbarComponents();
    QString defaultLauncher;

    static QVariant getSetting( const QString& key, QVariant defaultValue = QVariant() );
    static void setSetting( const QString& key, QVariant value );
    static QVariant getGroupSetting( const QString& group, const QString& key );
    static QVariant getSetting( const QString& group, const QString& key, QVariant defaultValue );
    static void setSetting( const QString& group, const QString& key, QVariant value );
    void setConnected(bool connected);
    static bool hasSetting(const QString& group, const QString& key);
    static bool hasSetting(const QString& key);
    static void removeSetting(const QString& group, const QString& key);
    static void removeSetting(const QString& key);
    static QString video();
    static QString audio();
    static QString image();
    static QString text();
    static QString archive();
    static QString cddvd();
    static QList<QVariant> getStatusbarShows(int n);
private:
    QFontDatabase fontDatabase;
    Juicer* juicer;
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
    void reset();
};

#endif

