#include "StudentWorld.h"
#include "GameConstants.h"
#include <string>

#include <sstream>  // defines the type std::ostringstream
#include <iomanip>  // defines the manipulator setw

#include <cmath>
#include <vector>

using namespace std;

GameWorld* createStudentWorld(string assetPath)
{
	return new StudentWorld(assetPath);
}

// Students:  Add code to this file, StudentWorld.h, Actor.h and Actor.cpp

double distance(double x1, double y1, double x2, double y2)
{
	double deltaX = x2 - x1;
	double deltaY = y2 - y1;

	double squaredDistance = deltaX * deltaX + deltaY * deltaY;

	return sqrt(squaredDistance);
}


bool Overlaps(double x, double y, Actor* other)
{

	double deltaX = x - other->getX();
	double deltaY = y - other->getY();

	return (deltaX * deltaX + deltaY * deltaY) < 100;

}


bool twoActorOverlaps (Actor* first, Actor*other)
{
	double ThisX = first->getX();
	double ThisY = first->getY();

	double OtherX = other->getX();
	double OtherY = other->getY();

	double deltaX = ThisX - OtherX;
	double deltaY = ThisY - OtherY;

	return (deltaX * deltaX + deltaY * deltaY) < 100;
}

bool overlapsWithPixelBlock(double x, double y, Actor* actor)  /// returns true if pixel (x,y) is within Actor's pixelblock
{
	double left = actor->getX();
	double right = left + SPRITE_WIDTH - 1;
	double bottom = actor->getY();
	double top = bottom + SPRITE_HEIGHT - 1;

	double xright = x + SPRITE_WIDTH - 1;
	double ytop = y + SPRITE_HEIGHT - 1;

	if ((x >= left && x <= right) && (y >= bottom && y <= top))// bottom - left corner
		return true;
	if ((xright >= left && xright <= right) && (y >= bottom && y <= top))// bottom-right corner
		return true;
	if ((x >= left && x <= right) && (ytop >= bottom && ytop <= top))//top-left corder
		return true;
	if ((xright >= left && xright <= right) && (ytop >= bottom && ytop <= top))//top-right corner
		return true;

	return false;
}

bool StudentWorld::OverlapsWithAnyActorPixelBlock(double x, double y) const
{
	bool overlaps = false;

	for (std::list<Actor*>::const_iterator ip = m_actorList.begin(); ip != m_actorList.end(); ip++)
	{
		if (overlapsWithPixelBlock(x, y, *ip))
		{
			overlaps = true;
			break;
		}
	}

	return overlaps;

}
bool StudentWorld::canMoveActor(double xTo, double yTo, Actor* movingActor) const
{
	for (std::list<Actor*>::const_iterator ip = m_actorList.begin(); ip != m_actorList.end(); ip++)
	{
		if (*ip == movingActor)
			continue; // same object then skip

		if (overlapsWithPixelBlock(xTo, yTo, *ip))
		{
			bool firstobj = movingActor->otherCanMoveOnTop();
			bool secondobj = (*ip)->otherCanMoveOnTop();

			if (!firstobj && !secondobj)  // both objects can't be placed on top
				return false;
		}
	}

	return true;

}

list<Actor*> StudentWorld::allOverlappingActors(Actor* interestedActor) const
{
	list<Actor*> overlappingActors;
	for (std::list<Actor*>::const_iterator ip = m_actorList.begin(); ip != m_actorList.end(); ip++)
	{
		if (*ip == interestedActor)
			continue; // same object then skip

		if (twoActorOverlaps(interestedActor, *ip ))
		{
			overlappingActors.push_back(*ip);
		}
	}

	return overlappingActors;
}


StudentWorld::StudentWorld(string assetPath)
: GameWorld(assetPath), m_main_character(nullptr)
{

}

