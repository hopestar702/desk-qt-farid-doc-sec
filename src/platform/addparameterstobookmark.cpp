/***************************************************************************
 *   Copyright (C) 2011 by Francesco Cecconi                               *
 *   francesco.cecconi@gmail.com                                           *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License.        *
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

#include "addparameterstobookmark.h"

addParametersToBookmark::addParametersToBookmark(QWidget* parent, const QString parameters)
: QDialog(parent)
{
    setupUi(this);

    connect(doneButt, SIGNAL(clicked(bool)),
            this, SLOT(exit()));
    connect(cancelButt, SIGNAL(clicked(bool)),
            this, SLOT(close()));
    lineProfilePar->setText(parameters);
}

addParametersToBookmark::~addParametersToBookmark()
{

}

void addParametersToBookmark::exit()
{
    if (!lineProfileName->text().isEmpty() && !lineProfilePar->text().isEmpty())
    {
        emit doneParBook(lineProfileName->text(), lineProfilePar->text());
        close();
    }
    else
    {
        QMessageBox::warning(this, tr("Warning - Nmapsi4"), tr("Insert profile name or/and profile parameters."), tr("Close"));
    }
}
