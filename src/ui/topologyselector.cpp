#include "topologyselector.h"
#include "ui_topologyselector.h"

TopologySelector::TopologySelector(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TopologySelector)
{
    ui->setupUi(this);
    this->setWindowTitle(tr("Topology"));
    this->setModal(true);
    //this->setWindowFlags(Qt::FramelessWindowHint);

    on_defaultLayers_clicked();
    ui->layers->setItemDelegateForColumn(1, &delegate);
    ui->layers->setHorizontalHeaderLabels({tr("Name"), tr("Neuron Count")});
    ui->layers->resizeColumnsToContents();
    ui->layers->horizontalHeader()->setStretchLastSection(true);
    ui->layers->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->layers->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->layers->setCurrentCell(0, 0);
}

TopologySelector::~TopologySelector()
{
    delete ui;
}

NN::InternalTopology TopologySelector::getTopology() const
{
    NN::InternalTopology topology;
    for(int i = 0; i < ui->layers->rowCount(); i++)
    {
        topology.append(ui->layers->item(i, 1)->data(0).toInt());
    }
    return topology;
}

void TopologySelector::on_defaultLayers_clicked()
{
    ui->layers->setRowCount(2);
    ui->layers->setColumnCount(2);

    QTableWidgetItem *item = new QTableWidgetItem(tr("Hidden Layer 1"));
    item->setFlags(item->flags() & ~Qt::ItemIsEditable);
    ui->layers->setItem(0, 0, item);
    item = new QTableWidgetItem(tr("Hidden Layer 2"));
    item->setFlags(item->flags() & ~Qt::ItemIsEditable);
    ui->layers->setItem(1, 0, item);
    item = new QTableWidgetItem();
    ui->layers->setItem(0, 1, item);
    item = new QTableWidgetItem();
    ui->layers->setItem(1, 1, item);
    ui->layers->item(0, 1)->setData(0, 40);
    ui->layers->item(1, 1)->setData(0, 10);

    ui->removeLayer->setEnabled(true);
}

void TopologySelector::on_removeLayer_clicked()
{
    int row = ui->layers->selectionModel()->selectedRows(1).at(0).row();
    ui->layers->removeRow(row);

    for(int i = 0; i < ui->layers->rowCount(); i++)
    {
        ui->layers->item(i, 0)->setData(0, tr("Hidden Layer ") + QString::number(i + 1));
    }

    if(ui->layers->rowCount() == 1)
        ui->removeLayer->setEnabled(false);
}

void TopologySelector::on_addLayer_clicked()
{
    int row = ui->layers->selectionModel()->selectedRows(1).at(0).row() + 1;
    ui->layers->insertRow(row);
    QTableWidgetItem *item = new QTableWidgetItem(tr("Hidden Layer ") + QString::number(row + 1));
    item->setFlags(item->flags() & ~Qt::ItemIsEditable);
    ui->layers->setItem(row, 0, item);
    item = new QTableWidgetItem();
    ui->layers->setItem(row, 1, item);
    ui->layers->item(row, 1)->setData(0, 1);

    for(int i = 0; i < ui->layers->rowCount(); i++)
    {
        ui->layers->item(i, 0)->setData(0, tr("Hidden Layer ") + QString::number(i + 1));
    }

    if(ui->layers->rowCount() > 1)
        ui->removeLayer->setEnabled(true);
}
