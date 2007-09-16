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
#include "qajdownloadwidget.h"

#include "juicer.h"

QAjDownloadWidget::QAjDownloadWidget( QXMLModule* xml, QWidget *parent ) : QAjListWidget( xml, parent )
{
    userStatusDescr["1"] = QObject::tr("unasked");
    userStatusDescr["2"] = QObject::tr("try to connect");
    userStatusDescr["3"] = QObject::tr("partner have to old vers.");
    userStatusDescr["4"] = QObject::tr("partner can't	 open file");
    userStatusDescr["5"] = QObject::tr("queueing");
    userStatusDescr["6"] = QObject::tr("no usable parts");
    userStatusDescr["7"] = QObject::tr("loading");
    userStatusDescr["8"] = QObject::tr("not enough disk space");
    userStatusDescr["9"] = QObject::tr("finished");
    userStatusDescr["11"] = QObject::tr("no connection possible");
    userStatusDescr["12"] = QObject::tr("try indirect");
    userStatusDescr["13"] = QObject::tr("paused");
    userStatusDescr["14"] = QObject::tr("queue full");
    userStatusDescr["15"] = QObject::tr("own limit reached");
    userStatusDescr["16"] = QObject::tr("indirect conn. rejected");
    downloadStatusDescr["-1"] = QObject::tr("loading");
    downloadStatusDescr["0"] = QObject::tr("searching");
    downloadStatusDescr["1"] = QObject::tr("error at creating");
    downloadStatusDescr["12"] = QObject::tr("finishing");
    downloadStatusDescr["13"] = QObject::tr("error at finishing");
    downloadStatusDescr["14"] = QObject::tr("finished");
    downloadStatusDescr["15"] = QObject::tr("canceling");
    downloadStatusDescr["16"] = QObject::tr("creating .dat");
    downloadStatusDescr["17"] = QObject::tr("canceled");
    downloadStatusDescr["18"] = QObject::tr("paused");

    linuxIcon = new QIcon(":/small/linux.png");
    windowsIcon = new QIcon(":/small/windows.png");
    macIcon = new QIcon(":/small/mac.png");
    solarisIcon = new QIcon(":/small/solaris.png");
    freeBsdIcon = new QIcon(":/small/freebsd.png");
    netwareIcon = new QIcon(":/small/netware.png");
    otherOsIcon = new QIcon();

    QTreeWidgetItem* l = new QTreeWidgetItem( );
    l->setText( FILENAME_DOWN_INDEX, tr("filename") );
    l->setText( SOURCES_DOWN_INDEX, tr("sources") );
    l->setText( SPEED_DOWN_INDEX, tr("speed") );
    l->setText( STATUS_DOWN_INDEX, tr("status") );
    l->setText( FINISHED_DOWN_INDEX, tr("finished") );
    l->setText( POWER_DOWN_INDEX, tr("power") );
    l->setIcon( POWER_DOWN_INDEX, QIcon(":/small/launch.png") );
    l->setText( SIZE_DOWN_INDEX, tr("size") );
    l->setText( FINISHED_SIZE_DOWN_INDEX, tr("finished") );
    l->setText( REMAIN_SIZE_DOWN_INDEX, tr("remaining") );
    l->setText( REMAIN_TIME_DOWN_INDEX, tr("eta") );
    l->setIcon( REMAIN_TIME_DOWN_INDEX, QIcon(":/small/clock.png") );
    l->setText( MISSING_DOWN_INDEX, tr("not seen") );
    setHeaderItem( l );

    for(int i=0; i<this->columnCount(); i++) {
        l->setTextAlignment( i, Qt::AlignHCenter );
    }

    currIdRoundRobin = -1;
    setAutoFillBackground(false);

    QObject::connect( this, SIGNAL( newSelection( bool ) ) , this, SLOT( selectionChanged( bool ) ) );
    QObject::connect( this, SIGNAL( itemChanged ( QTreeWidgetItem*, int ) ) , this, SLOT( itemChanged ( QTreeWidgetItem*, int ) ) );
    
    setIconSize( QSize( 100, 20 ) );

    initToolBar();
    initPopup();
    selectionChanged( false );
}

QAjDownloadWidget::~QAjDownloadWidget()
{
    delete linuxIcon;
    delete windowsIcon;
    delete macIcon;
    delete otherOsIcon;
    delete netwareIcon;
    delete solarisIcon;
    delete freeBsdIcon;
}

