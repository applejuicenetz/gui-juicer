/***************************************************************************
 *   Copyright (C) 2004 by Matthias Reif                                   *
 *   matthias.reif@informatik.tu-chemnitz.de                               *
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

#ifndef JUICER_H
#define JUICER_H

#include <QTimer>
#include <QMessageBox>
#include <QDateTime>
#include <QLineEdit>
#include <QToolButton>
#include <QSettings>
#include <QInputDialog>
#include <QCheckBox>
#include <QClipboard>
#include <QCloseEvent>
#include <QLabel>
#include <QMenu>
#include <QMenuBar>
#include <QKeySequence>
#include <QStatusBar>
#include <QToolBar>
#include <QToolButton>
#include <QFileDialog>
#include <QUrl>
#include <QDir>
#include <QTabWidget>
#include <QFile>
#include <QFileDialog>
#include <QTextStream>
#include <QNetworkInterface>
#include <QHostInfo>
#include <QSystemTrayIcon>

#include "ui_mainwindowbase.h"

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

#include "application.h"

#include "iconwidget.h"

#include "helpdialog.h"
#include "autoupdate.h"

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
    Juicer( const QStringList& argList, QSplashScreen *splash );
    ~Juicer();

    void setStatusBarText( const QString& downSpeed, const QString& upSpeed, const QString& credits, const QString& downSize, const QString& upSize );
    XMLModule *xml;
    DownloadModule* downloadModule;
    UploadModule *uploadModule;
    NetworkDialog *networkDialog;
    SearchModule *searchModule;
    ServerModule *serverModule;
    ShareModule *shareModule;
    IncomingModule *incomingModule;
    QHash<QString, QIcon> osIcons;

    void setFilesystemSeparator(const QString& separator);
    // returns filesystemseperator of the remote filesystem where the Core runs
    QString getFilesystemSeparator( ) const;
    bool isCoreLocal();
    void setCoreVersion( const QString& version );
    void connectedSince( const QString& since );
    void queueLinks( const QStringList& links );
    void setUploadFilename( const QString& shareId, const QString& filename );

    void sendToTray(  const QString& message1, const QString& message2  );

    bool isLocalhost( const QString& host );

    static QStringList getExec();
    void setClipboard(const QString& text);
    static QStringList getAjfspLinks(const QString& text, const QString& type = "[^|]*");
    void processLinks(const QString& text, const QString& type = "[^|]*");
    QString getTempDirectory() const;
    QString getIncomingDirectory() const;
    void setTempDirectory(const QString& folder);
    void setIncomingDirectory(const QString& folder);
    QString getAppPath() const;
    void setAppPath(const QString& path);

protected:
    void initToolBars();
    void connectActions();
    void initStatusBar();
    void initTrayIcon();
    void closeEvent( QCloseEvent* );
    void processQueuedLinks();
    void saveGUIState();
    QString showLoginDialog(const QString& message = "");

    QSystemTrayIcon* tray;
    QString password;
    QString appPath, incomingDir, tempDir, filesystemSeparator;

    QTimer *timer;
    QTimer *partListTimer;

    QWidget *prevTab;
    OptionsDialog *optionsDialog;

    ServerSocket *linkServer;

    QLabel *ajAddressLabel;
    QLineEdit *ajAddressEdit;
    QToolButton *ajAddressButton;

    IconWidget *downSpeedLabel, *upSpeedLabel, *creditsLabel, *downSizeLabel, *upSizeLabel, *coreVersionLabel, *connectedLabel;

    bool started, connected, localCore;

    QStringList queuedLinks;

    int firstModifiedCnt, firstModifiedMax;
    QSplashScreen *splash;
    QStringList clipboardTexts;

private slots:
    bool login(const QString& message = "<h3>Login</h3>", bool error = false);
    void openAjL();

    void about();
    void aboutQt();
    void timerSlot();
    void partListTimerSlot();
    void showOptions();
    void settingsReady(const AjSettings& settings);
    void xmlError(const QString& reason);
    void gotSession();

    void processLink(const QString& link);
    void processLink();
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
private slots:
    void clipboardChanged(QClipboard::Mode mode);
    void getNetworkInfo();
};

#endif
