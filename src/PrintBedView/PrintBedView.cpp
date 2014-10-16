#include "PrintBedView.h"

#include <QGraphicsScene>
#include <QGraphicsLineItem>
#include <QWheelEvent>
#include <QItemSelectionModel>

PrintBedView::PrintBedView(QWidget *parent)
	: QGraphicsView(parent),
	  m_gridPosition(Foreground),
	  m_scene(new QGraphicsScene(this)),
	  m_gridRect(0, 0, 200, 200)
{
	setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
	setDragMode(QGraphicsView::ScrollHandDrag);
	setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);

	// Add 5% extra space padding to each side of grid.
	qreal widthPadding = m_gridRect.width() * 0.05;
	qreal heightPadding = m_gridRect.height() * 0.05;
	QRectF sceneRect = m_gridRect.adjusted(-widthPadding, -heightPadding, widthPadding, heightPadding);

	m_scene->setSceneRect(sceneRect);
	setScene(m_scene);

	// Invert Y axis.
	scale(2, -2);
}

PrintBedView::~PrintBedView()
{
}

void PrintBedView::addItem(QGraphicsItem *item)
{
	m_scene->addItem(item);
}

void PrintBedView::setGridPosition(PrintBedView::GridPosition gridPosition)
{
	m_gridPosition = gridPosition;
	m_scene->update();
}

void PrintBedView::drawForeground(QPainter *painter, const QRectF &rect)
{
	if (m_gridPosition == Foreground) {
		drawGrid(painter, rect);
	}
}

void PrintBedView::drawBackground(QPainter *painter, const QRectF &rect)
{
	if (m_gridPosition == Background) {
		drawGrid(painter, rect);
	}
}

void PrintBedView::drawGrid(QPainter *painter, const QRectF &rect)
{
	double gridDensity = 10;
	int thickLineWidth = 3;

	double thickLineSceneWidth = thickLineWidth / transform().m11();
	QRectF toDraw = m_gridRect.intersected(rect.adjusted(-thickLineSceneWidth, -thickLineSceneWidth,
										   thickLineSceneWidth, thickLineSceneWidth));

	if (toDraw.isNull()) {
		return;
	}

	QPen defaultPen = painter->pen();
	QPen thickPen = defaultPen;
	thickPen.setWidth(thickLineWidth);
	thickPen.setCosmetic(true);

	int lineNo = static_cast<int>(toDraw.left() / gridDensity);
	if (toDraw.left() > 0) {
		++lineNo;
	}

	for (double x = lineNo * gridDensity; x <= toDraw.right(); x += gridDensity, ++lineNo) {
		if (lineNo % 5 == 0) {
			painter->setPen(thickPen);
			painter->drawLine(QPointF(x, toDraw.top()), QPointF(x, toDraw.bottom()));
			painter->setPen(defaultPen);
		} else {
			painter->drawLine(QPointF(x, toDraw.top()), QPointF(x, toDraw.bottom()));
		}
	}

	lineNo = static_cast<int>(toDraw.top() / gridDensity);
	if (toDraw.top() > 0) {
		++lineNo;
	}

	for (double y = lineNo * gridDensity; y <= toDraw.bottom(); y += gridDensity, ++lineNo) {
		if (lineNo % 5 == 0) {
			painter->setPen(thickPen);
			painter->drawLine(QPointF(toDraw.left(), y), QPointF(toDraw.right(), y));
			painter->setPen(defaultPen);
		} else {
			painter->drawLine(QPointF(toDraw.left(), y), QPointF(toDraw.right(), y));
		}
	}

}

void PrintBedView::wheelEvent(QWheelEvent *event)
{
	double zoomStep = 2.0;

	if (event->delta() > 0) {
		scale(zoomStep, zoomStep);
	} else {
		scale(1 / zoomStep, 1 / zoomStep);
	}

}
