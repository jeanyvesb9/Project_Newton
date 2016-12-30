#ifndef TOPOLOGYSELECTOR_H
#define TOPOLOGYSELECTOR_H

#include <QDialog>
#include <QModelIndexList>

#include "src/neuralNetwork/neuralnetwork.h"
#include "src/ui/spinboxdelegate.h"

namespace Ui {
class TopologySelector;
}

class TopologySelector : public QDialog
{
    Q_OBJECT

public:
    explicit TopologySelector(QWidget *parent = 0);
    ~TopologySelector();

    NN::InternalTopology getTopology() const;

private slots:
    void on_defaultLayers_clicked();
    void on_removeLayer_clicked();
    void on_addLayer_clicked();

private:
    Ui::TopologySelector *ui;
    SpinBoxDelegate delegate;
};

#endif // TOPOLOGYSELECTOR_H
