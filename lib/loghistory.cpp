/***************************************************************************
 *   Copyright (C) 2008 by Francesco Cecconi                               *
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

#include "loghistory.h"

// TODO insert in contructor definition
#define __CACHE_SIZE__ 10

logHistory::logHistory(QTreeWidget* treeLog, QString ConfigTag) {
     logTree = treeLog;
     configTag = ConfigTag;
}

logHistory::~logHistory() {
     // TODO
}

QList<QString> logHistory::historyReadUrl() {
     QSettings settings("nmapsi4","nmapsi4");
     QList<QString> urlList;
     urlList = settings.value(configTag, "NULL").toStringList();
     return urlList;
}

QList<QString> logHistory::historyReadUrlTime() {
     QSettings settings("nmapsi4","nmapsi4");
     QList<QString> urlListTime;
     urlListTime = settings.value("nmapsi4/urlListTime", "NULL").toStringList();
     return urlListTime;
}

void logHistory::historyUpdateUrl(QString url) {
     assert(!url.isEmpty());

     QSettings settings("nmapsi4","nmapsi4");
     QList<QString> urlList;
     urlList = historyReadUrl();

     if(urlList.contains("NULL")) {
	  urlList.removeFirst();
	  urlList.append(url);
	  settings.setValue(configTag, QVariant(urlList));
     } else {
	  if((urlList.size() == __CACHE_SIZE__) && (!urlList.contains(url)))  {
	       urlList.removeLast();
	       urlList.push_front(url);
	       settings.setValue(configTag, QVariant(urlList));
	  } else {
	       if(!urlList.contains(url)) {
		    urlList.push_front(url);
		    settings.setValue(configTag, QVariant(urlList));
	       }
	  }
     }
}

//
// TODO 
// Support for time in history QDateTime()
//
void logHistory::historyUpdateUrlTime(QString url, QString scanTime) {
     assert(!url.isEmpty());

     QSettings settings("nmapsi4","nmapsi4");
     QList<QString> urlList;
     // FIXME
     QList<QString> urlListTime;
     
     urlList = historyReadUrl();
     urlListTime = historyReadUrlTime();

     if(urlList.contains("NULL") && urlListTime.contains("NULL")) {
	  urlList.removeFirst();
	  urlList.append(url);
	  settings.setValue(configTag, QVariant(urlList));
	  urlListTime.removeFirst();
	  urlListTime.append(scanTime);
	  settings.setValue("nmapsi4/urlListTime", QVariant(urlListTime));
     } else {
	  if((urlList.size() == __CACHE_SIZE__) && (!urlList.contains(url)))  {
	       urlList.removeLast();
	       urlList.push_front(url);
	       settings.setValue(configTag, QVariant(urlList));
	       urlListTime.removeLast();
	       urlListTime.push_front(scanTime);
	       settings.setValue("nmapsi4/urlListTime", QVariant(urlListTime));
	  } else {
	       if(!urlList.contains(url)) {
		    urlList.push_front(url);
		    settings.setValue(configTag, QVariant(urlList));
		    urlListTime.push_front(scanTime);
		    settings.setValue("nmapsi4/urlListTime", QVariant(urlListTime));
	       } else {
		    // TODO update data row
		    qDebug() << "Update date row..";
		    int index = urlList.indexOf(url);
		    urlListTime[index].clear();
		    urlListTime[index].append(QDateTime::currentDateTime().toString("ddd MMMM d yy - hh:mm:ss.zzz"));
		    settings.setValue("nmapsi4/urlListTime", QVariant(urlListTime));
	       }
	  }
     }
}


void logHistory::updateThFile() {
     
     qDebug() << "logHistory::updateTreeHistory() -- call";     
     QSettings settings("nmapsi4","nmapsi4");
     QList<QString> urlList = historyReadUrl();
     logTree->clear();
     /*if(ItemListHistory.size() != 0)
       itemDeleteAll(ItemListHistory);*/
     logTree->setIconSize(QSize::QSize (32, 32));

     QFile *tmpFile = new QFile();
     if(!urlList.isEmpty() && urlList.first().compare("NULL")) {
	  foreach(QString item, urlList) {
	       tmpFile->setFileName(item);
	       if(tmpFile->exists()) {
			 historyItem = new QTreeWidgetItem(logTree);
			 historyItem->setIcon(0, QIcon(QString::fromUtf8(":/images/images/book.png")));
			 ItemListHistory.push_front(historyItem);
			 historyItem->setText(0,item);
	       } else {
		    if(urlList.contains(item)) {
			 qDebug() << "Remove Items...";
			 urlList.removeOne(item);
			 settings.setValue(configTag, QVariant(urlList));
		    }
/*		    if(urlList.isEmpty())
		    history->setText(0, "No Url Cache");*/
	       }
	  }
     } else {
	  history = new QTreeWidgetItem(logTree);
	  history->setIcon(0, QIcon(QString::fromUtf8(":/images/images/book.png")));
	  ItemListHistory.push_front(historyItem);
	  history->setText(0, "No Url Cache");
     }
     delete tmpFile;

}


void logHistory::updateTh() {
     qDebug() << "logHistory::updateTH() -- call";     
     QSettings settings("nmapsi4","nmapsi4");
     QList<QString> urlList = historyReadUrl();
     QList<QString> urlListTime = historyReadUrlTime();
     short index = 0;
     logTree->clear();
     /*if(ItemListHistory.size() != 0)
       itemDeleteAll(ItemListHistory);*/
     logTree->setIconSize(QSize::QSize (22, 22));

     if(!urlList.isEmpty() && urlList.first().compare("NULL") && urlListTime.first().compare("NULL")) {
	  foreach(QString item, urlList) {
	       historyItem = new QTreeWidgetItem(logTree);
	       historyItem->setIcon(0, QIcon(QString::fromUtf8(":/images/images/book.png")));
	       ItemListHistory.push_front(historyItem);
	       historyItem->setText(0,item);
	       historyItem->setText(1,urlListTime[index]);
	       index++;
	  }
     } else {
	  history = new QTreeWidgetItem(logTree);
	  history->setIcon(0, QIcon(QString::fromUtf8(":/images/images/book.png")));
	  ItemListHistory.push_front(historyItem);
	  if(!urlListTime.first().compare("NULL") && urlList.first().compare("NULL"))
	       history->setText(0, QApplication::translate("logHistory", 
							   "Your configuration file is too old, please delete it", 
							   0, QApplication::UnicodeUTF8));
	  else
	       history->setText(0, QApplication::translate("logHistory", 
							   "No Url Cache", 
							   0, QApplication::UnicodeUTF8));
     }
}
