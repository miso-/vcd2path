#ifndef PRINTBEDVIEW_H
#define PRINTBEDVIEW_H

#include <QGraphicsView>

class QGraphicsScene;

class PrintBedView : public QGraphicsView
{
	Q_OBJECT
	Q_DISABLE_COPY(PrintBedView)

public:
	enum GridPosition {Off, Foreground, Background};

	PrintBedView(QWidget *parent = 0);
	virtual ~PrintBedView();

	void addItem(QGraphicsItem *item);
	void clear() {
		m_scene->clear();
		m_scene->update();
	}
	void setGridPosition(GridPosition gridPosition);

protected:
	virtual void drawForeground(QPainter *painter, const QRectF &rect);
	virtual void drawBackground(QPainter *painter, const QRectF &rect);

private:
	void drawGrid(QPainter *painter, const QRectF &rect);
	virtual void wheelEvent(QWheelEvent *event);

	int m_gridPosition;
	QGraphicsScene *m_scene;
	QRectF m_gridRect;
};

#endif // PRINTBEDVIEW_H
