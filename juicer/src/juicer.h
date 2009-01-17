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

#include <QSystemTrayIcon>

#include "ui_qajmainwindowbase.h"

#include "qajdownloadmodule.h"
#include "qajuploadmodule.h"
#include "qajsearchmodule.h"
#include "qajservermodule.h"
#include "qajsharemodule.h"
#include "qajincomingmodule.h"

#include "qxmlmodule.h"
#include "qajoptionsdialog.h"
#include "qajlogindialog.h"
#include "qajnetworkdialog.h"
#include "qconvert.h"

#include "qajapplication.h"

#include "qajiconwidget.h"

static const QString WINDOWS = "1";
static const QString LINUX = "2";
static const QString MAC = "3";
static const QString SOLARIS = "4";
static const QString FREEBSD = "6";
static const QString NETWARE = "7";

class Juicer: public QMainWindow, public Ui::QAjMainWindowBase
{
    Q_OBJECT
public:
    Juicer( QStringList argList, QSplashScreen *splash );
    ~Juicer();

    void setStatusBarText( const QString& downSpeed, const QString& upSpeed, const QString& credits, const QString& downSize, const QString& upSize );
    QXMLModule *xml;
    QAjDownloadModule* downloadModule;
    QAjUploadModule *uploadModule;
    QAjNetworkDialog *networkDialog;
    QAjSearchModule *searchModule;
    QAjServerModule *serverModule;
    QAjShareModule *shareModule;
    QAjIncomingModule *incomingModule;
    QHash<QString, QIcon> osIcons;

    void setFilesystemSeparator( const QString& separator )
    {
        filesystemSeparator = separator;
    }
    QString getFilesystemSeparator( )
    {
        return filesystemSeparator;
    }
    void setCoreVersion( const QString& version );
    void connectedSince( const QString& since );
    void queueLinks( const QStringList& links );
    void setUploadFilename( const QString& shareId, const QString& filename );

    void createAjL( const QList<QAjItem *>& selectedItems );
    void sendToTray(  const QString& message1, const QString& message2  );

    static QStringList getExec();

    // returns filesystemseperator of the remote filesystem where the Core runs
    QString getFileSystemSeperator() const
    {
        return filesystemSeparator;
    }

protected:
    void initToolBars();
    void connectActions();
    void initStatusBar();
    void initTrayIcon();
    void closeEvent( QCloseEvent* );
    void processQueuedLinks();
    QString showLoginDialog(const QString& message = "");

    QSystemTrayIcon* tray;
    QString password;
    QString filesystemSeparator;

    QTimer *timer;
    QTimer *partListTimer;

    QWidget *prevTab;
    QAjOptionsDialog *optionsDialog;

    QAjServerSocket *linkServer;

    QLabel *ajAddressLabel;
    QLineEdit *ajAddressEdit;
    QToolButton *ajAddressButton;

    QAjIconWidget *downSpeedLabel, *upSpeedLabel, *creditsLabel, *downSizeLabel, *upSizeLabel, *coreVersionLabel, *connectedLabel;

    bool started, connected;

    QStringList queuedLinks;
    QDateTime zeroTime;

    int firstModifiedCnt, firstModifiedMax;
    QSplashScreen *splash;

private slots:
    bool login(const QString& message = "<h3>Login</h3>", bool error = false);
    void openAjL();

    void about();
    void aboutQt();
    void timerSlot();
    void partListTimerSlot();
    void showOptions();
    void showNetworkInfo();
    void settingsReady(const AjSettings& settings);
    void xmlError(const QString& reason);
    void gotSession();

    void processLink(const QString& link);
    void processLink();
    void processClipboard();

    void tabChanged(int index);

    void exitCore();

    void firstModified();

    void adjustColumns();
    void trayActivated(QSystemTrayIcon::ActivationReason reason);
    void lastWindowClosed();
    void downloadsFinished(const QList<QAjDownloadItem*>& list);
};

#endif
