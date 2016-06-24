#include "Ball.hpp"

#include <cmath>
#include <cassert>


Ball::Ball(const double xPos, const double yPos): xPos(xPos), yPos(yPos){
	assert(this->xPos >= 0);
	assert(this->yPos >= 0);
}

Ball::~Ball(){}

double Ball::getX() const{
	return this->xPos;
}

void Ball::setX(const double x){
	this->xPos = x;
}

double Ball::getY() const{
	return this->yPos;
}

void Ball::setY(const double y){
	this->yPos = y;
}


double Ball::calcDistance(const double x1, const double y1, const double x2, const double y2){
	const double resultDistance = std::sqrt(std::pow(x1 - x2, 2) + std::pow(y1 - y2, 2));
	return resultDistance;
}

double Ball::calcAngle(const double fromX, const double fromY, const double toX, const double toY){
	const double resultRadians = std::atan2(toY - fromY, toX - fromX);
	return resultRadians;
}

double Ball::distance(const Ball &o) const{
	return Ball::calcDistance(this->getX(), this->getY(), o.getX(), o.getY());
}

double Ball::F(const double r){
	const double resultF = (std::pow(r / unitInterval, -1) - std::pow(r / unitInterval, -2)) / unitInterval;
	return resultF;
}

void Ball::addForce(const double xPos, const double yPos){
	const double distance = Ball::calcDistance(this->getX(), this->getY(), xPos, yPos);
	const double angle = Ball::calcAngle(this->getX(), this->getY(), xPos, yPos);
	const double force = F(distance);

	this->forces.emplace_back(angle, force);
}

std::pair<double, double> Ball::calculateForce() const{
	std::pair<double, double> result(0, 0);

	for(const std::pair<double, double> &force : this->forces){
		const double xDiff = force.second * std::cos(force.first);
		const double yDiff = force.second * std::sin(force.first);

		result.first += xDiff;
		result.second += yDiff;
	}

	return result;
}

void Ball::drag(const double xDiff, const double yDiff){
	this->xPos += xDiff;
	this->yPos += yDiff;
}

void Ball::move(){
	const std::pair<double, double> force = this->calculateForce();
	this->forces.clear();

	this->drag(force.first, force.second);
}
