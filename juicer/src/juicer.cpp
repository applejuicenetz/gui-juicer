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

#include "juicer.h"

Juicer::Juicer( ) : QMainWindow( )
{
#ifdef Q_WS_WIN
    filesystemSeparator = "\\";
#else
    filesystemSeparator = "/";
#endif

    zeroTime = QDateTime( QDate(1970,1,1), QTime(0,0), Qt::UTC );
    QSettings lokalSettings;

    linkServer = new QAjServerSocket( QAjApplication::APP_PORT );
    connect( linkServer, SIGNAL( lineReady( QString ) ), this, SLOT( linkServerLine( QString ) ) );

    setWindowIcon(QIcon(":/juicer.png"));

    xml = new QXMLModule( this );

    QTabWidget* ajTab = new QTabWidget(this);
    ajDownloadWidget = new QAjDownloadWidget( xml, ajTab );
    ajUploadWidget = new QAjUploadWidget( xml, ajTab );
    ajSearchWidget = new QAjSearchWidget( xml, ajTab );

    ajServerMetaWidget = new QAjServerMetaWidget( ajTab );
    ajServerWidget = new QAjServerWidget( xml, ajServerMetaWidget );
    ajServerMetaWidget->setServerWidget( ajServerWidget );

    ajShareMetaWidget = new QAjShareMetaWidget( ajTab );
    ajShareWidget = new QAjShareWidget( filesystemSeparator, xml, ajShareMetaWidget );
    ajShareFilesWidget = new QAjShareFilesWidget( xml, ajShareMetaWidget );
    ajShareMetaWidget->setShareWidget( ajShareWidget );
    ajShareMetaWidget->setShareFilesWidget( ajShareFilesWidget );

    ajTab->setTabToolTip( ajTab->addTab( ajDownloadWidget, QIcon(":/small/down.png"), tr("Downloads") ), "dowloads" );
    ajTab->setTabToolTip( ajTab->addTab( ajUploadWidget, QIcon(":/small/up.png"), tr("Uploads") ), "uploads" );
    ajTab->setTabToolTip( ajTab->addTab( ajSearchWidget, QIcon(":/small/searching.png"), tr("Search") ), "servers" );
    ajTab->setTabToolTip( ajTab->addTab( ajServerMetaWidget, QIcon(":/small/server.png"), tr("Server") ), "searches" );
    ajTab->setTabToolTip( ajTab->addTab( ajShareMetaWidget, QIcon(":/small/shares.png"), tr("Shares") ), "shares" );

    ajIncomingWidget = new QAjIncomingWidget( xml, ajTab );
    ajTab->setTabToolTip( ajTab->addTab( ajIncomingWidget, QIcon(":/small/ftp.png"), tr("Incoming") ), "Incoming" );

    setCentralWidget( ajTab );
    prevTab = ajDownloadWidget;

    networkDialog = new QAjNetworkDialog( this );
    optionsDialog = new QAjOptionsDialog( this );

    initToolBars();

    file = new QMenu( tr("&AppleJuice"), this );
    menuBar()->addMenu( file );

    file->addAction( QIcon(":/small/configure.png"), tr("C&onfigure"), this, SLOT( showOptions() ), QKeySequence( Qt::CTRL+Qt::Key_O ) );
    file->addAction( QIcon(":/small/network.png"), tr("&Net Info"), networkDialog, SLOT( exec() ), QKeySequence( Qt::CTRL+Qt::Key_N ) );
    file->addSeparator();
    file->addAction( QIcon(":/small/exit.png"), tr("&Exit Core"), this, SLOT( exitCore() ), QKeySequence( Qt::CTRL+Qt::Key_E ) );
    file->addAction( QIcon(":/small/close.png"), tr("&Quit GUI"), qApp, SLOT( closeAllWindows() ), QKeySequence( Qt::CTRL+Qt::Key_Q ) );

    downloadMenuBar = menuBar()->addMenu( ajDownloadWidget->popup );
    searchMenuBar = menuBar()->addMenu( ajSearchWidget->popup );
    serverMenuBar = menuBar()->addMenu( ajServerWidget->popup );
    shareMenuBar = menuBar()->addMenu( ajShareWidget->popup );

    menuBar()->addSeparator();

    help = new QMenu( this );
    help->setTitle( tr("&Help") );
    help->addAction( tr("&About"), this, SLOT(about()), QKeySequence( Qt::Key_F1  ) );
    help->addAction( tr("About &Qt"), this, SLOT(aboutQt()) );
    help->addSeparator();

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

    lokalSettings.beginGroup( "MainWindow" );
    resize( lokalSettings.value( "size", QSize(1000, 600) ).toSize() );
    move( lokalSettings.value( "pos", QPoint(100, 100) ).toPoint() );
    lokalSettings.endGroup();

    connect( xml, SIGNAL( settingsReady( AjSettings ) ), this, SLOT( settingsReady( AjSettings ) ) );
    connect( xml, SIGNAL( error( int ) ), this, SLOT( xmlError( int ) ) );
    connect( xml, SIGNAL( gotSession() ), this, SLOT( gotSession() ) );
    connect( xml, SIGNAL( modifiedDone( ) ), ajDownloadWidget, SLOT( updateView( ) ) );
    connect( xml, SIGNAL( modifiedDone( ) ), this, SLOT( firstModified() ) );

    connected = false;
    password = lokalSettings.value( "password" ).toString();
    // no password in local file? => ask for it
    if ( password == NULL )
    {
        bool ok;
        password = QInputDialog::getText( this, "Juicer", "Enter core password:", QLineEdit::Password,  QString::null, &ok );
        if ( !ok ) // user canceld
            qApp->closeAllWindows();
        else
        {
            // save password in local file if user wants it
            if ( lokalSettings.value( "savePassword", "false" ).toString() == "true" )
                lokalSettings.setValue( "password", password );
        }
    }
    xml->setPassword( password );
    xml->setHost( lokalSettings.value("coreAddress", "localhost").toString(),
                  lokalSettings.value("/progeln.de/Juicer/xmlPort", 9851).toInt() );

     timer = new QTimer( this );
    connect( timer, SIGNAL( timeout() ), this, SLOT( timerSlot() ) );
    partListTimer = new QTimer( this );
    connect( partListTimer, SIGNAL( timeout() ), this, SLOT( partListTimerSlot() ) );

    connect( ajTab, SIGNAL( currentChanged( QWidget* ) ), this, SLOT( tabChanged( QWidget* ) ) );

    tabChanged( ajDownloadWidget );

    login();
}