int StudentWorld::init()
{
	Level lev(assetPath());
	string levelFile = getLevelFileName(getLevel());
	Level::LoadResult result = lev.loadLevel(levelFile);
	if (result == Level::load_fail_file_not_found)
		return GWSTATUS_PLAYER_WON;
	else if (result == Level::load_fail_bad_format)
		return GWSTATUS_LEVEL_ERROR;

	

	for (int i = 0; i < LEVEL_WIDTH; i++)
	{
		for (int j = 0; j < LEVEL_HEIGHT; j++)
		{
			
			Level::MazeEntry ge = lev.getContentsOf(i, j);

			switch (ge) 
			{
			case Level::empty:
				break;
			case Level::player:
			{
				Penelope* mc = new Penelope(i, j, this);
				m_main_character = mc;
				m_actorList.push_front(mc);
				break;
			}
			case Level::wall:
			{
				Wall* wp = new Wall(i, j, this);
				m_actorList.push_back(wp);
				break;
			}
			case Level::exit:
			{
				Actor* ep = new Exit(i, j, this);
				m_actorList.push_back(ep);
				break;
			}
			case Level::citizen:
			{
				Citizen* cp = new Citizen(i, j, this);
				m_actorList.push_back(cp);
				break;
			}
			case Level::pit:
			{
				Pit* p = new Pit(i, j, this);
				m_actorList.push_back(p);
				break;
			}
			case Level::vaccine_goodie:
			{
				VaccineGoodie* p = new VaccineGoodie(i, j, this);
				m_actorList.push_back(p);
				break;
			}
			case Level::gas_can_goodie:
			{
				GasCanGoodie* p = new GasCanGoodie(i, j, this);
				m_actorList.push_back(p);
				break;
			}
			case Level::landmine_goodie:
			{
				LandmineGoodie* p = new LandmineGoodie(i, j, this);
				m_actorList.push_back(p);
				break;
			}
			case Level::dumb_zombie:
			{
				DumbZombie* p = new DumbZombie(i, j, this);
				m_actorList.push_back(p);
				break;
			}
			case Level::smart_zombie:
			{
				SmartZombie* p = new SmartZombie(i, j, this);
				m_actorList.push_back(p);
				break;
			}
			}
		}
	}

	// Stats text
	string StatText = getStatText();
	setGameStatText(StatText);

    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
	m_main_character->doSomething(); // get Penelope to do something


	// call DoSomething for all Characters
		for (std::list<Actor*>::iterator it = m_actorList.begin(); it != m_actorList.end(); it++)
		{	
			if ((*it)->isAlive())  //object is active (not dead, and not removed)
			{
				(*it)->doSomething();

				bool PenelopeDied = !(m_main_character->isLivingActor());
				if(PenelopeDied)
					{
						decLives();
						playSound(SOUND_PLAYER_DIE);
						return GWSTATUS_PLAYER_DIED;
					}


				bool PenelopeFinished = (m_main_character->isLivingActor() && !(m_main_character->isAlive()));  // Penelope is still alive, but not active since she left the game
				if (PenelopeFinished)
				{
					playSound(SOUND_LEVEL_FINISHED);

					if (getLevel() == 99) // level 99
					{
						return GWSTATUS_PLAYER_WON;
					}

					return GWSTATUS_FINISHED_LEVEL;
				}
			}
		}



		// THIS CODE MAY BECOME PROBLEMATIC LATER IF I REDFINE THINGS
		// Removing Inactive or Dead Objects
		for (std::list<Actor*>::iterator it = m_actorList.begin(); it != m_actorList.end(); it++)  
		{
			bool Active = (*it)->isAlive();
			if (!Active)  // Object isn't active
			{
				delete *it;
				it = m_actorList.erase(it);
				it--;   // for the loop to go on
			}
		}
	
		// Update the game status line
		string StatText = getStatText();
		setGameStatText(StatText);



	// the player hasn’t completed the current level and hasn’t died, so
// continue playing the current level
	return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
	for (std::list<Actor*>::iterator it = m_actorList.begin(); it != m_actorList.end(); )
	{
		delete *it;
		it = m_actorList.erase(it);
	}
	m_main_character = nullptr;
}	