/*!
    \fn QAjDownloadWidget::initToolBar()
 */
void QAjDownloadWidget::initToolBar()
{
    toolBar = new QToolBar( "download operations", this );

    pauseDownloadButton = toolBar->addAction( QIcon(":/pause.png"), tr("pause download"), this, SLOT( pauseSlot() ) );
    pauseDownloadButton->setToolTip( tr("pause download") );

    resumeDownloadButton = toolBar->addAction( QIcon(":/resume.png"), tr("resume download"), this, SLOT( resumeSlot() ) );
    resumeDownloadButton->setToolTip( tr("resume download") );

    cancelDownloadButton = toolBar->addAction( QIcon(":/cancel.png"), tr("cancel download"), this, SLOT( cancelSlot() ) );

    partListButton = toolBar->addAction( QIcon(":/partlist.png"), tr("show part list"), this, SLOT( partListSlot() ) );
    renameDownloadButton = toolBar->addAction( QIcon(":/rename.png"), tr("rename download"), this, SLOT( renameSlot() ) );
    renamePlusDownloadButton = toolBar->addAction( QIcon(":/rename_plus.png"), tr("rename download by clipboard"), this, SLOT( renamePlusSlot() ) );

    openDownloadButton = toolBar->addAction( QIcon(":/exec.png"), tr("open download"), this, SLOT( openSlot() ) );

    copyLinkButton = toolBar->addAction( QIcon(":/text_block.png"), tr("copy ajfsp link to clipboard"), this, SLOT(linkSlot()) );

    clearDownloadButton = toolBar->addAction( QIcon(":/filter.png"), tr("remove finished/canceld download"), this, SLOT( cleanSlot() ) );

    toolBar->addSeparator();

    powerCheck = new QCheckBox( toolBar );
    powerCheck->setText( tr("Power Download:") );
    powerCheck->setChecked( false );
    powerCheck->adjustSize();
    toolBar->addWidget( powerCheck );

    powerSpin = new QDoubleSpinBox( toolBar );
    powerSpin->setRange( 2.2, 50.0 );
    powerSpin->setSingleStep( 0.1 );
    powerSpin->setDecimals( 1 );
    toolBar->addWidget( powerSpin );

    //connect( powerEdit, SIGNAL( returnPressed() ), this, SLOT( applyPowerDownload() ) );
    connect( powerSpin, SIGNAL( valueChanged( const QString&) ), this, SLOT( applyPowerDownload() ) );
    connect( powerSpin, SIGNAL( valueChanged( double ) ), this, SLOT( applyPowerDownload() ) );

    powerOkButton = toolBar->addAction( QIcon(":/ok.png"), tr("apply power download"), this, SLOT( applyPowerDownload() ) );

    powerMaxButton = toolBar->addAction( QIcon(":/launch.png"), tr("set all downloads to 1:50"), this, SLOT( maxPowerDownload() )  );

    #ifdef AJQTGUI_MODE_SPECIAL
        powerMaxButton->setVisible( );
    #else
        char* mode = getenv( "AJQTGUI_MODE" );
        powerMaxButton->setVisible((( mode != NULL )) && ( strcmp(mode, "SPECIAL") == 0 ));
    #endif
}


void QAjDownloadWidget::insertDownload(QString id, QString hash, QString fileName, QString status, QString size, QString ready, QString power, QString tempNumber)
{
    QAjDownloadItem *downloadItem = findDownload( id );
    if ( downloadItem == NULL )
    {
        downloadItem = new QAjDownloadItem( id, this );
        connect(downloadItem->powerSpin, SIGNAL(powerChanged(QString, double)), this, SLOT(applyPowerDownload(QString, double)));
        downloads[ id ] = downloadItem;
        if(powerMaxButton->isVisible())
        {
            xml->set( "setpowerdownload", "&Powerdownload="+QConvert::power( 50 )+"&id="+id );
        }
        downloadItem->update( hash, fileName, status, size, ready, power, tempNumber );
//         downloadItem->initPowerSpin();
        updateView( true );
    } else {
        downloadItem->update( hash, fileName, status, size, ready, power, tempNumber );
    }
}


