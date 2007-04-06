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

#include "ajqtgui.h"

AjQtGUI::AjQtGUI( ) : QMainWindow( )
{
	char* mode = getenv( "AJQTGUI_MODE" );
	// special always on in special release
	special = true;//( ( mode != NULL ) && ( strcmp(mode, "SPECIAL") == 0 ) );
	queuedLinks = NULL;
	filesystemSeparator = "\\";
	progressDialog = NULL;
	icons = new QAjIcons();
	
	linkServer = new QAjServerSocket( QAjApplication::APP_PORT );
	connect( linkServer, SIGNAL( lineReady( QString ) ), this, SLOT( linkServerLine( QString ) ) );
	
	setWindowIcon( *icons->ajqtguiPixmap );

	ajTab = new QAjTab( &filesystemSeparator, icons, special, this );
	setCentralWidget( ajTab );
	prevTab = ajTab->ajDownloadWidget;

	networkWidget = new QAjNetworkWidget(NULL);
	optionsDialog = new QAjOptionsDialog( this );
	optionsDialog->setSpecial( special );

	initToolBars();
	
	file = new QMenu( tr("&AppleJuice"), this );
	menuBar()->addMenu( file );

	file->addAction( *icons->configureSmallIcon, tr("C&onfigure"), this, SLOT( showOptions() ), QKeySequence( Qt::CTRL+Qt::Key_O ) );
	file->addAction( *icons->infoSmallIcon, tr("&Net Info"), this, SLOT( showNetworkInfo() ), QKeySequence( Qt::CTRL+Qt::Key_N ) );
	file->addSeparator();
	file->addAction( *icons->exitSmallIcon, tr("&Exit Core"), this, SLOT( exitCore() ), QKeySequence( Qt::CTRL+Qt::Key_E ) );
	file->addAction( *icons->closeSmallIcon, tr("&Quit GUI"), qApp, SLOT( closeAllWindows() ), QKeySequence( Qt::CTRL+Qt::Key_Q ) );
	
	downloadMenuBar = menuBar()->addMenu( /*tr("&Download"), */ajTab->ajDownloadWidget->popup );
	serverMenuBar = menuBar()->addMenu( /*tr("&Server"), */ajTab->ajServerWidget->popup );
	shareMenuBar = menuBar()->addMenu( /*tr("&Share"), */ajTab->ajShareWidget->popup );
	searchMenuBar = menuBar()->addMenu( /*tr("&Search"),*/ ajTab->ajSearchWidget->popup );

	menuBar()->addSeparator();

	help = new QMenu( this );
	help->setTitle( tr("&Help") );
	help->addAction( tr("&About"), this, SLOT(about()), QKeySequence( Qt::Key_F1  ) );
	help->addAction( tr("About &Qt"), this, SLOT(aboutQt()) );
	help->addSeparator();
	help->addAction( tr("What's &This"), this, SLOT(whatsThis()), QKeySequence( Qt::SHIFT+Qt::Key_F1 ) );
   menuBar()->addMenu( help );
   
	downSpeedLabel = new QLabel(this);
	upSpeedLabel = new QLabel(this);
	creditsLabel = new QLabel(this);
	downSizeLabel = new QLabel(this);
	upSizeLabel = new QLabel(this);
	coreVersionLabel = new QLabel(this);
	connectedLabel = new QLabel(this);

	statusBar()->addPermanentWidget( connectedLabel );
	statusBar()->addPermanentWidget( coreVersionLabel );
	statusBar()->addPermanentWidget( downSpeedLabel );
	statusBar()->addPermanentWidget( upSpeedLabel );
	statusBar()->addPermanentWidget( downSizeLabel );
	statusBar()->addPermanentWidget( upSizeLabel );
	statusBar()->addPermanentWidget( creditsLabel );

	QSettings lokalSettings;
	lokalSettings.beginGroup( "MainWindow" );
	resize( lokalSettings.value( "size", QSize(1000, 600) ).toSize() );
	move( lokalSettings.value( "pos", QPoint(100, 100) ).toPoint() );
	lokalSettings.endGroup();

	xml = new QXMLModule( this, ajTab );
	connect( xml, SIGNAL( settingsReady( AjSettings ) ), this, SLOT( settingsReady( AjSettings ) ) );
	connect( xml, SIGNAL( error( int ) ), this, SLOT( xmlError( int ) ) );
   connect( xml, SIGNAL( gotSession() ), this, SLOT( gotSession() ) );
   connect( xml, SIGNAL( modifiedDone( ) ), ajTab->ajDownloadWidget, SLOT( updateView( ) ) );
   connect( xml, SIGNAL( modifiedDone( ) ), this, SLOT( firstModified() ) );

	serverHttp = new QHttp();
	connect( serverHttp, SIGNAL( requestFinished ( int , bool ) ), this, SLOT( gotServer( int , bool ) ) );
	
	connected = false;
	password = lokalSettings.value( "password" ).toString();
	// no password in local file? => ask for it
	if( password == NULL )
	{
		bool ok;
		password = QInputDialog::getText( this, "AjQtGUI", "Enter core password:", QLineEdit::Password,  QString::null, &ok );
		if( !ok ) // user canceld
			qApp->closeAllWindows();
		else
		{
			// save password in local file if user wants it
			if( lokalSettings.value( "savePassword", "false" ).toString() == "true" )
				lokalSettings.setValue( "password", password );
		}
	}
	xml->setPassword( password );
	xml->setHost( lokalSettings.value("coreAddress", "localhost").toString(),
        lokalSettings.value("/progeln.de/AjQtGUI/xmlPort", 9851).toInt() );

	ftp = new QFtp();

	timer = new QTimer( this );
	connect( timer, SIGNAL( timeout() ), this, SLOT( timerSlot() ) );
	partListTimer = new QTimer( this );
	connect( partListTimer, SIGNAL( timeout() ), this, SLOT( partListTimerSlot() ) );
	
	connect( ajTab->ajDownloadWidget, SIGNAL( pause() ), this, SLOT( pauseDownload() ) );
	connect( ajTab->ajDownloadWidget, SIGNAL( resume() ), this, SLOT( resumeDownload() ) );
	connect( ajTab->ajDownloadWidget, SIGNAL( cancel() ), this, SLOT( cancelDownload() ) );
	connect( ajTab->ajDownloadWidget, SIGNAL( clean() ), this, SLOT( cleanDownload() ) );
	connect( ajTab->ajDownloadWidget, SIGNAL( partListRequest() ), this, SLOT( partListRequest() ) );
	connect( ajTab->ajDownloadWidget, SIGNAL( rename() ), this, SLOT( renameDownload() ) );
	
	connect( ajTab->ajServerWidget, SIGNAL( remove() ), this, SLOT( removeServer() ) );
	connect( ajTab->ajServerWidget, SIGNAL( connect() ), this, SLOT( connectServer() ) );
	connect( ajTab->ajServerWidget, SIGNAL( find() ), this, SLOT( findServer() ) );
	
	connect( ajTab->ajSearchWidget, SIGNAL( remove() ), this, SLOT( cancelSearch() ) );
	connect( ajTab->ajSearchWidget, SIGNAL( download() ), this, SLOT( downloadSearch() ) );
	connect( ajTab->ajSearchWidget, SIGNAL( doubleClicked ( Q3ListViewItem *, const QPoint &, int ) ), this, SLOT( downloadSearch() ) );
	
	
	connect( ajTab, SIGNAL( currentChanged( QWidget* ) ), this, SLOT( tabChanged( QWidget* ) ) );
	
	connect( ajTab->ajDownloadWidget, SIGNAL( itemSelectionChanged( ) ), this, SLOT( downloadSelectionChanged( ) ) );
	
	connect( ajTab->ajServerWidget, SIGNAL( newSelection( bool ) ), connectServerButton, SLOT( setEnabled( bool ) ) );
	connect( ajTab->ajServerWidget, SIGNAL( newSelection( bool ) ), removeServerButton, SLOT( setEnabled( bool ) ) );
	
	connect( ajTab->ajShareWidget, SIGNAL( newSelection( bool ) ), removeShareButton, SLOT( setEnabled( bool ) ) );

	connect( ajTab->ajFtpWidget, SIGNAL( newSelection( bool ) ), storeFtpButton, SLOT( setEnabled( bool ) ) );
	
	connect( ajTab->ajShareWidget, SIGNAL( insert() ), this, SLOT( addShare() ) );
	connect( ajTab->ajShareWidget, SIGNAL( remove() ), this, SLOT( removeShare() ) );
	connect( ajTab->ajShareWidget, SIGNAL( reload() ), this, SLOT( reloadShare() ) );

	connect( xml, SIGNAL( gotIds( QStringList) ) , this, SLOT( gotIds( QStringList ) ) );

	connect( ftp, SIGNAL( listInfo ( QUrlInfo ) ), this->ajTab->ajFtpWidget, SLOT( insert( QUrlInfo ) ) );
	connect( ajTab->ajFtpWidget, SIGNAL( itemDoubleClicked ( QTreeWidgetItem*, int ) ), this, SLOT( storeFtp( ) ) );
	
	login();
}

