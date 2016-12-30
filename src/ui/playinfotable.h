#ifndef PLAYINFOTABLE_H
#define PLAYINFOTABLE_H

#include <QWidget>
#include <QPainter>

class PlayInfoTable : public QWidget
{
    Q_OBJECT

public:
    explicit PlayInfoTable(unsigned int whites_captured, unsigned int blacks_captured, QFont adobeCleanLight, QFont segoeUiLight, QWidget *parent = 0);
    ~PlayInfoTable();

public slots:
    void updateCount(unsigned int whites_captured, unsigned int blacks_captured);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    unsigned int whites_captured;
    unsigned int blacks_captured;

    QFont adobeCleanLight;
    QFont segoeUILight;
};

#endif // PLAYINFOTABLE_H
