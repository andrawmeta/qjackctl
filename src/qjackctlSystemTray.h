// qjackctlSystemTray.h
//
/****************************************************************************
   Copyright (C) 2003-2004, rncbc aka Rui Nuno Capela. All rights reserved.

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public License
   as published by the Free Software Foundation; either version 2
   of the License, or (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.

*****************************************************************************/

#ifndef __qjackctlSystemTray_h
#define __qjackctlSystemTray_h

#include <qpopupmenu.h>
#include <qlabel.h>


//----------------------------------------------------------------------------
// qjackctlSystemTray -- Custom system tray widget.

class qjackctlSystemTray : public QLabel
{
    Q_OBJECT

public:

    // Constructor.
    qjackctlSystemTray(QWidget *pParent = 0, const char *pszName = 0);
    // Default destructor.
    ~qjackctlSystemTray();

signals:

    // Context menu signal.
    void contextMenuRequested(qjackctlSystemTray *pSystemTray, const QPoint& pos);

protected:

    // Inherited mouse event.
    void mousePressEvent(QMouseEvent *);

    // Toggle paraent visibility.
    void toggleParent();
};


#endif  // __qjackctlSystemTray_h

// end of qjackctlSystemTray.h