AjQtGUI::~AjQtGUI()
{
	delete ajTab;
	delete networkWidget;
	delete optionsDialog;
	delete file;
	delete help;
	delete downSpeedLabel;
	delete upSpeedLabel;
	delete creditsLabel;
	delete downSizeLabel;
	delete upSizeLabel;
	delete coreVersionLabel;
	delete connectedLabel;
	delete xml;
	delete serverHttp;
	delete timer;
	delete partListTimer;
	delete icons;
}

void AjQtGUI::initToolBars()
{
	QToolBar* ajTools = new QToolBar( "applejuice operations", this );
	ajTools->setToolTip( "applejuice operations" );
	
	ajTools->addAction( *icons->configureIcon , "configure", this, SLOT( showOptions() ) )->setToolTip("configure");
	ajTools->addAction( *icons->infoIcon , "aj network info", this, SLOT( showNetworkInfo() ) )->setToolTip("aj network info");
	
	QToolBar* ajLinks = new QToolBar( "applejuice links", this );
	ajLinks->setToolTip( "applejuice links" );
	
	clipboardButton = ajLinks->addAction( *icons->clipboardIcon, "process link from clipboard", this, SLOT( processClipboard() ) );
	clipboardButton->setToolTip( "process: " + qApp->clipboard()->text( QClipboard::Clipboard ) );
	connect( qApp->clipboard(), SIGNAL( dataChanged() ), this, SLOT( clipboardChanged() ) );
	
	ajAddressLabel = new QLabel(ajLinks);
	ajAddressLabel->setText("ajfsp link:");
	ajAddressLabel->adjustSize();
	ajLinks->addWidget( ajAddressLabel );
	
	ajAddressEdit = new QLineEdit(ajLinks);
	ajLinks->addWidget( ajAddressEdit );
	connect( ajAddressEdit, SIGNAL( returnPressed() ), this, SLOT( processLink() ) );

	ajLinks->addAction( "download", this, SLOT( processLink() ) );


//	ajLinks->setStretchableWidget( ajAddressEdit );
//	ajLinks->setHorizontallyStretchable( true );

// DOWNLOAD TOOLBAR

	downloadToolBar = new QToolBar( "download operations", this );
//	downloadToolBar->setNewLine( true );

	pauseDownloadButton = downloadToolBar->addAction( *icons->downloadPauseIcon, "pause download", this, SLOT( pauseDownload() ) );
	pauseDownloadButton->setToolTip( "pause download" );
	
	resumeDownloadButton = downloadToolBar->addAction( *icons->downloadResumeIcon, "resume download", this, SLOT( resumeDownload() ) );
	resumeDownloadButton->setToolTip( "resume download" );

	cancelDownloadButton = downloadToolBar->addAction( *icons->downloadCancelIcon, "cancel download", this, SLOT( cancelDownload() ) );
	
	partListButton = downloadToolBar->addAction( *icons->partListIcon, "show part list", this, SLOT( partListRequest() ) );
	renameDownloadButton = downloadToolBar->addAction( *icons->downloadRenameIcon, "rename download", this, SLOT( renameDownload() ) );
	
	clearDownloadButton = downloadToolBar->addAction( *icons->downloadFilterIcon, "remove finished/canceld download", this, SLOT( cleanDownload() ) );

	saveDownloadButton = downloadToolBar->addAction( *icons->downloadSaveIcon, "store file", this, SLOT( storeDownload() ) );
	

	pauseDownloadButton->setDisabled( true );
	resumeDownloadButton->setDisabled( true );
	cancelDownloadButton->setDisabled( true );
	partListButton->setDisabled( true );
	renameDownloadButton->setDisabled( true );
	saveDownloadButton->setDisabled( true );
	
	downloadToolBar->addSeparator();
	
	powerCheck = new QCheckBox( downloadToolBar );
	powerCheck->setText( tr("Power Download:") );
	powerCheck->setChecked( false );
	powerCheck->adjustSize();
	downloadToolBar->addWidget( powerCheck );

	powerSpin = new QDoubleSpinBox( downloadToolBar );
	powerSpin->setRange( 2.2, 50.0 );
	powerSpin->setSingleStep( 0.1 );
	powerSpin->setDecimals( 1 );
	downloadToolBar->addWidget( powerSpin );
	
	//connect( powerEdit, SIGNAL( returnPressed() ), this, SLOT( applyPowerDownload() ) );
	connect( powerSpin, SIGNAL( valueChanged( const QString&) ), this, SLOT( powerChanged( const  QString& ) ) );
	connect( powerSpin, SIGNAL( valueChanged( double ) ), this, SLOT( powerChanged( double ) ) );

	powerOkButton = downloadToolBar->addAction( *icons->powerOkIcon, "apply power download", this, SLOT( applyPowerDownload() ) );
	
	powerMaxButton = downloadToolBar->addAction( *icons->powerMaxIcon, "set all downloads to 1:50", this, SLOT( maxPowerDownload() )  );
	powerMaxButton->setVisible( special );

// UPLOAD TOOLBAR

	uploadToolBar = new QToolBar( "upload operations", this );
	
	uploadToolBar->addAction( *icons->dummyIcon, "" )->setDisabled( true );
	uploadToolBar->hide();

// SEARCH TOOLBAR

	searchToolBar = new QToolBar( "search operations", this );
	
	searchToolBar->addAction( *icons->downloadSmallIcon, "download", this, SLOT( downloadSearch() ) );
	searchToolBar->addAction( *icons->downloadCancelIcon, "cancel search", this, SLOT( cancelSearch() ) );

	searchToolBar->addSeparator();
	
	ajSearchLabel = new QLabel( searchToolBar );
	ajSearchLabel->setText( "search for:" );
	searchToolBar->addWidget( ajSearchLabel );
	ajSearchEdit = new QLineEdit( searchToolBar );
	ajSearchEdit->setMinimumWidth( 200 );
	searchToolBar->addWidget( ajSearchEdit );
	connect( ajSearchEdit, SIGNAL( returnPressed() ), this, SLOT( search() ) );
	ajSearchButton = searchToolBar->addAction( "search", this, SLOT( search() )  );

	searchToolBar->hide();

// SERVER TOOLBAR

	serverToolBar = new QToolBar( "server operations", this );
	
	connectServerButton = serverToolBar->addAction( *icons->serverConnectIcon, "connect to this server", this, SLOT( connectServer() ) );
	removeServerButton = serverToolBar->addAction( *icons->serverRemoveIcon, "remove server", this, SLOT( removeServer() ) );
	findServerButton = serverToolBar->addAction( *icons->newIcon, "find server", this, SLOT( findServer() ) );
	
	removeServerButton->setDisabled( true );
	connectServerButton->setDisabled( true );
	serverToolBar->hide();

// SHARE TOOLBAR

	shareToolBar = new QToolBar( "share operations", this );
	
	shareToolBar->addAction( *icons->insertIcon, "add share", this, SLOT( addShare() ) );
	removeShareButton = shareToolBar->addAction( *icons->removeIcon, "remove share", this, SLOT( removeShare() ) );
	applyShareButton = shareToolBar->addAction( *icons->powerOkIcon, "transmit to the core", this, SLOT( applyShare() ) );
	applyShareButton->setDisabled( true );
	removeShareButton->setDisabled( true );
	
	reloadSharedFilesButton = shareToolBar->addAction( *icons->reloadIcon, "reload shared files", this, SLOT( reloadShare() ) );

	shareToolBar->hide();

	// FTP TOOLBAR
	ftpToolBar = new QToolBar( "ftp operations", this );
	ftpToolBar->addAction( *icons->reloadIcon, "reload files", this, SLOT( reloadFtp() ) );
	storeFtpButton = ftpToolBar->addAction( *icons->downloadSaveIcon, "store file", this, SLOT( storeFtp() ) );
	storeFtpButton->setDisabled( true );
	
	ftpToolBar->hide();
	
	this->setIconSize( QSize(22, 22) );
	this->addToolBar( ajTools );
	this->addToolBar( ajLinks );
	this->addToolBarBreak( );
	downloadToolBar->setIconSize( QSize(22, 22) );
	this->addToolBar( downloadToolBar );
	this->addToolBar( uploadToolBar );
	this->addToolBar( searchToolBar );
	this->addToolBar( serverToolBar );
	this->addToolBar( shareToolBar );
	this->addToolBar( ftpToolBar );

}

