/***************************************************************************
 *   Copyright (C) 2009 by Francesco Cecconi                               *
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

#ifndef LOOKUPT_H
#define LOOKUPT_H

#include <QThread>
#include <QStringList>
#include <QString>
#include <QMetaType>
#include <QtNetwork/QHostInfo>


class lookUpT : public QThread
{
    Q_OBJECT

    public:
        lookUpT(QString hostname, QObject *parent = 0);

    signals:
        void threadEnd(QHostInfo, int, QString);

    private:
        QString host;
        QHostInfo info;

    private slots:
        void killLookup();

    protected:
        void run();
        QObject* par;
};

#endif
