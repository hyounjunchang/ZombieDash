#include "Actor.h"
#include "StudentWorld.h"

#include <list>
#include <vector>
using namespace std;

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp

const int GraphObject::up;
const int GraphObject::down;
const int GraphObject::left;
const int GraphObject::right;

void MovePixelsInTheDirection(double& x, double& y, Direction dir, double distance)  // takes in 2 coordinates.
{
	switch (dir)
	{
	case GraphObject::up:
		y += distance;
		break;
	case GraphObject::down:
		y -= distance;
		break;
	case GraphObject::left:
		x -= distance;
		break;
	case GraphObject::right:
		x += distance;
		break;
	}


}

double getnewXSpriteWidthAway(double x, Direction dir);
double getnewYSpriteHeightAway(double y, Direction dir);

void Actor::setDead()   // DEFAULT TO DOING NOTHING
{
	m_isAlive = false;
}
void Citizen::setDead()
{
	Actor::setDead();
	getWorld()->increaseScore(-1000);

	if (!isLivingActor())
	{
		getWorld()->playSound(SOUND_CITIZEN_DIE);
	}
}
void Penelope::setDead()
{
	Actor::setDead();
}

void DumbZombie::setDead()
{
	Actor::setDead();
	getWorld()->increaseScore(1000);
	getWorld()->playSound(SOUND_ZOMBIE_DIE);

	int rand = randInt(1, 10);
	if (rand == 1)  // 10% chance
	{
		Direction dir = getDirection();

		double x = getnewXSpriteWidthAway(getX(), dir);
		double y = getnewYSpriteHeightAway(getY(), dir);

		if (!(getWorld()->OverlapsWithAnyActorPixelBlock(x, y)))  // new item doesn't overlaps with any of the blocks
		{
			getWorld()->addVaccine(x, y);
		}
	}
}

void SmartZombie::setDead()
{
	Actor::setDead();
	getWorld()->increaseScore(2000);
	getWorld()->playSound(SOUND_ZOMBIE_DIE);
}
void Landmine::setDead()
{
	Actor::setDead();
	getWorld()->createExplosionWithLandmine(this);
}

void LivingThings::setToNotLiving()
{
	m_isliving = false;
}

bool Actor::otherCanMoveOnTop() const  // change for objects that can be overlapped
{
	return true;
}
bool LivingThings::otherCanMoveOnTop() const
{
	return false;
}

bool Wall::otherCanMoveOnTop() const
{
	return false;
}


bool Actor::isHuman() const   // THIS METHOD MAY BE PROBLEMATIC!!!
{
	return false;
}

bool Human::isHuman() const			// Penelope and Citizens are human
{
	return true;
}
bool Zombie::isHuman() const
{
	return false;
}

bool Actor::isZombie() const
{
	return false;
}
bool Human::isZombie() const
{
	return false;
}
bool Zombie::isZombie() const
{
	return true;
}


bool Actor::canBeDamaged() const
{
	return true;
}
bool Indestructable::canBeDamaged() const
{
	return false;
}
bool Flame::canBeDamaged() const
{
	return false;
}
bool Vomit::canBeDamaged() const
{
	return false;
}

bool Actor::isLivingThing() const
{
	return false;
}
bool LivingThings::isLivingThing() const
{
	return true;
}

bool Actor::canBlockFlames() const
{
	return false;
}

bool Indestructable::canBlockFlames() const
{
	return false;
}

bool BlocksFlames::canBlockFlames() const
{
	return true;
}

bool Actor::isAlive() const
{
	return m_isAlive;
}


void Actor::setAliveAgain()
{
	m_isAlive = true;
}


bool Actor::isLivingActor() const   // TRUE for non-living things, for living things, wheter it's alive
{
	return true;
}
bool LivingThings::isLivingActor() const
{
	return m_isliving;
}



bool Human::isInfected() const
{
	return m_isInfected;
}
int Human::getInfectionCounter() const
{
	return m_infectionCounter;
}

void Human::infect()
{
	m_isInfected = true;
}
void Citizen::infect()
{
	if (!isInfected()) // first time getting infected
	{
		getWorld()->playSound(SOUND_CITIZEN_INFECTED);
	}
	Human::infect();
}



void Human::addInfectionCounter()
{
	if (!m_isInfected)
		return;

	m_infectionCounter++;
}

