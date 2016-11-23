#include "aspectratiowidget.h"

AspectRatioWidget::AspectRatioWidget(QWidget *widget, QWidget *parent) :
    QWidget(parent)
{
    layout = new QBoxLayout(QBoxLayout::LeftToRight, this);

    // add spacer, then your widget, then spacer
    s1 = new QSpacerItem(0, 0, QSizePolicy::Fixed, QSizePolicy::Fixed);
    s2 = new QSpacerItem(0, 0, QSizePolicy::Fixed, QSizePolicy::Fixed);

    layout->addItem(s1);
    layout->addWidget(widget);
    layout->addItem(s2);

    w = layout->itemAt(1)->widget();
}

void AspectRatioWidget::resizeEvent(QResizeEvent *event)
{
    int width = event->size().width() - 30;
    int height = event->size().height() - 30;

    if(width > height)
    {
        s1->changeSize((width - height) / 2.0, 0, QSizePolicy::Fixed, QSizePolicy::Fixed);
        s2->changeSize((width - height) / 2.0, 0, QSizePolicy::Fixed, QSizePolicy::Fixed);
        w->setMinimumSize(height, height);
        w->setMaximumSize(height, height);
    }
    else
    {
        s1->changeSize(0, (width - height) / 2.0, QSizePolicy::Fixed, QSizePolicy::Fixed);
        s1->changeSize(0, (width - height) / 2.0, QSizePolicy::Fixed, QSizePolicy::Fixed);
        w->setMinimumSize(width, width);
        w->setMaximumSize(width, width);
    }
}