void QAjDownloadWidget::insertUser(QString downloadId, QString id, QString fileName, QString speed, QString status, QString power, QString queuePos, QString os, QTime time)
{
    QAjDownloadItem *downloadItem = findDownload( downloadId );
    if ( downloadItem == NULL )
    {
        downloadItem = new QAjDownloadItem( downloadId, this );
        downloads[ downloadId ] = downloadItem;
        downloadItem->setText( FILENAME_DOWN_INDEX, fileName );
    }
    QIcon *osIcon;
    if ( os == LINUX )
        osIcon = linuxIcon;
    else if ( os == WINDOWS )
        osIcon = windowsIcon;
    else if ( os == MAC )
        osIcon = macIcon;
    else if ( os == SOLARIS )
        osIcon = solarisIcon;
    else if ( os == FREEBSD )
        osIcon = freeBsdIcon;
    else if ( os == NETWARE )
        osIcon = netwareIcon;
    else
        osIcon = otherOsIcon;
    downloadItem->updateUser( id, fileName, speed, status, power, queuePos, userStatusDescr[status], osIcon, time );
}


bool QAjDownloadWidget::remove( QString id )
{
    if( removeDownload( id ) )
    {
        return true;
    }
    else
    {
        DownloadUser du = findParent( id );
        if( du.user != NULL )
        {
            du.download->decSources( du.user->getStatus() );
            du.download->removeUser( id );
            return true;
        }
    }
    return false;
}

void QAjDownloadWidget::cancelSlot()
{
    QString text = tr("Do you realy want to cancel") + " "
                    + QString::number(selectedItems().size()) + " "
                    + (selectedItems().size()>1?tr("downloads?"):tr("download?"));
    if ( QMessageBox::question( this, tr("Confirm"), text, QMessageBox::No, QMessageBox::Yes ) == QMessageBox::Yes )
    {
        processSelected( "canceldownload" );
    }
}

void QAjDownloadWidget::cleanSlot()
{
    xml->set( "cleandownloadlist" );
}

void QAjDownloadWidget::resumeSlot()
{
    processSelected( "resumedownload" );
}

void QAjDownloadWidget::pauseSlot()
{
    processSelected( "pausedownload" );
}

void QAjDownloadWidget::partListSlot()
{
    requestSelected( "downloadpartlist" );
}

void QAjDownloadWidget::renameSlot()
{
    QString oldFilename;
    QString newFilename;
    bool ok;
    QList<QAjItem *>  selectedItems = selectedAjItems();
    int i;
    for ( i=0; i<selectedItems.size(); i++ )
    {
        oldFilename = selectedItems[i]->text( FILENAME_DOWN_INDEX );
        newFilename = QInputDialog::getText( this, tr("rename download"), tr("enter new filename for ") + oldFilename, QLineEdit::Normal, oldFilename, &ok );
        newFilename = QString( QUrl::toPercentEncoding( newFilename ) );
        if ( ok && !newFilename.isEmpty() )
        {
            xml->set( "renamedownload", "&id=" + selectedItems[i]->getId() + "&name=" + newFilename );
        }
    }
}

void QAjDownloadWidget::renamePlusSlot()
{
    QString oldFilename;
    QString newFilename;
    QString newFilenameBase = qApp->clipboard()->text( QClipboard::Clipboard );
    QList<QAjItem *>  selectedItems = selectedAjItems();
    int i;
    for ( i=0; i<selectedItems.size(); i++ )
    {
        oldFilename = selectedItems[i]->text( FILENAME_DOWN_INDEX );
        newFilename = newFilenameBase;
        if (selectedItems.size() > 1)
        {
            newFilename += "_" + QString::number(i+1);
        }
        QStringList s = oldFilename.split(".");
        if (s.size() > 1)
        {
            newFilename += "." + s[s.size() - 1];
        }
        newFilename = QString( QUrl::toPercentEncoding( newFilename ) );
        if (!newFilename.isEmpty())
        {
            xml->set( "renamedownload", "&id=" + selectedItems[i]->getId() + "&name=" + newFilename );
        }
    }
}

