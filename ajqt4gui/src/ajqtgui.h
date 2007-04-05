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

#ifndef AJQTGUI_H
#define AJQTGUI_H


#include <QMainWindow>
#include <QTimer>
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
#include <QProgressDialog>
#include <QUrl>
#include <QFtp>

#include "qajtab.h"
#include "qajicons.h"
#include "qxmlmodule.h"
#include "qajoptionsdialog.h"
#include "qajlogindialog.h"
#include "qajnetworkwidget.h"
#include "qconvert.h"
#include "qajdownloaditem.h"

#include "qajserversocket.h"
#include "qajapplication.h"

#include "ftp.h"

class AjQtGUI: public QMainWindow
{
	Q_OBJECT
public:
	AjQtGUI( );
	~AjQtGUI();
	bool login();

	void setStatusBarText( QString downSpeed, QString upSpeed, QString credits, QString downSize, QString upSize );
	QAjNetworkWidget *networkWidget;
	
	void setFilesystemSeparator( QString separator ) { filesystemSeparator = separator; }
	QString getFilesystemSeparator( ) { return filesystemSeparator; }
	void setCoreVersion( QString version );
	void connectedSince( QString since );
	void queueLinks( QStringList* links );
	
protected:
	void initToolBars();

	void closeEvent( QCloseEvent* );
	
	QString getSelectedDownloads();
	
	void processQueuedLinks();

	QString password;
	
	QString filesystemSeparator;
	
	QTimer *timer;
	QTimer *partListTimer;

	QAjTab *ajTab;
	QWidget *prevTab;
	QAjOptionsDialog *optionsDialog;
	QXMLModule *xml;
	
	QHttp *serverHttp;
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
	
	QToolBar *downloadToolBar, *uploadToolBar, *searchToolBar, *serverToolBar, *shareToolBar, *ftpToolBar;
	
	QLabel *ajSearchLabel;
	QLineEdit *ajSearchEdit;
	QAction *ajSearchButton;
	
	QAction *downloadMenuBar, *serverMenuBar, *shareMenuBar, *searchMenuBar;
		
	QAjIcons *icons;
		
	bool connected;
	
	QDoubleSpinBox* powerSpin;
	QAction *powerUpButton, *powerDownButton, *powerOkButton, *powerMaxButton;
	QAction *pauseDownloadButton, *resumeDownloadButton;
	QAction *cancelDownloadButton, *clearDownloadButton,  *partListButton, *renameDownloadButton;
	QAction *saveDownloadButton;
	QAction *clipboardButton;
	QCheckBox *powerCheck;
	QAction *removeShareButton, *applyShareButton, *reloadSharedFilesButton;
	QAction *removeServerButton, *connectServerButton, *findServerButton;
	QAction *storeFtpButton;
	
	QProgressDialog* progressDialog;
	
	bool special;
	
	QStringList *queuedLinks;

	map<unsigned long, QAjDownloadItem*>::iterator partListIt, partListEnd;

	QFtp* ftp;
	
private slots:

	void about();
	void aboutQt();
	void timerSlot();
	void partListTimerSlot();
	void showOptions();
	void showNetworkInfo();
	void settingsReady( AjSettings settings );
	void xmlError( int code );
	void httpDone( bool error );
	
	void processSelected( int xmlCode, QString para = "" );
	void requestSelected( int xmlCode, QString para = "" );

	void powerChanged(const QString& );
	void applyPowerDownload();
	void maxPowerDownload();
	
	void cancelDownload();
	void cleanDownload();
	void resumeDownload();
	void pauseDownload();
	void partListRequest();
	void renameDownload();
	
	void removeServer();
	void connectServer();
	
	void processLink();
	void processClipboard();
	void downloadSearch();
	
	void addShare();
	void removeShare();
	void applyShare();
	
	void tabChanged( QWidget *tab );
	void downloadSelectionChanged( );
	
	void exitCore();
	
	void gotIds( QStringList ids );
	
	void clipboardChanged();
	
	void search();
	void cancelSearch();
	
	void findServer();
	void gotServer( int id, bool error );
	
	void reloadShare();
	
	void firstModified();
	
	void linkServerLine( QString line );
	void powerChanged( double value );

	void storeDownload();
	void reloadFtp();
	void storeFtp();
};


#endif
