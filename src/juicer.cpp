/***************************************************************************
 *   Copyright (C) 2004 - 2009 by Matthias Reif, Holger Gerth              *
 *   matthias.reif@informatik.tu-chemnitz.de                               *
 *   holger.gerth@informatik.tu-chemnitz.de                                *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the          *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, see http://www.gnu.org/licenses/     *
 ***************************************************************************/

#include "juicer.h"
#include "askagaindialog.h"

Juicer::Juicer(const QStringList& argList, QSplashScreen *splash, const QFileInfo& appFileInfo)
    : QMainWindow()
    , xml(0)
    , downloadModule(0)
    , uploadModule(0)
    , networkDialog(0)
    , searchModule(0)
    , serverModule(0)
    , shareModule(0)
    , incomingModule(0)
    , exitId(-1)
    , started(false)
    , connected(false)
    , localCore(false)
{
    setupUi(this);

    downloads->setCentralWidget(downloadsTreeWidget);
    downloads->addDockWidget(Qt::RightDockWidgetArea, partListDock);

    server->setCentralWidget(serverTreeWidget);
    server->addDockWidget(Qt::RightDockWidgetArea, welcomeDock);

    this->splash = splash;
    this->appFileInfo = appFileInfo;

    firstModifiedMax = 2;// + argList.size();

    linkServer = new ServerSocket(Application::APP_PORT);
    connect(linkServer, SIGNAL(lineReady(const QString&)), this, SLOT(processLinksAndNotify(const QString&)));

    osIcons[LINUX] = QIcon(":/small/linux.png");
    osIcons[WINDOWS] = QIcon(":/small/windows.png");
    osIcons[MAC] = QIcon(":/small/mac.png");
    osIcons[SOLARIS] = QIcon(":/small/solaris.png");
    osIcons[FREEBSD] = QIcon(":/small/freebsd.png");
    osIcons[NETWARE] = QIcon(":/small/netware.png");

    xml = new XMLModule(this);
    downloadModule = new DownloadModule(this, downloads);
    uploadModule = new UploadModule(this, uploads);
    searchModule = new SearchModule(this);
    serverModule = new ServerModule(this);
    shareModule = new ShareModule(this);
    incomingModule = new IncomingModule(this);

    prevTab = downloads;

    networkDialog = new NetworkDialog(this);
    optionsDialog = new OptionsDialog(this);
    autoUpdater = new AutoUpdate(appFileInfo.absolutePath(), this);

    initToolBars();
    connectActions();
    initStatusBar();

    resize(OptionsDialog::getSetting("MainWindow", "size", QSize(1000, 600)).toSize());
    move(OptionsDialog::getSetting("MainWindow", "pos", QPoint(100, 100)).toPoint());
    restoreState(OptionsDialog::getSetting("JuicerMain").toByteArray());
    downloads->restoreState(OptionsDialog::getSetting("DownloadsMain").toByteArray());
    server->restoreState(OptionsDialog::getSetting("ServerMain").toByteArray());

    connect(xml, SIGNAL(requestFinished(int , bool)), this, SLOT(requestFinished(int, bool)));
    connect(xml, SIGNAL(settingsReady(const AjSettings&)), this, SLOT(settingsReady(const AjSettings&)));
    connect(xml, SIGNAL(error(const QString&)), this, SLOT(xmlError(const QString&)));
    connect(xml, SIGNAL(gotSession()), this, SLOT(gotSession()));
    connect(xml, SIGNAL(modifiedDone()), downloadModule, SLOT(updateView()));
    connect(xml, SIGNAL(modifiedDone()), this, SLOT(firstModified()));

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(timerSlot()));

    connect(ajTab, SIGNAL(currentChanged(int)), this, SLOT(tabChanged(int)));

    tabChanged(ajTab->indexOf(downloads));

    // -- we need to do this as an event, because we can quit the application only if it is already in the event loop --
    QTimer::singleShot(0, this, SLOT(login()));

    queueLinks(argList);
    initTrayIcon();
    connect(downloadModule, SIGNAL(downloadsFinished(const QList<DownloadItem*>& )),this, SLOT(downloadsFinished(const QList<DownloadItem*>&)));

    connect(qApp->clipboard(), SIGNAL(changed(QClipboard::Mode)), this, SLOT(clipboardChanged(QClipboard::Mode)));
}

