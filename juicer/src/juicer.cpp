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

Juicer::Juicer( QStringList argList, QSplashScreen *splash ) : QMainWindow()
{
    started = false;
    this->splash = splash;
    connect( qApp, SIGNAL( lastWindowClosed () ), this, SLOT ( lastWindowClosed () ) );

    zeroTime = QDateTime( QDate(1970,1,1), QTime(0,0), Qt::UTC );
    firstModifiedMax = 2;// + argList.size();

    linkServer = new QAjServerSocket( QAjApplication::APP_PORT );
    connect( linkServer, SIGNAL( lineReady( QString ) ), this, SLOT( processLink( QString ) ) );

    setWindowIcon(QIcon(":/juicer.png"));

    xml = new QXMLModule( this );

    initTabs();

    networkDialog = new QAjNetworkDialog( this );
    optionsDialog = new QAjOptionsDialog( this );

    initToolBars();
    initMenuBar();
    initStatusBar();

    QSettings lokalSettings;
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

    timer = new QTimer( this );
    connect( timer, SIGNAL( timeout() ), this, SLOT( timerSlot() ) );
    partListTimer = new QTimer( this );
    connect( partListTimer, SIGNAL( timeout() ), this, SLOT( partListTimerSlot() ) );

    connect( ajTab, SIGNAL( currentChanged( int ) ), this, SLOT( tabChanged( int ) ) );

    tabChanged( ajTab->indexOf(ajDownloadWidget) );

    // -- we need to do this as an event, because we can quit the application only if the application is already in the event loop --
    QTimer::singleShot(0, this, SLOT(login()));

    queueLinks( argList );
    initTrayIcon();
    connect( ajDownloadWidget, SIGNAL( downloadsFinished( QList<QAjDownloadItem*>  ) ),this, SLOT( downloadsFinished( QList<QAjDownloadItem*> ) ) );
}

Juicer::~Juicer()
{}

/*!
    \fn Juicer::getPassword()
    gets the password of the core, either from a local file or by asking the user
    @deprecated use showLoginDialog()
    @return the password in clear text
 */
// QString Juicer::getPassword() {
//     QString password = QAjOptionsDialog::getSetting( "password", "" ).toString();
//     // -- no password in local file? => ask for it --
//     if ( password.isEmpty() ) {
//         showLoginDialog("Enter core password:");
//         bool ok;
//         password = QInputDialog::getText( this, "Juicer", tr("Enter core password:"), QLineEdit::Password,  QString::null, &ok );
//         if ( !ok ) { // -- user canceld --
//             qApp->quit();
//         } else {
//             // -- save password in local file if user wants it --
//             if ( QAjOptionsDialog::getSetting( "savePassword", false ).toBool() )
//                 QAjOptionsDialog::setSetting( "password", password );
//         }
//     }
//     return password;
// }


/*!
    \fn Juicer::initToolBars()
    initializes the tool bars
 */
