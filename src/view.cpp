#include "view.h"
#include "resources.h"
#include <QApplication>
#include <QKeyEvent>

View::View()
    : QGraphicsView(), m_gameScene(new GameScene(this))
{
    setScene(m_gameScene);
    resize(Resources::RESOLUTION.width()+2, Resources::RESOLUTION.height()+2);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}

void View::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
        case Qt::Key_Escape:
            QApplication::instance()->quit();
        break;
    }

    QGraphicsView::keyPressEvent(event);
}