Juicer::~Juicer() {
}

/*!
    \fn Juicer::initToolBars()
    initializes the tool bars
 */
void Juicer::initToolBars() {
    ajAddressLabel = new QLabel(ajLinks);
    ajAddressLabel->setText("ajfsp link:");
    ajAddressLabel->adjustSize();
    ajLinks->addWidget(ajAddressLabel);
    ajAddressEdit = new QLineEdit(ajLinks);
    ajLinks->addWidget(ajAddressEdit);
    connect(ajAddressEdit, SIGNAL(returnPressed()), this, SLOT(processLinkFromToolbar()));
    ajLinks->addAction(QIcon(":/ok.png"), tr("process link"), this, SLOT(processLinkFromToolbar()));
}

/*!
    \fn Juicer::connectActions()
 */
void Juicer::connectActions() {
    connect(actionConfigure, SIGNAL(triggered()), this, SLOT(showOptions()));
    connect(actionNet_Info, SIGNAL(triggered()), this, SLOT(getNetworkInfo()));
    connect(actionNet_Info, SIGNAL(triggered()), networkDialog, SLOT(exec()));
    connect(actionOpen_Aj_Link_List, SIGNAL(triggered()), this, SLOT(openAjL()));
    connect(actionAdjust_Columns, SIGNAL(triggered()), this, SLOT(adjustColumns()));
    connect(actionProcess_Link_From_Clipboard, SIGNAL(triggered()), this, SLOT(processClipboard()));
    connect(actionCheck_for_Update, SIGNAL(triggered()), autoUpdater, SLOT(check()));
    connect(actionExit_Core, SIGNAL(triggered()), this, SLOT(exitCore()));
    connect(actionQuit_GUI, SIGNAL(triggered()), this, SLOT(quit()));
    connect(actionManual, SIGNAL(triggered()), this, SLOT(showManual()));
    connect(actionAbout, SIGNAL(triggered()), this, SLOT(about()));
    connect(actionAbout_Qt, SIGNAL(triggered()), this, SLOT(aboutQt()));
    actionQuit_GUI->setShortcut(Qt::ControlModifier + Qt::Key_Q);
}

/*!
    \fn Juicer::initTrayIcon()
    initializes the tray icon if necessarry
 */
void Juicer::initTrayIcon() {
    tray = new QSystemTrayIcon(QIcon(":/juicer.png"), this);
    if(OptionsDialog::getSetting("useTray", false).toBool()) {
        tray->show();
        tray->setContextMenu(menuAppleJuice);
        connect(tray, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
                 this, SLOT(trayActivated(QSystemTrayIcon::ActivationReason)));
    } else {
        tray->hide();
    }
}

/*!
    \fn Juicer::closeEvent(QCloseEvent* ce)
    handler when closing the app, saves a lot of settings
    @param ce the close event
 */
