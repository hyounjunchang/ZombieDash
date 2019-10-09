#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"


// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

class StudentWorld;

class Actor : public GraphObject
{	
	public:
	virtual	void doSomething() = 0;

	virtual bool otherCanMoveOnTop() const;
	virtual bool isHuman() const;
	virtual bool isZombie() const;

	virtual bool canBeDamaged() const;
	virtual bool canBlockFlames() const;

	virtual bool isLivingThing() const;  // True for living things, false for everything else

	bool isAlive() const;  // Defaults to True
							// for living things, it returns whether object is alive (may not be in the screen if the exited)

	virtual void setDead();  
	void setAliveAgain();
	

	virtual bool isLivingActor() const; // Non-living things return true. Living things return m_isliving


	StudentWorld* getWorld() const
	{
		return SWorld;
	}


	Actor (int imageID, double startX, double startY, StudentWorld* sw, Direction dir = 0, int depth = 0, double size = 1.0) // default dir = 0 points to right
		: GraphObject(imageID, startX, startY, dir, depth, size), m_isAlive(true), SWorld(sw)
		{} 
	
private:
	bool m_isAlive;
	StudentWorld* SWorld;
};

class LivingThings : public Actor  // All living things Start out alive
{
	public:
		LivingThings(int imageID, double startX, double startY, StudentWorld* sw, Direction dir = 0, int depth = 0, double size = 1.0)
			: Actor(imageID, startX, startY, sw, dir, depth, size)
		{
			m_isliving = true;
			m_timeAlive = 0;
		}
		
		virtual bool isLivingThing() const;
		virtual bool isLivingActor() const;

		virtual void setToNotLiving();

		virtual bool isHuman() const = 0;
		virtual bool isZombie() const = 0;

		int timeAlive() const
		{
			return m_timeAlive;
		}
		void addTime()
		{
			m_timeAlive++;
		}


		virtual bool otherCanMoveOnTop() const;

	private:
		bool m_isliving;
		int m_timeAlive;
};

class Human : public LivingThings   // Penelope, Citizen
{
	public:
		Human(int imageID, double startX, double startY, StudentWorld* sw, Direction dir = 0, int depth = 0, double size = 1.0)
		: LivingThings(imageID, startX, startY, sw, dir, depth, size)
		{	
			m_isInfected = false;
			m_infectionCounter = 0;
		}

		//Accessor
		virtual bool isHuman() const;
		virtual bool isZombie() const;

		bool isInfected() const;

		int getInfectionCounter() const;
		virtual void infect();
		void addInfectionCounter();
		void cureInfection();
	
	
	private:
		bool m_isInfected;
		int m_infectionCounter;

};


class Penelope : public Human
{
	public:
		Penelope(int x, int y, StudentWorld* sw)
			: Human(IID_PLAYER, SPRITE_WIDTH*x, SPRITE_WIDTH*y, sw, right, 0),
			m_flamethrower(0), m_landmine(0), m_vaccine(0)
		{}
		virtual void doSomething();
		virtual void setDead();

		int get_FlameThrower() const;
		int get_LandMines() const;
		int get_Vaccines() const;

		void increaseFlamethrower(int amt)
		{
			m_flamethrower += amt;
		}
		void increaseLandmines(int amt)
		{
			m_landmine += amt;
		}
		void increaseVaccines(int amt)
		{
			m_vaccine += amt;
		}

		

		
private:
	int m_flamethrower, m_landmine, m_vaccine;

	//functions
	void useFlamethrower();
	void useLandmine();
	void useVaccine();
};

class Citizen : public Human
{
public:
	Citizen(int level_x, int level_y, StudentWorld* sw)
		:Human(IID_CITIZEN, SPRITE_WIDTH*level_x, SPRITE_WIDTH*level_y, sw, right, 0)
	{}

	virtual	void doSomething();
	virtual void setDead();

	virtual void infect();
	const int CITIZEN_MOVEMENT_DISTANCE = 2;

private:
};

class Zombie : public LivingThings
{
public:
	Zombie(int imageID, double startX, double startY, StudentWorld* sw, Direction dir = 0, int depth = 0, double size = 1.0)
		: LivingThings(imageID, startX, startY, sw, dir, depth, size), movementPlanDistance(0), plannedDir(GraphObject::right)
	{}

	virtual bool isHuman() const;
	virtual bool isZombie() const;

	int getMovementDistance() const
	{
		return movementPlanDistance;
	}

	void reduceMovementDistance()
	{
		if (movementPlanDistance > 0)
			movementPlanDistance--;
	}
	void setMovementDistance(int num)
	{
		if (num < 0)
			return;

		movementPlanDistance = num;
	}

	Direction getPlannedDirection() const
	{
		return plannedDir;
	}

	void setPlannedDirection(Direction dir) // also changes direction facing
	{
		plannedDir = dir;
		setDirection(dir);
	}


private:
	int movementPlanDistance;
	Direction plannedDir;


};

class DumbZombie : public Zombie
{
public:
	DumbZombie(int level_x, int level_y, StudentWorld* sw)
		:Zombie(IID_ZOMBIE, SPRITE_WIDTH * level_x, SPRITE_HEIGHT * level_y, sw, right, 0, 1.0)
	{}

	DumbZombie(double x, double y, StudentWorld* sw)
		:Zombie(IID_ZOMBIE, x, y, sw, right, 0, 1.0)
	{}

	virtual	void doSomething();
	virtual void setDead();

private:
	

};

class SmartZombie : public Zombie
{
public:
	SmartZombie(int level_x, int level_y, StudentWorld* sw)
		:Zombie(IID_ZOMBIE, SPRITE_WIDTH * level_x, SPRITE_HEIGHT * level_y, sw, right, 0, 1.0)
	{}