Juicer::~Juicer()
{}

void Juicer::initToolBars()
{
    QToolBar* ajTools = new QToolBar( "applejuice operations", this );
    ajTools->setToolTip( "applejuice operations" );

    ajTools->addAction( QIcon(":/configure.png"), "configure", this, SLOT( showOptions() ) )->setToolTip("configure");
    ajTools->addAction( QIcon(":/network.png"), "aj network info", this, SLOT( showNetworkInfo() ) )->setToolTip("aj network info");

    ajTools->addAction( QIcon(":/adjust.png"), "adjust columns", this, SLOT( adjustColumns() ) )->setToolTip("adjust columns");

    QToolBar* ajLinks = new QToolBar( "applejuice links", this );
    ajLinks->setToolTip( "applejuice links" );

    clipboardButton = ajLinks->addAction( QIcon(":/wizard.png"), "process link from clipboard", this, SLOT( processClipboard() ) );
    clipboardButton->setToolTip( "process link from clipboard" );

    ajAddressLabel = new QLabel(ajLinks);
    ajAddressLabel->setText("ajfsp link:");
    ajAddressLabel->adjustSize();
    ajLinks->addWidget( ajAddressLabel );

    ajAddressEdit = new QLineEdit(ajLinks);
    ajLinks->addWidget( ajAddressEdit );
    connect( ajAddressEdit, SIGNAL( returnPressed() ), this, SLOT( processLink() ) );

    ajLinks->addAction( QIcon(":/ok.png"), "process link", this, SLOT( processLink() ) );


    this->setIconSize( QSize(22, 22) );
    this->addToolBar( ajTools );
    this->addToolBar( ajLinks );
    this->addToolBarBreak( );
    ajDownloadWidget->toolBar->setIconSize( QSize(22, 22) );
    this->addToolBar( ajDownloadWidget->toolBar );
    this->addToolBar( ajUploadWidget->toolBar );
    this->addToolBar( ajSearchWidget->toolBar );
    this->addToolBar( ajServerWidget->toolBar );
    this->addToolBar( ajShareWidget->toolBar );
    this->addToolBar( ajIncomingWidget->toolBar );

}

