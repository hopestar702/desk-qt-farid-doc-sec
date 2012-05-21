/***************************************************************************
 *   Copyright (C) 2011-2012 by Francesco Cecconi                          *
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

#include "mainwindow.h"

QHash<QString, QString> MainWindow::defaultScanProfile() const
{
    // preload StringList with default static Scan profile
    QHash<QString, QString> tmpStaticProfileMap_;

    if (!m_userId)
    {
        tmpStaticProfileMap_.insert("Default","-sS -sV -O -T4 -v --traceroute");
        tmpStaticProfileMap_.insert("Default + Aggressive","-A -sS -sV -O -T4 -v --traceroute");
        tmpStaticProfileMap_.insert("Default + base nse script","--script=default,safe -sS -sV -O -T4 -v --traceroute");
        tmpStaticProfileMap_.insert(tr("Quick Scan"),"-T4 -F --traceroute");
        tmpStaticProfileMap_.insert(tr("Intense Scan"),"-T4 -v --traceroute");
        tmpStaticProfileMap_.insert(tr("Intense Scan, all TCP ports"),"-p 1-65535 -T4 -v --traceroute");
        // profile for full mode
        tmpStaticProfileMap_.insert(tr("Intense scan plus UDP"),"-sS -sU -T4 -v --traceroute");
        tmpStaticProfileMap_.insert(tr("Slow comprehensive scan"),
                                    "-sS -sU -T4 -v -PE -PP -PS80,443 -PA3389 -PU40125 -PY -g 53 --traceroute");
    }
    else
    {
        // for user mode
        tmpStaticProfileMap_.insert("Default","-sT -sV -T4 -v");
        tmpStaticProfileMap_.insert("Default + Aggressive","-A -sT -sV -T4 -v");
        tmpStaticProfileMap_.insert("Default + base nse script","--script=default,safe -sT -sV -T4 -v");
        tmpStaticProfileMap_.insert(tr("Intense Scan, all TCP ports"),"-p 1-65535 -T4 -v");
        tmpStaticProfileMap_.insert(tr("Intense Scan"),"-T4 -v");
        tmpStaticProfileMap_.insert(tr("Quick Scan"),"-T4 -F");
    }

    return tmpStaticProfileMap_;
}

void MainWindow::loadScanProfile()
{
    comboPar->clear();

    QHash<QString, QString> tmpStaticProfile_ = defaultScanProfile();

    QHash<QString, QString>::const_iterator i;
    for (i = tmpStaticProfile_.constBegin(); i != tmpStaticProfile_.constEnd(); ++i)
    {
        comboPar->insertItem(comboPar->count()+1,i.key());
    }
    
    comboPar->insertSeparator(comboPar->count()+1);

    // value from treeWidget parameters
    for(int index=0; index < treeBookPar->topLevelItemCount(); index++)
    {
        comboPar->addItem(treeBookPar->topLevelItem(index)->text(1));
    }

    comboParametersSelectedEvent();
}

void MainWindow::comboParametersSelectedEvent()
{
   // insert profile from comboPar to comboAdv
   int parIndex = comboPar->currentIndex();

    // if not 0
    QHash<QString, QString> tmpMap_ = defaultScanProfile();
    comboAdv->clear();
    if (parIndex <= tmpMap_.size())
    {
        // call static default profile for check
        comboAdv->insertItem(0, tmpMap_.value(comboPar->currentText()));
    }
    else
    {
        // saved user profile
        QList<QTreeWidgetItem *> resultList_ = treeBookPar->findItems(comboPar->currentText(),Qt::MatchExactly,1);
        comboAdv->insertItem(0, resultList_[0]->text(0));
    }
}

void MainWindow::resetComboParameters()
{
    comboAdv->setStyleSheet(QString::fromUtf8("color: rgb(153, 153, 153);"));
    comboParametersSelectedEvent();
}

QStringList MainWindow::loadExtensions()
{
    if(m_monitor->monitorHostNumber())
    {
        progressScan->setValue(55); // start progress bar
    }

    return comboAdv->lineEdit()->text().split(' ');
}