void Juicer::closeEvent(QCloseEvent* ce) {
     if(tray->isVisible() && !isMinimized()) {
        AskAgainDialog trayDialog(
                tr("Minimizing to tray"),
                tr("Tray Icon is enabled so Juicer runs minimized in the background.\nUse Quit GUI to close the GUI."),
                QDialogButtonBox::Ok,
                QStyle::SP_MessageBoxInformation,
                this);
        trayDialog.exec("minimize");
        showMinimized();
        setHidden(true);
        ce->ignore();
     } else {
        // make sure tray icon disappears when quit (Windows problem)
        if(tray->isVisible()) {
            tray->setVisible(false); // <2DO> doesn't work; possible qt bug?
        }
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
    int port = OptionsDialog::getSetting("xmlPort", 9851).toInt();
    // -- ok -> login --
    if(!password.isEmpty() || isLocalhost(host)) {
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
        if(OptionsDialog::getSetting("checkUpdates", true).toBool()) {
            this->autoUpdate();
        }
    } else {
        login("empty password", true);
    }
    return true;
}

bool Juicer::isLocalhost(const QString& host) {
    return (QHostInfo::fromName(host).hostName() == "localhost");
}

QString Juicer::showLoginDialog(const QString& message) {
    if(splash->isVisible()) {
        splash->close();
    }
    LoginDialog loginDialog(this);
    loginDialog.setHost(OptionsDialog::getSetting("coreAddress", "localhost").toString());
    loginDialog.setPort(OptionsDialog::getSetting("xmlPort", 9851).toInt());
    loginDialog.setPassword(OptionsDialog::getSetting("password", "").toString());
    loginDialog.setSavePassword(OptionsDialog::getSetting("savePassword", false).toBool());
    loginDialog.setHeader(message);
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
            lokalSettings.setValue("password", ret);
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
    downloadModule->partListTimer->stop();
    if(shareModule != NULL) {
        shareModule->reset();
    }
    login(reason, true);
}

/*!
    \fn Juicer::timerSlot()
    base timer slot, requests modifications from the core
 */
void Juicer::timerSlot() {
    if(connected) {
        xml->get("modified");
    }
}

void Juicer::showOptions() {
    if(connected) {
        xml->get("settings");
    }
    optionsDialog->setConnected(connected);
    if(optionsDialog->exec() == QDialog::Accepted) {
        // save options
        AjSettings settings = optionsDialog->getAjSettings();
        QApplication::setFont(optionsDialog->getFont());
        downloadModule->updateAlternatingRowColors();
        uploadModule->updateAlternatingRowColors();
        serverModule->updateAlternatingRowColors();
        searchModule->updateAlternatingRowColors();
        shareModule->updateAlternatingRowColors();
        incomingModule->updateAlternatingRowColors();
        initStatusBar();

        for(int i=0; i<optionsDialog->downloadTabList->count(); i++) {
            bool hidden = downloadsTreeWidget->isColumnHidden(i);
            bool hide = optionsDialog->downloadTabList->item(i)->checkState()==Qt::Unchecked;
            downloadsTreeWidget->setColumnHidden(i, hide);
            if(hidden && !hide) {
                qApp->processEvents();
                downloadsTreeWidget->resizeColumnToContents(i);
            }
        }
        for(int i=0; i<optionsDialog->uploadTabList->count(); i++) {
            bool hidden = uploadsTreeWidget->isColumnHidden(i);
            bool hide = optionsDialog->uploadTabList->item(i)->checkState()==Qt::Unchecked;
            uploadsTreeWidget->setColumnHidden(i, hide);
            if(hidden && !hide) {
                qApp->processEvents();
                uploadsTreeWidget->resizeColumnToContents(i);
            }
        }

        timer->stop();
        timer->setSingleShot(false);
        timer->start(OptionsDialog::getSetting("refresh", 3).toInt() * 1000);

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
            xml->set("setsettings", settingsString);
        }
    }
}

void Juicer::settingsReady(const AjSettings& settings) {
    downloadModule->setDirs(settings.tempDir, settings.incomingDir);
    incomingModule->setDir(settings.incomingDir);
    incomingModule->resetWatcher();
    if(optionsDialog != NULL) {
        optionsDialog->setAjSettings(settings);
//         optionsDialog->setSettings();
    }
}

void Juicer::gotSession() {
    connected = true;
    xml->get("information");
    xml->get("settings");
    QSettings lokalSettings;
    timerSlot();
    timer->setSingleShot(false);
    timer->start(lokalSettings.value("refresh", 3).toInt() * 1000);
    downloadModule->partListTimer->setSingleShot(false);
    downloadModule->partListTimer->start(5000);
    shareModule->reloadSlot();
}