void QAjDownloadWidget::openSlot()
{
    QStringList args = Juicer::getExec();
    QString exec = args.takeFirst();

    QString iDir, tDir;
    // determine the path
    AjSettings::LOCATION location = (AjSettings::LOCATION)QAjOptionsDialog::getSetting( "location", AjSettings::SAME ).toInt();

    if( location == AjSettings::SPECIFIC )
    {
        iDir = QAjOptionsDialog::getSetting( "incomingDirSpecific", "/" ).toString() + QDir::separator();
        tDir = QAjOptionsDialog::getSetting( "tempDirSpecific", "/" ).toString() + QDir::separator();
    }
    else if( location == AjSettings::SAME )
    {
        iDir = incomingDir.absolutePath() + QDir::separator();
        tDir = tempDir.absolutePath() + QDir::separator();
    }
    else // ftp
    {
        // TODO
        return;
    }

    QList<QAjItem*> items = selectedAjItems();
    int i;
    for (i=0; i<items.size(); i++)
    {
        QAjDownloadItem* ajDownloadItem = (QAjDownloadItem*)items[i];
        if( ajDownloadItem->getStatus() == DOWN_FINISHED )
        {
            args.push_back( iDir + ajDownloadItem->text( FILENAME_DOWN_INDEX ) );
        }
        else
        {
            args.push_back( tDir + ajDownloadItem->getTempNumber() + ".data" );
        }
        QProcess::startDetached( exec, args );
        args.pop_back();
    }
}

void QAjDownloadWidget::linkSlot() {

    QString link;

    QList<QAjItem *>  selectedItems = selectedAjItems();
    QAjDownloadItem* ajDownloadItem = (QAjDownloadItem*)selectedItems[0];
    link = ajDownloadItem->getLinkAJFSP();

    QApplication::clipboard()->setText(link);
}

void QAjDownloadWidget::selectionChanged(  bool oneSelected  )
{
    bool onePaused = false;
    bool oneActive = false;
    bool oneFinished = false;

    QList<QAjItem *>  selectedItems = selectedAjItems();
    int i;
    for ( i=0; i<selectedItems.size(); i++ )
    {
        QAjItem* downloadItem = selectedItems[i];
        if ( downloadItem->getStatus() == DOWN_PAUSED )
            onePaused = true;
        if ( ( downloadItem->getStatus() == DOWN_SEARCHING ) || ( downloadItem->getStatus() == DOWN_LOADING ) )
            oneActive = true;
        if ( downloadItem->getStatus() == DOWN_FINISHED )
            oneFinished = true;
        if ( onePaused && oneActive && oneFinished )
            break;
    }
    cancelDownloadButton->setEnabled( oneSelected );
    partListButton->setEnabled( oneSelected );
    renameDownloadButton->setEnabled( oneSelected );
    renamePlusDownloadButton->setEnabled( oneSelected );
    resumeDownloadButton->setEnabled( onePaused );
    pauseDownloadButton->setEnabled( oneActive );
    openDownloadButton->setEnabled( oneSelected );
    copyLinkButton->setEnabled( oneSelected );
}

void QAjDownloadWidget::updateView( bool force )
{
    QList<QAjDownloadItem*> finished;
    if( force || this->isVisible() )
    {
        int i;
        for (i=0; i<topLevelItemCount(); i++)
        {
            QAjDownloadItem* item = ((QAjDownloadItem*)topLevelItem(i));
            if( item->updateView( &downloadStatusDescr ) )
                finished << item;
        }
    }
    if( ! finished.isEmpty() )
        downloadsFinished( finished );
}

QAjDownloadItem* QAjDownloadWidget::findDownload( QString id )
{
    if (downloads.contains( id ))
        return downloads[id];
    else
        return NULL;
}

bool QAjDownloadWidget::removeDownload( QString id )
{
    if( downloads.contains( id ) )
    {
        // first remove it form the hashtable, than delete it
        QAjDownloadItem* item = downloads[ id ];
        downloads.remove( id );
        delete item;
        return true;
    }
    return false;
}


DownloadUser QAjDownloadWidget::findParent( QString id )
{
    DownloadUser du;
    du.download = NULL;
    du.user = NULL;
    int i;
    for (i=0; i<topLevelItemCount() && du.user == NULL; i++)
    {
        du.download = (QAjDownloadItem*)topLevelItem(i);
        du.user = du.download->findUser( id );
    }
    return du;
}


/*!
    \fn QAjDownloadWidget::getNextIdRoundRobin()
 */
QString QAjDownloadWidget::getNextIdRoundRobin()
{
    if (topLevelItemCount() < 1)
        return "";
    currIdRoundRobin ++;
    currIdRoundRobin %= topLevelItemCount();
    return ((QAjDownloadItem*)topLevelItem( currIdRoundRobin ))->getId();
}


/*!
    \fn QAjDownloadWidget::findDownloadByTempNum( QFileInfo tempFile )
 */