void Human::cureInfection()
{
	m_isInfected = false;
	m_infectionCounter = 0;
}


int Penelope::get_FlameThrower() const
{ return m_flamethrower; }
int Penelope::get_LandMines() const
{ return m_landmine; }
int Penelope::get_Vaccines() const
{ return m_vaccine; }

void Penelope::useFlamethrower()
{
	if (m_flamethrower > 0)
	{
		getWorld()->useFlameThrower(this);
		m_flamethrower--;
	}
}

void Penelope::useLandmine()
{
	if (m_landmine > 0)
	{
		getWorld()->createLandmine(this);
		m_landmine--;
	}

}

void Penelope::useVaccine()
{
	if (m_vaccine > 0)
	{
		cureInfection();
		m_vaccine--;
	}
}

// work on ALL THESE!!
void Penelope::doSomething()
{
	if (isInfected())
	{
		if (getInfectionCounter() == 500)
		{
			setDead();
			setToNotLiving();
		}
		addInfectionCounter();
		

	}

		if (!isLivingActor())
			return; // do nothing when dead

		int input;
		if (getWorld()->getKey(input))
		{

			bool positionChange = false;
			double newX, newY;
			switch (input)
			{
			case KEY_PRESS_DOWN:
				setDirection(GraphObject::down);
				newX = getX();
				newY = getY() - 4;
				positionChange = true;
				break;
			case KEY_PRESS_UP:
				setDirection(GraphObject::up);
				newX = getX();
				newY = getY() + 4;
				positionChange = true;
				break;

			case KEY_PRESS_LEFT:
				setDirection(GraphObject::left);
				newX = getX() - 4;
				newY = getY();
				positionChange = true;
				break;

			case KEY_PRESS_RIGHT:
				setDirection(GraphObject::right);
				newX = getX() + 4;
				newY = getY();
				positionChange = true;
				break;

			case KEY_PRESS_SPACE:
				useFlamethrower();
				break;

			case KEY_PRESS_TAB:
				useLandmine();
				break;

			case KEY_PRESS_ENTER:
				useVaccine();
				break;
			}

				//problem code is here!!!
				if (positionChange)
				{
					bool canMove = getWorld()->canMoveActor(newX, newY, this);

					if (canMove)
						moveTo(newX, newY);
				}
		}
}



void Exit::doSomething()
{
	getWorld()->movePeoplefromExit(this);
	
}

void Pit::doSomething()
{
	getWorld()->moveLivingThingsfromPit(this);
}
void Flame::doSomething()
{
	if (m_numTickActive == MAX_TICKS_ALLOWED())  // after 2 ticks, it dies
	{
		setDead();
		return;
	}
	else
	{
		getWorld()->destoryObjectsWithFlame(this);
		m_numTickActive++; // add tick to flame
	}
}
void Vomit::doSomething()
{
	if (m_numTickActive == MAX_TICKS_ALLOWED())  // after 2 ticks, it dies
	{
		setDead();
		return;
	}
	else
	{
		getWorld()->infectHumans(this);
		m_numTickActive++; // add tick to flame
	}
}
void VaccineGoodie::doSomething()
{
	if (!isAlive())
		return;

	getWorld()->checkVaccineGoodie(this);
}
void GasCanGoodie::doSomething()
{
	if (!isAlive())
		return;

	getWorld()->checkGasCanGoodie(this);
}
void LandmineGoodie::doSomething()
{
	if (!isAlive())
		return;

	getWorld()->checkLandmineGoodie(this);
}
void Landmine::doSomething()
{
	if (!isAlive())
	{
		return;   // do nothing if the object is dead
	}


	if (!m_isActiveLandmine)
	{
		m_SafetyTicks--;
		if (m_SafetyTicks == 0) // time is up
		{
			m_isActiveLandmine = true;
			return; // do nothing this tick
		}
	}
	else // it is (m_isActiveLandmine)
	{
		if (getWorld()->hasContactWithLivingThings(this)) // overlaps with living things
		{
			setDead(); // this will create the explosion
		}

	}
}


double getnewXSpriteWidthAway(double x, Direction dir)
{
	switch (dir)
	{
	case GraphObject::left:
		return x - SPRITE_WIDTH;
	case GraphObject::right:
		return x + SPRITE_WIDTH;
	default:
		return x;
	}

}