void Juicer::setStatusBarText(const QString& downSpeed, const QString& upSpeed,
        const QString& credits, const QString& downSize, const QString& upSize,
        const QString& openConnections) {
    QString downStreamString = Convert::bytes(downSpeed) + "/s";
    QString upStreamString = Convert::bytes(upSpeed) + "/s";
    QString creditsString = Convert::bytesExtra(credits);
    QString downSizeString = Convert::bytesExtra(downSize);
    QString upSizeString = Convert::bytesExtra(upSize);

    downSpeedLabel->setText(downStreamString);
    upSpeedLabel->setText(upStreamString);
    creditsLabel->setText(creditsString);
    downSizeLabel->setText(downSizeString);
    upSizeLabel->setText(upSizeString);
    connectionsLabel->setText(openConnections);

    QString m = QString("Juicer - appleJuice Qt4 GUI\n\nDownstream: %1\nUpstream: %2\nDownloaded: %3\nUploaded: %4\nCredits: %5");
    tray->setToolTip(m.arg(downStreamString).arg(upStreamString).arg(downSizeString).arg(upSizeString).arg(creditsString));
}


void Juicer::processClipboard() {
    processLink(qApp->clipboard()->text(QClipboard::Clipboard));
}


void Juicer::tabChanged(int index) {
    QWidget *tab = ajTab->widget(index);

    downloadToolBar->setVisible(tab == downloads);
    uploadToolBar->setVisible(tab == uploads);
    searchToolBar->setVisible(tab == search);
    serverToolBar->setVisible(tab == server);
    shareToolBar->setVisible(tab == shares);
    incomingToolBar->setVisible(tab == incoming);
    //coreToolBar->setVisible(tab == core);

    if((prevTab == shares) && (shareModule->isChanged()) && 
       (QMessageBox::question(this, tr("Question"), tr("You've changed your shares.\nDo you want to transfer the changes to the core?"), QMessageBox::Yes, QMessageBox::No) == QMessageBox::Yes)) {
        shareModule->commitSlot();
    }

    if(tab == incoming) {
        incomingModule->reload();
    }

    prevTab = tab;
}

void Juicer::exitCore() {
    if(QMessageBox::question(this, tr("Confirm"), tr("Do you realy want to exit the core?\nAll your credits will be lost!"), QMessageBox::No, QMessageBox::Yes) == QMessageBox::Yes) {
        exitId = xml->set("exitcore");
    }
}

void Juicer::setCoreVersion(const QString& version) {
    coreVersionLabel->setText(version);
}

void Juicer::connectedSince(const QString& since) {
    QDateTime& time = serverModule->setConnectedSince(since);
    if(time.isValid()) {
        connectedLabel->setText(time.toLocalTime().toString(Qt::LocalDate));
    } else {
        connectedLabel->setText(tr("NOT connected"));
    }
}

void Juicer::firstModified() {
    if (firstModifiedCnt <= firstModifiedMax) {
        if (firstModifiedCnt == firstModifiedMax) {
            downloadModule->updateView();
            downloadModule->adjustSizeOfColumns();
            uploadModule->adjustSizeOfColumns();
            searchModule->adjustSizeOfColumns();
            serverModule->adjustSizeOfColumns();
            shareModule->adjustSizeOfColumns();
            downloadModule->sortItemsInitially("DownloadWidget");
            uploadModule->sortItemsInitially("UploadWidget");
            serverModule->sortItemsInitially("ServerWidget");
            shareModule->sortItemsInitially("ShareWidget");
            incomingModule->sortItemsInitially("IncomingWidget");
            searchModule->sortItemsInitially("SearchWidget");
            processQueuedLinks();
            this->show();
            if(OptionsDialog::getSetting("checkUpdates", true).toBool()) {
                this->autoUpdate();
            }
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
                AskAgainDialog localCoreDialog(
                    "Information",
                    tr("The Core is not running on the local machine. In order to use the full functionality like directly opening downloads or the incoming view you have to specify the incoming and temporary directory in the options menu."),
                    QDialogButtonBox::Ok, QStyle::SP_MessageBoxInformation);
                localCoreDialog.exec("localCore");
            }
        }
        firstModifiedCnt++;
    }
}

