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


#include "partlistdialog.h"

PartListDialog::PartListDialog(QWidget* parent, Qt::WFlags fl) : QDialog( parent, fl ), Ui::partListDialog()
{
    setupUi(this);
/*    QVBoxLayout* l = new QVBoxLayout(frame);
    paintWidget = new PartsWidget(frame);
    paintWidget->setSizePolicy(QSizePolicy (QSizePolicy::Expanding, QSizePolicy::Expanding));
    l->addWidget(paintWidget);
    frame->setLayout(l);*/
    sizeIcon->setText("");

    QPixmap readyPixmap(15, 15);
    readyPixmap.fill(QColor(PartsWidget::READY_COLOR));
    readyIcon->setPixmap(readyPixmap);

    QPixmap availablePixmap(15, 15);
    availablePixmap.fill(QColor(PartsWidget::AVAILABLE_COLOR));
    availableIcon->setPixmap(availablePixmap);

    QPixmap missingPixmap(15, 15);
    missingPixmap.fill(QColor(PartsWidget::MISSING_COLOR));
    missingIcon->setPixmap(missingPixmap);

    QPixmap rarePixmap(15, 15);
    rarePixmap.fill(QColor(PartsWidget::RARE_COLOR));
    rareIcon->setPixmap(rarePixmap);

    connect(paintWidget, SIGNAL(painted()), this, SLOT(paintedSlot()));
}

PartListDialog::~PartListDialog()
{
}

/*!
    \fn PartListDialog::update( Q_ULLONG size, QLinkedList<PartsWidget::Part>& partList )
 */
void PartListDialog::update( qulonglong size, QLinkedList<PartsWidget::Part>& partList )
{
    if ( !isVisible() )
    {
        move( QCursor::pos() - QPoint( width()/2, height()/2 ) );
        show();
    }

//     if ( ! timer->isActive() )
//         timer->start( 5000 );
    paintWidget->update( size, partList );
}

/*!
    \fn PartListDialog::setFilename( const QString& filename )
 */
void PartListDialog::setFilename( const QString& filename )
{
    setWindowTitle( "Juicer - " + filename );
}


/*!
    \fn PartListDialog::paintedSlot()
 */
void PartListDialog::paintedSlot()
{
    sizeLabel->setText( Convert::bytes(paintWidget->size) );
    readyLabel->setText( QString::number( paintWidget->ready, 'f', 2 ) + "%" );
    availableLabel->setText( QString::number( paintWidget->available, 'f', 2 ) + "%" );
    rareLabel->setText( QString::number( paintWidget->lessSources, 'f', 2 ) + "%" );
    missingLabel->setText( QString::number( paintWidget->missing, 'f', 2 ) + "%" );
}