double getnewYSpriteHeightAway(double y, Direction dir)
{
	switch (dir)
	{
	case GraphObject::up:
		return y + SPRITE_HEIGHT;
	case GraphObject::down:
		return y - SPRITE_HEIGHT;
	default:
		return y;
	}
}

Direction getRandDirection()
{
	int rand = randInt(1, 4);
	switch(rand)
	{
	case 1:
		return GraphObject::left;
	case 2:
		return GraphObject::right;
	case 3:
		return GraphObject::up;
	case 4:
		return GraphObject::down;
	}

	return GraphObject::right; // by default and to get rid of warning
}

int randintfrom3to10()
{
	int rand = randInt(3, 10);
	return rand;
}

bool vomitIfHumanInRange(Zombie* z)
{
	double vomitX = getnewXSpriteWidthAway(z->getX(), z->getDirection());
	double vomitY = getnewYSpriteHeightAway(z->getY(), z->getDirection());


	bool humanWithin10PixVomit = z->getWorld()->humanIsWithin10Pixels(vomitX, vomitY);

	if (humanWithin10PixVomit)  // human is within 10 pixels of vomit distance
	{
		int rand = randInt(1, 3);

		if (rand == 1)  // 1 in 3 chance
		{
			z->getWorld()->createVomit(vomitX, vomitY);
			z->getWorld()->playSound(SOUND_ZOMBIE_VOMIT);
			return true;  // has vomited
		}

	}

	return false; // did not vomit
}

bool MoveZombieToDestination(Zombie* z)
{
	double destX = z->getX();
	double destY = z->getY();
	Direction dir = z->getDirection();

	MovePixelsInTheDirection(destX, destY, dir, 1);

	if (z->getWorld()->canMoveActor(destX, destY, z))
	{
		z->moveTo(destX, destY);
		z->reduceMovementDistance();

		return true;
	}
	else
	{
		z->setMovementDistance(0);
	}
	return false;
}

void DumbZombie::doSomething()
{
	if (!isAlive())
		return;

	if (timeAlive()%2 == 0)
	{
		addTime();
		return;
	}
	else
	{
		bool vomited = vomitIfHumanInRange(this);

		if (vomited)
		{
			return; // stop if you vomited.
		}

		
		if (getMovementDistance() == 0)  // needs new movement
		{
			setPlannedDirection(getRandDirection()); // chose random 
			setMovementDistance(randintfrom3to10()); 

		}

		// Moving zombies
		if (getMovementDistance() == 0) // do nothing when not supposed to.
			return;

		//MoveZombies
		MoveZombieToDestination(this);

		addTime(); // so next turn it is paralyzed
	}
}
void SmartZombie::doSomething()
{
	addTime(); // to create paralysis

	if (!isAlive())
		return;

	if (timeAlive() % 2 == 0)
	{
		return;
	}
	else // not paralyzed
	{
		//vomit
		bool vomited = vomitIfHumanInRange(this);

		if (vomited)
		{
			return; // stop if you vomited.
		}


		if (getMovementDistance() == 0)  // needs new movement
		{
			setMovementDistance(randintfrom3to10());

			if (getWorld()->distanceToNearestHuman(this) > 80)  // distance greater than 80
			{
				setPlannedDirection(getRandDirection()); // chose random 
			}
			else
			{
				vector<Direction> movesCloser;  // find directions that move closer

				movesCloser = getWorld()->directionMovesZombieCloserToHuman(this);


				if (movesCloser.size() == 1) // only one direction moves it closer
				{
					setPlannedDirection(movesCloser[0]);
				}
				if (movesCloser.size() == 2) // there can be at max 2 directions which makes it move closer
				{
					int rand = randInt(0, 1);

					switch (rand)
					{
					case 0:
						setPlannedDirection(movesCloser[0]);
						break;
					case 1:
						setPlannedDirection(movesCloser[1]);
						break;
					}
						
				}
			}
		}


		// Move Smart Zombies

		MoveZombieToDestination(this);
	}

}


