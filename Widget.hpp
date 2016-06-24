#ifndef WIDGET_H
#define WIDGET_H


#include <QBrush>
#include <QPen>
#include <QWidget>
#include <QPainter>
#include <QTimer>
#include <QPaintEvent>
#include <thread>
#include <atomic>

#include "Field.hpp"

class Widget : public QWidget
{
	Q_OBJECT


	const size_t fieldHeight, fieldWidth;
	Field field;
	std::thread fieldThread;
	std::atomic<bool> stopFlag;

	QPointF lastMousePressPosition;

	QBrush background = QBrush(QColor(64, 32, 64));
	QBrush circleBrush = QBrush(Qt::SolidPattern);
	QPen circlePen = QPen(QColor(Qt::GlobalColor::white));

public:
	Widget(QWidget *parent, const size_t fieldHeight, const size_t fieldWidth):
		QWidget(parent),
		fieldHeight(fieldHeight),
		fieldWidth(fieldWidth),
		field(fieldHeight, fieldWidth)
	{
		setFixedSize(this->fieldHeight, this->fieldWidth);
		circleBrush.setColor(Qt::GlobalColor::white);
	}

	~Widget(){
		this->stopFlag = true;
		this->fieldThread.join();
	}

	void render(){
		this->update();
	}

	void paintEvent(QPaintEvent *event){
		this->drawBalls(event, this->field.getBallsCoordinates());
	}

	void mousePressEvent(QMouseEvent *event) override{
		if(event->button() == Qt::LeftButton){
			this->field.fixBall(event->localPos().x() - Ball::size / 2, event->localPos().y() - Ball::size / 2);
			this->lastMousePressPosition = event->localPos();
		}
	}

	void mouseReleaseEvent(QMouseEvent *event) override{
		switch(event->button()){
		case Qt::LeftButton:{
			this->field.unfixBall();
			const double dragDistance = Ball::calcDistance(this->lastMousePressPosition.x(), this->lastMousePressPosition.y(), event->localPos().x(), event->localPos().y());
			if(dragDistance > Ball::size){
				const double xDrag = event->localPos().x() - this->lastMousePressPosition.x();
				const double yDrag = event->localPos().y() - this->lastMousePressPosition.y();
				this->field.dragBall(this->lastMousePressPosition.x(), this->lastMousePressPosition.y(), xDrag, yDrag);
			}
			else{
				this->field.removeBall(event->localPos().x() - Ball::size / 2, event->localPos().y() - Ball::size / 2);
			}
			break;
		}

		case Qt::RightButton:
			try{
				this->field.addBall(event->localPos().x() - Ball::size / 2, event->localPos().y() - Ball::size / 2);
			}
			catch(std::runtime_error &){

			}

			break;

		default:
			break; // turn those enum warnings off
		}
	}

	void drawBalls(QPaintEvent *event, const std::vector<std::pair<double, double>> &ballsCoords){
		QPainter painter;
		painter.begin(this);
		painter.setRenderHint(QPainter::Antialiasing);
		painter.fillRect(event->rect(), background);
		painter.setBrush(circleBrush);
		painter.setPen(circlePen);


		for(const auto coords : ballsCoords){
			const QRectF rect(coords.first, coords.second, Ball::size, Ball::size);
			painter.drawEllipse(rect);
		}


		painter.end();
	}

	void start(){
		this->stopFlag = false;

		this->fieldThread = std::thread([this](){
			const auto stepThrottle = std::chrono::microseconds(200);
			while(this->stopFlag == false){
				this->field.step();
				std::this_thread::sleep_for(stepThrottle);
			}
		});
	}

};

#endif // WIDGET_H

