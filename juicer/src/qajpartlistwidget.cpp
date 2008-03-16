/***************************************************************************
 *   Copyright (C) 2005 by Matthias Reif                                   *
 *   matthas.reif@informatik.tu-emnitz.de                                  *
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
#include "qajpartlistwidget.h"

QAjPartListWidget::QAjPartListWidget( QWidget *parent ) : QWidget( parent )
{
    this->setWindowIcon( QIcon(":/juicer.png") );
    this->setFixedSize( 600, 300 );
    paintWidget = new QAjPartsWidget( this );

    filesizeLabel = new QLabel(this);
    filesizeLabel->move( paintWidget->x(), paintWidget->y() + paintWidget->height() + 20 );
    filesizeLabel->resize( 100 , 30 );
    filesizeLabel->hide();
    readyLabel = new QLabel(this);
    readyLabel->move( paintWidget->x() + 30, paintWidget->y() + paintWidget->height() + 20 );
    readyLabel->resize( 120 , 30 );
    availableLabel = new QLabel(this);
    availableLabel->move( paintWidget->x() + 180, paintWidget->y() + paintWidget->height() + 20 );
    availableLabel->resize( 120 , 30 );
    lessSourcesLabel = new QLabel(this);
    lessSourcesLabel->move( paintWidget->x() + 180, paintWidget->y() + 27 + paintWidget->height() + 20 );
    lessSourcesLabel->resize( 190 , 30 );
    missingLabel = new QLabel(this);
    missingLabel->move( paintWidget->x() + 330, paintWidget->y() + paintWidget->height() + 20 );
    missingLabel->resize( 120 , 30 );

    okButton = new QPushButton( this );
    okButton->setText("OK");
    okButton->adjustSize();
    okButton->move( paintWidget->x() + paintWidget->width() - okButton->width(), paintWidget->y() + paintWidget->height() + 20 );

    connect( okButton, SIGNAL( clicked() ), this, SLOT( hide() ) );

    timer = new QTimer( this );
    id = "";
}


QAjPartListWidget::~QAjPartListWidget()
{}


/*!
    \fn QAjPartListWidget::update( Q_ULLONG size, QLinkedList<Part> partList )
 */
void QAjPartListWidget::update( qulonglong size, QLinkedList<Part> partList )
{
    if ( !isVisible() )
    {
        move( QCursor::pos() - QPoint( width()/2, height()/2 ) );
        show();
    }

    if ( ! timer->isActive() )
        timer->start( 5000 );

    this->size = size;
    this->partList = partList;
    Part closePart;
    closePart.fromPosition = size;
    closePart.type = -10;
    this->partList.push_back( closePart );

    paintWidget->update( size, this->partList );
    paintWidget->repaint();
    repaint();
}


/*!
    \fn QAjPartListWidget::paintEvent( QPaintEvent* )
 */
void QAjPartListWidget::paintEvent( QPaintEvent* )
{
    QPainter painter;
    painter.begin( this );

    painter.fillRect( paintWidget->x(), paintWidget->y() + paintWidget->height() + 20, 20, 20, QBrush(QColor(0, 220,0)) );

    painter.fillRect( paintWidget->x() + 150, paintWidget->y() + paintWidget->height() + 20, 20, 20, QBrush(QColor(0,0,220)) );

    painter.fillRect( paintWidget->x() + 150, paintWidget->y() + 30 + paintWidget->height() + 20, 20, 20, QBrush(QColor(220,220,0)) );

    painter.fillRect( paintWidget->x() + 300, paintWidget->y() + paintWidget->height() + 20, 20, 20, QBrush(QColor(240,0,0)) );

    painter.end();

    readyLabel->setText( QString::number( paintWidget->ready, 'f', 2 ) + "% finished" );
    availableLabel->setText( QString::number( paintWidget->available, 'f', 2 ) + "% seen" );
    lessSourcesLabel->setText( QString::number( paintWidget->lessSources, 'f', 2 ) + "% less than 5 sources" );
    missingLabel->setText( QString::number( paintWidget->missing, 'f', 2 ) + "% not seen" );

}


/*!
    \fn QAjPartListWidget::setFilename( QString filename )
 */
void QAjPartListWidget::setFilename( QString filename )
{
    setWindowTitle( "Juicer - " + filename );
}


/*!
    \fn QAjPartListWidget::setId( QString id )
 */
void QAjPartListWidget::setId( QString id )
{
    this->id = id;
}
