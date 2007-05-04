//
// C++ Interface: qajsharemetawidget
//
// Description: 
//
//
// Author: Holger Gerth <holger.gerth@informatik.tu-chemnitz.de>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef QAJSHAREMETAWIDGET_H
#define QAJSHAREMETAWIDGET_H

#include <QtGlobal>
#include <QMainWindow>
#include <QWidget>
#include <QDockWidget>

#include "qxmlmodule.h"
#include "qajsharewidget.h"
#include "qajsharefileswidget.h"

class Juicer;

/**
	@author Holger Gerth <holger.gerth@informatik.tu-chemnitz.de>
*/
class QAjShareMetaWidget : public QMainWindow
{
Q_OBJECT
public:
    QAjShareMetaWidget( QWidget *parent = 0 );

    ~QAjShareMetaWidget();
    void setShareWidget(QAjShareWidget* shareWidget);
    void setShareFilesWidget(QAjShareFilesWidget* shareFilesWidget);

    QAjShareWidget* shareWidget;
    QAjShareFilesWidget* sharedFilesWidget;
    QAction* showFiles;

    class AjShareFilesDockWidget : public QDockWidget
    {
    public:
        AjShareFilesDockWidget(QString title, QAjShareMetaWidget* parent)
            : QDockWidget(title, parent) {
            enabled = false;
        }
        QSize sizeHint() const {
            QSettings localSettings;
            localSettings.beginGroup("FilesDock");
            return localSettings.value("size", true).toSize();
        }
        bool enabled;
    protected:
        void closeEvent( QCloseEvent * event )
        {
            enabled = false;
            ((QAjShareMetaWidget*)this->parentWidget())->showFiles->setChecked(false);
            QWidget::closeEvent( event );
        }
    } *dock;

private slots:
    void shareFilesDockChanged( Qt::DockWidgetArea area );
    void dockVisibleSlot( bool visible );
    void selectionChanged( bool oneSelected );
    void setPrioritySlot();

};

#endif