StudentWorld::~StudentWorld()
{
	for (std::list<Actor*>::iterator it = m_actorList.begin(); it != m_actorList.end(); it++)
	{
		delete *it;
	}
}

 // required to use our provided class

string StudentWorld::getStatText() const
{
	string statText = "";

	string twosp = "  ";

	ostringstream oss;
	oss << "Score:" << twosp;
	oss.fill('0');  // 0s before the score

	int score = this->getScore();
	if (score < 0)
	{
		score *= -1;
		oss << "-"<< setw(5) << score << twosp;
	}
	else
	{
		oss << setw(6) << score << twosp;
	}

	oss << "Level:" << twosp << getLevel() << twosp;
	// issue after addin g this code here!
	oss << "Lives:" << twosp << getLives() << twosp;


	oss << "Vacc:" << twosp << m_main_character->get_Vaccines() << twosp;
	oss << "Flames:" << twosp << m_main_character->get_FlameThrower() << twosp;
	oss << "Mines:" << twosp << m_main_character->get_LandMines() << twosp;
	oss << "Infected:" << twosp << m_main_character->getInfectionCounter();

	string output = oss.str();
	return output;
}

string StudentWorld::getLevelFileName(int level) const
{
	ostringstream oss;
	oss << "level";

	if (level > 99)
	{
		level = -1;
	}
	oss.fill('0');
	oss << setw(2) << level << ".txt";

	string output = oss.str();
	return output;
}


void StudentWorld::movePeoplefromExit(Exit* exit)
{
	for (std::list<Actor*>::const_iterator ip = m_actorList.begin(); ip != m_actorList.end(); ip++)
	{
		if (*ip == exit) // ignore Overlaps
			continue;
		
		if (twoActorOverlaps(exit, *ip)) // if exit overlaps
		{
			bool isPenelope = (*ip == m_main_character);
			if ((*ip)->isHuman() && !isPenelope) // is citizen
			{
				increaseScore(1500); // 1500 since setDead() reduces score by 1000
				(*ip)->setDead();
				playSound(SOUND_CITIZEN_SAVED);
			}
			

			/*2. The exit must determine if it overlaps7 with Penelope. If so and all citizens have
			either exited the level or died, then:
			a.Inform the StudentWorld object that Penelope has finished the current level. */
			if (isPenelope)
			{
				if (getNumCitizens() == 0)  // all have exited or died
				{
					m_main_character->setDead();  // set Penelope inactive // at the move() if penelope is inactive and alive, game will go to next level
				}
			}
		}
	}
}


//PLAY THE SOUND FOR DYING
void StudentWorld::moveLivingThingsfromPit(Pit* pit)
{
	for (std::list<Actor*>::const_iterator ip = m_actorList.begin(); ip != m_actorList.end(); ip++)
	{
		if (*ip == pit) // ignore Overlaps
			continue;

		if (twoActorOverlaps(pit, *ip)) // if pit overlaps with an object
		{

			// WATCH OUT FOR THIS CODE// THIS MAY CAUSE PROBLEM IF DEFINITION OF DIE CHANGES!
			if ((*ip)->isLivingThing()) // is a living thing
			{
				LivingThings* lp = static_cast<LivingThings*>(*ip);
				
				lp->setToNotLiving();
				lp->setDead();

				//setDead plays the sounds
			}

		}
	}
}

void StudentWorld::destoryObjectsWithFlame(Flame* flm)
{
	for (std::list<Actor*>::const_iterator ip = m_actorList.begin(); ip != m_actorList.end(); ip++)
	{
		if (*ip == flm) // ignore Overlaps
			continue;

		if (twoActorOverlaps(flm, *ip)) // if pit overlaps with an object
		{
			if ((*ip)->canBeDamaged()) // object can be destroyed with flame
			{
				if ((*ip)->isLivingThing())
				{
					LivingThings* lv = static_cast<LivingThings*> (*ip);
					lv->setToNotLiving();
				}

				(*ip)->setDead(); // to activate landmine we keep this

				// we want to remove landmine to end infinte loop!

				if (*ip != m_main_character) // can't delete penelope or game crashes during move()
				{
					delete *ip;
					ip = m_actorList.erase(ip);  // erase it so we stop landmine infinite loop
					ip--;
				}
			

			}
		}
	}
}