void AjQtGUI::closeEvent( QCloseEvent* ce )
{
	QSettings lokalSettings;
	lokalSettings.beginGroup("MainWindow");
	lokalSettings.setValue( "size", size() );
	lokalSettings.setValue( "pos", pos() );
	lokalSettings.endGroup();
	ce->accept();
}

void AjQtGUI::about()
{
	QMessageBox::about( this, tr("AjQtGUI Info"),
		tr("appleJuice Qt GUI 0.2\n\nhttp://ajqtgui.sf.net\nhttp://www.progeln.de"));
}

void AjQtGUI::aboutQt()
{
	QMessageBox::aboutQt( this, tr("AjQtGUI: About Qt") );
}

void AjQtGUI::timerSlot()
{
    if( xml->session == "" )
        return;
    if( (!xml->hasPendingRequests()) )
    {
        xml->get( "modified" );
    }
}

void AjQtGUI::partListTimerSlot()
{
    QString id = ajTab->ajDownloadWidget->getNextIdRoundRobin();
    if(!id.isEmpty())
    {
        xml->get( "downloadpartlist", "&simple&id=" + id);
        ajTab->ajDownloadWidget->doItemsLayout();
    }
}

void AjQtGUI::showOptions()
{
	xml->get( "settings" );
	if( optionsDialog->exec() == QDialog::Accepted )
	{
		// save options
		AjSettings settings = optionsDialog->getAjSettings();
		QSettings lokalSettings;
		lokalSettings.setValue( "coreAddress",  settings.coreAddress );
		lokalSettings.setValue( "savePassword",  settings.savePassword );
		lokalSettings.setValue( "serverURL",  settings.serverURL );
		lokalSettings.setValue( "refresh",  settings.refresh );

		lokalSettings.setValue( "ftpServer",  settings.ftpServer );
		lokalSettings.setValue( "ftpPort",  settings.ftpPort );
		lokalSettings.setValue( "ftpUser",  settings.ftpUser );
		lokalSettings.setValue( "ftpPassword",  settings.ftpPassword );
		lokalSettings.setValue( "ftpDir",  settings.ftpDir );

		timer->stop();
		timer->setSingleShot( false );
		timer->start( lokalSettings.value( "refresh", 3 ).toInt() * 1000 );
		
		// save password in local file if user want's it
		if( settings.savePassword == "true" )
			lokalSettings.setValue( "password", password );
		else
			lokalSettings.remove( "password" );

		QString settingsString = "";
		settingsString += "&Nickname=" + settings.nick;
		settingsString += "&Port=" + settings.tcpPort;
		settingsString += "&XMLPort=" + settings.xmlPort;
		settingsString += "&AllowBrowse=" + settings.allowBrowse;
		settingsString += "&MaxUpload=" + QString::number(settings.maxUp.toInt() * 1024);
		settingsString += "&MaxDownload=" + QString::number(settings.maxDown.toInt() * 1024);
		settingsString += "&MaxSourcesPerFile=" + settings.maxSources;
		settingsString += "&MaxConnections=" + settings.maxCon;
		settingsString += "&AutoConnect=" + settings.autoconnect;
		settingsString += "&Speedperslot=" + settings.maxSlot;
		settingsString += "&Incomingdirectory=" + settings.incomingDir;
		settingsString += "&Temporarydirectory=" + settings.tempDir;
		settingsString += "&MaxNewConnectionsPerTurn=" + settings.maxNewCon;
		printf( "%s\n", settingsString.toAscii().data() );
		xml->set( SET_SETTINGS_XML, settingsString );
	}
}

