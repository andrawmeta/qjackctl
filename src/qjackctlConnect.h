// qjackctlConnect.h
//
/****************************************************************************
   Copyright (C) 2003, rncbc aka Rui Nuno Capela. All rights reserved.

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

#ifndef __qjackctlConnect_h
#define __qjackctlConnect_h

#include <qdragobject.h>
#include <qlistview.h>
#include <qheader.h>
#include <qlayout.h>
#include <qptrlist.h>
#include <qpixmap.h>
#include <qpainter.h>

// QListViewItem::rtti return values.
#define QJACKCTL_CLIENTITEM    1001
#define QJACKCTL_PORTITEM      1002


// Forward declarations.
class qjackctlPortItem;
class qjackctlClientItem;
class qjackctlClientList;
class qjackctlClientListView;
class qjackctlConnectorView;
class qjackctlConnectView;
class qjackctlConnect;

// Port list item.
class qjackctlPortItem : public QListViewItem
{
public:

    // Constructor.
    qjackctlPortItem(qjackctlClientItem *pClient, const QString& sPortName);
    // Default destructor.
    ~qjackctlPortItem();

    // Instance accessors.
    QString& clientName();
    QString& portName();

    // Complete client:port name helper.
    QString clientPortName();

    // Connections client item accessor.
    qjackctlClientItem *client();

    // Client port cleanup marker.
    void markPort(int iMark);
    void markClientPort(int iMark);

    int portMark();

    // Connected port list primitives.
    void addConnect(qjackctlPortItem *pPort);
    void removeConnect(qjackctlPortItem *pPort);

    // Connected port finders.
    qjackctlPortItem *findConnect(const QString& sClientPortName);
    qjackctlPortItem *findConnectPtr(qjackctlPortItem *pPortPtr);
    // Connection list accessor.
    QPtrList<qjackctlPortItem>& connects();

    // Special port name sorting virtual comparator.
    virtual int compare (QListViewItem* pPortItem, int iColumn, bool bAscending) const;

    // To virtually distinguish between list view items.
    virtual int rtti() const;

private:

    // Instance variables.
    qjackctlClientItem *m_pClient;
    QString      m_sPortName;
    int          m_iPortMark;

    // Connection cache list.
    QPtrList<qjackctlPortItem> m_connects;
};


// Client list item.
class qjackctlClientItem : public QListViewItem
{
public:

    // Constructor.
    qjackctlClientItem(qjackctlClientList *pClientList, const QString& sClientName);
    // Default destructor.
    ~qjackctlClientItem();

    // Port list primitive methods.
    void addPort(qjackctlPortItem *pPort);
    void removePort(qjackctlPortItem *pPort);

    // Port finder.
    qjackctlPortItem *findPort(const QString& sPortName);

    // Instance accessors.
    QString& clientName();

    // Readable flag accessor.
    bool isReadable();

    // Client list accessor.
    qjackctlClientList *clientlist();
    
    // Port list accessor.
    QPtrList<qjackctlPortItem>& ports();

    // Client port cleanup marker.
    void markClient(int iMark);
    void markClientPorts(int iMark);
    void cleanClientPorts(int iMark);

    int clientMark();

    // To virtually distinguish between list view items.
    virtual int rtti() const;

private:

    // Instance variables.
    qjackctlClientList *m_pClientList;
    QString m_sClientName;
    int     m_iClientMark;

    QPtrList<qjackctlPortItem> m_ports;
};


// Jack client list.
class qjackctlClientList : public QObject
{
    Q_OBJECT

public:

    // Constructor.
    qjackctlClientList(qjackctlClientListView *pListView, bool bReadable);
    // Default destructor.
    ~qjackctlClientList();

    // Client list primitive methods.
    void addClient(qjackctlClientItem *pClient);
    void removeClient(qjackctlClientItem *pClient);

    // Client finder.
    qjackctlClientItem *findClient(const QString& sClientName);
    // Client:port finder.
    qjackctlPortItem   *findClientPort(const QString& sClientPort);

    // List view accessor.
    qjackctlClientListView *listView();

    // Readable flag accessor.
    bool isReadable();

    // Client list accessor.
    QPtrList<qjackctlClientItem>& clients();
    
    // Client ports cleanup marker.
    void markClientPorts(int iMark);
    void cleanClientPorts(int iMark);

    // Client:port refreshner (return newest item count).
    virtual int updateClientPorts() = 0;

private:

    // Instance variables.
    qjackctlClientListView *m_pListView;
    bool m_bReadable;

    QPtrList<qjackctlClientItem> m_clients;
};


//----------------------------------------------------------------------------
// qjackctlClientListView -- Client list view, supporting drag-n-drop.

class qjackctlClientListView : public QListView
{
    Q_OBJECT

public:

    // Constructor.
    qjackctlClientListView(QWidget *pParent, qjackctlConnectView *pConnectionsView);
    // Default destructor.
    ~qjackctlClientListView();

    // Auto-open timer methods.
    void setAutoOpenTimeout(int iAutoOpenTimeout);
    int autoOpenTimeout();

protected slots:

    // Auto-open timeout slot.
    void timeoutSlot();

protected:

    // Drag-n-drop stuff -- reimplemented virtual methods.
    virtual void dragEnterEvent(QDragEnterEvent *pDragEnterEvent);
    virtual void dragMoveEvent(QDragMoveEvent *pDragMoveEvent);
    virtual void dragLeaveEvent(QDragLeaveEvent *);
    virtual void dropEvent(QDropEvent *pDropEvent);
    virtual QDragObject *dragObject();
    // Context menu request event handler.
    virtual void contextMenuEvent(QContextMenuEvent *);

private:

    // Bindings.
    qjackctlConnectView *m_pConnectView;

    // Drag-n-drop stuff.
    QListViewItem *dragDropItem(const QPoint& epos);

    // Auto-open timer.
    int     m_iAutoOpenTimeout;
    QTimer *m_pAutoOpenTimer;
    // Item we'll eventually drop something.
    QListViewItem *m_pDragDropItem;
};


//----------------------------------------------------------------------------
// qjackctlConnectorView -- Jack port connector widget.

class qjackctlConnectorView : public QWidget
{
    Q_OBJECT

public:

    // Constructor.
    qjackctlConnectorView(QWidget *pParent, qjackctlConnectView *pConnectView);
    // Default destructor.
    ~qjackctlConnectorView();

public slots:

    // Useful slots (should this be protected?).
    void listViewChanged(QListViewItem *);
    void contentsMoved(int, int);

protected:

    // Specific event handlers.
    virtual void paintEvent(QPaintEvent *);
    virtual void resizeEvent(QResizeEvent *);
    // Context menu request event handler.
    virtual void contextMenuEvent(QContextMenuEvent *);
    
private:

    // Drawing methods.
    void drawConnectionLine(QPainter& p, int x1, int y1, int x2, int y2, int h1, int h2);
    void drawConnections();
    
    // Local instance variables.
    qjackctlConnectView *m_pConnectView;
};


//----------------------------------------------------------------------------
// qjackctlConnectView -- Connections view integrated widget.

class qjackctlConnectView : public QWidget
{
    Q_OBJECT

public:

    // Constructor.
    qjackctlConnectView(QWidget *pParent = 0, const char *pszName = 0);
    // Default destructor.
    ~qjackctlConnectView();

    // Widget accesors.
    QGridLayout            *GridLayout()    { return m_pGridLayout; }
    qjackctlClientListView *OListView()     { return m_pOListView; }
    qjackctlClientListView *IListView()     { return m_pIListView; }
    qjackctlConnectorView  *ConnectorView() { return m_pConnectorView; }

    // Connections object binding methods.
    void setBinding(qjackctlConnect *pConnect);
    qjackctlConnect *binding();
    
    // Client list accessors.
    qjackctlClientList *OClientList();
    qjackctlClientList *IClientList();

public slots:

    // Common context menu slot.
    void contextMenu(const QPoint& pos);
    
private:

    // Child controls.
    QGridLayout            *m_pGridLayout;
    qjackctlClientListView *m_pOListView;
    qjackctlClientListView *m_pIListView;
    qjackctlConnectorView  *m_pConnectorView;

    // The main binding object.
    qjackctlConnect *m_pConnect;
};


//----------------------------------------------------------------------------
// qjackctlConnect -- Connections model integrated object.

class qjackctlConnect : public QObject
{
    Q_OBJECT

public:

    // Constructor.
    qjackctlConnect(qjackctlConnectView *pConnectView);
    // Default destructor.
    ~qjackctlConnect();

    // Explicit connection tests.
    bool canConnectSelected();
    bool canDisconnectSelected();
    bool canDisconnectAll();

    // Client list accessors.
    qjackctlClientList *OClientList();
    qjackctlClientList *IClientList();

public slots:

    // Complete contents refreshner; return dirty status.
    void refresh();

    // Explicit connection slots.
    bool connectSelected();
    bool disconnectSelected();
    bool disconnectAll();

signals:

    // Connection change signal.
    void connectChanged();

protected:

    // Connect/Disconnection primitives.
    virtual void connectPorts(qjackctlPortItem *pOPort, qjackctlPortItem *pIPort) = 0;
    virtual void disconnectPorts(qjackctlPortItem *pOPort, qjackctlPortItem *pIPort) = 0;

    // Update port connection references.
    virtual void updateConnections() = 0;

    // These must be accessed by the descendant constructor.
    qjackctlConnectView *connectView();
    void setOClientList(qjackctlClientList *pOClientList);
    void setIClientList(qjackctlClientList *pIClientList);
    
private:

    // Dunno. But this may avoid some conflicts.
    bool startExclusive();
    void endExclusive();

    // Connection methods (unguarded).
    bool canConnectSelectedEx();
    bool canDisconnectSelectedEx();
    bool canDisconnectAllEx();
    bool connectSelectedEx();
    bool disconnectSelectedEx();
    bool disconnectAllEx();

    // Connect/Disconnection local primitives.
    void connectPortsEx(qjackctlPortItem *pOPort, qjackctlPortItem *pIPort);
    void disconnectPortsEx(qjackctlPortItem *pOPort, qjackctlPortItem *pIPort);

    // Instance variables.
    qjackctlConnectView *m_pConnectView;
    // These must be created on the descendant constructor.
    qjackctlClientList *m_pOClientList;
    qjackctlClientList *m_pIClientList;
    int m_iExclusive;
};


#endif  // __qjackctlConnect_h

// end of qjackctlConnect.h