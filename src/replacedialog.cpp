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

#include "replacedialog.h"

ReplaceDialog::ReplaceDialog(QItemList& items, int itemCol, QWidget* parent, Qt::WFlags fl) : QDialog( parent, fl ), Ui::ReplaceDialog() {
    setupUi(this);
    errorMsg = validLabel->text();
    connect(beforeEdit, SIGNAL(textChanged(const QString &)), this, SLOT(createPreview()));
    connect(afterEdit, SIGNAL(textChanged(const QString &)), this, SLOT(createPreview()));
    connect(typeComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(createPreview()));
    connect(caseSensitiveCheckBox, SIGNAL(toggled(bool)), this, SLOT(createPreview()));
    //linkLabel->setText("<a href='http://doc.trolltech.com/4.5/qregexp.html#details'>more...</a>");

    table->setRowCount(items.size());
    for(int i=0; i<items.size(); i++) {
        QString oldName = items.at(i)->text(itemCol);
        table->setItem(i, ORIGINAL, new QTableWidgetItem(oldName));
        table->setItem(i, ARROW, new QTableWidgetItem(QIcon(":/small/go-next.png"), ""));
        table->setItem(i, PREVIEW, new QTableWidgetItem());
    }
    createPreview();
}

ReplaceDialog::~ReplaceDialog() {
}

void ReplaceDialog::reject() {
  QDialog::reject();
}

void ReplaceDialog::accept() {
    bool same = false;
    for(int row1=0; row1<table->rowCount() && !same; row1++) {
        for(int row2=0; row2<table->rowCount() && !same; row2++) {
            same = (row1 != row2) && (table->item(row1, PREVIEW)->text() == table->item(row2, PREVIEW)->text());
        }
    }
    if(!same || QMessageBox::question(this, tr("Equal Filenames"),
             tr("Two or more files have the same new filename.\nAre you sure you want to continue?"),
             QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes) {
        QDialog::accept();
    }
}


/*!
    \fn ReplaceDialog::createPreview()
 */
void ReplaceDialog::createPreview() {
    re.setPattern(beforeEdit->text());
    re.setCaseSensitivity(caseSensitiveCheckBox->isChecked()?Qt::CaseSensitive:Qt::CaseInsensitive);
    re.setPatternSyntax(getPatternSyntax());
    validLabel->setText(re.isValid()?"":errorMsg);
    buttonBox->button(QDialogButtonBox::Ok)->setEnabled(re.isValid());
    if(re.isValid()) {
        for(int row=0; row<table->rowCount(); row++) {
            table->item(row, PREVIEW)->setText(replace(table->item(row, ORIGINAL)->text()));
            table->item(row, PREVIEW)->setForeground(QBrush(Qt::black));
        }
    } else {
        for(int row=0; row<table->rowCount(); row++) {
            table->item(row, PREVIEW)->setForeground(QBrush(Qt::gray));
        }
    }
    table->resizeColumnsToContents();
    // -- some extra spacing --
    table->setColumnWidth(ORIGINAL, table->columnWidth(ORIGINAL) + 20);
    table->setColumnWidth(ARROW, table->columnWidth(ARROW) + 15);
}


/*!
    \fn ReplaceDialog::replace(const QString& src)
 */
QString ReplaceDialog::replace(const QString& src) {
    return QString(src).replace(re, afterEdit->text());
}


/*!
    \fn ReplaceDialog::getPatternSyntax()
 */
QRegExp::PatternSyntax ReplaceDialog::getPatternSyntax() {
    // -- somehow dirty... --
    QString s = typeComboBox->currentText();
    if(s.contains("fixed", Qt::CaseInsensitive)) {
        return QRegExp::FixedString;
    } else if(s.contains("wildcard", Qt::CaseInsensitive)) {
        return QRegExp::Wildcard;
    } else if(s.contains("perl", Qt::CaseInsensitive)) {
        return QRegExp::RegExp;
    } else if(s.contains("greedy", Qt::CaseInsensitive)) {
        return QRegExp::RegExp2;
    } else {
        return QRegExp::FixedString;
    }
}