void Juicer::initToolBars()
{
    QToolBar* ajTools = new QToolBar( tr("applejuice operations"), this );
    ajTools->setToolTip( tr("applejuice operations") );

    ajTools->addAction( QIcon(":/configure.png"), tr("configure"), this, SLOT( showOptions() ) )->setToolTip(tr("configure"));
    ajTools->addAction( QIcon(":/network.png"), tr("aj network info"), this, SLOT( showNetworkInfo() ) )->setToolTip(tr("aj network info"));
    ajTools->addAction( QIcon(":/folder_open.png"), tr("Open AJL file"), this, SLOT( openAjL() ) )->setToolTip(tr("Open AJ link list file"));
    
    ajTools->addAction( QIcon(":/adjust.png"), tr("adjust columns"), this, SLOT( adjustColumns() ) )->setToolTip(tr("adjust columns"));

    QToolBar* ajLinks = new QToolBar( tr("applejuice links"), this );
    ajLinks->setToolTip( tr("applejuice links") );

    clipboardButton = ajLinks->addAction( QIcon(":/wizard.png"), tr("process link from clipboard"), this, SLOT( processClipboard() ) );
    clipboardButton->setToolTip( tr("process link from clipboard") );

    ajAddressLabel = new QLabel(ajLinks);
    ajAddressLabel->setText("ajfsp link:");
    ajAddressLabel->adjustSize();
    ajLinks->addWidget( ajAddressLabel );

    ajAddressEdit = new QLineEdit(ajLinks);
    ajLinks->addWidget( ajAddressEdit );
    connect( ajAddressEdit, SIGNAL( returnPressed() ), this, SLOT( processLink() ) );

    ajLinks->addAction( QIcon(":/ok.png"), tr("process link"), this, SLOT( processLink() ) );

    setIconSize( QSize(22, 22) );
    addToolBar( ajTools );
    addToolBar( ajLinks );
    addToolBarBreak( );
    ajDownloadWidget->toolBar->setIconSize( QSize(22, 22) );
    addToolBar( ajDownloadWidget->toolBar );
    addToolBar( ajUploadWidget->toolBar );
    addToolBar( ajSearchWidget->toolBar );
    addToolBar( ajServerWidget->toolBar );
    addToolBar( ajShareWidget->toolBar );
    addToolBar( ajIncomingWidget->toolBar );
}

/*!
    \fn Juicer::initMenuBar()
    initializes the menu bar
 */
void Juicer::initMenuBar() {
    file = new QMenu( tr("&AppleJuice"), this );
    menuBar()->addMenu( file );

    file->addAction( QIcon(":/small/configure.png"), tr("C&onfigure"), this, SLOT( showOptions() ), QKeySequence( Qt::CTRL+Qt::Key_O ) );
    file->addAction( QIcon(":/small/network.png"), tr("&Net Info"), networkDialog, SLOT( exec() ), QKeySequence( Qt::CTRL+Qt::Key_N ) );
    file->addSeparator();
    file->addAction( QIcon(":/small/folder_open.png"), tr("&Open Aj Link List"), this, SLOT( openAjL() ) );
    file->addSeparator();
    file->addAction( QIcon(":/small/exit.png"), tr("&Exit Core"), this, SLOT( exitCore() ), QKeySequence( Qt::CTRL+Qt::Key_E ) );
    file->addAction( QIcon(":/small/close.png"), tr("&Quit GUI"), qApp, SLOT( quit() ), QKeySequence( Qt::CTRL+Qt::Key_Q ) );

    menuBar()->addMenu( ajDownloadWidget->popup );
    menuBar()->addMenu( ajSearchWidget->popup );
    menuBar()->addMenu( ajServerWidget->popup );
    menuBar()->addMenu( ajShareWidget->popup );
    menuBar()->addMenu( ajIncomingWidget->popup );

    menuBar()->addSeparator();

    help = new QMenu( this );
    help->setTitle( tr("&Help") );
    help->addAction( tr("&About"), this, SLOT(about()), QKeySequence( Qt::Key_F1  ) );
    help->addAction( tr("About &Qt"), this, SLOT(aboutQt()) );
    help->addSeparator();

    menuBar()->addMenu( help );
}

/*!
    \fn Juicer::initTabs()
    initializes the tab widgets
 */