void AjQtGUI::settingsReady( AjSettings settings )
{
	if( optionsDialog != NULL )
	{
		QSettings lokalSettings;
		settings.coreAddress = lokalSettings.value( "coreAddress", "localhost" ).toString();
		settings.savePassword = lokalSettings.value( "savePassword",  "false" ).toString();
		settings.serverURL = lokalSettings.value( "serverURL",  "http://www.applejuicenet.de/18.0.html" ).toString();
		settings.refresh = lokalSettings.value( "refresh", 3 ).toInt();

		settings.ftpServer = lokalSettings.value( "ftpServer", "localhost" ).toString();
		settings.ftpPort = lokalSettings.value( "ftpPort", "21" ).toString();
		settings.ftpUser = lokalSettings.value( "ftpUser", "anonymous" ).toString();
		settings.ftpPassword = lokalSettings.value( "ftpPassword", "" ).toString();
		settings.ftpDir = lokalSettings.value( "ftpDir", "/" ).toString();
		
		optionsDialog->setAjSettings( settings );
	}
}

bool AjQtGUI::login()
{
	ajTab->ajDownloadWidget->clear();
//	ajTab->ajUploadWidget->clear();
	ajTab->ajServerWidget->clear();
	ajTab->ajSearchWidget->clear();
	//xml->clearPendingRequests();
	connected = false;
	progressDialog = new QProgressDialog( tr("please wait") + "...", "cancel", 0, 6, this );
	progressDialog->setMinimumDuration(0);
	progressDialog->setValue( 0 );
	connect( progressDialog, SIGNAL( canceled() ), qApp, SLOT( quit() ) );
	qApp->processEvents();
   xml->get( "getsession" );
	return true;
}

