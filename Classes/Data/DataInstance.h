#ifndef __DATAINSTANCE_H__
#define __DATAINSTANCE_H__

#include "Common.h"
#include "Protocol/RoleStateProtocol.h"
#include "Protocol/MagicStateProtocol.h"
#include "Magic/BaseMagic.h"
#include "Role/BaseRole.h"
#include "Item/Equipment.h"
#include "Data/Item.h"
#include <array>

#define MAXHERONUMS 6
#define STHMINUS -1
#define STHADD 1

class DataInstance 
{
public:
	static DataInstance* getInstance();	// get instance

	/*******************data********************/
	bool _isFirstPlay;

	int _chapterDownNum = 1;			// chapter passed numbers
	int _levelStateSet[5] = { 1,0,0,0,0 };	// level state 0: disable, 1: done, 2: battle
	int _currency[2] = { 0 };	// 0 : coin, 1 : ingot

	int _equipUpgradeTimes = 0;		// record equip upgrade times

	// current chapter
	int _curChapter = 0;
	// current level
	int _curLevel = 0;

	// current battle queue
	std::array<int, 6> _curBattleRoleQue;

	// owned hero list 
	std::map<int, spOwnedHero> _ownedHeroList;	// id and hero brief
	// owned magic list
	std::map<int, int> _ownedMagicList;	// magic list	id & level
	// owned equip list
	std::map<int, int> _ownedEquipList; // equip list	id & level
	// owned item list
	std::map<int, int> _ownedItemList; // item list	id & num
	// player's tasks achieve progress list
	std::vector<int> _playerCurTaskList;

public:
	// total item list
	std::map<int, spBaseItem> _baseItemList;
	// all drop list
	std::map<int,spDropItem> _allDropItemList;	// all drop item in game
	// all shop items
	std::vector<spFragment> _allShopItemList; // all shop items
	// all topUp list
	std::vector<spTopUpItem> _allTopItemList;	// all top items
	// all task info list
	std::vector<spTaskItem> _allTaskItemList;	//all task items
	// task award list
	std::vector<spTaskAwardItem> _allTaskAwardList;	// all task award items

public:
	/******************model******************/
	std::map<int, spRoleConfig> _heroModel;
	std::map<int, spMagicConfig> _magicModel;
	std::map<int, spEquipmentConfig> _equipModel;
	std::map<int, spRoleConfig> _monsterModel;
	std::map<int, chapterConfig> _allChapterInfo; // all chapter
	std::map<int, stageConfig> _curChapterStagesInfo;	// all stages in current chapter
	std::map<std::string, std::string> _textModel;

public:
	// hero ConfigFromJson
	void heroConfigFromJson(int heroId, int heroLevel, spRoleConfig& spconf, const char* filepath);
	// monster ConfigFromJson
	void monsConfigFromJson(int monsId, int monsLevel, spRoleConfig& spconf, const char* filepath);
	// magic configFromJson
	void magicConfigFromJson(int magicId, int magicLevel, spMagicConfig& magconf, const char* filepath);
	// equipment configFromJson
	void equipConfigFromJson(int equipId, int equipLevel, spEquipmentConfig& equipConf, const char* filepath);

	// store owned item
	void storeOwnedItem();
	// get owned item
	void getOwnedItem();
	// store owned hero
	void storeOwnedHero();
	// get owned hero info
	void getOwnedHero();
	// store owned magic
	void storeOwnedMagic();
	// get owned magic info
	void getOwnedMagic();
	// store owned equip
	void storeOwnedEquip();
	// get owned equip info
	void getOwnedEquip();
	// get task list
	void getTaskList();
	// store task list
	void storeTaskList();

public:
	/***interface function***/
	// pass the level
	void passLevelandStore();
	// get chapter info
	void getChapterInfo();
	// get level info
	void getLevelInfo(int chapter);
	// text init
	void allTextInit();
	// chapter info init
	void chapterInfoInit();
	// stages info init
	void stagesInfoWithChapter(int id);
	// init all item list
	void baseItemListInit();
	// init drop item list
	void dropItemListInit();
	// init top up item list
	void topUpItemListInit();
	// init shop item list
	void fragmentItemlistInit();
	// push back hero in current battle queue, return insert position in queue
	int heroBattleQueueAddIn(int id);
	// remove hero from current battle queue
	bool heroRemoveFromBattleQue(int id);
	// hero has already in queue
	bool isHeroInQue(int id);
	// load user hero data
	bool loadHeroData();
	// load user equip data
	bool loadEquipData();
	// load user magic data
	bool loadMagicData();
	// load battle data
	bool loadBattleData();
	// mosterConfig
	spRoleConfig getMonsterConifgWithId(int id);
	// heroConfig
	spRoleConfig getRoleConfigWithId(int id);
	// magicConfig
	spMagicConfig getMagicConfigWithId(int id);
	// equipConfig
	spEquipmentConfig getEquipConfigWithId(int id);
	// upgrade hero
	void upgradeHero(int id);
	// upgrade magic
	void upgradeMagic(int id);
	// upgrade equip
	void upgradeEquip(int id);

	// get current battle hero numbers
	int getHeroNumInCurBattleQue();

	// alter currency 
	// para: incremental need sign 1 , default -1
	// currencyIndex: 0- coin 1- ingot
	bool alterCurrency(const int currencyIndex, int incremental = 0, int sign = STHMINUS);

	// alter material
	// para: incremental need sign 1 , default -1
	bool alterMaterial(const int MateId, int increment = 0, int sign = STHMINUS);

	// alter exp
	void alterExp(int incremental = 0);

	// get new hero
	void getNewHero(int heroId, int level = 0);

	// toggle task
	void toggleTask(int curId, int nextId);
private:
	DataInstance();
	~DataInstance();

private:
	static DataInstance* _instance;
};

#endif