void Juicer::processQueuedLinks() {
    while(!queuedLinks.isEmpty()) {
        processLink(queuedLinks.takeFirst());
    }
}

void Juicer::queueLinks(const QStringList& links) {
    queuedLinks = links;
}


/*!
    \fn Juicer::setUploadFilename(const QString& shareId, const QString& filename)
 */
void Juicer::setUploadFilename(const QString& shareId, const QString& filename) {
    uploadModule->setFilename(shareId, downloadModule->findDownloadByTempNum(filename));
}


/*!
    \fn Juicer::adjustColumns()
 */
void Juicer::adjustColumns() {
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
    if(incoming->isVisible())
        incomingModule->adjustSizeOfColumns();
}


/*!
    \fn Juicer::getExec()
 */
QStringList Juicer::getExec() {
    QSettings lokalSettings;
    QString launcher = lokalSettings.value("launcher", DEFAULT_LAUNCHER).toString().simplified();

    QStringList args;
    if(launcher == KDE_LAUNCHER) {
        args.clear();
        args << "kfmclient" << "exec";
    } else if(launcher == GNOME_LAUNCHER) {
        args.clear();
        args << "gnome-open";
    } else if(launcher == MAC_LAUNCHER) {
        args.clear();
        args << "open";
    } else if(launcher == WIN_LAUNCHER) {
        args.clear();
        args << "start" << "\"\"";
    } else {
        args << launcher.split(" ");
    }
    return args;
}


/*!
    \fn Juicer::addToStatusBar(IconWidget* widget)
 */
IconWidget* Juicer::addToStatusBar(IconWidget* widget) {
    statusBar()->addPermanentWidget(widget);
    statusBarWidgets.append(widget);
    return widget;
}

/*!
    \fn Juicer::initStatusBar()
 */
void Juicer::initStatusBar() {
    static bool first = true;
    if(first) {
        profilesWidget = new QWidget(statusBar());
        profilesBox = new QHBoxLayout();
        profileGroup = new QButtonGroup(statusBar());
        profilesWidget->setLayout(profilesBox);
        statusBar()->addPermanentWidget(profilesWidget);

        warnFirewallLabel = new QLabel(this);
        statusBar()->addPermanentWidget(warnFirewallLabel);
        connectedLabel = addToStatusBar(new IconWidget(":/small/connected.png", "0", tr("Connected Since"), QBoxLayout::LeftToRight, this, 2, 2));
        connectionsLabel = addToStatusBar(new IconWidget(":/small/connections.png", "0", tr("Number Of Open Connections"), QBoxLayout::LeftToRight, this, 2, 2));
        coreVersionLabel = addToStatusBar(new IconWidget(":/small/version.png", "0", tr("Core Version"), QBoxLayout::LeftToRight, this, 2, 2));
        downSpeedLabel = addToStatusBar(new IconWidget(":/small/downstream.png", "0", tr("Downstream"), QBoxLayout::LeftToRight, this, 2, 2));
        upSpeedLabel = addToStatusBar(new IconWidget(":/small/upstream.png", "0", tr("Upstream"), QBoxLayout::LeftToRight, this, 2, 2));
        downSizeLabel = addToStatusBar(new IconWidget(":/small/downloaded.png", "0", tr("Downloaded"), QBoxLayout::LeftToRight, this, 2, 2));
        upSizeLabel = addToStatusBar(new IconWidget(":/small/uploaded.png", "0", tr("Uploaded"), QBoxLayout::LeftToRight, this, 2, 2));
        creditsLabel = addToStatusBar(new IconWidget(":/small/credits.png", "0", tr("Credits"), QBoxLayout::LeftToRight, this, 2, 2));
        first = false;
        connect(profileGroup, SIGNAL(buttonClicked(QAbstractButton*)), this, SLOT(profileButtonClicked(QAbstractButton*)));
    }
    QList<QVariant> show = OptionsDialog::getStatusbarShows(statusBarWidgets.size());
    for(int i=0; i<statusBarWidgets.size(); i++) {
        statusBarWidgets.at(i)->setVisible(show.at(i).toBool());
    }
    // -- remove all profile radio buttons --
    QList<QAbstractButton *> buttons = profileGroup->buttons();
    for(int i=0; i<buttons.size(); i++) {
        profileGroup->removeButton(buttons.at(i));
        profilesBox->removeWidget(buttons.at(i));
        delete buttons.at(i);
    }
    // -- add profile radio buttons if enabled --
    if(OptionsDialog::getSetting("profilesStatusbar", false).toBool()) {
        QStringList profiles = OptionsDialog::getSetting("profiles").toStringList();
        for(int i=0; i<profiles.size(); i++) {
            QRadioButton* profileButton = new QRadioButton(profiles.at(i));
            profileGroup->addButton(profileButton);
            profilesBox->addWidget(profileButton);
        }
        setCurrentProfile();
    }
}