void Juicer::closeEvent( QCloseEvent* ce )
{
    QSettings lokalSettings;
    lokalSettings.beginGroup("MainWindow");
    lokalSettings.setValue( "size", size() );
    lokalSettings.setValue( "pos", pos() );
    lokalSettings.endGroup();
    #if QT_VERSION < 0x040300
    lokalSettings.setValue( "welcomePos", ajServerMetaWidget->dockWidgetArea(ajServerMetaWidget->dock) );
    lokalSettings.setValue( "filesPos", ajShareMetaWidget->dockWidgetArea(ajShareMetaWidget->dock) );
    #endif
    lokalSettings.setValue( "welcomeVisible", ajServerMetaWidget->dock->enabled );
    lokalSettings.setValue( "filesVisible", ajShareMetaWidget->dock->enabled );
    ce->accept();
}

void Juicer::about()
{
    QMessageBox::about( this, tr("Juicer Info"),
                        tr("Juicer \n\nhttp://ajqtgui.sf.net\nhttp://www.progeln.de"));
}

void Juicer::aboutQt()
{
    QMessageBox::aboutQt( this, tr("Juicer: About Qt") );
}

void Juicer::timerSlot()
{
    if ( xml->session == "" )
        return;
    xml->get( "modified" );
}

void Juicer::partListTimerSlot()
{
    QString id = ajDownloadWidget->getNextIdRoundRobin();
    if (!id.isEmpty())
    {
        xml->get( "downloadpartlist", "&simple&id=" + id);
        ajDownloadWidget->doItemsLayout();
    }
}

void Juicer::showOptions()
{
    xml->get( "settings" );
    if ( optionsDialog->exec() == QDialog::Accepted )
    {
        // save options
        AjSettings settings = optionsDialog->getAjSettings();
        QSettings lokalSettings;
        lokalSettings.setValue( "coreAddress",  settings.coreAddress );
        lokalSettings.setValue( "savePassword",  settings.savePassword );
        lokalSettings.setValue( "showSplash",  settings.showSplash );
        lokalSettings.setValue( "serverURL",  settings.serverURL );
        lokalSettings.setValue( "refresh",  settings.refresh );

        lokalSettings.setValue( "launcher",  settings.launcher );
        lokalSettings.setValue( "location",  settings.location );
        lokalSettings.setValue( "incomingDirSpecific",  settings.incomingDirSpecific );
        lokalSettings.setValue( "tempDirSpecific",  settings.tempDirSpecific );

        lokalSettings.beginGroup( "ftp" );
        lokalSettings.setValue( "server",  settings.ftpServer );
        lokalSettings.setValue( "port",  settings.ftpPort );
        lokalSettings.setValue( "user",  settings.ftpUser );
        lokalSettings.setValue( "password",  settings.ftpPassword );
        lokalSettings.setValue( "dir",  settings.ftpDir );
        lokalSettings.endGroup();

        lokalSettings.setValue( "fetchServersOnStartup",  settings.fetchServersOnStartup );
        lokalSettings.setValue( "language",  settings.language );

        timer->stop();
        timer->setSingleShot( false );
        timer->start( lokalSettings.value( "refresh", 3 ).toInt() * 1000 );

        // save password in local file if user want's it
        if ( settings.savePassword == "true" )
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
        xml->set( "setsettings", settingsString );
    }
}

void Juicer::settingsReady( AjSettings settings )
{
    ajDownloadWidget->setDirs( QFileInfo( settings.tempDir ), QFileInfo( settings.incomingDir ) );
    ajIncomingWidget->setDir( settings.incomingDir );
    if ( optionsDialog != NULL )
    {
        QSettings lokalSettings;
        settings.coreAddress = lokalSettings.value( "coreAddress", "localhost" ).toString();
        settings.savePassword = lokalSettings.value( "savePassword",  "false" ).toString();
        settings.showSplash = lokalSettings.value( "showSplash",  "true" ).toString();
        settings.serverURL = lokalSettings.value( "serverURL",  "http://www.applejuicenet.de/18.0.html" ).toString();
        settings.refresh = lokalSettings.value( "refresh", 3 ).toInt();

        settings.launcher = lokalSettings.value( "launcher", optionsDialog->launchCombo->itemText(0)).toString();

        settings.location = (AjSettings::LOCATION)lokalSettings.value( "location", AjSettings::SAME ).toInt();
        settings.tempDirSpecific = lokalSettings.value( "tempDirSpecific", "/" ).toString();
        settings.incomingDirSpecific = lokalSettings.value( "incomingDirSpecific", "/" ).toString();

        lokalSettings.beginGroup("ftp");
        settings.ftpServer = lokalSettings.value( "server", "localhost" ).toString();
        settings.ftpPort = lokalSettings.value( "port", "21" ).toString();
        settings.ftpUser = lokalSettings.value( "user", "anonymous" ).toString();
        settings.ftpPassword = lokalSettings.value( "password", "" ).toString();
        settings.ftpDir = lokalSettings.value( "dir", "/" ).toString();
        lokalSettings.endGroup();

        settings.fetchServersOnStartup = lokalSettings.value( "fetchServersOnStartup", false ).toBool();
        settings.language = lokalSettings.value( "language", QLocale::system().name() );

        optionsDialog->setAjSettings( settings );
    }
}