void AjQtGUI::setStatusBarText( QString downSpeed, QString upSpeed, QString credits, QString downSize, QString upSize )
{
	downSpeedLabel->setText( tr("Downstream: ") + QConvert::bytes( downSpeed ) + tr("/s") );
	upSpeedLabel->setText( tr("Upstream: ") + QConvert::bytes( upSpeed ) + tr("/s") );
	creditsLabel->setText( tr("Credits: ") + QConvert::bytesExtra( credits ) );
	downSizeLabel->setText( tr("Downloaded: ") + QConvert::bytesExtra( downSize ) );
	upSizeLabel->setText( tr("Uploaded: ") + QConvert::bytesExtra( upSize ) );
}

void AjQtGUI::xmlError( int code )
{
    connected = false;
	timer->stop();
	partListTimer->stop();
	if( progressDialog != NULL )
	{
		delete progressDialog;
		progressDialog = NULL;
	}
	QSettings lokalSettings;
	QString errorString;
	if( code == 302 )
		errorString = "Either wrong password or connection lost.";
	else
		errorString = xml->errorString() + ".";
	
	QAjLoginDialog *loginDialog = new QAjLoginDialog(
		lokalSettings.value("coreAddress", "localhost").toString(),
		lokalSettings.value("xmlPort", "9851").toString(),
		lokalSettings.value("/progeln.de/AjQtGUI/password", "").toString(),
		errorString, this, "logionDialog" );
	if( loginDialog->exec() == QDialog::Accepted )
	{
		password = loginDialog->getPassword();
		xml->abort();
		xml->setPassword( loginDialog->getPassword() );
		xml->setHost( loginDialog->getHost(), loginDialog->getPort().toInt() );
		lokalSettings.setValue( "coreAddress", loginDialog->getHost() );
		lokalSettings.setValue( "xmlPort", loginDialog->getPort() );
		if( lokalSettings.value( "savePassword", "false" ).toString() == "true" )
			lokalSettings.setValue( "password",  password);
		login();
	}
	delete loginDialog;
}

void AjQtGUI::gotSession()
{
    connected = true;
    QSettings lokalSettings;
    timerSlot();
    timer->setSingleShot( false );
    timer->start( lokalSettings.value( "refresh", 3 ).toInt() * 1000 );
    partListTimer->setSingleShot( false );
    partListTimer->start( 3000 );
}

void AjQtGUI::httpDone( bool error )
{
/*	{
		xml->get( GET_SESSION_XML );
		xml->get( EXTRA_INFORMATION_XML );
		xml->get( GET_SETTINGS_XML );
		xml->get( SHARE_XML );
		xml->get( SEARCH_XML );

		//processQueuedLinks();
		
		connected = true;
		disconnect( ajTab->ajDownloadWidget, SLOT( updateView( ) ) );
		connect( xml, SIGNAL( modifiedDone( ) ), ajTab->ajDownloadWidget, SLOT( updateView( ) ) );
		connect( xml, SIGNAL( modifiedDone( ) ), this, SLOT( firstModified() ) );

		QSettings lokalSettings;
		timer->setSingleShot( false );
		timer->start( lokalSettings.value( "refresh", 3 ).toInt() * 1000 );
		partListIt = ajTab->ajDownloadWidget->getFirstDownload();
		partListEnd = ajTab->ajDownloadWidget->getEndOfDownloads();
	}*/
}

void AjQtGUI::showNetworkInfo()
{
	networkWidget->resize( 300, 200 );
	networkWidget->exec();
}

void AjQtGUI::powerChanged( const QString& )
{
	powerCheck->setChecked( true );
}

void AjQtGUI::applyPowerDownload()
{
	float value;
	if( powerCheck->isChecked() )
		value = powerSpin->value();
	else
		value = 1.0;
	processSelected( SET_POWER_XML, "&Powerdownload=" + QConvert::power( value ) );
}

void AjQtGUI::maxPowerDownload()
{
// 	map<unsigned long, QAjDownloadItem*>::iterator downloadsIt = ajTab->ajDownloadWidget->getFirstDownload();
// 	map<unsigned long, QAjDownloadItem*>::iterator downloadsEnd = ajTab->ajDownloadWidget->getEndOfDownloads();
// 	while( downloadsIt != downloadsEnd )
// 	{
// 		xml->set( SET_POWER_XML, "&Powerdownload=" + QConvert::power( 50 ) + "&id=" + downloadsIt->second->text(ID_DOWN_INDEX)  );
// 		downloadsIt++;
// 	}
}

void AjQtGUI::processSelected( int xmlCode, QString para )
{
	QList<QTreeWidgetItem *>  selectedItems = ajTab->ajDownloadWidget->selectedItems();
	int i;
	for( i=0; i<selectedItems.size(); i++ )
	{
		xml->set( xmlCode, para + "&id=" + selectedItems.at(i)->text(ID_DOWN_INDEX) );
	}
}

void AjQtGUI::requestSelected( QString request, QString para )
{
	QList<QTreeWidgetItem *>  selectedItems = ajTab->ajDownloadWidget->selectedItems();
	int i;
	for( i=0; i<selectedItems.size(); i++ )
	{
		xml->get( request, para + "&id=" + selectedItems.at(i)->text(ID_DOWN_INDEX) );
	}
}

void AjQtGUI::processLink()
{
	QString link = QString( QUrl::toPercentEncoding( ajAddressEdit->text().trimmed() ) );
	xml->set( PROCESS_LINK_XML, "&link=" + link );
	ajAddressEdit->clear();
}

void AjQtGUI::processClipboard()
{
	QString link = QString( QUrl::toPercentEncoding( qApp->clipboard()->text( QClipboard::Clipboard ).trimmed() ) );
	xml->set( PROCESS_LINK_XML, "&link=" + link );
}

