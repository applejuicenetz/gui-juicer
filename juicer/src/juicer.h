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
#include <QTextEdit>
#include <QToolButton>
#include <QSettings>
#include <QInputDialog>
#include <QString>
#include <QCheckBox>
#include <QTreeWidget>
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
#include <QFileInfo>
#include <QDir>
#include <QSplitter>
#include <QTabWidget>

#include "qajdownloadwidget.h"
#include "qajuploadwidget.h"
#include "qajsearchwidget.h"
#include "qajserverwidget.h"
#include "qajservermetawidget.h"
#include "qajsharewidget.h"
#include "qajincomingwidget.h"

#include "qxmlmodule.h"
#include "qajoptionsdialog.h"
#include "qajlogindialog.h"
#include "qajnetworkdialog.h"
#include "qconvert.h"
#include "qajdownloaditem.h"

#include "qajserversocket.h"
#include "qajapplication.h"

class Juicer: public QMainWindow
{
    Q_OBJECT
public:
    Juicer( );
    ~Juicer();
    bool login();

    void setStatusBarText( QString downSpeed, QString upSpeed, QString credits, QString downSize, QString upSize );
    QAjNetworkDialog *networkDialog;
    QAjDownloadWidget *ajDownloadWidget;
    QAjUploadWidget *ajUploadWidget;
    QAjSearchWidget *ajSearchWidget;
    QAjServerMetaWidget *ajServerMetaWidget;
    QAjServerWidget *ajServerWidget;
    QAjShareWidget *ajShareWidget;
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

    static QStringList getExec();

protected:
    void initToolBars();
    void closeEvent( QCloseEvent* );
    void processQueuedLinks();

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

    QLabel *downSpeedLabel;
    QLabel *upSpeedLabel;
    QLabel *creditsLabel;
    QLabel *downSizeLabel;
    QLabel *upSizeLabel;
    QLabel *coreVersionLabel;
    QLabel *connectedLabel;

    QAction *downloadMenuBar, *serverMenuBar, *shareMenuBar, *searchMenuBar;

    QAction *clipboardButton;

    bool connected;

    QAction *openIncomingButton;

    QStringList queuedLinks;
    QDateTime zeroTime;

    int firstModifiedCnt;

private slots:

    void about();
    void aboutQt();
    void timerSlot();
    void partListTimerSlot();
    void showOptions();
    void showNetworkInfo();
    void settingsReady( AjSettings settings );
    void xmlError( int code );
    void gotSession();

    void processLink();
    void processClipboard();

    void tabChanged( QWidget *tab );

    void exitCore();

    void firstModified();

    void linkServerLine( QString line );

    void adjustColumns();
};


#endif
