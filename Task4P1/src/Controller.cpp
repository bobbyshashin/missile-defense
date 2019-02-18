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
			// std::cout << "4" << std::endl;
			float max_speed = g.playerSpeed;
			// std::cout << "5" << std::endl;
			float picked_x = pickSafeSpot(g);

			if(picked_x < curr_x)
				p->x -= max_speed;
			else if(picked_x > curr_x)
				p->x += max_speed;
			else
				p->x = curr_x;
			std::cout << "time: " << currentTime << std::endl;
			// p->x = pickSafeSpot(g); // some value

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

			if(it->t - currentTime <= 200 * g.getTimeStep()){ // look ahead in 200 timesteps
				int lower_bound = std::floor(std::max(it->x - g.explosionSize, float(0)));
				int upper_bound = std::ceil(std::min(it->x + g.explosionSize, float(max_width)));

				for(int i = lower_bound; i <= upper_bound; i++)
					dangerZone[i] = 1;
			}


		}

		const std::list<Explosion> explosions = g.getExplosions();

		for(std::list<Explosion>::const_iterator it = explosions.begin(); it != explosions.end(); it++){

			int lower_bound = std::floor(std::max(it->x - g.explosionSize, float(0)));
			int upper_bound = std::ceil(std::min(it->x + g.explosionSize, float(max_width)));

			for(int i = lower_bound; i <= upper_bound; i++)
				dangerZone[i] = 1;
		}

		// Randomly pick

		// int random_index = std::rand() % max_width;

		// while(dangerZone[random_index] == 1 ){

		// 	random_index = std::rand() % max_width;
		// }

		int x = p->x;
		// std::cout << x << std::endl;

		if(dangerZone[x] == 0){ // current place is safe
			/*
			int right = max_width;
			int left = 0;
			for(int i = x; i <= max_width; i++){
				if(dangerZone[i] == 1){
					right = i;
					break;
				}
			}
			// std::cout << "8" << std::endl;
				
			for(int i = x; i >= 0; i--){
				if(dangerZone[i] == 1){
					left = i;
					break;
				}
			}
			x = (left + right) / 2;
			*/
			
			predictions.clear();

			return x;
		}

		else{

			while(dangerZone[x] == 1){

				int right = max_width;
				int left = 0;
				for(int i = x; i <= max_width; i++){

					if(dangerZone[i] == 0){
						right = i;
						break;
					}
				}

				for(int i = x; i >= 0; i--){
					if(dangerZone[i] == 0){
						left = i;
						break;
					}
				}


				if(x - left > right - x)
					x = right;
				else
					x = left;

				/*
				int right1 = max_width;
				int right2 = max_width;
				bool right_stop_flag = false;


				int left1 = 0;
				int left2 = 0;
				bool left_stop_flag = false;

				for(int i = x; i <= max_width; i++){

					if(!right_stop_flag && dangerZone[i] == 0){
						right1 = i;
						right_stop_flag = true;
					}
					else if(right_stop_flag && dangerZone[i] == 1){
						right2 = i;
						break;
					}

				}

				for(int i = x; i >= 0; i--){
					if(!left_stop_flag && dangerZone[i] == 0){
						left1 = i;
						left_stop_flag = true;
					}
					else if(left_stop_flag && dangerZone[i] == 1){
						left2 = i;
						break;
					}
				}
				int right_mid = (right2 + right1) / 2;
				int left_mid = (left2 + left1) / 2;

				if(x - left_mid > right_mid - x)
					x = right_mid;
				else
					x = left_mid;
				*/


			}


			predictions.clear();
			return x;
		}

	}
}