/*!
    \fn Juicer::trayActivated(QSystemTrayIcon::ActivationReason reason)
 */
void Juicer::trayActivated(QSystemTrayIcon::ActivationReason reason) {
    if(reason == QSystemTrayIcon::Trigger) {
        setVisible(!isVisible());
        if(isMinimized()) {
            showNormal();
            activateWindow();    // make widget active, otherwise one must first click at the tab in the task bar to get Juicer in front
        }
    }
}


/*!
    \fn Juicer::downloadsFinished(const QList<DownloadItem*>& list)
    show a message with all finished downloads in tray icon
    @param list list with all finished downloads to show
 */
void Juicer::downloadsFinished(const QList<DownloadItem*>& list) {
    if(QSystemTrayIcon::supportsMessages()) {
        QString msg = "";
        for(int i=0; i<list.size(); i++) {
            msg += list[i]->text(DownloadItem::FILENAME_COL) + "\n";
        }
        tray->showMessage(tr("Download finished"), msg, QSystemTrayIcon::Information, 3000);
    }
}

/*!
    \fn Juicer::openAjL()
 */
void Juicer::openAjL() {
    QString ajListFileName = QFileDialog::getOpenFileName(
                              this,
                              tr("Select AJ link list file"),
                              QString::null,
                              tr("AJ Link Lists (*.ajl)"));

    if(!ajListFileName.isNull()) {
        QFile ajListFile(ajListFileName);
        if(ajListFile.exists()) {
            if(!ajListFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
                tray->showMessage(tr("Error while opening file"), ajListFileName, QSystemTrayIcon::Information, 3000);
            } else {
                QTextStream in(&ajListFile);
                QString line = in.readLine();
                while(line.compare("100") != 0) {
                    if(in.atEnd()) {
                        tray->showMessage(tr("No valid AJ list file"), ajListFileName,
                                          QSystemTrayIcon::Information, 3000);
                        break;
                    }
                    line = in.readLine();
                }
                if(!in.atEnd()) {
                    while (!in.atEnd()) {
                        QString filename = in.readLine();
                        QString filehash = in.readLine();
                        QString filesize = in.readLine();
                        QString link = "ajfsp://file|" + filename + "|" + filehash + "|" + filesize + "/";
                        this->processLink(link);
                    }
                    tray->showMessage(tr("Files successfully added from link list"),
                                      ajListFileName, QSystemTrayIcon::Information, 3000);
                }
                ajListFile.close();
            }
        } else {
            tray->showMessage(tr("No such file"), ajListFileName, QSystemTrayIcon::Information, 3000);
        }
    }

}

