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
#include "handlerdialog.h"

Juicer::Juicer( const QStringList& argList, QSplashScreen *splash )
    : QMainWindow()
    , xml(0)
    , downloadModule(0)
    , uploadModule(0)
    , networkDialog(0)
    , searchModule(0)
    , serverModule(0)
    , shareModule(0)
    , incomingModule(0)
    , started(false)
    , connected(false)
    , localCore(false)
{
    setupUi( this );

    downloads->setCentralWidget(downloadsTreeWidget);
    downloads->addDockWidget(Qt::RightDockWidgetArea, partListDock);

    server->setCentralWidget(serverTreeWidget);
    server->addDockWidget(Qt::RightDockWidgetArea, welcomeDock);

    this->splash = splash;

    firstModifiedMax = 2;// + argList.size();

    linkServer = new ServerSocket( Application::APP_PORT );
    connect( linkServer, SIGNAL( lineReady( QString ) ), this, SLOT( processLink( QString ) ) );

//     setWindowIcon(QIcon(":/juicer.png"));
    osIcons[LINUX] = QIcon(":/small/linux.png");
    osIcons[WINDOWS] = QIcon(":/small/windows.png");
    osIcons[MAC] = QIcon(":/small/mac.png");
    osIcons[SOLARIS] = QIcon(":/small/solaris.png");
    osIcons[FREEBSD] = QIcon(":/small/freebsd.png");
    osIcons[NETWARE] = QIcon(":/small/netware.png");


    xml = new QXMLModule(this);
    downloadModule = new DownloadModule(this);
    uploadModule = new UploadModule(this);
    searchModule = new SearchModule(this);
    serverModule = new ServerModule(this);
    shareModule = new ShareModule(this);
    incomingModule = new IncomingModule(this);

    prevTab = downloads;

    networkDialog = new NetworkDialog( this );
    optionsDialog = new OptionsDialog( this );

    initToolBars();
    connectActions();
    initStatusBar();

    resize(OptionsDialog::getSetting("MainWindow", "size", QSize(1000, 600)).toSize());
    move(OptionsDialog::getSetting("MainWindow", "pos", QPoint(100, 100)).toPoint());
    restoreState(OptionsDialog::getSetting("JuicerMain").toByteArray());
    downloads->restoreState(OptionsDialog::getSetting("DownloadsMain").toByteArray());
    server->restoreState(OptionsDialog::getSetting("ServerMain").toByteArray());

    connect( xml, SIGNAL( settingsReady( const AjSettings& ) ), this, SLOT( settingsReady( const AjSettings& ) ) );
    connect( xml, SIGNAL( error( QString ) ), this, SLOT( xmlError( QString ) ) );
    connect( xml, SIGNAL( gotSession() ), this, SLOT( gotSession() ) );
    connect( xml, SIGNAL( modifiedDone( ) ), downloadModule, SLOT( updateView( ) ) );
    connect( xml, SIGNAL( modifiedDone( ) ), this, SLOT( firstModified() ) );

    timer = new QTimer( this );
    partListTimer = new QTimer( this );
    connect( timer, SIGNAL( timeout() ), this, SLOT( timerSlot() ) );
    connect( partListTimer, SIGNAL( timeout() ), this, SLOT( partListTimerSlot() ) );
    connect( partListTimer, SIGNAL( timeout() ), downloadModule, SLOT( partListSlot() ) );

    connect( ajTab, SIGNAL( currentChanged( int ) ), this, SLOT( tabChanged( int ) ) );

    tabChanged( ajTab->indexOf(downloads) );

    // -- we need to do this as an event, because we can quit the application only if the application is already in the event loop --
    QTimer::singleShot(0, this, SLOT(login()));

    queueLinks( argList );
    initTrayIcon();
    connect(downloadModule, SIGNAL( downloadsFinished( const QList<DownloadItem*>&  ) ),this, SLOT( downloadsFinished( const QList<DownloadItem*>& ) ) );
}

Juicer::~Juicer() {
}

/*!
    \fn Juicer::initToolBars()
    initializes the tool bars
 */
