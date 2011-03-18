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

#include "../../mainwin.h"

void nmapClass::callSearchHistoryVuln() {
#ifndef VULN_NO_DEBUG
    qDebug() << "searchVuln:: call...";
#endif

    if (!actionAdd_Bookmark->isEnabled()) {
        actionAdd_Bookmark->setEnabled(true);
        action_Add_BookmarkToolBar->setEnabled(true);
    }
    logHistory *history = new logHistory("nmapsi4/cacheVuln", hostCache);
    history->searchHistory(comboVulnRis->currentText(), comboVulnRis);
    delete history;
     
}

void nmapClass::callVulnCheck() {
    comboVulnRis->clear();
    comboVulnRis->insertItem(0,treeBookVuln->currentItem()->text(0));
    tabVuln->setCurrentIndex(0);
    searchVulnNG();
}

void nmapClass::vulnPostScan() {
    actSearch->setEnabled(true);
    actStop->setEnabled(false);
}

void nmapClass::updateComboVuln(const QString& value) {

    comboVulnRis->clear();

    if(comboVuln->currentIndex()) {
        comboVulnRis->insertItem(0,value);
    } else {
        comboVulnRis->lineEdit()->clear();
    }

}

void nmapClass::updateComboWebV() {

    // clear comboWeb on first time
    comboWebV->clear();
    
    //TODO:: value from treeWidget url
    /*for(int index=0; index < treeBookPar->topLevelItemCount(); index++) {
        comboPar->insertItem(1, treeBookPar->topLevelItem(index)->text(0));
    }*/
    
    // insert default static url in comboWeb vulnerabilitier
    QMap<QString, QString> tmpStaticProfileUrl_ = defaultUrlVuln();
    foreach (const QString key, tmpStaticProfileUrl_.keys()) {
	comboWebV->insertItem(1, key);
    }
    
    //TODO:: just a separetor from default and user saved url
    //comboWebV->insertSeparator(1);
}

