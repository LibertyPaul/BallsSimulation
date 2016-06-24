#ifndef FIELD_HPP
#define FIELD_HPP

#include "Ball.hpp"

#include <vector>
#include <utility>
#include <mutex>
#include <chrono>
#include <random>
#include <algorithm>

class Field{
	const double xMax, yMax;

	std::vector<Ball> balls;
	mutable std::mutex ballsAccess;

	bool hasFixedBall;
	std::vector<Ball>::const_iterator fixedBall;

public:
	Field(const double xMax, const double yMax):
		xMax(xMax),
		yMax(yMax),
		balls(),
		hasFixedBall(false),
		fixedBall(balls.cend()){}

	~Field(){}

	void addBall(const double xPos, const double yPos){
		std::lock_guard<std::mutex> lg(this->ballsAccess);
		this->balls.emplace_back(xPos, yPos);
	}

private:
	std::vector<Ball>::const_iterator findBallByPosition(const double xPos, const double yPos) const{
		return std::find_if(this->balls.cbegin(), this->balls.cend(), [xPos, yPos](const Ball &ball){
			return Ball::calcDistance(xPos, yPos, ball.getX(), ball.getY()) < Ball::size;
		});
	}

	std::vector<Ball>::iterator findBallByPosition(const double xPos, const double yPos){
		return std::find_if(this->balls.begin(), this->balls.end(), [xPos, yPos](const Ball &ball){
			return Ball::calcDistance(xPos, yPos, ball.getX(), ball.getY()) < Ball::size;
		});
	}

public:
	void removeBall(const double xPos, const double yPos){
		std::lock_guard<std::mutex> lg(this->ballsAccess);

		const std::vector<Ball>::const_iterator ball = findBallByPosition(xPos, yPos);

		if(ball == this->fixedBall){
			this->fixedBall = this->balls.cend();
		}

		if(ball != this->balls.cend()){
			this->balls.erase(ball);
		}
	}

	void fixBall(const double xPos, const double yPos){
		std::lock_guard<std::mutex> lg(this->ballsAccess);
		this->hasFixedBall = true;
		this->fixedBall = this->findBallByPosition(xPos, yPos);
	}

	void unfixBall(){
		std::lock_guard<std::mutex> lg(this->ballsAccess);
		this->hasFixedBall = false;
		this->fixedBall = this->balls.cend();
	}

	void dragBall(const double xPos, const double yPos, const double xDiff, const double yDiff){
		const auto ball = findBallByPosition(xPos, yPos);
		if(ball != this->balls.cend()){
			ball->drag(xDiff, yDiff);
		}
	}

	void addRandom(const size_t count){
		std::default_random_engine rg(std::chrono::system_clock::now().time_since_epoch().count());
		std::uniform_real_distribution<double> xDistr(0, this->xMax);
		std::uniform_real_distribution<double> yDistr(0, this->yMax);

		for(size_t i = 0; i < count; ++i){
			this->addBall(xDistr(rg), yDistr(rg));
		}
	}

	std::vector<std::pair<double, double>> getBallsCoordinates() const{
		std::vector<std::pair<double, double>> result;
		result.reserve(this->balls.size());

		this->ballsAccess.lock();

		for(const Ball &ball : this->balls){
			result.emplace_back(ball.getX(), ball.getY());
		}

		this->ballsAccess.unlock();

		return result;
	}

	void step(){
		std::lock_guard<std::mutex> lg(this->ballsAccess);
		for(auto movable = this->balls.begin(); movable != this->balls.end(); ++movable){
			if(this->hasFixedBall && movable == this->fixedBall){
				continue;
			}

			for(auto moving = this->balls.cbegin(); moving != this->balls.cend(); ++moving){
				if(movable == moving){ // C++14 allows us to compare const and non-const iterators
					continue;
				}

				movable->addForce(moving->getX(), moving->getY());
			}
		}

		for(auto movable = this->balls.begin(); movable != this->balls.end(); ++movable){
			if(this->hasFixedBall && movable == this->fixedBall){
				continue;
			}

			movable->move();

			if(movable->getX() == NAN){
				movable->setX(0);
			}

			if(movable->getY() == NAN){
				movable->setY(0);
			}

			if(movable->getX() < 0){
				movable->setX(0);
			}

			if(movable->getX() > this->xMax - Ball::size){
				movable->setX(this->xMax - Ball::size);
			}


			if(movable->getY() < 0){
				movable->setY(0);
			}

			if(movable->getY() > this->yMax - Ball::size){
				movable->setY(this->yMax - Ball::size);
			}
		}
	}

};

#endif // FIELD_HPP
