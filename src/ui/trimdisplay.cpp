#include "trimdisplay.h"

TrimDisplay::TrimDisplay(QWidget *parent)
    : QWidget(parent),
      selected{Selected::None}, initialResize{true}
{

}

void TrimDisplay::newFrame(QImage img)
{
    originalImage = QPixmap::fromImage(img);
    repaint();
}

void TrimDisplay::setNoConnection()
{
    originalImage = QPixmap();
    selected = Selected::None;
    repaint();
}

void TrimDisplay::setCorners(QPoint nul, QPoint nur, QPoint nbl, QPoint nbr)
{
    scaled = originalImage.scaled(width(), height(), Qt::KeepAspectRatio);
    QRectF newRect = QRectF(this->width() / 2.0 - scaled.width() / 2.0, this->height() / 2.0 - scaled.height() / 2.0, scaled.width(), scaled.height());

    ul = mapQPointToOriginalImage(nul, originalImage.rect(), newRect);
    ur = mapQPointToOriginalImage(nur, originalImage.rect(), newRect);
    bl = mapQPointToOriginalImage(nbl, originalImage.rect(), newRect);
    br = mapQPointToOriginalImage(nbr, originalImage.rect(), newRect);

    ul = QPointF(ul.x() + newRect.x(), ul.y() + newRect.y());
    ur = QPointF(ur.x() + newRect.x(), ur.y() + newRect.y());
    bl = QPointF(bl.x() + newRect.x(), bl.y() + newRect.y());
    br = QPointF(br.x() + newRect.x(), br.y() + newRect.y());

    repaint();
    emit cornersMoved(nul, nur, nbl, nbr);
}

void TrimDisplay::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    if(originalImage.isNull())
    {
        painter.setPen(Qt::NoPen);
        painter.setBrush(QColor(0x77, 0x77, 0x77, 0xFF));
        painter.drawRect(this->rect());
        painter.setPen(QPen(QColor(0x00, 0x00, 0x00, 0xFF), 1));
        painter.drawText(this->rect(), Qt::AlignCenter, tr("Not Available"));
    }
    else
    {
        painter.setPen(Qt::NoPen);
        painter.setBrush(QColor(0x00, 0x00, 0x00, 0x00));
        painter.drawRect(this->rect());
        scaled = originalImage.scaled(width(), height(), Qt::KeepAspectRatio);
        imageRect = QRectF(this->width() / 2.0 - scaled.width() / 2.0, this->height() / 2.0 - scaled.height() / 2.0, scaled.width(), scaled.height());
        painter.drawPixmap(imageRect.toRect(), scaled);
        painter.setBrush(QColor(0x00, 0xFF, 0x00, 0xFF));
        painter.drawEllipse(ul, 5, 5);
        painter.drawEllipse(ur, 5, 5);
        painter.drawEllipse(bl, 5, 5);
        painter.drawEllipse(br, 5, 5);
        painter.setPen(QPen(QColor(0x00, 0xFF, 0x00, 0xFF), 2));
        painter.drawLine(ul, ur);
        painter.drawLine(ur, br);
        painter.drawLine(br, bl);
        painter.drawLine(bl, ul);
    }
}

void TrimDisplay::mousePressEvent(QMouseEvent *event)
{
    if(originalImage.isNull())
    {
        return;
    }

    QPointF p = event->pos() - ul;
    if(p.manhattanLength() <= 10)
    {
        selected = Selected::UL;
        return;
    }
    p = event->pos() - ur;
    if(p.manhattanLength() <= 10)
    {
        selected = Selected::UR;
        return;
    }
    p = event->pos() - bl;
    if(p.manhattanLength() <= 10)
    {
        selected = Selected::BL;
        return;
    }
    p = event->pos() - br;
    if(p.manhattanLength() <= 10)
    {
        selected = Selected::BR;
        return;
    }
}

void TrimDisplay::mouseReleaseEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
    selected = Selected::None;
}

void TrimDisplay::mouseMoveEvent(QMouseEvent *event)
{
    if(event->buttons() & Qt::LeftButton && imageRect.contains(event->pos()))
    {
        if(selected == Selected::UL)
        {
            ul = event->pos();
        }
        else if(selected == Selected::UR)
        {
            ur = event->pos();
        }
        else if(selected == Selected::BL)
        {
            bl = event->pos();
        }
        else if(selected == Selected::BR)
        {
            br = event->pos();
        }

        if(selected != Selected::None)
        {
            repaint();

            QPoint cul(ul.x() - imageRect.x(), ul.y() - imageRect.y());
            QPoint cur(ur.x() - imageRect.x(), ur.y() - imageRect.y());
            QPoint cbl(bl.x() - imageRect.x(), bl.y() - imageRect.y());
            QPoint cbr(br.x() - imageRect.x(), br.y() - imageRect.y());

            emit cornersMoved(mapQPointToOriginalImage(cul, imageRect, originalImage.rect()).toPoint(), mapQPointToOriginalImage(cur, imageRect, originalImage.rect()).toPoint(),
                              mapQPointToOriginalImage(cbl, imageRect, originalImage.rect()).toPoint(), mapQPointToOriginalImage(cbr, imageRect, originalImage.rect()).toPoint());
        }
    }
}

void TrimDisplay::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event);
    if(initialResize)
    {
        ul = QPoint(width() / 4, height() / 4);
        ur = QPoint(width() / 4 * 3, height() / 4);
        bl = QPoint(width() / 4, height() / 4 * 3);
        br = QPoint(width() / 4 * 3, height() / 4 * 3);
        initialResize = false;
    }
    else if(!originalImage.isNull())
    {
        QPointF cul(ul.x() - imageRect.x(), ul.y() - imageRect.y());
        QPointF cur(ur.x() - imageRect.x(), ur.y() - imageRect.y());
        QPointF cbl(bl.x() - imageRect.x(), bl.y() - imageRect.y());
        QPointF cbr(br.x() - imageRect.x(), br.y() - imageRect.y());

        scaled = originalImage.scaled(width(), height(), Qt::KeepAspectRatio);
        QRectF newRect = QRectF(this->width() / 2.0 - scaled.width() / 2.0, this->height() / 2.0 - scaled.height() / 2.0, scaled.width(), scaled.height());

        cul = mapQPointToOriginalImage(cul, imageRect, newRect);
        cur = mapQPointToOriginalImage(cur, imageRect, newRect);
        cbl = mapQPointToOriginalImage(cbl, imageRect, newRect);
        cbr = mapQPointToOriginalImage(cbr, imageRect, newRect);

        ul = QPointF(cul.x() + newRect.x(), cul.y() + newRect.y());
        ur = QPointF(cur.x() + newRect.x(), cur.y() + newRect.y());
        bl = QPointF(cbl.x() + newRect.x(), cbl.y() + newRect.y());
        br = QPointF(cbr.x() + newRect.x(), cbr.y() + newRect.y());

        emit cornersMoved(mapQPointToOriginalImage(cul, imageRect, originalImage.rect()).toPoint(), mapQPointToOriginalImage(cur, imageRect, originalImage.rect()).toPoint(),
                          mapQPointToOriginalImage(cbl, imageRect, originalImage.rect()).toPoint(), mapQPointToOriginalImage(cbr, imageRect, originalImage.rect()).toPoint());
    }
}

QPointF TrimDisplay::mapQPointToOriginalImage(QPointF p, QRectF from, QRectF to) const
{
    p.setX((p.x() / from.width()) * to.width());
    p.setY((p.y() / from.height()) * to.height());
    return p;
}