void AjQtGUI::downloadSearch()
{
	QList<QTreeWidgetItem *>  selectedItems = ajTab->ajSearchWidget->selectedItems();
	int i;
	for( i=0; i<selectedItems.size(); i++ )
	{
		if( selectedItems.at(i)->text( CHECKSUM_SEARCH_INDEX ) != "" )
		{
			QAjSearchItem *searchItem = (QAjSearchItem*)selectedItems.at(i);
			QString link = "ajfsp://file|";
			link += searchItem->text( TEXT_SEARCH_INDEX );
			link += "|" + searchItem->text( CHECKSUM_SEARCH_INDEX );
			link += "|" + searchItem->size + "/";
			link = QString( QUrl::toPercentEncoding(link) );
			xml->set( PROCESS_LINK_XML, "&link=" +link );
		}
	}
}

void AjQtGUI::cancelDownload()
{
	if( QMessageBox::question( this, "Confirm", "Do you realy want to cancel this download(s)?", QMessageBox::No, QMessageBox::Yes ) == QMessageBox::Yes )
	{
		processSelected( CANCEL_DOWNLOAD_XML );
	}
}

void AjQtGUI::cleanDownload()
{
	xml->set( CLEAN_DOWNLOAD_XML );
}
void AjQtGUI::pauseDownload()
{
	processSelected( PAUSE_DOWNLOAD_XML );
}
void AjQtGUI::resumeDownload()
{
	processSelected( RESUME_DOWNLOAD_XML );
}

void AjQtGUI::partListRequest( )
{
	requestSelected( "downloadpartlist" );
}

void AjQtGUI::renameDownload()
{
	QString oldFilename;
	QString newFilename;
	bool ok;
	QList<QTreeWidgetItem *>  selectedItems = ajTab->ajDownloadWidget->selectedItems();
	int i;
	for( i=0; i<selectedItems.size(); i++ )
	{
			oldFilename = selectedItems.at(i)->text( FILENAME_DOWN_INDEX );
			newFilename = QInputDialog::getText( this, "rename download", "enter new filename for " + oldFilename, QLineEdit::Normal, oldFilename, &ok );
			newFilename = QString( QUrl::toPercentEncoding( newFilename ) );
			if( ok && !newFilename.isEmpty() )
			{
				xml->set( RENAME_DOWNLOAD_XML, "&id=" + selectedItems.at(i)->text( ID_DOWN_INDEX ) + "&name=" + newFilename );
			}
	}

}


void AjQtGUI::removeServer()
{
	QList<QTreeWidgetItem *>  selectedItems = ajTab->ajServerWidget->selectedItems();
	int i;
	for( i=0; i<selectedItems.size(); i++ )
	{
		xml->set( REMOVE_SERVER_XML, "&id=" + selectedItems.at(i)->text(ID_SERVER_INDEX) );
	}
}

void AjQtGUI::connectServer()
{
	QList<QTreeWidgetItem *>  selectedItems = ajTab->ajServerWidget->selectedItems();
	if( ! selectedItems.empty() )
	{
		xml->set( CONNECT_SERVER_XML, "&id=" + selectedItems.first()->text(ID_SERVER_INDEX) );
	}
}

void AjQtGUI::addShare()
{
	QString dir = QFileDialog::getExistingDirectory( this, "Choose a directory" );
	if( dir != "" )
	{
		int result = QMessageBox::question( this, "question", "Share subdirectories?", QMessageBox::Yes, QMessageBox::No );
		QString mode;
		if( result == QMessageBox::Yes )
			mode = "subdirectory";
		else
			mode = "directory";
		ajTab->ajShareWidget->insertShare( dir, mode );
	}
	ajTab->ajShareWidget->setChanged();
	applyShareButton->setEnabled( true );
}

void AjQtGUI::removeShare()
{
	QList<QTreeWidgetItem *>  selectedItems = ajTab->ajShareWidget->selectedItems();
	int i;
	for( i=0; i<selectedItems.size(); i++ )
	{
		//delete it.current();
		selectedItems.at(i)->setFlags( 0 );
	}
	ajTab->ajShareWidget->setChanged();
	applyShareButton->setEnabled( true );
}

void AjQtGUI::applyShare()
{
	QString sharesString;
	int i;
	int cnt = 1;
	for( i=0; i<ajTab->ajShareWidget->topLevelItemCount() ; i++)
	{
		QTreeWidgetItem* item = ajTab->ajShareWidget->topLevelItem( i );
		if( item->flags() & Qt::ItemIsEnabled )
		{
			sharesString += "&sharedirectory" + QString::number(cnt) + "=" + item->text( PATH_SHARE_INDEX );
			sharesString += "&sharesub" + QString::number(cnt) + "=" + ((QAjShareItem*)item)->recursiv;
			cnt++;
		}
	}
	sharesString = "&countshares=" + QString::number( cnt-1 );
	xml->set( SET_SETTINGS_XML, sharesString );
	xml->get( "settings" );
	ajTab->ajShareWidget->setChanged( false );
	applyShareButton->setDisabled( true );
}

QString AjQtGUI::getSelectedDownloads()
{
	QString ids = "";
	QList<QTreeWidgetItem *>  selectedItems = ajTab->ajDownloadWidget->selectedItems();
	int i;
	for( i=0; i<selectedItems.size(); i++ )
	{
		if( i == 0 )
			ids += "&id=" + selectedItems.at(i)->text(ID_DOWN_INDEX);
		else
			ids += "&id" + QString::number(i) + "=" + selectedItems.at(i)->text(ID_DOWN_INDEX);
	}
	return ids;
}