std::vector<Direction> PossibleMomvementDirections(Citizen* c)
{
	double x = c->getX();
	double y = c->getY();

	vector<Direction> possibleDir;
	
	//left
	if (c->getWorld()->canMoveActor(x - 2, y, c))
		possibleDir.push_back(GraphObject::left);

	if (c->getWorld()->canMoveActor(x + 2, y, c))
		possibleDir.push_back(GraphObject::right);

	if (c->getWorld()->canMoveActor(x, y-2, c))
		possibleDir.push_back(GraphObject::down);

	if (c->getWorld()->canMoveActor(x, y+2, c))
		possibleDir.push_back(GraphObject::up);

	return possibleDir;

}

void Citizen::doSomething()
{
	if (!isAlive())
		return;

	addTime();// to create paralysis Ticks
	//2.
	if (isInfected())
	{
		addInfectionCounter();
		if (getInfectionCounter() >= 500)
		{
			getWorld()->turnInfectedHumanToZombie(this);

			return;
		}
	}

	if (timeAlive() % 2 == 0)  // paralysis turn
	{
		return;
	}
	else
	{
		double xcoord = getX();
		double ycoord = getY();

		double dist_p = getWorld()->distanceToPenelope(this);
		double dist_z = getWorld()->distanceToClosestZombie(xcoord,ycoord); // dist_z is -1 if there are no zombies

		if ((dist_p < dist_z || dist_z == -1) && dist_p <= 80)
		{
			vector<Direction> dirs = getWorld()->DirectionCloserToPenelope(this);


			// current 
			double newx = getX();
			double newy = getY();

			if (dirs.size() == 1) //only one direction
			{
				MovePixelsInTheDirection(newx, newy, dirs[0], CITIZEN_MOVEMENT_DISTANCE);

				if (getWorld()->canMoveActor(newx, newy, this)) // can move actor to new coordinate
				{
					moveTo(newx, newy);
					return;
				}

			}

			newx = getX();
			newy = getY();
			if (dirs.size() == 2) // 2 directions
			{
				int rand = randInt(0, 1);

				if (rand == 0)
				{
					MovePixelsInTheDirection(newx, newy, dirs[0], CITIZEN_MOVEMENT_DISTANCE);
					if (getWorld()->canMoveActor(newx, newy, this)) // can move actor to new coordinate
					{
						moveTo(newx, newy);
						return;
					}
					else // can't move to first direction
					{
						newx = getX();
						newy = getY();
						MovePixelsInTheDirection(newx, newy, dirs[1], CITIZEN_MOVEMENT_DISTANCE);

						if (getWorld()->canMoveActor(newx, newy, this)) // can move actor in second direction
						{
							moveTo(newx, newy);
							return;
						}
					}

				}
				else // rand == 1
				{
					MovePixelsInTheDirection(newx, newy, dirs[1], CITIZEN_MOVEMENT_DISTANCE);
					if (getWorld()->canMoveActor(newx, newy, this)) // can move actor to new coordinate
					{
						moveTo(newx, newy);
						return;
					}
					else // can't move to first direction
					{
						newx = getX();
						newy = getY();
						MovePixelsInTheDirection(newx, newy, dirs[0], CITIZEN_MOVEMENT_DISTANCE);

						if (getWorld()->canMoveActor(newx, newy, this)) // can move actor in second direction
						{
							moveTo(newx, newy);
							return;
						}
					}
				}
			}
		}

			//Step 7
			if (dist_z <= 80)
			{


				vector<Direction> possibleMovements = PossibleMomvementDirections(this);

				vector<double> newDistances;
				for (int i = 0; i < possibleMovements.size(); i++)  // get the new distances!
				{
					double currX = getX();
					double currY = getY();

					MovePixelsInTheDirection(currX, currY, possibleMovements[i], CITIZEN_MOVEMENT_DISTANCE);

					double newDist = getWorld()->distanceToClosestZombie(currX, currY);

					newDistances.push_back(newDist);
				}


				int index = -1;
				double maxDistance = dist_z;

				for (int j = 0; j < newDistances.size(); j++)
				{
					if (newDistances[j] > maxDistance)
					{
						index = j;  // set j to this distance
						maxDistance = newDistances[j];
					}
				}


				if (index != -1) // there exists an index where it moves it farther
				{
					double moveX = getX();
					double moveY = getY();
					MovePixelsInTheDirection(moveX, moveY, possibleMovements[index], CITIZEN_MOVEMENT_DISTANCE);

					moveTo(moveX, moveY);
				}
			}
	}
}
