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

#include "mainwin.h"

namespace varDiscover {
    QList<discover*> listDiscover;
    QList<QTreeWidgetItem*> listTreeItemDiscover;
    int ipCounter = 0;
    QStringList sendList;
    QStringList recvList;
    bool _discoverIsActive = false;
}

void nmapClass::startDiscover()
{
    // take local interfaces
    comboDiscover->clear();
    comboDiscover->insertItem(0, "Select Interface");

    discover *discoverPtr = new discover(uid);
    foreach (const QNetworkInterface &interface, discoverPtr->getAllInterfaces(discover::AllInterfaceWithAddress))
    {
        comboDiscover->insertItem(1, interface.name());
    }

    delete discoverPtr;
}

void nmapClass::discoverIp(const QString& interface)
{
    // ip from interface and discover ip range
    discover *discover_ = new discover(uid);

    QList<QNetworkAddressEntry> entryList_ = discover_->getAddressEntries(interface);

    if (!entryList_.isEmpty())
    {
        QNetworkAddressEntry entry_ = discover_->getAddressEntries(interface).first();
        QString ipString_ = entry_.ip().toString();

        QHostAddress address(ipString_);

        // TODO:: ipv6 support
        if (!ipString_.contains("127.0.0.1") && address.protocol() != QAbstractSocket::IPv6Protocol)
        {
            // active discover buttton
            startDiscoverButt->setEnabled(true);
            QStringList ipSplit_ = ipString_.split('.');
            int ipStart = ipSplit_[3].toInt();
            ipSplit_.removeLast();
            QString ipClass_ = ipSplit_.join(".");
            ipClass_.append(".");
            lineIpDiscover->setText(ipClass_);
            spinBeginDiscover->setValue(ipStart);
            spinEndDiscover->setValue(ipStart+10);
        }
        else
        {
            // reset discover value
            lineIpDiscover->clear();
            startDiscoverButt->setEnabled(false);
            spinBeginDiscover->setValue(0);
            spinEndDiscover->setValue(0);
        }
    }
    else
    {
        // reset discover value
        lineIpDiscover->clear();
        startDiscoverButt->setEnabled(false);
        spinBeginDiscover->setValue(0);
        spinEndDiscover->setValue(0);
    }

    delete discover_;
}

void nmapClass::discoverIpState()
{
    // start ip discover
    // disable start discover button
    startDiscoverButt->setEnabled(false);
    stopDiscoverButt->setEnabled(true);
    // clear tree discover
    discoveryClear();

    QStringList ipList_;
    for (int index = spinBeginDiscover->value(); index <= spinEndDiscover->value(); ++index)
    {
        QString tmpIp_ = lineIpDiscover->text().append(QString::number(index));
        ipList_.append(tmpIp_);
    }

    QStringList parameters;
    if (!uid)
    {
        parameters.append(discoverProbesCombo->currentText());
    }
    else
    {
        parameters.append("--tcp-connect");
    }

    discover *discoverPtr = new discover(uid);
    varDiscover::listDiscover.push_back(discoverPtr);
    connect(discoverPtr, SIGNAL(endPing(QStringList,bool,QByteArray)),
            this, SLOT(pingResult(QStringList,bool,QByteArray)));

    varDiscover::_discoverIsActive = true;
    discoverPtr->isUp(ipList_,this,parameters);
    varDiscover::ipCounter = ipList_.size();
    nseNumber->display(varDiscover::ipCounter);
}

void nmapClass::pingResult(QStringList hostname, bool state, const QByteArray callBuff)
{
    // decrement ping ip counter
    if (varDiscover::_discoverIsActive)
    {
        --varDiscover::ipCounter;
    }
    nseNumber->display(varDiscover::ipCounter);
    // set values in treeDiscover widget
    treeDiscover->setIconSize(QSize(24,24));
    QTextStream stream(callBuff);

    if (state)
    {
        QTreeWidgetItem *item = new QTreeWidgetItem(treeDiscover);
        item->setIcon(0, QIcon(QString::fromUtf8(":/images/images/document-preview-archive.png")));
        item->setIcon(1, QIcon(QString::fromUtf8(":/images/images/flag_green.png")));
        varDiscover::listTreeItemDiscover.push_back(item);
        item->setText(0, hostname[hostname.size()-1]);
        item->setText(1, tr("is Up"));

        while (!stream.atEnd())
        {
            QString line = stream.readLine();
            if ((line.startsWith(QLatin1String("RCVD")) || line.startsWith(QLatin1String("RECV")))
                && line.contains(hostname[hostname.size()-1]))
            {
                varDiscover::recvList.push_back(line);
            }
            else if (line.startsWith(QLatin1String("SENT")) && line.contains(hostname[hostname.size()-1]))
            {
                varDiscover::sendList.push_back(line);
            }
        }
    } /*else {
	//qDebug() << "DEBUG:: " << hostname[1] << " Ip is Up:: " << state;
    }*/

    if (!varDiscover::ipCounter)
    {
        freelist<discover*>::itemDeleteAll(varDiscover::listDiscover);
        startDiscoverButt->setEnabled(true);
        stopDiscoverButt->setEnabled(false);
    }
}

void nmapClass::discoveryClear()
{
    freelist<QTreeWidgetItem*>::itemDeleteAll(varDiscover::listTreeItemDiscover);
    freelist<discover*>::itemDeleteAll(varDiscover::listDiscover);
    varDiscover::recvList.clear();
    varDiscover::sendList.clear();
    discoverScanButt->setEnabled(false);
}

void nmapClass::stopDiscover()
{
    startDiscoverButt->setEnabled(true);
    stopDiscoverButt->setEnabled(false);
    varDiscover::ipCounter = 0;
    varDiscover::_discoverIsActive = false;
    nseNumber->display(varDiscover::ipCounter);
    //emit signal
    emit killDiscover();
}

void nmapClass::updateSRdata()
{
    int index = treeDiscover->indexOfTopLevelItem(treeDiscover->currentItem());
    textDiscoverRec->setText(varDiscover::recvList[index]);
    textDiscoverSend->setText(varDiscover::sendList[index]);
}

void nmapClass::callScanDiscover()
{
    if(treeDiscover->currentItem())
    {
        updateFontHost();
        // clear history setItemText fails
        hostEdit->insertItem(0, treeDiscover->currentItem()->text(0));
        SWscan->setCurrentIndex(0);
        startScan();
    }
}

void nmapClass::runtimeScanDiscover()
{
    // show discover send/recv data
    updateSRdata();

    if (!discoverScanButt->isEnabled())
    {
        discoverScanButt->setEnabled(true);
    }
}

void nmapClass::defaultDiscoverProbes()
{
    /* Modes Probe
    * --tcp,--udp,--arp,--tr
    */
    if (!uid)
    {
        if (!discoverProbesCombo->isVisible())
        {
            discoverProbesCombo->setVisible(true);
            labelProbesModes->setVisible(true);
        }
        discoverProbesCombo->insertItem(0, "--icmp");
        discoverProbesCombo->insertItem(1, "--tcp");
        discoverProbesCombo->insertItem(2, "--udp");
        discoverProbesCombo->insertItem(3, "--arp");
        discoverProbesCombo->insertItem(4, "--tr");
    }
    else
    {
        discoverProbesCombo->setVisible(false);
        labelProbesModes->setVisible(false);
    }
}
