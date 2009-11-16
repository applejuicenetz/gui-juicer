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

#ifndef JUICER_H
#define JUICER_H

#include <QTimer>
#include <QMessageBox>
#include <QDateTime>
#include <QClipboard>
#include <QCloseEvent>
#include <QUrl>
#include <QDir>
#include <QFile>
#include <QFileDialog>
#include <QTextStream>
#include <QNetworkInterface>
#include <QHostInfo>
#include <QSystemTrayIcon>
#include <QSplashScreen>
#include <QDesktopServices>

#include "ui_mainwindowbase.h"

#include "application.h"
#include "downloadmodule.h"
#include "uploadmodule.h"
#include "searchmodule.h"
#include "servermodule.h"
#include "sharemodule.h"
#include "incomingmodule.h"

#include "xmlmodule.h"
#include "optionsdialog.h"
#include "logindialog.h"
#include "networkdialog.h"
#include "convert.h"
#include "serversocket.h"
#include "iconwidget.h"
#include "autoupdate.h"
#include "aboutdialog.h"

static const QString WINDOWS = "1";
static const QString LINUX = "2";
static const QString MAC = "3";
static const QString SOLARIS = "4";
static const QString FREEBSD = "6";
static const QString NETWARE = "7";

class Juicer: public QMainWindow, public Ui::MainWindowBase
{
    Q_OBJECT
public:
    Juicer(const QStringList& argList, QSplashScreen *splash, const QFileInfo& appFileInfo);
    ~Juicer();

    XMLModule *xml;
    DownloadModule* downloadModule;
    UploadModule *uploadModule;
    NetworkDialog *networkDialog;
    SearchModule *searchModule;
    ServerModule *serverModule;
    ShareModule *shareModule;
    IncomingModule *incomingModule;
    QHash<QString, QIcon> osIcons;
    QList<IconWidget*> statusBarWidgets;   /// hide-able statusbar widgets
    OptionsDialog *optionsDialog;

    void setStatusBarText(const QString& downSpeed, const QString& upSpeed,
                          const QString& credits, const QString& downSize,
                          const QString& upSize, const QString& openConnections);
    void setFilesystemSeparator(const QString& separator);
    // returns filesystemseperator of the remote filesystem where the Core runs
    QString getFilesystemSeparator() const;
    bool isCoreLocal();
    void setCoreVersion(const QString& version);
    void connectedSince(const QString& since);
    void queueLinks(const QStringList& links);
    void setUploadFilename(const QString& shareId, const QString& filename);
    void sendToTray( const QString& message1, const QString& message2 );
    bool isLocalhost(const QString& host);
    static QStringList getExec();
    void setClipboard(const QString& text);
    static QStringList getAjfspLinks(const QString& text, const QString& type = "[^|]*");
    QStringList processLinks(const QString& text, const QString& type = "[^|]*");
    QString getTempDirectory() const;
    QString getIncomingDirectory() const;
    void setTempDirectory(const QString& folder);
    void setIncomingDirectory(const QString& folder);
    QString getAppPath() const;
    void setFirewalled(bool  firewalled);
    void autoUpdate();
    void setCurrentProfile();
    void notifyAjfspLink(QStringList& links);

protected:
    void initToolBars();
    void connectActions();
    IconWidget* addToStatusBar(IconWidget* widget);
    void initStatusBar();
    void initTrayIcon();
    void closeEvent(QCloseEvent*);
    void processQueuedLinks();
    void saveGUIState();
    QString showLoginDialog(const QString& message = "");

    QSystemTrayIcon* tray;
    QString password;
    QString incomingDir, tempDir, filesystemSeparator;
    QFileInfo appFileInfo;
    QTimer *timer;
    int exitId;
    QWidget *prevTab;
    ServerSocket *linkServer;
    QLabel *ajAddressLabel;
    QLineEdit *ajAddressEdit;
    QToolButton *ajAddressButton;
    QLabel *warnFirewallLabel;
    IconWidget *downSpeedLabel, *upSpeedLabel, *creditsLabel, *downSizeLabel,
               *upSizeLabel, *coreVersionLabel, *connectedLabel, *connectionsLabel;
    bool started, connected, localCore;
    QStringList queuedLinks;
    int firstModifiedCnt, firstModifiedMax;
    QSplashScreen *splash;
    QStringList clipboardTexts;
    AutoUpdate* autoUpdater;
    QWidget* profilesWidget;
    QHBoxLayout* profilesBox;
    QButtonGroup* profileGroup;

private slots:
    void processLinksAndNotify(const QString& text, const QString& type = "[^|]*");

    bool login(const QString& message = "<h3>Login</h3>", bool error = false);
    void openAjL();
    void about();
    void aboutQt();
    void timerSlot();
    void showOptions();
    void settingsReady(const AjSettings& settings);
    void xmlError(const QString& reason);
    void gotSession();
    void processLink(const QString& link);
    void processLinkFromToolbar();
    void processClipboard();
    void tabChanged(int index);
    void quit();
    void exitCore();
    void firstModified();
    void adjustColumns();
    void trayActivated(QSystemTrayIcon::ActivationReason reason);
    void downloadsFinished(const QList<DownloadItem*>& list);
    void hostLookedUp(const QHostInfo& host);
    void showManual();
    void clipboardChanged(QClipboard::Mode mode);
    void getNetworkInfo();
    void profileButtonClicked(QAbstractButton* button);
public slots:
    void requestFinished(int id, bool error);
};

#endif