void StudentWorld:: pickUpGoodie(Goodies* goodie)
{
	increaseScore(50);
	goodie->setDead();
	playSound(SOUND_GOT_GOODIE);
}

void StudentWorld::checkGasCanGoodie(GasCanGoodie* gc)
{
	if (twoActorOverlaps(gc, m_main_character)) // Penelope overlaps with gas Can goodie
	{
		pickUpGoodie(gc);
		m_main_character->increaseFlamethrower(5);
	}
}

void StudentWorld::checkLandmineGoodie(LandmineGoodie* lg)
{
	if (twoActorOverlaps(lg, m_main_character)) // Penelope overlaps with gas Can goodie
	{
		pickUpGoodie(lg);
		m_main_character->increaseLandmines(2);
	}
}

void StudentWorld::checkVaccineGoodie(VaccineGoodie* vg)
{
	if (twoActorOverlaps(vg, m_main_character)) // Penelope overlaps with gas Can goodie
	{
		pickUpGoodie(vg);
		m_main_character->increaseVaccines(1);
	}
}

bool StudentWorld::overlapsWithWallOrExit (double x, double y) const
{
	list<Actor*> overlappingActors;
	for (std::list<Actor*>::const_iterator ip = m_actorList.begin(); ip != m_actorList.end(); ip++)
	{
		if ((*ip)->canBlockFlames()) // Wall or Exit
		{

			if (Overlaps(x, y, *ip))
			{
				return true;
			}

		}
	}

	return false;
}


void StudentWorld::useFlameThrower(Penelope* p) // use Penelope's direction
{
		playSound(SOUND_PLAYER_FIRE);
		Direction dir = p->getDirection();

		double px = p->getX();
		double py = p->getY();

		double newx, newy;

		for (int i = 1; i <= 3; i++)
		{
			if (dir ==  GraphObject::up)
			{
				newx = px;
				newy = py + (i * SPRITE_HEIGHT);

				if (overlapsWithWallOrExit(newx, newy))  // flames can't be shot through walls
				{
					break;
				}
				
				Flame* fup = new Flame(newx, newy, GraphObject::up, this);
				m_actorList.push_back(fup);
			}
			else if (dir == GraphObject::down)
			{
				newx = px;
				newy = py - (i * SPRITE_HEIGHT);

				if (overlapsWithWallOrExit(newx, newy))
				{
					break;
				}

				Flame* fdown = new Flame(newx, newy, GraphObject::down, this);
				m_actorList.push_back(fdown);
			}
			else if (dir == GraphObject::left)
			{
				newx = px - (i *SPRITE_WIDTH);
				newy = py;

				if (overlapsWithWallOrExit(newx, newy))
				{
					break;
				}

				Flame* fleft = new Flame(newx, newy, GraphObject::left, this);
				m_actorList.push_back(fleft);
			}
			else if (dir == GraphObject::right)
			{
				newx = px + (i *SPRITE_WIDTH);
				newy = py;

				if (overlapsWithWallOrExit(newx, newy))
				{
					break;
				}

				Flame* fright = new Flame(newx, newy, GraphObject::right, this);
				m_actorList.push_back(fright);
			}
			else
			{
				;
			}
		}
}

void StudentWorld::createLandmine(Penelope* p)
{
	double x = p->getX();
	double y = p->getY();

	Landmine* lm = new Landmine(x, y, this);
	m_actorList.push_back(lm);

}

