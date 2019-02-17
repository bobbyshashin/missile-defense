#pragma once

namespace mrsd
{
	struct Enemy
	{
		int x, y;
		float turretAngle, minAngle, maxAngle, turretSpeed;
		int minForce, maxForce;
		float firingSpeed;
		float firingChance;
		float firingDelay;
		float firingRandomness;
	};
}