QString QAjDownloadWidget::findDownloadByTempNum( QFileInfo tempFile )
{
    if (tempDir.absolutePath() == tempFile.absolutePath())
    {
        QString tempNum = tempFile.baseName();
        QAjDownloadItem* item;
        int i;
        for ( i=0; i<topLevelItemCount(); i++ )
        {
            item = (QAjDownloadItem*)topLevelItem(i);
            if ( item->getTempNumber() == tempNum )
                return item->text( FILENAME_DOWN_INDEX );
        }
    }
    return tempFile.fileName();
}

void QAjDownloadWidget::storeDownloadFtp()
{
    FTP* ftp = NULL;
    QString filename, localDir;
    QList<QTreeWidgetItem *>  selectedItems = this->selectedItems();

    QString dir = QAjOptionsDialog::getSetting( "ftp", "dir", "/" ).toString();

    if ( ! dir.endsWith( '/' ) )
    {
        dir += '/';
    }
    ftp = new FTP( this );

    int i;
    for ( i=0; i<selectedItems.size(); i++ )
    {
        filename = selectedItems.at(i)->text( FILENAME_DOWN_INDEX );
        localDir = QFileDialog::getExistingDirectory( this, "save \"" + filename + "\" + to" );
        if ( localDir != "" )
        {
            if ( ! localDir.endsWith( QDir::separator() ) )
            {
                localDir += QDir::separator();
            }
            QFile* dstFile = new QFile( localDir + filename );
            if ( ! dstFile->exists() )
            {
                dstFile->open( QIODevice::WriteOnly );
                ftp->add( dir + filename, dstFile );
            }
        }
    }
    ftp->start();
}

void QAjDownloadWidget::processSelected( QString request, QString para )
{
    QList<QAjItem *>  selectedItems = selectedAjItems();
    int i;
    for ( i=0; i<selectedItems.size(); i++ )
    {
        xml->set( request, para + "&id=" + selectedItems[i]->getId() );
    }
}

void QAjDownloadWidget::requestSelected( QString request, QString para )
{
    QList<QAjItem *>  selectedItems = selectedAjItems();
    int i;
    for ( i=0; i<selectedItems.size(); i++ )
    {
        xml->get( request, para + "&id=" + selectedItems[i]->getId() );
    }
}

void QAjDownloadWidget::applyPowerDownload()
{
    float value;
    if ( powerCheck->isChecked() )
        value = powerSpin->value();
    else
        value = 1.0;
    processSelected( "setpowerdownload", "&Powerdownload=" + QConvert::power( value ) );
}


void QAjDownloadWidget::applyPowerDownload(QString id, double value)
{
    xml->set( "setpowerdownload", "&Powerdownload="+QConvert::power( value )+"&id="+id );
}

void QAjDownloadWidget::maxPowerDownload()
{
    QList<QString> ids = downloads.keys();
    int i;
    for ( i=0; i<ids.size(); i++ )
    {
        xml->set( "setpowerdownload", "&Powerdownload="+QConvert::power( 50 )+"&id="+ids[i] );
    }
}



/*!
    \fn QAjDownloadWidget::setDirs( QFileInfo tmpDir, QFileInfo inDir )
 */
void QAjDownloadWidget::setDirs( QFileInfo tmpDir, QFileInfo inDir )
{
    this->tempDir = tmpDir;
    this->incomingDir = inDir;
}



/*!
    \fn QAjDownloadWidget::initPopup()
 */
void QAjDownloadWidget::initPopup()
{
    popup->setTitle( tr("&Download") );
    popup->addAction( pauseDownloadButton );
    popup->addAction( resumeDownloadButton );
    popup->addAction( cancelDownloadButton );
    popup->addAction( partListButton );
    popup->addAction( renameDownloadButton );
    popup->addAction( renamePlusDownloadButton );
    popup->addAction( openDownloadButton );
    popup->addAction( copyLinkButton );
    popup->addSeparator();
    popup->addAction( clearDownloadButton );
}


/*!
    \fn QAjDownloadWidget::findDownload( QString size, QString hash )
 */
QAjDownloadItem* QAjDownloadWidget::findDownload( QString size, QString hash )
{
    QList<QAjDownloadItem*> list = downloads.values();
    int i;
    for( i=0; i<list.size(); i++ )
    {
        if( QString::number((int)list[i]->getSize()) == size && list[i]->getHash() == hash )
            return list[i];
    }
    return NULL;
}

void QAjDownloadWidget::itemChanged( QTreeWidgetItem* item, int column )
{

}