void Juicer::initToolBars()
{
    ajAddressLabel = new QLabel(ajLinks);
    ajAddressLabel->setText("ajfsp link:");
    ajAddressLabel->adjustSize();
    ajLinks->addWidget( ajAddressLabel );

    ajAddressEdit = new QLineEdit(ajLinks);
    ajLinks->addWidget( ajAddressEdit );
    connect( ajAddressEdit, SIGNAL( returnPressed() ), this, SLOT( processLink() ) );

    ajLinks->addAction( QIcon(":/ok.png"), tr("process link"), this, SLOT( processLink() ) );
}

/*!
    \fn Juicer::connectActions()
 */
void Juicer::connectActions() {
    connect(actionConfigure, SIGNAL(triggered()), this, SLOT(showOptions()));
    connect(actionNet_Info, SIGNAL(triggered()), networkDialog, SLOT(exec()));
    connect(actionOpen_Aj_Link_List, SIGNAL(triggered()), this, SLOT(openAjL()));
    connect(actionAdjust_Columns, SIGNAL(triggered()), this, SLOT( adjustColumns()));
    connect(actionProcess_Link_From_Clipboard, SIGNAL(triggered()), this, SLOT(processClipboard()));
    connect(actionExit_Core, SIGNAL(triggered()), this, SLOT(exitCore()));
    connect(actionQuit_GUI, SIGNAL(triggered()), this, SLOT(quit()));
    connect(actionAbout, SIGNAL(triggered()), this, SLOT(about()));
    connect(actionAbout_Qt, SIGNAL(triggered()), this, SLOT(aboutQt()));
}

/*!
    \fn Juicer::initTrayIcon()
    initializes the tray icon if necessarry
 */
