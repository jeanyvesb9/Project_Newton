#ifndef TRIMDISPLAY_H
#define TRIMDISPLAY_H

#include <QWidget>
#include <QPainter>
#include <QPixmap>
#include <QDebug>
#include <QMouseEvent>

class TrimDisplay : public QWidget
{
    Q_OBJECT
public:
    explicit TrimDisplay(QWidget *parent = 0);

    enum Selected { None, UL, UR, BL, BR };

signals:
    void cornersMoved(QPoint ul, QPoint ur, QPoint bl, QPoint br);

public slots:
    void newFrame(QImage img);
    void setNoConnection();
    void setCorners(QPoint nul, QPoint nur, QPoint nbl, QPoint nbr);

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

private:
    QPixmap originalImage;
    QPixmap scaled;
    QRectF imageRect;
    QPointF ul, ur, bl, br;
    Selected selected;
    bool initialResize;

    QPointF mapQPointToOriginalImage(QPointF p, QRectF from, QRectF to) const;
};

#endif // TRIMDISPLAY_H
