#ifndef __EQUIPMENT_H__
#define __EQUIPMENT_H__

#include "Common.h"

class spEquipmentConfig {
public:
	int _id;					// equipment id
	std::string _name;			// name
	int _level;					// level
	int _cost;					// cost
	int _Attack;				// physics attack
	int _physicsDefence;		// physics defense 
	int _magicDefence;			// magic defense 
	int _health;				// health
	int _criticalHitRate;		// critical hit rate 
};

class Equipment
{
public:
	Equipment();
	~Equipment();

	spEquipmentConfig eq;
};

#endif