void Juicer::initTabs() {
    ajTab = new QTabWidget(this);
    ajDownloadWidget = new QAjDownloadWidget( xml, ajTab );
    ajUploadWidget = new QAjUploadWidget( xml, ajTab );
    ajSearchWidget = new QAjSearchWidget( xml, ajTab );

    ajServerMetaWidget = new QAjServerMetaWidget( ajTab );
    ajServerWidget = new QAjServerWidget( xml, ajServerMetaWidget );
    ajServerMetaWidget->setServerWidget( ajServerWidget );

    ajShareMetaWidget = new QAjShareMetaWidget( ajTab );
    ajShareWidget = new QAjShareWidget( xml, ajShareMetaWidget );
    ajShareFilesWidget = new QAjShareFilesWidget( xml, ajShareMetaWidget );
    ajShareMetaWidget->setShareWidget( ajShareWidget );
    ajShareMetaWidget->setShareFilesWidget( ajShareFilesWidget );

    ajTab->setTabToolTip( ajTab->addTab( ajDownloadWidget, QIcon(":/small/down.png"), tr("Downloads") ), tr("Dowloads") );
    ajTab->setTabToolTip( ajTab->addTab( ajUploadWidget, QIcon(":/small/up.png"), tr("Uploads") ), tr("Uploads") );
    ajTab->setTabToolTip( ajTab->addTab( ajSearchWidget, QIcon(":/small/searching.png"), tr("Search") ), tr("Search") );
    ajTab->setTabToolTip( ajTab->addTab( ajServerMetaWidget, QIcon(":/small/server.png"), tr("Server") ), tr("Servers") );
    ajTab->setTabToolTip( ajTab->addTab( ajShareMetaWidget, QIcon(":/small/shares.png"), tr("Shares") ), tr("Shares") );

    ajIncomingWidget = new QAjIncomingWidget( xml, ajTab );
    ajTab->setTabToolTip( ajTab->addTab( ajIncomingWidget, QIcon(":/small/ftp.png"), tr("Incoming") ), tr("Incoming") );

    setCentralWidget( ajTab );
    prevTab = ajDownloadWidget;
}

/*!
    \fn Juicer::initTrayIcon()
    initializes the tray icon if necessarry
 */
void Juicer::initTrayIcon()
{
    tray = new QSystemTrayIcon( QIcon(":/juicer.png"), this );
    if( QAjOptionsDialog::getSetting( "useTray", false ).toBool() )
    {
        tray->setVisible(true);
        tray->setContextMenu( file );
        connect(tray, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT( trayActivated( QSystemTrayIcon::ActivationReason ) ) );
    } else {
        tray->setVisible(false);
    }
}

/*!
    \fn Juicer::closeEvent( QCloseEvent* ce )
    handler when closing the app, saves a lot of settings
    @param ce the close event
 */
void Juicer::closeEvent( QCloseEvent* ce ) {
    QSettings lokalSettings;
    lokalSettings.beginGroup("MainWindow");
    lokalSettings.setValue( "size", size() );
    lokalSettings.setValue( "pos", pos() );
    lokalSettings.endGroup();
    lokalSettings.beginGroup("WelcomeDock");
    lokalSettings.setValue( "pos", ajServerMetaWidget->dockWidgetArea(ajServerMetaWidget->dock) );
    lokalSettings.setValue( "size", ajServerMetaWidget->dock->size() );
    lokalSettings.setValue( "visible", ajServerMetaWidget->dock->enabled );
    lokalSettings.endGroup();
    lokalSettings.beginGroup("FilesDock");
    lokalSettings.setValue( "pos", ajShareMetaWidget->dockWidgetArea(ajShareMetaWidget->dock) );
    lokalSettings.setValue( "size", ajShareMetaWidget->dock->size() );
    lokalSettings.setValue( "visible", ajShareMetaWidget->dock->enabled );
    lokalSettings.endGroup();
    ajDownloadWidget->saveSortOrder("DownloadWidget");
    ajUploadWidget->saveSortOrder("UploadWidget");
    ajServerWidget->saveSortOrder("ServerWidget");
    ajSearchWidget->saveSortOrder("SearchWidget");
    ajIncomingWidget->saveSortOrder("IncomingWidget");
    ajShareWidget->saveSortOrder("ShareWidget");
    ce->accept();
}


/*!
    \fn Juicer::login()
    logs into the core by requesting a session
    @return always true (may change this)
 */