	SmartZombie(double x, double y, StudentWorld*sw)
		:Zombie(IID_ZOMBIE, x, y, sw, right, 0, 1.0)
	{}

	virtual	void doSomething();
	virtual void setDead();
private:

};


class Goodies : public Actor //
{
public:
	Goodies(int imageID, double startX, double startY, StudentWorld* sw, Direction dir = 0, int depth = 0, double size = 1.0)
		: Actor(imageID, startX, startY, sw, dir, depth, size)
	{}

	private:
};

class VaccineGoodie : public Goodies
{
	// also starts out alive!!
public:
	VaccineGoodie(int level_x, int level_y, StudentWorld* sw)  // level coordinates
		: Goodies(IID_VACCINE_GOODIE, SPRITE_WIDTH * level_x, SPRITE_HEIGHT * level_y, sw, right, 1, 1.0)
	{}

	VaccineGoodie (double x, double y, StudentWorld* sw)
		: Goodies(IID_VACCINE_GOODIE, x, y, sw, right, 1, 1.0)
	{}
	virtual	void doSomething();

private:

};

class GasCanGoodie : public Goodies
{
public:
	// also starts out alive
	GasCanGoodie(int level_x, int level_y, StudentWorld* sw)  // level coordinates
		: Goodies(IID_GAS_CAN_GOODIE, SPRITE_WIDTH * level_x, SPRITE_HEIGHT * level_y, sw, right, 1, 1.0)
	{}

	virtual	void doSomething();

private:
	
};

class LandmineGoodie : public Goodies
{
public:
	LandmineGoodie(int level_x, int level_y, StudentWorld* sw)  // level coordinates
		: Goodies(IID_LANDMINE_GOODIE, SPRITE_WIDTH * level_x, SPRITE_HEIGHT * level_y, sw, right, 1, 1.0)
	{}

	virtual	void doSomething();

private:

};

class Weapons : public Actor //
{
public:
	Weapons(int imageID, double startX, double startY, StudentWorld* sw, Direction dir = 0, int depth = 0, double size = 1.0)
		: Actor(imageID, startX, startY, sw, dir, depth, size)
	{}

private:
};

class Projectiles : public Weapons
{
public:
	Projectiles(int imageID, double startX, double startY, StudentWorld* sw, Direction dir = 0, int depth = 0, double size = 1.0)
		: Weapons(imageID, startX, startY, sw, dir, depth, size)
	{}
private:
	
};



class Landmine : public Weapons
{
public:
	Landmine(double xcoord, double ycoord, StudentWorld* sw)
		:Weapons(IID_LANDMINE, xcoord, ycoord, sw, right, 1, 1.0), m_isActiveLandmine(false), m_SafetyTicks(30)
	{}

virtual	void doSomething();
virtual void setDead();

private:
	bool m_isActiveLandmine;
	int m_SafetyTicks;

};

class Flame : public Projectiles
{
public:
	Flame(double xcoord, double ycoord, Direction dir, StudentWorld* sw)
		:Projectiles(IID_FLAME, xcoord, ycoord,sw, dir, 0, 1.0), m_numTickActive(0)
	{}
	virtual	void doSomething();
	virtual bool canBeDamaged() const;

	int MAX_TICKS_ALLOWED() const{ return 2; }	
	int getTimeActive() const { return m_numTickActive; }
private:
	int m_numTickActive;  // how long the flame was alive for

};

class Vomit : public Projectiles
{
public:
	Vomit(double xcoord, double ycoord, StudentWorld* sw)
		:Projectiles(IID_VOMIT, xcoord, ycoord, sw, right, 0, 1.0), m_numTickActive(0)
	{}
	virtual	void doSomething();
	virtual bool canBeDamaged() const;

	int MAX_TICKS_ALLOWED() const { return 2; }
	int getTimeActive() const { return m_numTickActive; }

private:
	int m_numTickActive;
};


class Indestructable : public Actor
{
public:
	Indestructable(int imageID, double startX, double startY, StudentWorld* sw, Direction dir = 0, int depth = 0, double size = 1.0) // default dir = 0 points to right
		: Actor(imageID, startX, startY, sw, dir, depth, size)
	{}

	virtual bool canBeDamaged() const;
	virtual bool canBlockFlames() const;

private:
};

class Pit : public Indestructable
{
public:
	Pit(int level_x, int level_y, StudentWorld* sw)  // created with pixel coordinates
		:Indestructable(IID_PIT, SPRITE_WIDTH*level_x, SPRITE_WIDTH*level_y,sw, right, 0, 1.0)
	{}
	Pit(double x, double y, StudentWorld* sw)
		:Indestructable(IID_PIT, x, y, sw, right, 0, 1.0)
	{}

	virtual	void doSomething();
private:

};

class BlocksFlames :public Indestructable
{
public:
	BlocksFlames(int imageID, double startX, double startY, StudentWorld* sw, Direction dir = 0, int depth = 0, double size = 1.0) // default dir = 0 points to right
		: Indestructable(imageID, startX, startY, sw, dir, depth, size)
	{}

	virtual bool canBlockFlames() const;

private:

};

class Wall : public BlocksFlames
{
	public:
		Wall(int x, int y, StudentWorld* sw) // block location
		: BlocksFlames(IID_WALL, SPRITE_WIDTH*x, SPRITE_WIDTH*y, sw,right) // default starts at right
		{}
		virtual void doSomething()
		{}
		virtual bool otherCanMoveOnTop() const;

};

class Exit : public BlocksFlames
{
public:
	Exit(int x, int y, StudentWorld* sw)
		: BlocksFlames(IID_EXIT, SPRITE_WIDTH*x, SPRITE_WIDTH*y, sw, right, 1, 1.0)
	{}
	virtual void doSomething();

private:

};


#endif // ACTOR_H_