void AjQtGUI::tabChanged( QWidget *tab )
{
	QSettings lokalSettings;
	if( tab != ajTab->ajDownloadWidget )
	{
		downloadToolBar->hide();
		downloadMenuBar->setDisabled( true );
	}
	else
	{
		downloadToolBar->show();
		downloadMenuBar->setEnabled( true );
	}
	if( tab != ajTab->ajUploadWidget )
	{
		uploadToolBar->hide();
	}
	else
	{
		uploadToolBar->show();
		if( ! ajTab->ajUploadWidget->wasSizeAdjusted() )
		{
			ajTab->ajUploadWidget->adjustSizeOfColumns();
		}
	}
	if( tab != ajTab->ajSearchWidget )
	{
		searchToolBar->hide();
		searchMenuBar->setDisabled( true );
	}
	else
	{
		searchToolBar->show();
		searchMenuBar->setEnabled( true );
	}
	if( tab != ajTab->ajServerWidget )
	{
		serverToolBar->hide();
		serverMenuBar->setDisabled( true );
	}
	else
	{
		if( ! ajTab->ajServerWidget->wasSizeAdjusted() )
		{
			ajTab->ajServerWidget->adjustSizeOfColumns();
		}
		serverToolBar->show();
		serverMenuBar->setEnabled( true );
	}
	if( tab != ajTab->ajShareWidget )
	{
		shareToolBar->hide();
		shareMenuBar->setDisabled( true );
	}
	else
	{
		if( ! ajTab->ajShareWidget->wasSizeAdjusted() )
		{
			ajTab->ajShareWidget->adjustSizeOfColumns();
		}
		shareToolBar->show();
		shareMenuBar->setEnabled( true );
	}
	if( (prevTab == ajTab->ajShareWidget) && ( ajTab->ajShareWidget->wasChanged() ) )
	{
		if( QMessageBox::question( this, "question", "You've changed your shares.\nDo you want to transfer it to the core?", QMessageBox::Yes, QMessageBox::No ) == QMessageBox::Yes)
		{
			applyShare();
		}
	}
	if( tab != ajTab->ajFtpWidget )
	{
		ftpToolBar->hide();
	}
	else
	{
		ftpToolBar->show();
		reloadFtp();
	}
	prevTab = tab;
}

void AjQtGUI::downloadSelectionChanged( )
{
	bool oneSelected = false;
	bool onePaused = false;
	bool oneActive = false;
	bool oneFinished = false;
	
	QList<QTreeWidgetItem *>  selectedItems = ajTab->ajDownloadWidget->selectedItems();
	int i;
	for( i=0; i<selectedItems.size(); i++ )
	{
		oneSelected = true;
		QAjDownloadItem *downloadItem = (QAjDownloadItem*)selectedItems.at(i);
		if( downloadItem->getStatus() == DOWN_PAUSED )
			onePaused = true;
		if( ( downloadItem->getStatus() == DOWN_SEARCHING ) || ( downloadItem->getStatus() == DOWN_LOADING ) )
			oneActive = true;
		if( downloadItem->getStatus() == DOWN_FINISHED )
			oneFinished = true;
		if( oneSelected && onePaused && oneActive && oneFinished )
			break;
	}
	cancelDownloadButton->setEnabled( oneSelected );
	partListButton->setEnabled( oneSelected );
	renameDownloadButton->setEnabled( oneSelected );
	saveDownloadButton->setEnabled( oneFinished );
	resumeDownloadButton->setEnabled( onePaused );
	pauseDownloadButton->setEnabled( oneActive );
}

void AjQtGUI::exitCore()
{
	if( QMessageBox::question( this, "Confirm", "Do you realy want to exit the core?\n All your credits will be lost!", QMessageBox::No, QMessageBox::Yes) == QMessageBox::Yes)
	{
		xml->set( EXIT_CORE_XML );
	}
}

void AjQtGUI::clipboardChanged()
{
	clipboardButton->setToolTip( "process: " + qApp->clipboard()->text( QClipboard::Clipboard ) );
}

void AjQtGUI::search()
{
	QString text( QUrl::toPercentEncoding( ajSearchEdit->text() ) );
	xml->set( SEARCH_XML, "&search=" + text );
	ajSearchEdit->clear();
}

void AjQtGUI::cancelSearch()
{
	QList<QTreeWidgetItem *>  selectedItems = ajTab->ajSearchWidget->selectedItems();
	int i;
	for( i=0; i<selectedItems.size(); i++ )
	{
		xml->set( CANCEL_SEARCH_XML, "&id=" + selectedItems.at(i)->text( ID_SEARCH_INDEX ));
	}
}

void AjQtGUI::findServer()
{
	QSettings lokalSettings;
	QString serverURL = lokalSettings.value( "serverURL", "http://www.applejuicenet.de/18.0.html" ).toString();
	
	QString host;
	if( serverURL.indexOf( "http://", 0, Qt::CaseInsensitive ) == 0 )
		host = serverURL.section( '/', 2, 2);
	else
		host = serverURL.section( '/', 0, 0);

	printf("get server from: %s\n", host.toLatin1().data() );

	serverHttp->setHost( host );
	serverHttp->get( serverURL );
}

void AjQtGUI::gotServer( int , bool error )
{
    if( error )
    {
        QMessageBox::critical( NULL, "error", "Could not fetch server source." , QMessageBox::Abort, QMessageBox::Cancel );
    }
    else
    {
        QString data( serverHttp->readAll() );
        // TODO: use regular expressions
        int begin, start, end;
        begin = 0;
        QString link;
        // while we found an ajfsp server link
        while( ( start = data.indexOf( "ajfsp://server|", begin, Qt::CaseInsensitive ) ) != -1 )
        {
            end = data.indexOf( "/", start + 15 );
            link = QString( QUrl::toPercentEncoding( data.mid(start, end - start +1) ) );
            xml->set( PROCESS_LINK_XML, "&link=" + link );
            begin = end;
        }
    }
}

void AjQtGUI::reloadShare()
{
	ajTab->ajShareWidget->clearShares();
	ajTab->ajShareWidget->clear();
	xml->get( "settings" );
	xml->get( "share" );
}

void AjQtGUI::setCoreVersion( QString version )
{
	coreVersionLabel->setText( "Core: " + version );
}

