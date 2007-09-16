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

Juicer::Juicer( QStringList argList ) : QMainWindow()
{
// #ifdef Q_WS_WIN
//     filesystemSeparator = "\\";
// #else
//     filesystemSeparator = "/";
// #endif

    connect( qApp, SIGNAL( lastWindowClosed () ), this, SLOT ( lastWindowClosed () ) );

    zeroTime = QDateTime( QDate(1970,1,1), QTime(0,0), Qt::UTC );
    firstModifiedMax = 4 + argList.size();

    linkServer = new QAjServerSocket( QAjApplication::APP_PORT );
    connect( linkServer, SIGNAL( lineReady( QString ) ), this, SLOT( linkServerLine( QString ) ) );

    setWindowIcon(QIcon(":/juicer.png"));

    xml = new QXMLModule( this );

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
    
    QString password = QAjOptionsDialog::getSetting( "corePassword", "" ).toString();
    // no password in local file? => ask for it
    if ( password.isEmpty() )
    {
        bool ok;
        password = QInputDialog::getText( this, "Juicer", "Enter core password:", QLineEdit::Password,  QString::null, &ok );
        if ( !ok ) // user canceld
            qApp->closeAllWindows();
        else
        {
            // save password in local file if user wants it
            if ( QAjOptionsDialog::getSetting( "savePassword", false ).toBool() )
                QAjOptionsDialog::setSetting( "password", password );
        }
    }

    xml->setPassword( password );
    xml->setHost( QAjOptionsDialog::getSetting("coreAddress", "localhost").toString(),
                  QAjOptionsDialog::getSetting("xmlPort", 9851).toInt() );

    timer = new QTimer( this );
    connect( timer, SIGNAL( timeout() ), this, SLOT( timerSlot() ) );
    partListTimer = new QTimer( this );
    connect( partListTimer, SIGNAL( timeout() ), this, SLOT( partListTimerSlot() ) );

    connect( ajTab, SIGNAL( currentChanged( QWidget* ) ), this, SLOT( tabChanged( QWidget* ) ) );
    connect( ajTab, SIGNAL( currentChanged( int ) ), this, SLOT( tabChanged( int ) ) );

    tabChanged( ajDownloadWidget );

    login();
    queueLinks( argList );

    if( QAjOptionsDialog::getSetting( "useTray", false ).toBool() )
    {
        tray = new QSystemTrayIcon( QIcon(":/juicer.png"), this );
        tray->setVisible(true);
        tray->setContextMenu( file );
        connect(tray, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT( trayActivated( QSystemTrayIcon::ActivationReason ) ) );
    }
    else
    {
        tray = NULL;
    }
    connect( ajDownloadWidget, SIGNAL( downloadsFinished( QList<QAjDownloadItem*>  ) ),this, SLOT( downloadsFinished( QList<QAjDownloadItem*> ) ) );
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

void Juicer::settingsReady( AjSettings settings )
{
    ajDownloadWidget->setDirs( QFileInfo( settings.tempDir ), QFileInfo( settings.incomingDir ) );
    ajIncomingWidget->setDir( settings.incomingDir );
    if ( optionsDialog != NULL )
    {
        optionsDialog->setAjSettings( settings );
        optionsDialog->setSettings();
    }
}

bool Juicer::login()
{
    firstModifiedCnt = 0;
    ajDownloadWidget->clear();
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

void Juicer::processLink( QString link)
{
    QStringList s = link.split("|");
    if(s.size() > 3 )
    {
        QString size = s[3].split("/")[0];
        QString hash = s[2];
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
    xml->set( "processlink", "&link=" + QString( QUrl::toPercentEncoding( link ) ) );
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
    tabChanged(ajTab->widget( index ));
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
    delete this;
}


/*!
    \fn Juicer::downloadsFinished( QList<QAjDownloadItem*> list )
 */
void Juicer::downloadsFinished( QList<QAjDownloadItem*> list )
{
    if( tray != NULL && QSystemTrayIcon::supportsMessages() )
    {
        QString msg = "";
        int i;
        for( i=0; i<list.size(); i++ )
        {
            msg += list[i]->text( FILENAME_DOWN_INDEX ) + "\n";
        }
        tray->showMessage( "Download finished", msg, QSystemTrayIcon::Information, 3000 );
    }
}