void StudentWorld::infectHumans(Vomit* v)
{
	for (std::list<Actor*>::const_iterator ip = m_actorList.begin(); ip != m_actorList.end(); ip++)
	{
		bool isHuman = (*ip)->isHuman();
		bool overlaps = twoActorOverlaps(v, *ip);

		if (isHuman && overlaps) // only do if is human and it overlaps
		{

			Human* hp = static_cast<Human*>(*ip);

			bool infected = hp->isInfected();

			if (!infected)
				hp->infect();
		}
	}
}

void StudentWorld::turnInfectedHumanToZombie(Human* h)
{
	

	double x = h->getX();
	double y = h->getY();

	if (h != m_main_character) // it is not PENELOPE
	{
		// we only want the zombie sound to play so we are not going to setToNotLiving()
		h->setDead(); // set the person Dead  -- decreases the points!!!
		playSound(SOUND_ZOMBIE_BORN);
		createZombieFromHuman(x, y);
	}


}

void StudentWorld::createZombieFromHuman(double x, double y)
{
	int randNum = randInt(1, 100); //creating random number


	if (randNum <= 70)  // 70%
	{
		DumbZombie* dz = new DumbZombie(x, y, this);
		m_actorList.push_back(dz);
	}
	else
	{
		SmartZombie* z = new SmartZombie(x, y, this);
		m_actorList.push_back(z);
	}
}


bool StudentWorld::hasContactWithLivingThings(Landmine* lm) // Activate Landmine when it is stepped on  (NOT WITH FLAMES YET...!)
{
	bool hasContactwithLiving = false;
	
	for (std::list<Actor*>::const_iterator ip = m_actorList.begin(); ip != m_actorList.end(); ip++)
	{
		bool isLiving = (*ip)->isLivingThing();
		
		if (isLiving)
		{
			if (twoActorOverlaps(*ip, lm)) // 2 actors overlap!
			{
				hasContactwithLiving = true;
			}
		}
	}

	return hasContactwithLiving;

}


//
void StudentWorld::createExplosionWithLandmine(Landmine* lm)
{
	double x = lm->getX();
	double y = lm->getY();

	playSound(SOUND_LANDMINE_EXPLODE);
	
	Pit* p = new Pit(x,y,this); // create newPit
	m_actorList.push_back(p);

	//create 9 flames

	double flamex;
	double flamey;

	for (int i = -1; i <= 1; i++)
	{
		for (int j = -1; j <= 1; j++)
		{
			flamex = x + i * SPRITE_WIDTH;
			flamey = y + j * SPRITE_HEIGHT;


			Flame* fl = new Flame(flamex, flamey, GraphObject::up, this);
			m_actorList.push_back(fl);

		}
	}


}



bool StudentWorld:: humanIsWithin10Pixels(double x, double y) const
{
	bool within10pix = false;

	for (std::list<Actor*>::const_iterator ip = m_actorList.begin(); ip != m_actorList.end(); ip++)
	{
		if ((*ip)->isHuman())
		{
			double humanX = (*ip)->getX();
			double humanY = (*ip)->getY();

			double dist = distance(x, y, humanX, humanY);

			if (dist <= 10)
				within10pix = true;
		}
	}


	return within10pix;
}

void StudentWorld::createVomit(double x, double y)
{
	Vomit* v = new Vomit(x, y, this);
	m_actorList.push_back(v);

}

void StudentWorld::addVaccine(double x, double y)
{
	VaccineGoodie* va = new VaccineGoodie(x, y, this);
	m_actorList.push_back(va);
}

double StudentWorld::distanceToNearestHuman(SmartZombie* sz)
{
	double minimum = 1000000; // value larger than max possible distance
	for (std::list<Actor*>::const_iterator ip = m_actorList.begin(); ip != m_actorList.end(); ip++)
	{
		if ((*ip)->isHuman())
		{
			double humanX = (*ip)->getX();
			double humanY = (*ip)->getY();

			double dist = distance(sz->getX(), sz->getY(), humanX, humanY);

			if (dist < minimum)
			{
				minimum = dist;
			}

		}
	}

	return minimum;
}


