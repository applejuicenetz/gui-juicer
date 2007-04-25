//
// C++ Implementation: qajsharemetawidget
//
// Description: 
//
//
// Author: Holger Gerth <holger.gerth@informatik.tu-chemnitz.de>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "qajsharemetawidget.h"

QAjShareMetaWidget::QAjShareMetaWidget( QWidget *parent ) : QMainWindow(parent)
{
    dock = new AjShareFilesDockWidget( tr("files"), this );
    #if QT_VERSION >= 0x040300
        connect( dock, SIGNAL(dockLocationChanged( Qt::DockWidgetArea )), this, SLOT(shareFilesDockChanged(Qt::DockWidgetArea)));
    #endif

    this->sharedFilesWidget = NULL;

//     connect( shareWidget, SIGNAL( newSelection( bool ) ) , this, SLOT( selectionChanged( bool ) ) );

}


QAjShareMetaWidget::~QAjShareMetaWidget()
{
    delete sharedFilesWidget;
}


/*!
    \fn QAjShareMetaWidget::shareFilesDockChanged( Qt::DockWidgetArea area )
    prepared for qt 4.3
 */
void QAjShareMetaWidget::shareFilesDockChanged( Qt::DockWidgetArea area )
{
    QSettings localSettings;
    localSettings.setValue("filesPos", area);
}


/*!
    \fn QAjShareMetaWidget::setShareWidget(QAjShareWidget* shareWidget)
 */
void QAjShareMetaWidget::setShareWidget(QAjShareWidget* shareWidget)
{
    this->shareWidget = shareWidget;
    this->setCentralWidget(shareWidget);

    shareWidget->popup->addSeparator();
    showFiles = new QAction("show shared files in this directory", this );
    showFiles->setCheckable( true );
    QObject::connect(showFiles, SIGNAL(toggled(bool)), dock, SLOT(setVisible(bool)));
    QObject::connect(showFiles, SIGNAL(toggled(bool)), this, SLOT(dockVisibleSlot(bool)));
    shareWidget->popup->addAction( showFiles );

    QSettings localSettings;
    showFiles->setChecked(localSettings.value("filesVisible", true).toBool());
}

void QAjShareMetaWidget::setShareFilesWidget(QAjShareFilesWidget* shareFilesWidget) {
    this->sharedFilesWidget = shareFilesWidget;
    dock->setWidget(sharedFilesWidget);

    QSettings localSettings;
    addDockWidget((Qt::DockWidgetArea)localSettings.value("filesPos", Qt::RightDockWidgetArea).toInt(), dock);
    dock->setVisible(localSettings.value("filesVisible", true).toBool());

    shareWidget->copyLinkButton = shareWidget->toolBar->addAction( QIcon(":/text_block.png"), "copy ajfsp link to clipboard", sharedFilesWidget, SLOT(linkSlot()) );

    shareWidget->copyLinkButton->setEnabled( false );

    sharedFilesWidget->popup->addAction( shareWidget->copyLinkButton );

    QObject::connect( sharedFilesWidget, SIGNAL( newSelection( bool ) ) ,
                      shareWidget->copyLinkButton, SLOT( setEnabled( bool ) ) );
}


/*!
    \fn QAjShareMetaWidget::dockVisibleSlot( bool visible )
 */
void QAjShareMetaWidget::dockVisibleSlot( bool visible )
{
    dock->enabled = visible;
}

void QAjShareMetaWidget::selectionChanged( bool oneSelected )
{
    if ( oneSelected && (this->sharedFilesWidget != NULL ) ) {
      QList<QAjItem *> selectedItems = this->shareWidget->selectedAjItems();

      QAjShareItem *shareItem =  (QAjShareItem*)selectedItems[0];

      sharedFilesWidget->updateVisibleFiles( shareItem->path );
    }

}