bool Juicer::login()
{
    firstModifiedCnt = 0;
    ajDownloadWidget->clear();
//	ajUploadWidget->clear();
    ajServerWidget->clear();
    ajSearchWidget->clear();
    ajIncomingWidget->clear();
    ajShareWidget->clear();
    connected = false;
    qApp->processEvents();
    xml->get( "getsession" );
    return true;
}

void Juicer::setStatusBarText( QString downSpeed, QString upSpeed, QString credits, QString downSize, QString upSize )
{
    downSpeedLabel->setText( tr("Downstream: ") + QConvert::bytes( downSpeed ) + tr("/s") );
    upSpeedLabel->setText( tr("Upstream: ") + QConvert::bytes( upSpeed ) + tr("/s") );
    creditsLabel->setText( tr("Credits: ") + QConvert::bytesExtra( credits ) );
    downSizeLabel->setText( tr("Downloaded: ") + QConvert::bytesExtra( downSize ) );
    upSizeLabel->setText( tr("Uploaded: ") + QConvert::bytesExtra( upSize ) );
}

void Juicer::xmlError( int code )
{
    connected = false;
    timer->stop();
    partListTimer->stop();
    QSettings lokalSettings;
    QString errorString;
    if ( code == 302 )
        errorString = "Either wrong password or connection lost.";
    else
        errorString = xml->errorString() + ".";

    QAjLoginDialog *loginDialog = new QAjLoginDialog(
                                      lokalSettings.value("coreAddress", "localhost").toString(),
                                      lokalSettings.value("xmlPort", "9851").toString(),
                                      lokalSettings.value("/progeln.de/Juicer/password", "").toString(),
                                      errorString, this, "logionDialog" );
    if ( loginDialog->exec() == QDialog::Accepted )
    {
        password = loginDialog->getPassword();
        xml->abort();
        xml->setPassword( loginDialog->getPassword() );
        xml->setHost( loginDialog->getHost(), loginDialog->getPort().toInt() );
        lokalSettings.setValue( "coreAddress", loginDialog->getHost() );
        lokalSettings.setValue( "xmlPort", loginDialog->getPort() );
        if ( lokalSettings.value( "savePassword", "false" ).toString() == "true" )
            lokalSettings.setValue( "password",  password);
        login();
    }
    delete loginDialog;
}

void Juicer::gotSession()
{
    connected = true;
    xml->get( "information" );
    xml->get( "settings" );
    processQueuedLinks();
    QSettings lokalSettings;
    timerSlot();
    timer->setSingleShot( false );
    timer->start( lokalSettings.value( "refresh", 3 ).toInt() * 1000 );
    partListTimer->setSingleShot( false );
    partListTimer->start( 3000 );
    xml->get( "share" );
}

void Juicer::showNetworkInfo()
{
    networkDialog->exec();
}


void Juicer::processLink()
{
    QString link = QString( QUrl::toPercentEncoding( ajAddressEdit->text().trimmed() ) );
    xml->set( "processlink", "&link=" + link );
    ajAddressEdit->clear();
}

void Juicer::processClipboard()
{
    QString link = QString( QUrl::toPercentEncoding( qApp->clipboard()->text( QClipboard::Clipboard ).trimmed() ) );
    xml->set( "processlink", "&link=" + link );
}


