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

#include "addparbook.h"

mainParamClass::mainParamClass(const QString parameters) 
{
    setupUi(this);
    
    connect(doneButt, SIGNAL(clicked(bool)),
            this, SLOT(exit()));
    connect(cancelButt, SIGNAL(clicked(bool)),
            this, SLOT(close()));
    lineProfilePar->setText(parameters);
}

mainParamClass::~mainParamClass() 
{
    
}

void mainParamClass::exit() 
{
    if (!lineProfileName->text().isEmpty() && !lineProfilePar->text().isEmpty()) 
    {
        emit doneParBook(lineProfileName->text(), lineProfilePar->text());
        close();
    } 
    else 
    {
        // TODO lineEdit is Empty
    }
}
