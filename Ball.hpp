#ifndef BALL_HPP
#define BALL_HPP

#include <vector>

class Ball{
	double xPos, yPos;
	std::vector<std::pair<double, double>> forces;

public:
	static constexpr std::size_t size = 10;
	static constexpr std::size_t unitInterval = size * 10;

	Ball(const double xPos = 0, const double yPos = 0);
	~Ball();

	double getX() const;
	void setX(const double x);
	double getY() const;
	void setY(const double y);
	static double calcDistance(const double x1, const double y1, const double x2, const double y2);
	double distance(const Ball &o) const;
	void addForce(const double xPos, const double yPos);
	void drag(const double xDiff, const double yDiff);
	void move();


private:
	std::pair<double, double> calculateForce() const;
	double F(const double r);
	static double calcAngle(const double fromX, const double fromY, const double toX, const double toY);
};

#endif // BALL_HPP