bool Juicer::login(QString message) {
    ajDownloadWidget->clear();
    ajServerWidget->clear();
    ajSearchWidget->clear();
    ajIncomingWidget->clear();
    ajShareWidget->clear();
    connected = false;
    QString password;
    if(!QAjOptionsDialog::hasSetting("coreAddress") || !QAjOptionsDialog::hasSetting("password")) {
        password = showLoginDialog(message);
    } else {
        password = QAjOptionsDialog::getSetting("password", "").toString();
    }
    QString host = QAjOptionsDialog::getSetting("coreAddress", "localhost").toString();
    int port = QAjOptionsDialog::getSetting("xmlPort", 9851 ).toInt();
    // -- ok -> login --
    if(!password.isEmpty()) {
        firstModifiedCnt = 0;
        xml->setPassword(password);
        xml->setHost(host, port);
        xml->get("getsession");
    // -- no password <- ignore at login --
    } else if(started) {
        optionsDialog->setSettings();
        this->show();
    }
    return true;
}

QString Juicer::showLoginDialog(QString message) {
    if(splash->isVisible()) {
        splash->close();
    }
    QAjLoginDialog loginDialog(this);
    loginDialog.setHost( QAjOptionsDialog::getSetting( "coreAddress", "localhost" ).toString() );
    loginDialog.setPort( QAjOptionsDialog::getSetting("xmlPort", 9851 ).toInt() );
    loginDialog.setPassword( QAjOptionsDialog::getSetting( "password", "" ).toString() );
    loginDialog.setSavePassword( QAjOptionsDialog::getSetting( "savePassword", false ).toBool() );
    loginDialog.setHeader( message );
    int result = loginDialog.exec();
    QString ret = "";
    // -- Ignore --
    if(loginDialog.ignore) {
        // -- force started (don't wait for the core) --
        started = true;
    // -- OK --
    } else if (result == QDialog::Accepted) {
        ret = loginDialog.getPassword();
        QSettings lokalSettings;
        lokalSettings.setValue("coreAddress", loginDialog.getHost());
        lokalSettings.setValue("xmlPort", loginDialog.getPort());
        bool savePassword = loginDialog.getSavePassword();
        lokalSettings.setValue("savePassword", savePassword);
        if(savePassword) {
            lokalSettings.setValue( "password", ret);
        }
    // -- Cancel --
    } else {
        qApp->quit();
    }
    return ret;
}

/*!
    \fn Juicer::timerSlot()
    base timer slot, requests modifications from the core
 */
void Juicer::timerSlot() {
    if(connected) {
        xml->get( "modified" );
    }
}

/*!
    \fn Juicer::partListTimerSlot()
    requests the part list of the 'next' download
 */
void Juicer::partListTimerSlot() {
    if(connected) {
        QString id = ajDownloadWidget->getNextIdRoundRobin();
        if (!id.isEmpty()) {
            xml->get( "downloadpartlist", "&simple&id=" + id);
            ajDownloadWidget->doItemsLayout();
        }
    }
}

