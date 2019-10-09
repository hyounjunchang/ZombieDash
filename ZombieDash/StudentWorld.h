#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include <string>

#include <list>
#include <vector>
#include "Level.h"
#include "Actor.h"



// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class Actor;
class Penelope;

class StudentWorld : public GameWorld
{
public:
    StudentWorld(std::string assetPath);
    virtual int init();
    virtual int move();
    virtual void cleanUp();
	~StudentWorld();

	bool canMoveActor(double xTo, double yTo, Actor* movingActor) const;  //checks if an actor can move to a new spot

	bool OverlapsWithAnyActorPixelBlock(double x, double y) const; // checks if a coordinate has an actor block

	void movePeoplefromExit(Exit* exit);
	void moveLivingThingsfromPit(Pit* pit);
	void destoryObjectsWithFlame(Flame* flm);
	void checkGasCanGoodie(GasCanGoodie* gc);
	void checkLandmineGoodie(LandmineGoodie* lg);
	void checkVaccineGoodie(VaccineGoodie* vg);

	void useFlameThrower(Penelope* p);
	void createLandmine(Penelope* p);

	void infectHumans(Vomit* v);


	void turnInfectedHumanToZombie(Human* h);

	bool hasContactWithLivingThings(Landmine* lm);
	void createExplosionWithLandmine(Landmine* lm);

	bool humanIsWithin10Pixels(double x, double y) const;
	void createVomit(double x, double y);

	void addVaccine(double x, double y);

	double distanceToNearestHuman(SmartZombie* sz);
		
	std::vector<Direction> directionMovesZombieCloserToHuman(Zombie* z);

	double distanceToPenelope(Citizen* c);

	double distanceToClosestZombie(double citizenx, double citizeny); // returns -1 if there are no zombies!!

	std::vector<Direction>DirectionCloserToPenelope(Citizen* c);
	

private:
	std::list<Actor*> m_actorList;  // has Penelope in the front of the List .. So don't delte Penelope pointer twice!!
	Penelope* m_main_character;

	// functions
	std::string getStatText() const;
	std::string getLevelFileName(int level) const;
	std::list<Actor*> allOverlappingActors(Actor* interestedActor) const;  // Returns pointers to actors that all overlap
	void pickUpGoodie(Goodies* goodie);
	bool overlapsWithWallOrExit(double x, double y) const;
	void createZombieFromHuman(double x, double y);

	Human* closestHuman(Zombie* z);
	Zombie* closestZombie(double citizenX, double citizenY);

	int getNumCitizens();

	
};

#endif // STUDENTWORLD_H_