void Juicer::sendToTray(const QString& message1, const QString& message2) {
    tray->showMessage(message1, message2, QSystemTrayIcon::Information, 3000);
}


void Juicer::about() {
    AboutDialog().exec();
}

void Juicer::aboutQt() {
    QMessageBox::aboutQt(this, tr("Juicer: About Qt"));
}


void Juicer::hostLookedUp(const QHostInfo &host) {
    QList<QHostAddress> coreAddresses = host.addresses();
    QList<QHostAddress> localAddresses = QNetworkInterface::allAddresses();
    for(int i=0; i<localAddresses.size() && !localCore; i++) {
        localCore = (coreAddresses.contains(localAddresses[i]));
    }
}


/*!
    \fn Juicer::showManual()
 */
void Juicer::showManual() {
     QDesktopServices::openUrl(QUrl("http://applejuicer.net/manual"));
}


/*!
    \fn Juicer::clipboardChanged(QClipboard::Mode mode)
 */
void Juicer::clipboardChanged(QClipboard::Mode mode) {
    if(OptionsDialog::getSetting("observeClipboard", false).toBool()) {
        processLinksAndNotify(qApp->clipboard()->text(mode));
    }
    clipboardTexts.clear();
}


/*!
    \fn Juicer::setClipboard(const QString& text)
 */
void Juicer::setClipboard(const QString& text) {
    clipboardTexts.append(text);
    QApplication::clipboard()->setText(text);
}


/*!
    \fn Juicer::getAjfspLinks(const QString& text, const QString& type)
 */
QStringList Juicer::getAjfspLinks(const QString& text, const QString& type) {
    QStringList result;
    QRegExp re("ajfsp://"+type+"[|][^/]*[/]");
    int d = 0;
    while((d = re.indexIn(text, d)) >= 0) {
        result.append(text.mid(d, re.matchedLength()));
        d += re.matchedLength();
    }
    return result;
}


void Juicer::processLinkFromToolbar() {
    processLink(ajAddressEdit->text().trimmed());
    ajAddressEdit->clear();
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
            if((file = shareModule->findFile(size, hash)) != NULL) {
                QMessageBox::information(this, tr("Information"), tr("The file seems to be already in the share\n\n%1").arg(file->getFilename()));
            } else if((download = downloadModule->findDownload(size, hash)) != NULL) {
                QMessageBox::information(this, tr("Information"),
                    tr("The file seems to be already in the download list\n\n%1").arg(download->text(DownloadItem::FILENAME_COL)));
            }
        }
        encodedLink = s[0] + "|" + QUrl::toPercentEncoding(name)  + "|" + hash + "|" + size + "/";
    }
    xml->set("processlink", "&link=" + encodedLink);
}


/*!
    \fn Juicer::processLinks(const QString& text, const QString& type)
 */
QStringList Juicer::processLinks(const QString& text, const QString& type) {
    QStringList links = getAjfspLinks(text, type);
    QMutableStringListIterator it(links);
    while(it.hasNext()) {
        QString link = it.next();
        // -- prevent links added into the clipboard out of Juicer --
        if(!clipboardTexts.contains(link)) {
            processLink(link);
        } else {
            it.remove();
        }
    }
    return links;
}


/*!
    \fn Juicer::processLinksAndNotify(const QString& text, const QString& type)
 */
void Juicer::processLinksAndNotify(const QString& text, const QString& type) {
    QStringList links = processLinks(text, type);
    notifyAjfspLink(links);
}


/*!
    \fn Juicer::setFilesystemSeparator(const QString& separator)
 */
void Juicer::setFilesystemSeparator(const QString& separator) {
    filesystemSeparator = separator;
}

/*!
    \fn Juicer::getFilesystemSeparator() const
 */
