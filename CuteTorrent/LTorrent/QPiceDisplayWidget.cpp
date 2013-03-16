#include "QPiceDisplayWidget.h"
#include <QGraphicsScene>
#include <QGraphicsView>
QPiceDisplayWidget::QPiceDisplayWidget(void)
{
	QGraphicsScene scene;
	scene.addText("Hello, world!");

	QGraphicsView view(&scene);
	view.show();
}

QPiceDisplayWidget::~QPiceDisplayWidget(void)
{
}