void AjQtGUI::connectedSince( QString since )
{
	if( since != "0" )
	{
		connectedLabel->setText( "connected" );
	}
	else
	{
		connectedLabel->setText( "NOT connected" );
	}
}

void AjQtGUI::firstModified()
{
	if( progressDialog != NULL )
	{
		progressDialog->setValue( progressDialog->value() + 1 );
		qApp->processEvents();
		if( progressDialog->value() == -1 )
		{
			delete progressDialog;
			progressDialog = NULL;
/*			partListIt = ajTab->ajDownloadWidget->getFirstDownload();
			partListEnd = ajTab->ajDownloadWidget->getEndOfDownloads();
			QTimer::singleShot( 100, this, SLOT( partListTimerSlot() ) );*/
			ajTab->ajDownloadWidget->adjustSizeOfColumns();
			ajTab->ajDownloadWidget->sortItems( 0, Qt::AscendingOrder );
		}
      delete progressDialog;
      progressDialog = NULL;
	}
}

void AjQtGUI::linkServerLine( QString line )
{
	line = QString( QUrl::toPercentEncoding( line.trimmed() ) );
	xml->set( PROCESS_LINK_XML, "&link=" + line );
}

void AjQtGUI::processQueuedLinks()
{
	if( queuedLinks != NULL )
	{
		while( queuedLinks->size() > 0 )
		{
			QString link = QString( QUrl::toPercentEncoding( queuedLinks->front() ) );
			xml->set( PROCESS_LINK_XML, "&link=" + link );
			queuedLinks->pop_front();
		}
	}
}

void AjQtGUI::queueLinks( QStringList* links )
{
	queuedLinks = links;
	if( connected )
		processQueuedLinks();
}


/*!
    \fn AjQtGUI::powerChanged( double value )
 */
void AjQtGUI::powerChanged( double value )
{
	applyPowerDownload();
}

void AjQtGUI::storeDownload()
{
	FTP* ftp = NULL;
	QString filename, localDir;
	bool ok;
	QList<QTreeWidgetItem *>  selectedItems = ajTab->ajDownloadWidget->selectedItems();

	QSettings lokalSettings;
	QString server = lokalSettings.value( "ftpServer", "localhost" ).toString();
	int port = lokalSettings.value( "ftpPort", "21" ).toInt();
	QString user = lokalSettings.value( "ftpUser", "anonymous" ).toString();
	QString password = lokalSettings.value( "ftpPassword", "" ).toString();
	QString dir = lokalSettings.value( "ftpDir", "/" ).toString();
	if( ! dir.endsWith( '/' ) )
	{
		dir += '/';
	}
	ftp = new FTP( server, port, user, password, true, this );
	
	int i;
	for( i=0; i<selectedItems.size(); i++ )
	{
		filename = selectedItems.at(i)->text( FILENAME_DOWN_INDEX );
		localDir = QFileDialog::getExistingDirectory( this, "save \"" + filename + "\" + to" );
		if( localDir != "" )
		{
			if( ! localDir.endsWith( QDir::separator() ) )
			{
				localDir += QDir::separator();
			}
			QFile* dstFile = new QFile( localDir + filename );
			if( ! dstFile->exists() )
			{
				dstFile->open( QIODevice::WriteOnly );
				ftp->add( dir + filename, dstFile );
			}
		}
	}
	ftp->start();
}


/*!
    \fn AjQtGUI::reloadFtp()
 */
void AjQtGUI::reloadFtp()
{
	ajTab->ajFtpWidget->clear();
	
	QSettings lokalSettings;
	QString server = lokalSettings.value( "ftpServer", "localhost" ).toString();
	int port = lokalSettings.value( "ftpPort", "21" ).toInt();
	QString user = lokalSettings.value( "ftpUser", "anonymous" ).toString();
	QString password = lokalSettings.value( "ftpPassword", "" ).toString();
	QString dir = lokalSettings.value( "ftpDir", "/" ).toString();

	if( ftp->state() != QFtp::Unconnected )
	{
		ftp->close();
	}
	ftp->connectToHost( server, port );
	ftp->login( user, password );
	ftp->setTransferMode( QFtp::Passive );
	ftp->list( dir );
}


/*!
    \fn AjQtGUI::storeFtp()
 */
void AjQtGUI::storeFtp()
{
	QString filename, localDir;
	bool ok;
	QList<QTreeWidgetItem *>  selectedItems = ajTab->ajFtpWidget->selectedItems();

	QSettings lokalSettings;
	QString server = lokalSettings.value( "ftpServer", "localhost" ).toString();
	int port = lokalSettings.value( "ftpPort", "21" ).toInt();
	QString user = lokalSettings.value( "ftpUser", "anonymous" ).toString();
	QString password = lokalSettings.value( "ftpPassword", "" ).toString();
	QString dir = lokalSettings.value( "ftpDir", "/" ).toString();
	if( ! dir.endsWith( '/' ) )
	{
		dir += '/';
	}
	FTP* ftp = new FTP( server, port, user, password, true, this );
	
	int i;
	for( i=0; i<selectedItems.size(); i++ )
	{
		filename = selectedItems.at(i)->text( FILENAME_FTP_INDEX );
		localDir = QFileDialog::getExistingDirectory( this, "save \"" + filename + "\" + to" );
		if( localDir != "" )
		{
			if( ! localDir.endsWith( QDir::separator() ) )
			{
				localDir += QDir::separator();
			}
			QFile* dstFile = new QFile( localDir + filename );
			if( ! dstFile->exists() )
			{
				dstFile->open( QIODevice::WriteOnly );
				ftp->add( dir + filename, dstFile );
			}
			else
			{
				QMessageBox::critical( this, "error", "\"" + dstFile->fileName() + "\" already exists", QMessageBox::Ok, QMessageBox::NoButton );
			}
		}
	}
	ftp->start();
}
