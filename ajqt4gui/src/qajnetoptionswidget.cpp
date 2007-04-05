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
#include "qajnetoptionswidget.h"

QAjNetOptionsWidget::QAjNetOptionsWidget( QWidget *parent, const char *name) : QWidget( parent )
{
	int s = 0;
	maxDownLabel = new QLabel( this );
	maxDownLabel2 = new QLabel( this );
	maxDownSpin = new QSpinBox( this );
	maxDownSpin->setRange( 1, 999999 );
	maxDownSpin->setSingleStep( 1 );
	maxUpLabel = new QLabel(this);
	maxUpLabel2 = new QLabel(this);
	maxUpSpin = new QSpinBox( this );
	maxUpSpin->setRange( 1, 999999 );
	maxUpSpin->setSingleStep( 1 );
	maxSlotLabel = new QLabel(this);
	maxSlotLabel2 = new QLabel(this);
	maxSlotSpin = new QSpinBox( this );
	maxSlotSpin->setRange( 1, 999999 );
	maxSlotSpin->setSingleStep( 1 );
	maxSourcesLabel = new QLabel(this);
	maxSourcesSpin = new QSpinBox( this );
	maxSourcesSpin->setRange( 1, 999999 );
	maxSourcesSpin->setSingleStep( 1 );
	maxConLabel = new QLabel(this);
	maxConSpin = new QSpinBox( this );
	maxConSpin->setRange( 1, 999999 );
	maxConSpin->setSingleStep( 1 );
	maxNewConLabel = new QLabel(this);
	maxNewConSpin = new QSpinBox( this );
	maxNewConSpin->setRange( 1, 200 );
	maxNewConSpin->setSingleStep( 1 );
	tcpPortLabel = new QLabel(this);
	tcpPortEdit = new QLineEdit(this);

	maxDownLabel->setText( tr("max. download speed:") );
	maxUpLabel->setText( tr("max. upload speed:") );
	maxSlotLabel->setText( tr("max. speed per slot:") );
	maxSourcesLabel->setText( tr("max. sources per file:") );
	maxConLabel->setText( tr("max. connections:") );
	maxNewConLabel->setText( tr("max. new connections per 10s:") );
	maxDownLabel2->setText( tr("kByte/s") );
	maxUpLabel2->setText( tr("kByte/s") );
	maxSlotLabel2->setText( tr("kByte/s") );
	tcpPortLabel->setText( tr("tcp Port:") );
	
	maxDownLabel->adjustSize();
	maxDownLabel2->adjustSize();
	maxUpLabel->adjustSize();
	maxUpLabel2->adjustSize();
	maxSlotLabel->adjustSize();
	maxSlotLabel2->adjustSize();
	maxSourcesLabel->adjustSize();
	maxConLabel->adjustSize();
	maxNewConLabel->adjustSize();
	tcpPortLabel->adjustSize();
	
	maxDownSpin->resize( 80, maxDownLabel->height() +4 );
	maxUpSpin->resize( 80, maxUpLabel->height() +4 );
	maxSlotSpin->resize( 80, maxSlotLabel->height() +4 );
	maxSourcesSpin->resize( 80, maxSourcesLabel->height() +4 );
	maxConSpin->resize( 80, maxConLabel->height() +4 );
	maxNewConSpin->resize( 80, maxNewConLabel->height() +4 );
	tcpPortEdit->resize( 100, tcpPortLabel->height() +4 );
	
	tcpPortEdit->setAlignment( Qt::AlignRight );
	
	int verStart = 10;
	int verSpace = maxDownLabel->height() + 14;

	maxDownLabel->move( 10, verStart + verSpace * 0 );
	maxUpLabel->move( 10, verStart + verSpace * 1 );
	maxSlotLabel->move( 10, verStart + verSpace * 2 );
	maxSourcesLabel->move( 10, verStart + verSpace * 3 );
	maxConLabel->move( 10, verStart + verSpace * 4 );
	maxNewConLabel->move( 10, verStart + verSpace * 5 );
	tcpPortLabel->move( 10, verStart + verSpace * 6 );
	
	ACT_S( maxDownLabel );
	ACT_S( maxUpLabel );
	ACT_S( maxSlotLabel );
	ACT_S( maxSourcesLabel );
	ACT_S( maxConLabel );
	ACT_S( maxNewConLabel );
	ACT_S( tcpPortLabel );
	
	maxDownSpin->move( s + 15, maxDownLabel->y() -2 );
	maxUpSpin->move( s + 15, maxUpLabel->y() -2 );
	maxSlotSpin->move( s + 15, maxSlotLabel->y() -2 );
	maxSourcesSpin->move( s + 15, maxSourcesLabel->y() -2 );
	maxConSpin->move( s + 15, maxConLabel->y() -2 );
	maxNewConSpin->move( s + 15, maxNewConLabel->y() -2 );
	tcpPortEdit->move( s + 15, tcpPortLabel->y() -2 );
	
	maxDownLabel2->move( maxDownSpin->x()+maxDownSpin->width() +5, maxDownSpin->y() );
	maxUpLabel2->move( maxUpSpin->x()+maxUpSpin->width() +5, maxUpSpin->y() );
	maxSlotLabel2->move( maxSlotSpin->x()+maxSlotSpin->width() +5, maxSlotSpin->y() );
	
	setMinimumHeight( tcpPortLabel->y() + tcpPortLabel->height() );

}

QAjNetOptionsWidget::~QAjNetOptionsWidget()
{
	delete maxDownLabel;
	delete maxDownLabel2;
	delete maxDownSpin;
	delete maxUpLabel;
	delete maxUpLabel2;
	delete maxUpSpin;
	delete maxSlotLabel;
	delete maxSlotLabel2;
	delete maxSlotSpin;
	delete tcpPortLabel;
	delete tcpPortEdit;

	delete maxSourcesLabel;
	delete maxSourcesSpin;
	delete maxConSpin;
	delete maxConLabel;
	delete maxNewConLabel;
	delete maxNewConSpin;
}

void QAjNetOptionsWidget::setAjSettigs( AjSettings settings )
{
	maxDownSpin->setValue( settings.maxDown.toInt()/1024 );
	maxUpSpin->setValue( settings.maxUp.toInt()/1024 );
	maxSlotSpin->setValue( settings.maxSlot.toInt()/1024 );
	maxSourcesSpin->setValue( settings.maxSources.toInt() );
	maxConSpin->setValue( settings.maxCon.toInt() );
	maxNewConSpin->setValue( settings.maxNewCon.toInt() );
	tcpPortEdit->setText( settings.tcpPort );
	
}
