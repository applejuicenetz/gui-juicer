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

#include "qajdownloadwidget.h"
#include "qajuploadwidget.h"
#include "qajsearchwidget.h"
#include "qajserverwidget.h"
#include "qajservermetawidget.h"
#include "qajsharewidget.h"
#include "qajsharefileswidget.h"
#include "qajsharemetawidget.h"
#include "qajincomingwidget.h"

#include "qxmlmodule.h"
#include "qajoptionsdialog.h"
#include "qajlogindialog.h"
#include "qajnetworkdialog.h"
#include "qconvert.h"

#include "qajapplication.h"

#include "qajiconwidget.h"

class Juicer: public QMainWindow
{
    Q_OBJECT
public:
    Juicer( QStringList argList, QSplashScreen *splash );
    ~Juicer();
    QString getPassword();
    bool login();

    void setStatusBarText( QString downSpeed, QString upSpeed, QString credits, QString downSize, QString upSize );
    QAjNetworkDialog *networkDialog;
    QAjDownloadWidget *ajDownloadWidget;
    QAjUploadWidget *ajUploadWidget;
    QAjSearchWidget *ajSearchWidget;
    QAjServerMetaWidget *ajServerMetaWidget;
    QAjServerWidget *ajServerWidget;
    QAjShareWidget *ajShareWidget;
    QAjShareMetaWidget *ajShareMetaWidget;
    QAjShareFilesWidget *ajShareFilesWidget;
    QAjIncomingWidget *ajIncomingWidget;

    void setFilesystemSeparator( QString separator )
    {
        filesystemSeparator = separator;
    }
    QString getFilesystemSeparator( )
    {
        return filesystemSeparator;
    }
    void setCoreVersion( QString version );
    void connectedSince( QString since );
    void queueLinks( QStringList links );
    void setUploadFilename( QString shareId, QString filename );

    void createAjL( QList<QAjItem *>  selectedItems );
    void sendToTray(  QString message1, QString message2  );

    static QStringList getExec();

protected:
    void initToolBars();
    void closeEvent( QCloseEvent* );
    void processQueuedLinks();
    void initStatusBar();

    QTabWidget* ajTab;
    QSystemTrayIcon* tray;
    QString password;
    QString filesystemSeparator;

    QTimer *timer;
    QTimer *partListTimer;

    QWidget *prevTab;
    QAjOptionsDialog *optionsDialog;
    QXMLModule *xml;

    QAjServerSocket *linkServer;

    QLabel *ajAddressLabel;
    QLineEdit *ajAddressEdit;
    QToolButton *ajAddressButton;

    QMenu* file;
    QMenu* help;

    QAjIconWidget *downSpeedLabel;
    QAjIconWidget *upSpeedLabel;
    QAjIconWidget *creditsLabel;
    QAjIconWidget *downSizeLabel;
    QAjIconWidget *upSizeLabel;
    QAjIconWidget *coreVersionLabel;
    QAjIconWidget *connectedLabel;

    QAction *clipboardButton;

    bool connected;

    QAction *openIncomingButton;

    QStringList queuedLinks;
    QDateTime zeroTime;

    int firstModifiedCnt, firstModifiedMax;
    QSplashScreen *splash;

private slots:

    void openAjL();

    void about();
    void aboutQt();
    void timerSlot();
    void partListTimerSlot();
    void showOptions();
    void showNetworkInfo();
    void settingsReady( AjSettings settings );
    void xmlError( int code );
    void gotSession();

    void processLink( QString link );
    void processLink();
    void processClipboard();

    void tabChanged( int index );
    void tabChanged( QWidget *tab );

    void exitCore();

    void firstModified();

    void linkServerLine( QString line );

    void adjustColumns();
    void trayActivated ( QSystemTrayIcon::ActivationReason reason );
    void lastWindowClosed();
    void downloadsFinished( QList<QAjDownloadItem*> list );
};


#endif