void Juicer::tabChanged( QWidget *tab )
{
    QSettings lokalSettings;
    ajDownloadWidget->setActive( tab == ajDownloadWidget );
    ajUploadWidget->setActive( tab == ajUploadWidget );
    ajSearchWidget->setActive( tab == ajSearchWidget );
    ajServerWidget->setActive( tab == ajServerMetaWidget );
    ajShareWidget->setActive( tab == ajShareMetaWidget );
    ajIncomingWidget->setActive( tab == ajIncomingWidget );

    if ( (prevTab == ajShareWidget) && ( ajShareWidget->changed ) )
    {
        if ( QMessageBox::question( this, "question", "You've changed your shares.\nDo you want to transfer it to the core?", QMessageBox::Yes, QMessageBox::No ) == QMessageBox::Yes)
        {
            ajShareWidget->commitSlot();
        }
    }

    if(tab == ajIncomingWidget)
        ajIncomingWidget->reload();

    prevTab = tab;
}

void Juicer::exitCore()
{
    if ( QMessageBox::question( this, "Confirm", "Do you realy want to exit the core?\n All your credits will be lost!", QMessageBox::No, QMessageBox::Yes) == QMessageBox::Yes)
    {
        xml->set( "exitcore" );
    }
}

void Juicer::setCoreVersion( QString version )
{
    coreVersionLabel->setText( "Core: " + version );
}

void Juicer::connectedSince( QString since )
{
    if( since != "0" )
    {
        QString time = zeroTime.addMSecs( since.toULongLong() ).toLocalTime().toString( Qt::LocalDate );
        connectedLabel->setText( "connected since " + time );
    }
    else
    {
        connectedLabel->setText( "NOT connected" );
    }
}

void Juicer::firstModified()
{
    if (firstModifiedCnt < 5)
    {
        if (firstModifiedCnt == 4)
        {
            ajDownloadWidget->updateView( true );
            ajDownloadWidget->adjustSizeOfColumns();
            ajUploadWidget->adjustSizeOfColumns();
            ajServerWidget->adjustSizeOfColumns();
            ajShareWidget->adjustSizeOfColumns();
            ajDownloadWidget->sortItems( 0, Qt::AscendingOrder );
            ajUploadWidget->sortItems( 0, Qt::AscendingOrder );
            ajServerWidget->sortItems( 0, Qt::AscendingOrder );
            ajShareWidget->sortItems( 0, Qt::AscendingOrder );
            this->show();

            QSettings lokalSettings;
            if(lokalSettings.value( "fetchServersOnStartup", false ).toBool())
            {
                ajServerWidget->findSlot();
            }
        }
        firstModifiedCnt++;
    }
}

void Juicer::linkServerLine( QString line )
{
    line = QString( QUrl::toPercentEncoding( line.trimmed() ) );
    xml->set( "processlink", "&link=" + line );
}

void Juicer::processQueuedLinks()
{
    while ( ! queuedLinks.isEmpty() )
    {
        QString link = QString( QUrl::toPercentEncoding( queuedLinks.takeFirst() ) );
        xml->set( "processlink", "&link=" + link );
    }
}

void Juicer::queueLinks( QStringList links )
{
    queuedLinks = links;
}


/*!
    \fn Juicer::setUploadFilename( QString shareId, QString filename )
 */
void Juicer::setUploadFilename( QString shareId, QString filename )
{
    ajUploadWidget->setFilename( shareId, ajDownloadWidget->findDownloadByTempNum( QFileInfo(filename) ) );
}


/*!
    \fn Juicer::adjustColumns()
 */
void Juicer::adjustColumns()
{
    if ( ajDownloadWidget->isVisible() )
        ajDownloadWidget->adjustSizeOfColumns();
    if ( ajUploadWidget->isVisible() )
        ajUploadWidget->adjustSizeOfColumns();
    if ( ajServerWidget->isVisible() )
        ajServerWidget->adjustSizeOfColumns();
    if ( ajShareWidget->isVisible() )
        ajShareWidget->adjustSizeOfColumns();
}


/*!
    \fn Juicer::getExec()
 */
QStringList Juicer::getExec()
{
    QSettings lokalSettings;
    QString launcher = lokalSettings.value( "launcher", DEFAULT_LAUNCHER ).toString().simplified();

    QStringList args;
    if ( launcher == KDE_LAUNCHER )
    {
        args.clear();
        args << "kfmclient";
        args << "exec";
    }
    else if ( launcher == GNOME_LAUNCHER )
    {
        args.clear();
        args << "gnome-open";
    }
    else if ( launcher == MAC_LAUNCHER )
    {
        args.clear();
        args << "open";
    }
    else if ( launcher == WIN_LAUNCHER )
    {
        args.clear();
        args << "start";
        args << "\"\"";
    }
    else
    {
        args << launcher.split(" ");
    }
    return args;
}
