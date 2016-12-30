#include "playinfotable.h"

PlayInfoTable::PlayInfoTable(unsigned int whites_captured, unsigned int blacks_captured, QFont adobeCleanLight, QFont segoeUiLight, QWidget *parent)
    : QWidget(parent),
      whites_captured{whites_captured}, blacks_captured{blacks_captured}, adobeCleanLight{adobeCleanLight}, segoeUILight{segoeUiLight}
{
    this->segoeUILight.setPixelSize(25);
    this->adobeCleanLight.setPixelSize(17);
}

PlayInfoTable::~PlayInfoTable()
{

}

void PlayInfoTable::updateCount(unsigned int whites_captured, unsigned int blacks_captured)
{
    this->whites_captured = whites_captured;
    this->blacks_captured = blacks_captured;
    repaint();
}

void PlayInfoTable::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(Qt::NoPen);
    painter.setBrush(QColor(0x00, 0x00, 0x00, 0x00));
    painter.drawRect(this->rect());
    painter.setPen(QPen(QColor(0xEE, 0xEE, 0xEE, 0xFF), 1.5));
    painter.drawLine(0, 30, width(), 30);
    painter.drawLine(QPointF(width() / 2.0, 0), QPointF(width() / 2.0, height()));

    painter.setFont(segoeUILight);
    painter.drawText(5, 23, tr("Whites"));
    painter.drawText(QPointF(width() / 2.0 + 6.5, 23), tr("Blacks"));
    painter.setFont(adobeCleanLight);
    painter.drawText(QRectF(6, 38, width() / 2.0 - 11.5, 14), tr("Captured Pieces:"));
    painter.drawText(QRectF(6, 38, width() / 2.0 - 11.5, 14), Qt::AlignRight, QString::number(whites_captured));
    painter.drawText(QRectF(width() / 2.0 + 7.5, 38, width() / 2.0 - 11.5, 14), tr("Captured Pieces:"));
    painter.drawText(QRectF(width() / 2.0 + 7.5, 38, width() / 2.0 - 11.5, 14), Qt::AlignRight, QString::number(blacks_captured));
}
