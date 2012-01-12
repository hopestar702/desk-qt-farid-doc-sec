/***************************************************************************
 *   Copyright (C) 2012 by Francesco Cecconi                               *
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


#ifndef PROFILERDIALOG_H
#define PROFILERDIALOG_H

#include <QtGui/QDialog>
#include <QtGui/QListWidgetItem>
#include <QtGui/QMessageBox>
#include <QtCore/QDebug>

#include "ui_profilerdialog.h"
#include "nsemanager.h"

namespace Ui
{
    class profilerDialog;
}

class profilerManager : public QDialog, public Ui::profilerDialog
{
    Q_OBJECT

    friend class nseManager;

public:
    profilerManager(QWidget* parent = 0);
    ~profilerManager();

private:
    QListWidgetItem *m_profileW;
    QListWidgetItem *m_scanW;
    QListWidgetItem *m_toolW;
    QListWidgetItem *m_discoverW;
    QListWidgetItem *m_timingW;
    QListWidgetItem *m_nseW;
    nseManager* _nseManager;
    int m_uid;

    QStringList buildExtensions();
    void loadDefaultComboValues();
    void setNormalProfile();
    void setFullVersionProfile();
    void resetOptions();
    void createQList();
    void reloadScanParameters();

signals:
    void doneParBook(const QString profileName, const QString profilePar);

private slots:
    void optionListUpdate();
    void update_portCombo();
    void update_options();
    void update_comboVerbosity();
    void exit();
    void updateBaseOptions();

};

#endif // PROFILEDIALOG_H