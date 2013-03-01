#include <QMenu>
#include <QSortFilterProxyModel>
#include "servers_widget.h"
#include "preferences.h"
#include "servers_table_model.h"

servers_widget::servers_widget(QWidget *parent) :
    QWidget(parent), m_smodel(NULL)
{
    setupUi(this);
    // prepare menus
    m_connect_menu = new QMenu(this);
    m_disconnect_menu = new QMenu(this);

    //fileDownload->setIcon(QIcon(":/emule/search/Download.png"));
    m_connect_action = new QAction(this);
    m_connect_action->setText(tr("Connect to"));
    m_connect_action->setIcon(QIcon(":/emule/common/ConnectDo.ico"));
    m_disconnect_action = new QAction(this);
    m_disconnect_action->setText(tr("Disconnect from"));
    m_disconnect_action->setIcon(QIcon(":/emule/common/ConnectStop.ico"));
    m_remove_action = new QAction(this);
    m_remove_action->setText(tr("Remove"));
    m_remove_action->setIcon(QIcon(":/emule/common/DeleteSelected.ico"));
    m_remove_all_action = new QAction(this);
    m_remove_all_action->setText(tr("Remove all"));
    m_remove_all_action->setIcon(QIcon(":/emule/common/DeleteAll.ico"));

    m_connect_menu->addAction(m_connect_action);
    m_connect_menu->addSeparator();
    m_connect_menu->addAction(m_remove_action);
    m_connect_menu->addAction(m_remove_all_action);

    m_disconnect_menu->addAction(m_connect_action);
    m_disconnect_menu->addSeparator();
    m_disconnect_menu->addAction(m_remove_action);
    m_disconnect_menu->addAction(m_remove_all_action);

    connect(m_connect_action, SIGNAL(triggered()), this, SLOT(connect_slot()));
    connect(m_disconnect_action, SIGNAL(triggered()), this, SLOT(disconnect_slot()));
    connect(m_remove_action, SIGNAL(triggered()), this, SLOT(remove_slot()));
    connect(m_remove_all_action, SIGNAL(triggered()), this, SLOT(removeAll_slot()));

    m_smodel = new servers_table_model(this);
    m_smodel->load();

    m_sort_model = new QSortFilterProxyModel(this);
    m_sort_model->setSourceModel(m_smodel);
    tableServers->setModel(m_sort_model);

    Preferences pref;

    if (!hsplit->restoreState(pref.value("ServersWidget/HSplitter").toByteArray()))
    {
        QList<int> sz;
        sz << 500 << 100;
        hsplit->setSizes(sz);
    }

    tableServers->horizontalHeader()->setContextMenuPolicy(Qt::CustomContextMenu);
    vsplit->restoreState(pref.value("ServersWidget/VSplitter").toByteArray());
    tableServers->horizontalHeader()->restoreState(pref.value("ServersWidget/TableServers").toByteArray());

    connect(tableServers->horizontalHeader(), SIGNAL(sortIndicatorChanged(int, Qt::SortOrder)),
            this, SLOT(serversSortChanged(int, Qt::SortOrder)));

    connect(tableServers->horizontalHeader(), SIGNAL(customContextMenuRequested(const QPoint&)),
            this, SLOT(displayHeaderMenu(const QPoint&)));

    connect(tableServers, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(displayPopupMenu(const QPoint&)));

    tableServers->horizontalHeader()->setSortIndicator(servers_table_model::DC_NAME, Qt::AscendingOrder);
}

servers_widget::~servers_widget()
{
    Preferences pref;
    pref.setValue("ServersWidget/VSplitter", vsplit->saveState());
    pref.setValue("ServersWidget/HSplitter", hsplit->saveState());
    pref.setValue("ServersWidget/TableServers", tableServers->horizontalHeader()->saveState());
}

void servers_widget::serversSortChanged(int column, Qt::SortOrder order)
{
    m_sort_model->sort(column, order);
}

void servers_widget::displayHeaderMenu(const QPoint&)
{
    QMenu hideshowColumn(this);
    hideshowColumn.setTitle(tr("Column visibility"));
    QList<QAction*> actions;

    for (int i=0; i < m_smodel->columnCount(); ++i)
    {
        QAction *myAct = hideshowColumn.addAction(
            m_smodel->headerData(i, Qt::Horizontal, Qt::DisplayRole).toString());
        myAct->setCheckable(true);
        myAct->setChecked(!tableServers->isColumnHidden(i));
        actions.append(myAct);
    }

    // Call menu
    QAction *act = hideshowColumn.exec(QCursor::pos());

    if (act)
    {
        int col = actions.indexOf(act);
        Q_ASSERT(col >= 0);
        tableServers->setColumnHidden(col, !tableServers->isColumnHidden(col));
        if (!tableServers->isColumnHidden(col) && tableServers->columnWidth(col) <= 5)
            tableServers->setColumnWidth(col, 100);
    }
}

void servers_widget::on_btnAdd_clicked()
{
    // add new server
    editName->clear();
    editIP->clear();
}

void servers_widget::on_editIP_textChanged(const QString &arg1)
{
    switchAddBtn();
}

void servers_widget::on_editName_textChanged(const QString &arg1)
{
    switchAddBtn();
}

void servers_widget::switchAddBtn()
{
    if (!editName->text().isEmpty() && !editIP->text().isEmpty())
    {
        btnAdd->setEnabled(true);
    }
    else
    {
        btnAdd->setEnabled(false);
    }
}

void servers_widget::connect_slot()
{

}

void servers_widget::disconnect_slot()
{

}

void servers_widget::remove_slot()
{

}

void servers_widget::removeAll_slot()
{

}


void servers_widget::on_tableServers_customContextMenuRequested(const QPoint &pos)
{
    m_connect_menu->exec(QCursor::pos());
}