void Juicer::initTrayIcon()
{
    tray = new QSystemTrayIcon( QIcon(":/juicer.png"), this );
    if( OptionsDialog::getSetting( "useTray", false ).toBool() )
    {
        tray->setVisible(true);
        tray->setContextMenu(menuAppleJuice);
        connect( tray, SIGNAL( activated( QSystemTrayIcon::ActivationReason ) ),
                 this, SLOT( trayActivated( QSystemTrayIcon::ActivationReason ) ) );
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
     if ( tray->isVisible() && !isMinimized() ) {

        HandlerDialog trayDialog(
                tr("Minimizing to tray"),
                tr("Tray Icon is enabled so Juicer runs minimized in the background.\nUse Quit GUI to close the GUI."),
                QDialogButtonBox::Ok,
                QStyle::SP_MessageBoxInformation,
                this);
        trayDialog.exec("minimize");
        showMinimized();
        setHidden( true );
        ce->ignore();
     } else {
        // make sure tray icon disappears when quit (Windows problem)
        if ( true == tray->isVisible() ) tray->setVisible(false); // <2DO> doesn't work; possible qt bug?
        quit();
        ce->accept();
    }
}

void Juicer::quit() {
    saveGUIState();
    downloads->close();
    server->close();
    qApp->quit();
}

void Juicer::saveGUIState() {
    OptionsDialog::setSetting("MainWindow", "size", size());
    OptionsDialog::setSetting("MainWindow", "pos", pos());
    downloadModule->saveSortOrder("DownloadWidget");
    uploadModule->saveSortOrder("UploadWidget");
    searchModule->saveSortOrder("SearchWidget");
    serverModule->saveSortOrder("ServerWidget");
    shareModule->saveSortOrder("ShareWidget");
    incomingModule->saveSortOrder("IncomingWidget");

    OptionsDialog::setSetting("JuicerMain", this->saveState());
    OptionsDialog::setSetting("DownloadsMain", downloads->saveState());
    OptionsDialog::setSetting("ServerMain", server->saveState());
}

/*!
    \fn Juicer::login()
    logs into the core by requesting a session
    @param message text to show at the login dialog
    @param error is this an retry? (force showing the login dialog)
    @return always true (may change this)
 */
bool Juicer::login(const QString& message, bool error) {
    connected = false;
    QString password;
    if(error || !OptionsDialog::hasSetting("coreAddress") || !OptionsDialog::hasSetting("password")) {
        password = showLoginDialog(message);
    } else {
        password = OptionsDialog::getSetting("password", "").toString();
    }
    QString host = OptionsDialog::getSetting("coreAddress", "localhost").toString();
    int port = OptionsDialog::getSetting("xmlPort", 9851 ).toInt();
    // -- ok -> login --
    if( !password.isEmpty() || isLocalhost( host ) ) {
        firstModifiedCnt = 0;
        xml->setPassword(password);
        xml->setHost(host, port);
        xml->get("getsession");
        // -- resolve hostname of core to determine incoming location --
        QHostInfo::lookupHost(host, this, SLOT(hostLookedUp(QHostInfo)));
    // -- ignore at login --
    } else if(started) {
        optionsDialog->setSettings();
        this->show();
    } else {
        login("empty password", true);
    }
    return true;
}

bool Juicer::isLocalhost( const QString& host )
{
    return ( QHostInfo::fromName( host ).hostName() == "localhost" );
}

QString Juicer::showLoginDialog(const QString& message) {
    if(splash->isVisible()) {
        splash->close();
    }
    LoginDialog loginDialog(this);
    loginDialog.setHost( OptionsDialog::getSetting( "coreAddress", "localhost" ).toString() );
    loginDialog.setPort( OptionsDialog::getSetting("xmlPort", 9851 ).toInt() );
    loginDialog.setPassword( OptionsDialog::getSetting( "password", "" ).toString() );
    loginDialog.setSavePassword( OptionsDialog::getSetting( "savePassword", false ).toBool() );
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

void Juicer::xmlError(const QString& reason) {
    connected = false;
    timer->stop();
    partListTimer->stop();
    if ( shareModule != NULL ) shareModule->reset();
    login(reason, true);
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
        QString id = downloadModule->getNextIdRoundRobin();
        if (!id.isEmpty()) {
            xml->get( "downloadpartlist", "&simple&id=" + id);
            downloadsTreeWidget->doItemsLayout();
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
        downloadsTreeWidget->setAlternatingRowColors( altRows );
        uploadsTreeWidget->setAlternatingRowColors( altRows );
        serverTreeWidget->setAlternatingRowColors( altRows );
        searchsTreeWidget->setAlternatingRowColors( altRows );
        sharesTreeWidget->setAlternatingRowColors( altRows );
        incomingTreeWidget->setAlternatingRowColors( altRows );
        initStatusBar();

        timer->stop();
        timer->setSingleShot( false );
        timer->start( OptionsDialog::getSetting("refresh", 3).toInt() * 1000 );

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

void Juicer::settingsReady( const AjSettings& settings )
{
    downloadModule->setDirs( settings.tempDir, settings.incomingDir );
    incomingModule->setDir( settings.incomingDir );
    if ( optionsDialog != NULL )
    {
        optionsDialog->setAjSettings( settings );
//         optionsDialog->setSettings();
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

void Juicer::setStatusBarText( const QString& downSpeed, const QString& upSpeed, const QString& credits, const QString& downSize, const QString& upSize )
{
    QString downStreamString = tr("Downstream: %1/s").arg(QConvert::bytes( downSpeed ));
    QString upStreamString = tr("Upstream: %1/s").arg(QConvert::bytes( upSpeed ));
    QString creditsString = tr("Credits: %1").arg(QConvert::bytesExtra( credits ));
    QString downSizeString = tr("Downloaded: %1").arg(QConvert::bytesExtra( downSize ));
    QString upSizeString = tr("Uploaded: %1").arg(QConvert::bytesExtra( upSize ));

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

void Juicer::processLink(const QString& link) {
    QString encodedLink = QUrl::fromPercentEncoding(link.trimmed().toUtf8());
    QStringList s = encodedLink.split("|");
    if(s.size() > 3) {
        QString name = s[1];
        QString hash = s[2];
        QString size = s[3].split("/")[0];
        if(s[0].toLower() == "ajfsp://file") {
            ShareFileItem* file;
            DownloadItem* download;
            if((file = shareModule->findFile( size, hash )) != NULL) {
                QMessageBox::information( this, tr("Information"), tr("The file seems to be already in the share\n\n%1").arg(file->getFilename()));
            } else if((download = downloadModule->findDownload(size, hash)) != NULL) {
                QMessageBox::information( this, tr("Information"),
                    tr("The file seems to be already in the download list\n\n%1").arg(download->text(DownloadItem::FILENAME_COL)));
            }
        }
        encodedLink = s[0] + "|" + QUrl::toPercentEncoding( name )  + "|" + hash + "|" + size + "/";
    }
    xml->set("processlink", "&link=" + encodedLink);
}

void Juicer::processLink() {
    processLink(ajAddressEdit->text().trimmed());
    ajAddressEdit->clear();
}

void Juicer::processClipboard() {
    processLink(qApp->clipboard()->text(QClipboard::Clipboard));
}


void Juicer::tabChanged( int index ) {
    QWidget *tab = ajTab->widget( index );

    downloadToolBar->setVisible(tab == downloads);
    uploadToolBar->setVisible(tab == uploads);
    searchToolBar->setVisible(tab == search);
    serverToolBar->setVisible(tab == server);
    shareToolBar->setVisible(tab == shares);
    incomingToolBar->setVisible(tab == incoming);

    if((prevTab == shares) && (shareModule->isChanged()) && 
       (QMessageBox::question( this, tr("Question"), tr("You've changed your shares.\nDo you want to transfer the changes to the core?"), QMessageBox::Yes, QMessageBox::No ) == QMessageBox::Yes)) {
        shareModule->commitSlot();
    }

    if(tab == incoming) {
        incomingModule->reload();
    }

    prevTab = tab;
}

void Juicer::exitCore()
{
    if ( QMessageBox::question( this, tr("Confirm"), tr("Do you realy want to exit the core?\nAll your credits will be lost!"), QMessageBox::No, QMessageBox::Yes) == QMessageBox::Yes)
    {
        xml->set( "exitcore" );
    }
}

void Juicer::setCoreVersion(const QString& version) {
    coreVersionLabel->setText(tr("Core: %1").arg(version));
}

void Juicer::connectedSince(const QString& since) {
    QDateTime& time = serverModule->setConnectedSince(since);
    if(time.isValid()) {
        connectedLabel->setText(tr("connected since %1").arg(time.toLocalTime().toString(Qt::LocalDate)));
    } else {
        connectedLabel->setText(tr("NOT connected"));
    }
}

void Juicer::firstModified() {
    if (firstModifiedCnt <= firstModifiedMax) {
        if (firstModifiedCnt == firstModifiedMax) {
            downloadModule->updateView( true );
            downloadModule->adjustSizeOfColumns();
            uploadModule->adjustSizeOfColumns();
            searchModule->adjustSizeOfColumns();
            serverModule->adjustSizeOfColumns();
            shareModule->adjustSizeOfColumns();
            downloadModule->sortItemsInitially("DownloadWidget");
            uploadModule->sortItemsInitially("UploadWidget");
            serverModule->sortItemsInitially("ServerWidget");
            shareModule->sortItemsInitially( "ShareWidget");
            incomingModule->sortItemsInitially("IncomingWidget");
            searchModule->sortItemsInitially("SearchWidget");
            processQueuedLinks();
            this->show();
            started = true;
            optionsDialog->setSettings();
            // -- close splash screen if used --
            if(splash->isVisible()) {
                splash->finish(this);
            }
            QSettings lokalSettings;
            if(lokalSettings.value("fetchServersOnStartup", false).toBool()) {
                serverModule->searchSlot();
            }
            // -- if the core is not on localhost, warn the user --
            if(!localCore && optionsDialog->sameComputerRadio->isChecked()) {
                HandlerDialog localCoreDialog(
                    "Information",
                    tr("The Core is not running on the local machine. In order to use the full functionality like directly opening downloads or the incoming view you have to specify the incoming and temporary directory in the options menu."),
                    QDialogButtonBox::Ok, QStyle::SP_MessageBoxInformation);
                localCoreDialog.exec("localCore");
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

void Juicer::queueLinks( const QStringList& links )
{
    queuedLinks = links;
}


/*!
    \fn Juicer::setUploadFilename( const QString& shareId, const QString& filename )
 */
void Juicer::setUploadFilename( const QString& shareId, const QString& filename )
{
      uploadModule->setFilename( shareId, downloadModule->findDownloadByTempNum( filename ) );
}


/*!
    \fn Juicer::adjustColumns()
 */
void Juicer::adjustColumns()
{
    if(downloads->isVisible())
        downloadModule->adjustSizeOfColumns();
    if(uploads->isVisible())
        uploadModule->adjustSizeOfColumns();
    if(search->isVisible())
        searchModule->adjustSizeOfColumns();
    if(server->isVisible())
        serverModule->adjustSizeOfColumns();
    if(shares->isVisible())
        shareModule->adjustSizeOfColumns();
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
        downSpeedLabel = new IconWidget(":/small/downstream.png", "0", QBoxLayout::LeftToRight, this, 2, 2);
        upSpeedLabel = new IconWidget(":/small/upstream.png", "0", QBoxLayout::LeftToRight, this, 2, 2);
        creditsLabel = new IconWidget(":/small/credits.png", "0", QBoxLayout::LeftToRight, this, 2, 2);
        downSizeLabel = new IconWidget(":/small/downloaded.png", "0", QBoxLayout::LeftToRight, this, 2, 2);
        upSizeLabel = new IconWidget(":/small/uploaded.png", "0", QBoxLayout::LeftToRight, this, 2, 2);
        coreVersionLabel = new IconWidget(":/small/version.png", "0", QBoxLayout::LeftToRight, this, 2, 2);
        connectedLabel = new IconWidget(":/small/connected.png", "0", QBoxLayout::LeftToRight, this, 2, 2);
        statusBar()->addPermanentWidget( connectedLabel );
        statusBar()->addPermanentWidget( coreVersionLabel );
        statusBar()->addPermanentWidget( downSpeedLabel );
        statusBar()->addPermanentWidget( upSpeedLabel );
        statusBar()->addPermanentWidget( downSizeLabel );
        statusBar()->addPermanentWidget( upSizeLabel );
        statusBar()->addPermanentWidget( creditsLabel );
        first = false;
    }
    QStringList show = OptionsDialog::getSetting( "statusbarComponents", optionsDialog->getDefaultStatusbarComponents() ).toStringList();
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
    if( reason == QSystemTrayIcon::Trigger )
    {
        setVisible( !isVisible() );
        if ( isMinimized() ) {
            showNormal();
            activateWindow();    // make widget active, otherwise one must first click at the tab in the task bar to get Juicer in front
        }
    }
}


/*!
    \fn Juicer::downloadsFinished( const QList<DownloadItem*>& list )
    show a message with all finished downloads in tray icon
    @param list list with all finished downloads to show
 */
void Juicer::downloadsFinished( const QList<DownloadItem*>& list )
{
    if( QSystemTrayIcon::supportsMessages() )
    {
        QString msg = "";
        int i;
        for( i=0; i<list.size(); i++ )
        {
            msg += list[i]->text( DownloadItem::FILENAME_COL ) + "\n";
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

        QFile ajListFile( ajListFileName );

        if ( ajListFile.exists() ) {
            if (!ajListFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
                tray->showMessage( tr("Error while opening file"), ajListFileName, QSystemTrayIcon::Information, 3000 );
            }
            else {
                QTextStream in(&ajListFile);
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

                ajListFile.close();
            }
        }
        else {
            tray->showMessage( tr("No such file"), ajListFileName, QSystemTrayIcon::Information, 3000 );
        }
    }

}

void Juicer::sendToTray( const QString& message1, const QString& message2 ) {
    tray->showMessage( message1, message2, QSystemTrayIcon::Information, 3000 );
}


void Juicer::about() {
    QMessageBox::about( this, tr("Juicer Info"),
                        tr("Juicer\n\nGUI for appleJuice Filesharing\n\nURL: http://ajqtgui.sf.net"));
}

void Juicer::aboutQt() {
    QMessageBox::aboutQt( this, tr("Juicer: About Qt") );
}


void Juicer::hostLookedUp(const QHostInfo &host) {
    QList<QHostAddress> coreAddresses = host.addresses();
    QList<QHostAddress> localAddresses = QNetworkInterface::allAddresses();
    for(int i=0; i<localAddresses.size() && !localCore; i++) {
        localCore = (coreAddresses.contains(localAddresses[i]));
    }
}
