#include "playmenu.h"

PlayMenu::PlayMenu(bool cameraConnected, bool boardConnected, QFont adobeCleanLight, QFont segoeUILight, QWidget *parent) :
    QWidget(parent),
    state{State::NormalMenuState}, index{0}, cameraConnected{cameraConnected}, boardConnected{boardConnected},
    adobeCleanLight{adobeCleanLight}, segoeUILight{segoeUILight}
{
    setFixedSize(400, cameraConnected ? 126 : 106);
    maxIndex = cameraConnected ? 4 : 3;
}

PlayMenu::~PlayMenu()
{

}

void PlayMenu::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setPen(QPen(QColor(0x55, 0x55, 0x55, 0xFF), 4));
    painter.setBrush(QColor(0x33, 0x33, 0x33, 0xFF));
    painter.drawRect(this->rect());
    painter.setPen(Qt::NoPen);

    if(state == State::NormalMenuState)
    {
        int h = drawTitle(tr("Game Paused"), painter);
        QStringList menuOptions = {tr("Resume"), tr("New Game"), tr("Help"), tr("Quit Game")};
        if(cameraConnected)
            menuOptions.insert(2, tr("Calibrate Board Camera"));
        drawOptions(menuOptions, index, h, painter);
    }
    else if(state == State::NewGameState)
    {
        int h = drawTitle(tr("New Game"), painter);
        h += drawText(tr("A new game file will be created in the same folder as the current one. Do you want to continue?"), h, painter);
        drawOptions({tr("Yes"), tr("No")}, index, (cameraConnected ? h + 20 : h + 3), painter); //+25 for bottom
    }
    else if(state == State::CameraCalibrationState)
    {

    }
    else if(state == State::HelpState)
    {
        int h = drawTitle(tr("Help"), painter);
        QFile f(QStringLiteral(":/assets/text/help.txt")); //TODO: SELECT BASED ON SYSTEM LANGUAJE
        f.open(QFile::ReadOnly | QFile::Text);
        QTextStream ts(&f);
        h += drawText(ts.readAll(), h, painter);
        drawOptions({tr("Back")}, index, h, painter);
    }
    else if(state == State::QuitState)
    {
        int h = drawTitle(tr("Quit Game"), painter);
        h += drawText(tr("Progress is saved. Do you want to continue?"), h, painter);
        drawOptions({tr("Yes"), tr("No")}, index, (cameraConnected ? h + 40 : h + 22), painter);
    }
}

int PlayMenu::drawTitle(QString title, QPainter &painter)
{
    int h = 20;
    painter.setBrush(QColor(0x55, 0x55, 0x55, 0xFF));
    painter.drawRect(QRectF(0, 0, width(), h));
    painter.setBrush(Qt::NoBrush);
    painter.setPen(QPen(QColor(0xFF, 0xFF, 0xFF, 0xFF), 1));
    segoeUILight.setPixelSize(h * 0.7);
    painter.setFont(segoeUILight);
    painter.drawText(QRectF(0, 0, width(), h), Qt::AlignCenter, title);
    return h + 2;
}

int PlayMenu::drawOptions(QStringList options, int index, int startingHeight, QPainter &painter)
{
    int h = 20;
    int totalHeight = 0;
    for(int i = 0; i < options.size(); i++)
    {
        auto &option = options.at(i);

        painter.setPen(Qt::NoPen);
        if(i != index)
            painter.setBrush(QColor(0x33, 0x33, 0x33, 0xFF));
        else
            painter.setBrush(QColor(0x44, 0x44, 0x44, 0xFF));
        painter.drawRect(QRectF(4, startingHeight + totalHeight, width() - 8, h));

        painter.setBrush(Qt::NoBrush);
        painter.setPen(QPen(QColor(0xFF, 0xFF, 0xFF, 0xFF), 1));
        adobeCleanLight.setPixelSize(13);
        painter.setFont(adobeCleanLight);
        painter.drawText(QRectF(8, startingHeight + totalHeight + 2, width() - 16, h - 1), Qt::AlignVCenter, option);
        totalHeight += 20;
    }
    return totalHeight;
}

int PlayMenu::drawText(QString text, int startingHeight, QPainter &painter, int size)
{
    painter.setBrush(Qt::NoBrush);
    painter.setPen(QPen(QColor(0xEE, 0xEE, 0xEE, 0xFF), 1));
    adobeCleanLight.setPixelSize(size);
    QRectF textBoundRect;
    painter.drawText(QRectF(8, startingHeight, width() - 16, 10), Qt::TextWordWrap | Qt::TextDontClip, text, &textBoundRect);
    return textBoundRect.height();
}

void PlayMenu::nextPressed()
{
    if(state == State::NormalMenuState || state == State::NewGameState || state == State::QuitState)
    {
        index = (index + 1) % (maxIndex + 1);
    }
    repaint();
}

void PlayMenu::okPressed()
{
    if(state == State::NormalMenuState)
    {
        normalMenuIndex = index;
        if(index == 0)
        {
            hide();
            emit resumeGame();
            deleteLater();
        }
        if(index == 1)
        {
            state = State::NewGameState;
            index = 0;
            maxIndex = 1;
        }
        if(index == 2)
        {
            state = cameraConnected ? State::CameraCalibrationState : State::HelpState;
            setFixedSize(cameraConnected ? 400 : 700, cameraConnected ? 126 : 539); //Set height based on system languaje for help text;
            index = 0;
        }
        if(index == 3)
        {
            state = cameraConnected ? State::HelpState : State::QuitState;
            setFixedSize(cameraConnected ? 700 : 400, cameraConnected ? 539 : 106);
            index = 0;
            maxIndex = 1; //Only for QuitState;
        }
        if(index == 4)
        {
            state = State::QuitState;
            index = 0;
            maxIndex = 1;
        }
    }
    else if(state == State::NewGameState)
    {
        if(index == 0)
        {

        }
        else
        {
            backToNormalMenu();
        }
    }
    else if(state == State::CameraCalibrationState)
    {
        backToNormalMenu();
    }
    else if(state == State::HelpState)
    {
        backToNormalMenu();
    }
    else if(state == State::QuitState)
    {
        if(index == 0)
        {

        }
        else
        {
            backToNormalMenu();
        }
    }

    repaint();
}

void PlayMenu::backToNormalMenu()
{
    state = State::NormalMenuState;
    setFixedSize(400, cameraConnected ? 128 : 108);
    index = normalMenuIndex;
    maxIndex = cameraConnected ? 4 : 3;
}

void PlayMenu::keyPressEvent(QKeyEvent *event)
{
    switch(event->key())
    {
    case Qt::Key_Escape:
        if(state == State::NormalMenuState)
        {
            hide();
            emit resumeGame();
            deleteLater();
        }
        else if(state == State::NewGameState || state == State::CameraCalibrationState || state == State::HelpState || state == State::QuitState)
        {
            backToNormalMenu();
            repaint();
        }
        break;
    case Qt::Key_Enter:
    case Qt::Key_Return:
        okPressed();
        break;
    case Qt::Key_Down:
    case Qt::Key_Right:
        nextPressed();
        break;
    case Qt::Key_Up:
    case Qt::Key_Left:
        if(state == State::NormalMenuState || state == State::NewGameState || state == State::QuitState)
        {
            index = (index - 1) < 0 ? maxIndex : index - 1;
        }
        repaint();
        break;
    }
}

void PlayMenu::mousePressEvent(QMouseEvent *event)
{

}

void PlayMenu::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event);
    move(parentWidget()->window()->rect().center() - this->rect().center());
}