void Juicer::showOptions()
{
    if(connected) {
        xml->get( "settings" );
    }
    optionsDialog->setConnected(connected);
    if ( optionsDialog->exec() == QDialog::Accepted ) {
        // save options
        AjSettings settings = optionsDialog->getAjSettings();
        QApplication::setFont(optionsDialog->getFont());
        bool altRows = optionsDialog->altRowsCheckBox->isChecked();
        ajDownloadWidget->setAlternatingRowColors( altRows );
        ajUploadWidget->setAlternatingRowColors( altRows );
        ajServerWidget->setAlternatingRowColors( altRows );
        ajIncomingWidget->setAlternatingRowColors( altRows );
        ajSearchWidget->setAlternatingRowColors( altRows );
        ajShareFilesWidget->setAlternatingRowColors( altRows );
        initStatusBar();

        timer->stop();
        timer->setSingleShot( false );
        timer->start( QAjOptionsDialog::getSetting("refresh", 3).toInt() * 1000 );

        if(connected) {
            QString settingsString = "";
            settingsString += "&Nickname=" + settings.nick;
            settingsString += "&Port=" + settings.tcpPort;
            settingsString += "&XMLPort=" + settings.xmlPort;
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
}

void Juicer::settingsReady( AjSettings settings )
{
    ajDownloadWidget->setDirs( QFileInfo( settings.tempDir ), QFileInfo( settings.incomingDir ) );
    ajIncomingWidget->setDir( settings.incomingDir );
    if ( optionsDialog != NULL )
    {
        optionsDialog->setAjSettings( settings );
//         optionsDialog->setSettings();
    }
}

void Juicer::xmlError( int code )
{
    connected = false;
    timer->stop();
    partListTimer->stop();
    if ( code == 302 ) {
        login("Either wrong password or connection lost.");
    } else {
        login(xml->getErrorString() + ".");
    }
}

void Juicer::gotSession()
{
    connected = true;
    xml->get( "information" );
    xml->get( "settings" );
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

void Juicer::setStatusBarText( QString downSpeed, QString upSpeed, QString credits, QString downSize, QString upSize )
{
    QString downStreamString = tr("Downstream: ") + QConvert::bytes( downSpeed ) + tr("/s");
    QString upStreamString = tr("Upstream: ") + QConvert::bytes( upSpeed ) + tr("/s");
    QString creditsString = tr("Credits: ") + QConvert::bytesExtra( credits );
    QString downSizeString = tr("Downloaded: ") + QConvert::bytesExtra( downSize );
    QString upSizeString = tr("Uploaded: ") + QConvert::bytesExtra( upSize );

    downSpeedLabel->setText( downStreamString );
    upSpeedLabel->setText( upStreamString );
    creditsLabel->setText( creditsString );
    downSizeLabel->setText( downSizeString );
    upSizeLabel->setText( upSizeString );

    // show all information via tray icon
    tray->setToolTip( "Juicer - appleJuice Qt4 GUI\n\n" +
        downStreamString + "\n" +
        upStreamString + "\n" +
        creditsString + "\n" +
        downSizeString + "\n" +
        upSizeString
    );
}

void Juicer::processLink( QString link)
{
    link = QUrl::fromPercentEncoding( link.trimmed().toUtf8() );
    QStringList s = link.split("|");
    if(s.size() > 3)
    {
        QString name = s[1];
        QString hash = s[2];
        QString size = s[3].split("/")[0];
//         printf("%s\n%s\n", hash.toLatin1().data(), size.toLatin1().data() );
        if(s[0].toLower() == "ajfsp://file" ) {
            QAjShareFileItem* file;
            QAjDownloadItem* download;
            if( (file = ajShareFilesWidget->findFile( size, hash )) != NULL )
            {
                QMessageBox::information( this, tr("information"), tr("The file seems to be already in the share")+":\n\n"+file->getFilename());
            }
            else if( (download = ajDownloadWidget->findDownload( size, hash )) != NULL )
            {
                QMessageBox::information( this, tr("information"), tr("The file seems to be already in the download list")+":\n\n"+download->text( FILENAME_DOWN_INDEX ));
            }
        }
        link = s[0] + "|" + QUrl::toPercentEncoding( name )  + "|" + hash + "|" + size + "/";
    }
    xml->set( "processlink", "&link=" + link );
}

void Juicer::processLink()
{
    processLink( ajAddressEdit->text().trimmed() );
    ajAddressEdit->clear();
}

void Juicer::processClipboard()
{
    processLink( qApp->clipboard()->text( QClipboard::Clipboard ).trimmed() );
}


void Juicer::tabChanged( int index )
{
    QWidget *tab = ajTab->widget( index );
    QSettings lokalSettings;
    ajDownloadWidget->setActive( tab == ajDownloadWidget );
    ajUploadWidget->setActive( tab == ajUploadWidget );
    ajSearchWidget->setActive( tab == ajSearchWidget );
    ajServerWidget->setActive( tab == ajServerMetaWidget );
    ajShareWidget->setActive( tab == ajShareMetaWidget );
    ajIncomingWidget->setActive( tab == ajIncomingWidget );

    if ( (prevTab == ajShareWidget) && ( ajShareWidget->changed ) )
    {
        if ( QMessageBox::question( this, tr("question"), tr("You've changed your shares.\nDo you want to transfer the changes to the core?"), QMessageBox::Yes, QMessageBox::No ) == QMessageBox::Yes)
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
    if ( QMessageBox::question( this, tr("Confirm"), tr("Do you realy want to exit the core?\nAll your credits will be lost!"), QMessageBox::No, QMessageBox::Yes) == QMessageBox::Yes)
    {
        xml->set( "exitcore" );
    }
}

void Juicer::setCoreVersion( QString version )
{
    coreVersionLabel->setText( tr("Core: ") + version );
}

void Juicer::connectedSince( QString since )
{
    if( since != "0" )
    {
        QString time = zeroTime.addMSecs( since.toULongLong() ).toLocalTime().toString( Qt::LocalDate );
        connectedLabel->setText( tr("connected since") + " " + time );
    }
    else
    {
        connectedLabel->setText( tr("NOT connected") );
    }
}

void Juicer::firstModified()
{
    if (firstModifiedCnt <= firstModifiedMax)
    {
        if (firstModifiedCnt == firstModifiedMax)
        {
            ajDownloadWidget->updateView( true );
            ajDownloadWidget->adjustSizeOfColumns();
            ajUploadWidget->adjustSizeOfColumns();
            ajServerWidget->adjustSizeOfColumns();
            ajShareWidget->adjustSizeOfColumns();
            ajDownloadWidget->sortItemsInitially("DownloadWidget");
            ajUploadWidget->sortItemsInitially("UploadWidget");
            ajServerWidget->sortItemsInitially("ServerWidget");
            ajShareWidget->sortItemsInitially( "ShareWidget");
            ajIncomingWidget->sortItemsInitially("IncomingWidget");
            ajSearchWidget->sortItemsInitially("SearchWidget");
            processQueuedLinks();
            this->show();
            started = true;
            optionsDialog->setSettings();
            // -- close splash screen if used --
            if(splash->isVisible()) {
                splash->finish(this);
            }
            
            QSettings lokalSettings;
            if(lokalSettings.value( "fetchServersOnStartup", false ).toBool())
            {
                ajServerWidget->findSlot();
            }
        }
        firstModifiedCnt++;
    }
}

void Juicer::processQueuedLinks()
{
    while ( ! queuedLinks.isEmpty() )
        processLink( queuedLinks.takeFirst() );
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
    if ( ajShareMetaWidget->sharedFilesWidget->isVisible() )
        ajShareMetaWidget->sharedFilesWidget->adjustSizeOfColumns();
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


/*!
    \fn Juicer::initStatusBar()
 */
void Juicer::initStatusBar()
{
    static bool first = true;
    if(first) {
        downSpeedLabel = new QAjIconWidget(":/small/downstream.png", "0", QBoxLayout::LeftToRight, this, 2, 2);
        upSpeedLabel = new QAjIconWidget(":/small/upstream.png", "0", QBoxLayout::LeftToRight, this, 2, 2);
        creditsLabel = new QAjIconWidget(":/small/credits.png", "0", QBoxLayout::LeftToRight, this, 2, 2);
        downSizeLabel = new QAjIconWidget(":/small/downloaded.png", "0", QBoxLayout::LeftToRight, this, 2, 2);
        upSizeLabel = new QAjIconWidget(":/small/uploaded.png", "0", QBoxLayout::LeftToRight, this, 2, 2);
        coreVersionLabel = new QAjIconWidget(":/small/version.png", "0", QBoxLayout::LeftToRight, this, 2, 2);
        connectedLabel = new QAjIconWidget(":/small/connected.png", "0", QBoxLayout::LeftToRight, this, 2, 2);
        statusBar()->addPermanentWidget( connectedLabel );
        statusBar()->addPermanentWidget( coreVersionLabel );
        statusBar()->addPermanentWidget( downSpeedLabel );
        statusBar()->addPermanentWidget( upSpeedLabel );
        statusBar()->addPermanentWidget( downSizeLabel );
        statusBar()->addPermanentWidget( upSizeLabel );
        statusBar()->addPermanentWidget( creditsLabel );
        first = false;
    }
    QStringList show = QAjOptionsDialog::getSetting( "statusbarComponents", optionsDialog->getDefaultStatusbarComponents() ).toStringList();
    connectedLabel->setVisible(show.contains(CONNECTED_SINCE));
    coreVersionLabel->setVisible(show.contains(CORE_VERSION));
    downSpeedLabel->setVisible(show.contains(DOWNSTREAM));
    upSpeedLabel->setVisible(show.contains(UPSTREAM));
    downSizeLabel->setVisible(show.contains(DOWNLOADED));
    upSizeLabel->setVisible(show.contains(UPLOADED));
    creditsLabel->setVisible(show.contains(CREDITS));

}


/*!
    \fn Juicer::trayActivated( QSystemTrayIcon::ActivationReason reason )
 */
void Juicer::trayActivated( QSystemTrayIcon::ActivationReason reason )
{
    if(reason == QSystemTrayIcon::Trigger)
    {
        this->setVisible(!this->isVisible());
    }
}


/*!
    \fn Juicer::lastWindowClosed()
 */
void Juicer::lastWindowClosed()
{
   if( !tray->isVisible() ) {
        if(started) {
            qApp->quit();
        }
    } else {
        delete this;
    }
}


/*!
    \fn Juicer::downloadsFinished( QList<QAjDownloadItem*> list )
    show a message with all finished downloads in tray icon
    @param list list with all finished downloads to show
 */
void Juicer::downloadsFinished( QList<QAjDownloadItem*> list )
{
    if( QSystemTrayIcon::supportsMessages() )
    {
        QString msg = "";
        int i;
        for( i=0; i<list.size(); i++ )
        {
            msg += list[i]->text( FILENAME_DOWN_INDEX ) + "\n";
        }
        tray->showMessage( tr("Download finished"), msg, QSystemTrayIcon::Information, 3000 );
    }
}

/*!
    \fn Juicer::openAjL()
 */
void Juicer::openAjL()
{
    QString ajListFileName = QFileDialog::getOpenFileName(
                              this,
                              tr("Select AJ link list file"),
                              QString::null,
                              tr("AJ Link Lists (*.ajl)"));

    if ( !ajListFileName.isNull() ) {

        QFile *ajListFile = new QFile( ajListFileName );

        if ( ajListFile->exists() ) {
            if (!ajListFile->open(QIODevice::ReadOnly | QIODevice::Text)) {
                tray->showMessage( tr("Error while opening file"), ajListFileName, QSystemTrayIcon::Information, 3000 );
            }
            else {
                QTextStream in(ajListFile);
                QString line = in.readLine();
                while ( line.compare("100") != 0) {
                    if ( in.atEnd() ) {
                        tray->showMessage( tr("No valid AJ list file"), ajListFileName,
                                          QSystemTrayIcon::Information, 3000 );
                        break;
                    }
                    line = in.readLine();
                }
                if ( !in.atEnd() ) {
                    while (!in.atEnd()) {
                        QString filename = in.readLine();
                        QString filehash = in.readLine();
                        QString filesize = in.readLine();

                        QString link = "ajfsp://file|" + filename + "|" + filehash + "|" + filesize + "/";

                        this->processLink( link );
                    }

                    tray->showMessage( tr("Files successfully added from link list"),
                                      ajListFileName, QSystemTrayIcon::Information, 3000 );
                }

                ajListFile->close();
            }
        }
        else {
            tray->showMessage( tr("No such file"), ajListFileName, QSystemTrayIcon::Information, 3000 );
        }

        delete ajListFile;
    }

}

/*!
 * @deprecated
 */
void Juicer::createAjL( QList<QAjItem *>  selectedItems )
{
    QString ajListFileName = QFileDialog::getSaveFileName(
                              this,
                              tr("Enter file name"),
                              QString::null,
                              tr("AJ Link Lists (*.ajl)"));

    if ( !ajListFileName.endsWith(".ajl") ) {
        ajListFileName += ".ajl";
        tray->showMessage( tr("Filename of link list changed"), tr("changed to: ") + ajListFileName, QSystemTrayIcon::Information, 3000 );
    }

    if ( !ajListFileName.isNull() ) {

        QFile *ajListFile = new QFile( ajListFileName );

        if ( ajListFile->exists() ) {
            ajListFile->remove();
        }


        if (ajListFile->open(QIODevice::WriteOnly | QIODevice::Text)) {

            QString message = "appleJuice link list\nCreated by Juicer, the appleJuice GUI based on Qt4.\n\n";
            message += "The developers of Juicer take no responsibility for the files listed below!\n";
            ajListFile->write( message.toAscii());
            ajListFile->write( "100\n" );

            ajListFile->setPermissions( QFile::ReadOwner | QFile::WriteOwner |
                                        QFile::ReadUser | QFile::WriteUser |
                                        QFile::ReadGroup | QFile::ReadOther );

            for ( int i=0; i<selectedItems.size(); i++ ) {

//                 QString filename = selectedItems[i]->text(FILENAME_DOWN_INDEX) + '\n';
//                 QString filehash = selectedItems[i]->getHash() + '\n';
//                 QString filesize = QString::number( (int)selectedItems[i]->getSize() ) + '\n';

                ajListFile->write( QString( selectedItems[i]->getFilename() + '\n' ).toAscii());
                ajListFile->write( QString( selectedItems[i]->getHash() + '\n' ).toAscii());
                ajListFile->write( QString( QString::number( (int)selectedItems[i]->getSize() ) + '\n' ).toAscii());
            }

            ajListFile->flush();
            ajListFile->close();

            tray->showMessage( tr("AJ link list successfully created"), ajListFileName, QSystemTrayIcon::Warning, 3000 );
        }
        else {
            QFile::FileError errorCode = ajListFile->error();
            QString error;
            switch ( errorCode ) {
                case QFile::ReadError: error = tr("An error occurred when reading from the file.");
                                       break;
                case QFile::WriteError: error = tr("An error occurred when writing to the file.");
                                       break;
                case QFile::FatalError: error = tr("A fatal error occurred.");
                                       break;
                case QFile::ResourceError: error = tr("Not enough disk space.");
                                       break;
                case QFile::OpenError: error = tr("The file could not be opened.");
                                       break;
                case QFile::AbortError: error = tr("The operation was aborted.");
                                       break;
                case QFile::TimeOutError: error = tr("A timeout occurred.");
                                       break;
                case QFile::PermissionsError: error = tr("The file could not be accessed.");
                                       break;
                default: error = tr("An unspecified error occurred.");
            }
            tray->showMessage( tr("Error while saving link list."), tr("The error message was:\n\n") + error, QSystemTrayIcon::Information, 3000 );
        }

        delete ajListFile;
    }

}

void Juicer::sendToTray( QString message1, QString message2 ) {
    tray->showMessage( message1, message2, QSystemTrayIcon::Information, 3000 );
}


void Juicer::about() {
    QMessageBox::about( this, tr("Juicer Info"),
                        tr("Juicer \n\nhttp://ajqtgui.sf.net"));
}

void Juicer::aboutQt() {
    QMessageBox::aboutQt( this, tr("Juicer: About Qt") );
}