QString Juicer::getFilesystemSeparator() const {
    return filesystemSeparator;
}

/*!
    \fn Juicer::isCoreLocal()
 */
bool Juicer::isCoreLocal() {
    return localCore;
}


/*!
    \fn Juicer::setIncomingDirectory(const QString& folder)
 */
void Juicer::setIncomingDirectory(const QString& folder) {
    incomingDir = folder;
}


/*!
    \fn Juicer::setTempDirectory(const QString& folder)
 */
void Juicer::setTempDirectory(const QString& folder) {
    tempDir = folder;
}


/*!
    \fn Juicer::getIncomingDirectory() const
 */
QString Juicer::getIncomingDirectory() const {
    return incomingDir;
}


/*!
    \fn Juicer::getTempDirectory() const
 */
QString Juicer::getTempDirectory() const {
    return tempDir;
}

/*!
    \fn Juicer::getAppPath() const
 */
QString Juicer::getAppPath() const {
    return appFileInfo.absoluteFilePath();
}


/*!
    \fn Juicer::getNetworkInfo()
 */
void Juicer::getNetworkInfo() {
    xml->get("information");
}


/*!
    \fn Juicer::setFirewalled(bool firewalled)
 */
void Juicer::setFirewalled(bool firewalled) {
    if(firewalled) {
        warnFirewallLabel->setPixmap(QPixmap(":/warning.png"));
        warnFirewallLabel->setToolTip(tr("Firewalled"));
    } else {
        warnFirewallLabel->clear();
        warnFirewallLabel->setToolTip("");
    }
}


/*!
    \fn Juicer::autoUpdate()
 */
void Juicer::autoUpdate() {
    autoUpdater->check();
}


/*!
    \fn Juicer::requestFinished(int id, bool error)
 */
void Juicer::requestFinished(int id, bool error) {
    if(!error && id == exitId && OptionsDialog::getSetting("quitGUIAfterCoreExit", true).toBool()) {
        this->quit();
    }
}


/*!
    \fn Juicer::profileButtonClicked(QAbstractButton* button)
 */
void Juicer::profileButtonClicked(QAbstractButton* button) {
    QHash<QString, QVariant> profile = OptionsDialog::getGroupSetting("profile", button->text()).toHash();
    QString settingsString = "";
    settingsString += "&MaxUpload=" + QString::number(profile["maxUp"].toInt() * 1024);
    settingsString += "&MaxDownload=" + QString::number(profile["maxDown"].toInt() * 1024);
    settingsString += "&MaxSourcesPerFile=" + profile["maxSources"].toString();
    settingsString += "&MaxConnections=" + profile["maxCon"].toString();
    settingsString += "&Speedperslot=" + profile["maxSlot"].toString();
    settingsString += "&MaxNewConnectionsPerTurn=" + profile["maxNewCon"].toString();
    xml->set("setsettings", settingsString);
    OptionsDialog::setSetting("currentProfile", button->text());
    optionsDialog->setCurrentProfile();
}


/*!
    \fn Juicer::setCurrentProfile()
 */
void Juicer::setCurrentProfile() {
    if(OptionsDialog::hasSetting("currentProfile")) {
        QString current = OptionsDialog::getSetting("currentProfile", "").toString();
        QList<QAbstractButton *> buttons = profileGroup->buttons();
        for(int i=0; i<buttons.size(); i++) {
            if(buttons.at(i)->text() == current) {
                buttons.at(i)->setChecked(true);
                break;
            }
        }
    }
}


/*!
    \fn Juicer::notifyAjfspLink(QStringList& links)
 */
void Juicer::notifyAjfspLink(QStringList& links) {
    if(!links.isEmpty()) {
        QString msg;
        for(int i=0; i<links.size(); i++) {
            QStringList parts = links.at(i).split('|');
            msg += parts[1] + "\n";
        }
        tray->showMessage("Processed Link", msg.trimmed());
    }
}
