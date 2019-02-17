#include "Controller.h"
#include <cmath>
#include <iostream>
#include <cstdlib>
namespace mrsd
{

	void Controller::control(const mrsd::Game& g, float t)
	{
		if(p != 0) // if the player exists
		{
			currentTime = g.getGameTime();

			float curr_x = p->x;
			determineSafeSpots(g);
			p->x = pickSafeSpot(g); // some value

		}
	}

	void Controller::createPlayer(Game& g)
	{
		if(p == 0)
		{
			p = new Player();
			p->dead = true;
		}
		if(p->dead)
		{
			p->dead = false;
			p->x = g.getWidth()/2;
			g.newPlayer(p);
		}
	}

	Prediction Controller::trackProjectile(const Projectile& p, const Game& g)
	{
		Prediction pred;

		float gravity = std::abs(g.getGravity());
		float vy = p.vy;
		float y = p.y;

		float t_total = (vy + sqrt(vy * vy + 2.0 * y)) / gravity;

		pred.x = p.x + p.vx * t_total;
		pred.t = currentTime + t_total;

		return pred;
	}

	void Controller::determineSafeSpots(const Game& g)
	{
		const std::list<Projectile> projectiles = g.getProjectiles();

		for(std::list<Projectile>::const_iterator it = projectiles.begin(); it != projectiles.end(); it++)
		{
			Prediction pred = trackProjectile(*it, g);

			// Maybe insertion sort based on time here?
			predictions.push_back(pred);

		}


	}

	int Controller::pickSafeSpot(const Game& g)
	{
		int max_width = g.getWidth();

		int dangerZone[max_width+1] = {0};

		for(std::vector<Prediction>::iterator it = predictions.begin(); it != predictions.end(); it++){

			if(it->t - currentTime <= 5 * g.getTimeStep()){
				int lower_bound = std::floor(std::max(it->x - g.explosionSize, float(0)));
				int upper_bound = std::ceil(std::min(it->x + g.explosionSize, float(max_width)));

				for(int i = lower_bound; i <= upper_bound; i++)
					dangerZone[i] = 1;
			}


		}

		const std::list<Explosion> explosions = g.getExplosions();
		for(std::list<Explosion>::const_iterator it = explosions.begin(); it != explosions.end(); it++)
		{
			int lower_bound = std::floor(std::max(it->x - g.explosionSize, float(0)));
			int upper_bound = std::ceil(std::min(it->x + g.explosionSize, float(max_width)));
			std::cout << lower_bound << std::endl;
			std::cout << upper_bound << std::endl;
			for(int i = lower_bound; i <= upper_bound; i++)
				dangerZone[i] = 1;
		}



		int random_index = std::rand() % max_width;

		while(dangerZone[random_index] == 1 ){

			random_index = std::rand() % max_width;
		}

		predictions.clear();

		// std::cout << g.getTimeStep() << std::endl; // 0.1
		// std::cout << g.explosionSize << std::endl; // 5
		// std::cout << g.explosionTime << std::endl; // 2
		return random_index;

	}
}
