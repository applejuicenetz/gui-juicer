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

Juicer::Juicer( QStringList argList, QSplashScreen *splash )
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
{
    setupUi( this );
    QSettings lokalSettings;

    lokalSettings.beginGroup("PartListDock");
    downloads->setCentralWidget(downloadsTreeWidget);
    downloads->addDockWidget((Qt::DockWidgetArea)lokalSettings.value("pos", Qt::RightDockWidgetArea).toInt(), partListDock);
    partListDock->setVisible(lokalSettings.value("visible", true).toBool());
    partListDock->setFloating(lokalSettings.value("float", false).toBool());
    lokalSettings.endGroup();

    lokalSettings.beginGroup("WelcomeDock");
    server->setCentralWidget(serverTreeWidget);
    server->addDockWidget((Qt::DockWidgetArea)lokalSettings.value("pos", Qt::RightDockWidgetArea).toInt(), welcomeDock);
    welcomeDock->setVisible(lokalSettings.value("visible", true).toBool());
    welcomeDock->setFloating(lokalSettings.value("float", false).toBool());
    lokalSettings.endGroup();


    this->splash = splash;
    connect( qApp, SIGNAL( lastWindowClosed () ), this, SLOT ( lastWindowClosed () ) );

    zeroTime = QDateTime( QDate(1970,1,1), QTime(0,0), Qt::UTC );
    firstModifiedMax = 2;// + argList.size();

    linkServer = new QAjServerSocket( QAjApplication::APP_PORT );
    connect( linkServer, SIGNAL( lineReady( QString ) ), this, SLOT( processLink( QString ) ) );

    setWindowIcon(QIcon(":/juicer.png"));
    osIcons[LINUX] = QIcon(":/small/linux.png");
    osIcons[WINDOWS] = QIcon(":/small/windows.png");
    osIcons[MAC] = QIcon(":/small/mac.png");
    osIcons[SOLARIS] = QIcon(":/small/solaris.png");
    osIcons[FREEBSD] = QIcon(":/small/freebsd.png");
    osIcons[NETWARE] = QIcon(":/small/netware.png");


    xml = new QXMLModule(this);
    downloadModule = new QAjDownloadModule(this);
    uploadModule = new QAjUploadModule(this);
    searchModule = new QAjSearchModule(this);
    serverModule = new QAjServerModule(this);
    shareModule = new QAjShareModule(this);
    incomingModule = new QAjIncomingModule(this);

    prevTab = downloads;

    networkDialog = new QAjNetworkDialog( this );
    optionsDialog = new QAjOptionsDialog( this );

    initToolBars();
    connectActions();
    initStatusBar();

    lokalSettings.beginGroup( "MainWindow" );
    resize( lokalSettings.value( "size", QSize(1000, 600) ).toSize() );
    move( lokalSettings.value( "pos", QPoint(100, 100) ).toPoint() );
    lokalSettings.endGroup();

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
    connect(downloadModule, SIGNAL( downloadsFinished( const QList<QAjDownloadItem*>&  ) ),this, SLOT( downloadsFinished( const QList<QAjDownloadItem*>& ) ) );
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
    connect(actionQuit_GUI, SIGNAL(triggered()), qApp, SLOT(quit()));
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
    if( QAjOptionsDialog::getSetting( "useTray", false ).toBool() )
    {
        tray->setVisible(true);
        tray->setContextMenu(menuAppleJuice);
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
    downloads->close();
    server->close();
    QSettings lokalSettings;
    lokalSettings.beginGroup("MainWindow");
    lokalSettings.setValue( "size", size() );
    lokalSettings.setValue( "pos", pos() );
    lokalSettings.endGroup();
    downloadModule->saveSortOrder("DownloadWidget");
    uploadModule->saveSortOrder("UploadWidget");
    searchModule->saveSortOrder("SearchWidget");
    serverModule->saveSortOrder("ServerWidget");
    shareModule->saveSortOrder("ShareWidget");
    incomingModule->saveSortOrder("IncomingWidget");

    lokalSettings.beginGroup("PartListDock");
    lokalSettings.setValue("pos", downloads->dockWidgetArea(partListDock));
    lokalSettings.setValue("float", partListDock->isFloating());
    lokalSettings.endGroup();

    lokalSettings.beginGroup("WelcomeDock");
    lokalSettings.setValue("pos", server->dockWidgetArea(welcomeDock));
    lokalSettings.setValue("float", welcomeDock->isFloating());
    lokalSettings.endGroup();

    ce->accept();
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
    if(error || !QAjOptionsDialog::hasSetting("coreAddress") || !QAjOptionsDialog::hasSetting("password")) {
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
    // -- ignore at login --
    } else if(started) {
        optionsDialog->setSettings();
        this->show();
    } else {
        login("empty password", true);
    }
    return true;
}

QString Juicer::showLoginDialog(const QString& message) {
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

void Juicer::settingsReady( const AjSettings& settings )
{
    downloadModule->setDirs( QFileInfo( settings.tempDir ), QFileInfo( settings.incomingDir ) );
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

void Juicer::processLink(const QString& link) {
    QString encodedLink = QUrl::fromPercentEncoding(link.trimmed().toUtf8());
    QStringList s = encodedLink.split("|");
    if(s.size() > 3) {
        QString name = s[1];
        QString hash = s[2];
        QString size = s[3].split("/")[0];
        if(s[0].toLower() == "ajfsp://file") {
            QAjShareFileItem* file;
            QAjDownloadItem* download;
            if((file = shareModule->findFile( size, hash )) != NULL) {
                QMessageBox::information( this, tr("information"), tr("The file seems to be already in the share")+":\n\n"+file->getFilename());
            } else if((download = downloadModule->findDownload(size, hash)) != NULL) {
                QMessageBox::information( this, tr("information"),
                    tr("The file seems to be already in the download list")+":\n\n"+download->text(QAjDownloadItem::FILENAME_COL));
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
       (QMessageBox::question( this, tr("question"), tr("You've changed your shares.\nDo you want to transfer the changes to the core?"), QMessageBox::Yes, QMessageBox::No ) == QMessageBox::Yes)) {
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
    coreVersionLabel->setText(tr("Core: ") + version);
}

void Juicer::connectedSince(const QString& since) {
    QDateTime& time = serverModule->setConnectedSince(since);
    if(time.isValid()) {
        connectedLabel->setText(tr("connected since") + " " + time.toLocalTime().toString(Qt::LocalDate));
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
    uploadModule->setFilename( shareId, downloadModule->findDownloadByTempNum( QFileInfo(filename) ) );
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
    \fn Juicer::downloadsFinished( const QList<QAjDownloadItem*>& list )
    show a message with all finished downloads in tray icon
    @param list list with all finished downloads to show
 */
void Juicer::downloadsFinished( const QList<QAjDownloadItem*>& list )
{
    if( QSystemTrayIcon::supportsMessages() )
    {
        QString msg = "";
        int i;
        for( i=0; i<list.size(); i++ )
        {
            msg += list[i]->text( QAjDownloadItem::FILENAME_COL ) + "\n";
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
void Juicer::createAjL( const QList<QAjItem *>&  selectedItems )
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

//                 QString filename = selectedItems[i]->text(FILENAME_COL) + '\n';
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

void Juicer::sendToTray( const QString& message1, const QString& message2 ) {
    tray->showMessage( message1, message2, QSystemTrayIcon::Information, 3000 );
}


void Juicer::about() {
    QMessageBox::about( this, tr("Juicer Info"),
                        tr("Juicer \n\nhttp://ajqtgui.sf.net"));
}

void Juicer::aboutQt() {
    QMessageBox::aboutQt( this, tr("Juicer: About Qt") );
}