Human* StudentWorld:: closestHuman(Zombie* z)
{
	double minimum = 1000000; // value larger than max possible distance

	Human* closest = m_main_character; // penelope
	for (std::list<Actor*>::const_iterator ip = m_actorList.begin(); ip != m_actorList.end(); ip++)
	{
		if ((*ip)->isHuman())
		{
			Human* hp = static_cast<Human*>(*ip);

			double humanX = hp->getX();
			double humanY = hp->getY();

			double dist = distance(z->getX(), z->getY(), humanX, humanY);

			if (dist < minimum)
			{
				minimum = dist;
				closest = hp;
			}

		}
	}

	return closest;
}

std::vector<Direction> StudentWorld::directionMovesZombieCloserToHuman(Zombie* z)
{
	vector<Direction> closerDirections;
	
	Human* closest = closestHuman(z);

	double zombieX = z->getX();
	double zombieY = z->getY();
	
	double humanX = closest->getX();
	double humanY = closest->getY();

	// Left Direction
	
	if (humanX < zombieX)
		closerDirections.push_back(GraphObject::left);


	// Right
	if (humanX > zombieX)
		closerDirections.push_back(GraphObject::right);

	// Down 
	if (humanY < zombieY)
		closerDirections.push_back(GraphObject::down);

	//Up
	if (humanY > zombieY)
		closerDirections.push_back(GraphObject::up);

	return closerDirections;
}


double StudentWorld::distanceToPenelope(Citizen* c)
{
	double x = c->getX();
	double y = c->getY();
	
	double PenelopeX = m_main_character->getX();
	double PenelopeY = m_main_character->getY();

	return distance(x, y, PenelopeX, PenelopeY);
}

Zombie* StudentWorld::closestZombie(double citizenX, double citizenY)
{
	double minimum = 1000000; // value larger than max possible distance

	Zombie* closest = nullptr; // nullptr if no closest zombie
	for (std::list<Actor*>::const_iterator ip = m_actorList.begin(); ip != m_actorList.end(); ip++)
	{
		if ((*ip)->isZombie())
		{
			Zombie* hp = static_cast<Zombie*>(*ip);

			double ZombieX = hp->getX();
			double ZombieY = hp->getY();

			double dist = distance(citizenX, citizenY, ZombieX, ZombieY);

			if (dist < minimum)
			{
				minimum = dist;
				closest = hp;
			}

		}
	}

	return closest;
}

double StudentWorld::distanceToClosestZombie(double citizenx, double citizeny) //returns -1 if there is no zombie!!
{
	Zombie* closest = closestZombie(citizenx, citizeny);

	if (closest == nullptr) // no zombie
	{
		return -1;
	}
	
	double zombieX = closest->getX();
	double zombieY = closest->getY();

	double dist = distance(citizenx, citizeny, zombieX, zombieY);
	return dist;

}

std::vector<Direction> StudentWorld::DirectionCloserToPenelope(Citizen* c)
{
	vector<Direction> closerDirections;
	
	Penelope* pen = m_main_character;

	double citizenX = c->getX();
	double citizenY = c->getY();

	double penX = pen->getX();
	double penY = pen->getY();

	// Left Direction

	if (penX < citizenX)
		closerDirections.push_back(GraphObject::left);


	// Right
	if (penX > citizenX)
		closerDirections.push_back(GraphObject::right);

	// Down 
	if (penY < citizenY)
		closerDirections.push_back(GraphObject::down);

	//Up
	if (penY > citizenY)
		closerDirections.push_back(GraphObject::up);
	return closerDirections;
}

int StudentWorld::getNumCitizens()
{
	int count = 0;
	for (std::list<Actor*>::const_iterator ip = m_actorList.begin(); ip != m_actorList.end(); ip++)
	{
		if ((*ip)->isHuman())
		{
			count++;
		}
	}
	count--; // to remove penelope
	return count;
}
