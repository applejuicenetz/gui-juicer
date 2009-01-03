//
// C++ Implementation: 
//
// Description: 
//
//
// Author: Matthias Reif <matthias.reif@informatik.tu-chemnitz.de>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//


#include "qajpartlistdialog.h"

QAjPartListDialog::QAjPartListDialog(QWidget* parent, Qt::WFlags fl) : QDialog( parent, fl ), Ui::partListDialog()
{
    setupUi(this);
    QVBoxLayout* l = new QVBoxLayout(frame);
    paintWidget = new QAjPartsWidget(frame);
    paintWidget->setSizePolicy(QSizePolicy (QSizePolicy::Expanding, QSizePolicy::Expanding));
    l->addWidget(paintWidget);
    frame->setLayout(l);
    sizeIcon->setText("");

    QPixmap readyPixmap(15, 15);
    readyPixmap.fill(QColor(QAjPartsWidget::READY_COLOR));
    readyIcon->setPixmap(readyPixmap);

    QPixmap availablePixmap(15, 15);
    availablePixmap.fill(QColor(QAjPartsWidget::AVAILABLE_COLOR));
    availableIcon->setPixmap(availablePixmap);

    QPixmap missingPixmap(15, 15);
    missingPixmap.fill(QColor(QAjPartsWidget::MISSING_COLOR));
    missingIcon->setPixmap(missingPixmap);

    QPixmap rarePixmap(15, 15);
    rarePixmap.fill(QColor(QAjPartsWidget::RARE_COLOR));
    rareIcon->setPixmap(rarePixmap);

    connect(paintWidget, SIGNAL(painted()), this, SLOT(paintedSlot()));
}

QAjPartListDialog::~QAjPartListDialog()
{
}

/*!
    \fn QAjPartListDialog::update( Q_ULLONG size, QLinkedList<Part>& partList )
 */
void QAjPartListDialog::update( qulonglong size, QLinkedList<Part>& partList )
{
    if ( !isVisible() )
    {
        move( QCursor::pos() - QPoint( width()/2, height()/2 ) );
        show();
    }

//     if ( ! timer->isActive() )
//         timer->start( 5000 );

    Part closePart;
    closePart.fromPosition = size ;
    closePart.type = -10;
    partList.push_back( closePart );

    paintWidget->update( size, partList );
}

/*!
    \fn QAjPartListDialog::setFilename( const QString& filename )
 */
void QAjPartListDialog::setFilename( const QString& filename )
{
    setWindowTitle( "Juicer - " + filename );
}


/*!
    \fn QAjPartListDialog::paintedSlot()
 */
void QAjPartListDialog::paintedSlot()
{
    sizeLabel->setText( QConvert::bytes(paintWidget->size) );
    readyLabel->setText( QString::number( paintWidget->ready, 'f', 2 ) + "%" );
    availableLabel->setText( QString::number( paintWidget->available, 'f', 2 ) + "%" );
    rareLabel->setText( QString::number( paintWidget->lessSources, 'f', 2 ) + "%" );
    missingLabel->setText( QString::number( paintWidget->missing, 'f', 2 ) + "%" );
}
