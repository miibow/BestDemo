#include "DataInstance.h"
#include "json/rapidjson.h"
#include "json/document.h"
#include "json/writer.h"
#include "json/stringbuffer.h"
#include "Resource/Resource.h"

USING_NS_CC;

DataInstance* DataInstance::_instance = nullptr;

DataInstance::DataInstance()
{
	/******************************test*************************************/
	auto userData = UserDefault::getInstance();
	_curBattleRoleQue = { -1,-1,-1,-1,-1,-1 };

	_playerCurTaskList = { 0, 1, 2 };

	_ownedHeroList.insert(std::pair<int, spOwnedHero>(0, spOwnedHero()));

	for (int i = 0; i < 8; ++i)
		_ownedEquipList.insert(std::pair<int, int>(i, 0));

	for (int i = 0; i < 12; ++i)
		_ownedMagicList.insert(std::pair<int, int>(i, 0));

	_isFirstPlay = userData->getBoolForKey("firstGame", true);

	_equipUpgradeTimes = userData->getIntegerForKey("equipUpgradeTimes", 0);
	_currency[1] = userData->getIntegerForKey("ingot", 1000);
	_currency[0] = userData->getIntegerForKey("coin", 1000);

	allTextInit();
}

DataInstance::~DataInstance()
{
	storeOwnedHero();
	storeOwnedEquip();
	storeOwnedItem();
	storeOwnedMagic();
}

DataInstance* DataInstance::getInstance()
{
	if (_instance == nullptr)
	{
		_instance = new DataInstance();
	}

	return _instance;
}

void DataInstance::storeOwnedItem()
{
	rapidjson::Document doc;
	doc.SetObject();
	rapidjson::Document::AllocatorType& allocator = doc.GetAllocator();
	rapidjson::Value array(rapidjson::kArrayType);
	for (auto it = _ownedItemList.begin(); it != _ownedItemList.end(); ++it)
	{
		rapidjson::Value object(rapidjson::kObjectType);
		object.AddMember("id", (*it).first, allocator);
		object.AddMember("num", (*it).second, allocator);
		array.PushBack(object, allocator);
	}
	doc.AddMember("itemList", array, allocator);

	rapidjson::StringBuffer buffer;
	rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
	doc.Accept(writer);

	std::string path = FileUtils::getInstance()->getWritablePath() + userItemData;

	FILE* file = fopen(path.c_str(), "wb");
	if (file)
	{
		fputs(buffer.GetString(), file);
		fclose(file);
	}
}

void DataInstance::getOwnedItem()
{
	std::string path = FileUtils::getInstance()->getWritablePath() + userItemData;
	if (!FileUtils::getInstance()->isFileExist(path))
		return;

	rapidjson::Document doc;
	std::string str = FileUtils::getInstance()->getStringFromFile(path);
	doc.Parse<rapidjson::kParseDefaultFlags>(str.c_str());

	if (doc.HasParseError())
	{
		log("GetParseError %s\n", doc.GetParseError());
	}

	if (doc.IsObject() && doc.HasMember("itemList"))
	{
		rapidjson::Value& itemList = doc["itemList"];
		itemList.Size() ? _ownedItemList.clear() : void(0);
		for (int i = 0; i < itemList.Size(); ++i)
		{
			if (itemList[i].HasMember("id") && itemList[i].HasMember("num"))
			{
				_ownedItemList.insert(std::pair<int, int>(itemList[i]["id"].GetInt(), itemList[i]["num"].GetInt()));
			}
		}
	}
}

void DataInstance::storeOwnedHero()
{
	rapidjson::Document doc;
	doc.SetObject();
	rapidjson::Document::AllocatorType& allocator = doc.GetAllocator();
	rapidjson::Value array(rapidjson::kArrayType);
	for (auto it = _ownedHeroList.begin(); it != _ownedHeroList.end(); ++it)
	{
		rapidjson::Value object(rapidjson::kObjectType);
		object.AddMember("id", (*it).first, allocator);
		object.AddMember("level", (*it).second.m_iLevel, allocator); 
		rapidjson::Value equipList(rapidjson::kArrayType);
		for (auto a : it->second._equipList)
			equipList.PushBack(a, allocator);
		object.AddMember("equipList", equipList, allocator);

		array.PushBack(object, allocator);
	}
	doc.AddMember("heroList", array, allocator);

	rapidjson::StringBuffer buffer;
	rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
	doc.Accept(writer);

	std::string path = FileUtils::getInstance()->getWritablePath() + userHeroData;

	FILE* file = fopen(path.c_str(), "wb");
	if (file)
	{
		fputs(buffer.GetString(), file);
		fclose(file);
	}
}

void DataInstance::getOwnedHero()
{
	std::string path = FileUtils::getInstance()->getWritablePath() + userHeroData;
	if (!FileUtils::getInstance()->isFileExist(path))
		return;

	rapidjson::Document doc;
	std::string str = FileUtils::getInstance()->getStringFromFile(path);
	doc.Parse<rapidjson::kParseDefaultFlags>(str.c_str());

	if (doc.HasParseError())
	{
		log("GetParseError %s\n", doc.GetParseError());
	}

	if (doc.IsObject() && doc.HasMember("heroList"))
	{
		rapidjson::Value& herolist = doc["heroList"];
		herolist.Size() ? _ownedHeroList.clear() : void(0);
		for (int i = 0; i < herolist.Size(); ++i)
		{
			if (herolist[i].HasMember("id") && herolist[i].HasMember("level") && herolist[i].HasMember("equipList"))
			{
				auto temp = spOwnedHero();
				temp.m_iLevel = herolist[i]["level"].GetInt();
				temp._equipList.clear();
				rapidjson::Value& list = herolist[i]["equipList"];
				for (int i =0; i < list.Size(); ++i)
					temp._equipList.push_back(list[i].GetInt());
				_ownedHeroList.insert(std::pair<int, spOwnedHero>(herolist[i]["id"].GetInt(), temp));
			}
		}
	}
}

void DataInstance::storeOwnedMagic()
{
	rapidjson::Document doc;
	doc.SetObject();
	rapidjson::Document::AllocatorType& allocator = doc.GetAllocator();
	rapidjson::Value array(rapidjson::kArrayType);
	for (auto it = _ownedMagicList.begin(); it != _ownedMagicList.end(); ++it)
	{
		rapidjson::Value object(rapidjson::kObjectType);
		object.AddMember("id", (*it).first, allocator);
		object.AddMember("level", (*it).second, allocator);
		array.PushBack(object, allocator);
	}
	doc.AddMember("magicList", array, allocator);

	rapidjson::StringBuffer buffer;
	rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
	doc.Accept(writer);

	std::string path = FileUtils::getInstance()->getWritablePath() + userMagicData;

	FILE* file = fopen(path.c_str(), "wb");
	if (file)
	{
		fputs(buffer.GetString(), file);
		fclose(file);
	}
}

void DataInstance::getOwnedMagic()
{
	std::string path = FileUtils::getInstance()->getWritablePath() + userMagicData;
	if (!FileUtils::getInstance()->isFileExist(path))
		return;

	rapidjson::Document doc;
	std::string str = FileUtils::getInstance()->getStringFromFile(path);
	doc.Parse<rapidjson::kParseDefaultFlags>(str.c_str());

	if (doc.HasParseError())
	{
		log("GetParseError %s\n", doc.GetParseError());
	}

	if (doc.IsObject() && doc.HasMember("magicList"))
	{
		rapidjson::Value& magiclist = doc["magicList"];
		magiclist.Size() ? _ownedMagicList.clear() : void(0);
		for (int i = 0; i < magiclist.Size(); ++i)
		{
			if (magiclist[i].HasMember("id") && magiclist[i].HasMember("level"))
			{
				_ownedMagicList.insert(std::pair<int, int>(magiclist[i]["id"].GetInt(), magiclist[i]["level"].GetInt()));
			}
		}
	}
}

void DataInstance::storeOwnedEquip()
{
	rapidjson::Document doc;
	doc.SetObject();
	rapidjson::Document::AllocatorType& allocator = doc.GetAllocator();
	rapidjson::Value array(rapidjson::kArrayType);
	for (auto it = _ownedEquipList.begin(); it != _ownedEquipList.end(); ++it)
	{
		rapidjson::Value object(rapidjson::kObjectType);
		object.AddMember("id", (*it).first, allocator);
		object.AddMember("level", (*it).second, allocator);
		array.PushBack(object, allocator);
	}
	doc.AddMember("equipList", array, allocator);

	rapidjson::StringBuffer buffer;
	rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
	doc.Accept(writer);

	std::string path = FileUtils::getInstance()->getWritablePath() + userEquipData;

	FILE* file = fopen(path.c_str(), "wb");
	if (file)
	{
		fputs(buffer.GetString(), file);
		fclose(file);
	}
}

void DataInstance::getOwnedEquip()
{
	std::string path = FileUtils::getInstance()->getWritablePath() + userEquipData;
	if (!FileUtils::getInstance()->isFileExist(path))
		return;

	rapidjson::Document doc;
	std::string str = FileUtils::getInstance()->getStringFromFile(path);
	doc.Parse<rapidjson::kParseDefaultFlags>(str.c_str());

	if (doc.HasParseError())
	{
		log("GetParseError %s\n", doc.GetParseError());
	}

	if (doc.IsObject() && doc.HasMember("equipList"))
	{
		_ownedEquipList.clear();
		rapidjson::Value& equipList = doc["equipList"];
		equipList.Size() ? _ownedEquipList.clear() : void(0);
		for (int i = 0; i < equipList.Size(); ++i)
		{
			if (equipList[i].HasMember("id") && equipList[i].HasMember("level"))
			{
				_ownedEquipList.insert(std::pair<int, int>(equipList[i]["id"].GetInt(), equipList[i]["level"].GetInt()));
			}
		}
	}
}

void DataInstance::storeTaskList()
{
	rapidjson::Document doc;
	doc.SetObject();
	rapidjson::Document::AllocatorType& allocator = doc.GetAllocator();
	rapidjson::Value array(rapidjson::kArrayType);
	for (auto i = 0; i < _playerCurTaskList.size(); ++i)
	{
		array.PushBack(_playerCurTaskList[i], allocator);
	}
	doc.AddMember("taskList", array, allocator);

	rapidjson::StringBuffer buffer;
	rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
	doc.Accept(writer);

	std::string path = FileUtils::getInstance()->getWritablePath() + userTaskData;

	FILE* file = fopen(path.c_str(), "wb");
	if (file)
	{
		fputs(buffer.GetString(), file);
		fclose(file);
	}
}

void DataInstance::getTaskList()
{
	std::string path = FileUtils::getInstance()->getWritablePath() + userTaskData;
	if (!FileUtils::getInstance()->isFileExist(path))
		return;

	rapidjson::Document doc;
	std::string str = FileUtils::getInstance()->getStringFromFile(path);
	doc.Parse<rapidjson::kParseDefaultFlags>(str.c_str());

	if (doc.HasParseError())
	{
		log("GetParseError %s\n", doc.GetParseError());
	}

	if (doc.IsObject() && doc.HasMember("taskList"))
	{
		_playerCurTaskList.clear();
		rapidjson::Value& taskList = doc["taskList"];
		taskList.Size() ? _playerCurTaskList.clear() : void(0);
		for (int i = 0; i < taskList.Size(); ++i)
		{
			_playerCurTaskList.push_back(taskList[i].GetInt());
		}
	}
}

void DataInstance::monsConfigFromJson(int monsId, int monsLevel, spRoleConfig& spConf, const char* filepath)
{
	if (!FileUtils::getInstance()->isFileExist(filepath))
	{
		log("%s is not exist", filepath);
		return;
	}

	rapidjson::Document doc;
	std::string str = FileUtils::getInstance()->getStringFromFile(filepath);
	doc.Parse<rapidjson::kParseDefaultFlags>(str.c_str());

	if (doc.HasParseError())
	{
		log("%s", doc.GetParseError());
	}

	rapidjson::Value& val = doc["monster"];
	rapidjson::Value& level = val[(rapidjson::SizeType)monsId];
	val = doc["grow"];

	rapidjson::Value& growRate = level["growRate"];
	rapidjson::Value& initProp = level["initProp"];

	if (level.HasMember("id") && level.HasMember("name") && level.HasMember("occupation") &&
		level.HasMember("attackSpeed") && level.HasMember("walkSpeed") && level.HasMember("type") &&
		level.HasMember("cri") && level.HasMember("handLength"))
	{
		spConf._id = monsId;
		spConf._name = _textModel.at(level["name"].GetString());
		spConf._occupation = _textModel.at(level["occupation"].GetString());
		spConf._level = monsLevel;
		spConf._handLength = level["handLength"].GetInt();
		spConf._Attack = initProp[0].GetInt() + growRate[0].GetInt() / 10000.0f * val[monsLevel]["growProp"][0].GetInt();
		spConf._physicsDefence = initProp[1].GetInt() + growRate[1].GetInt() / 10000.0f * val[monsLevel]["growProp"][1].GetInt();
		spConf._magicDefence = initProp[2].GetInt() + growRate[2].GetInt() / 10000.0f * val[monsLevel]["growProp"][2].GetInt();
		spConf._health = initProp[3].GetInt() + growRate[3].GetInt() / 10000.0f * val[monsLevel]["growProp"][3].GetInt();
		spConf._criticalHitRate = level["cri"].GetInt();
		spConf._type = level["type"].GetInt();
		spConf._attackSpeed = level["attackSpeed"].GetInt() / 10000.0f;
		spConf._walkSpeed = level["walkSpeed"].GetInt() / 10000.0f;
	}

	rapidjson::Value& magic = level["skill"];
	for (int i = 0; i < magic.Size(); ++i)
	{
		spConf._magicList.push_back(magic[i].GetInt());
	}
}

void DataInstance::heroConfigFromJson(int heroId, int heroLevel, spRoleConfig& spConf, const char* filepath)
{
	if (!FileUtils::getInstance()->isFileExist(filepath))
	{
		log("%s is not exist", filepath);
		return;
	}

	rapidjson::Document doc;
	std::string str = FileUtils::getInstance()->getStringFromFile(filepath);
	doc.Parse<rapidjson::kParseDefaultFlags>(str.c_str());

	if (doc.HasParseError())
	{
		log("%s", doc.GetParseError());
	}

	rapidjson::Value& val = doc["hero"];
	rapidjson::Value& level = val[(rapidjson::SizeType)heroId];
	val = doc["grow"];

	rapidjson::Value& growRate = level["growRate"];
	rapidjson::Value& initProp = level["initProp"];

	if (level.HasMember("id") && level.HasMember("name") && level.HasMember("occupation") &&
		level.HasMember("attackSpeed") && level.HasMember("walkSpeed") && level.HasMember("type") &&
		level.HasMember("cri") && level.HasMember("addInterval") && level.HasMember("addCost") &&
		level.HasMember("handLength"))
	{
		spConf._id = heroId;
		spConf._name = _textModel.at(level["name"].GetString());
		spConf._occupation = _textModel.at(level["occupation"].GetString());
		spConf._level = heroLevel;
		spConf._handLength = level["handLength"].GetInt();
		spConf._Attack = initProp[0].GetInt() + growRate[0].GetInt() / 10000.0f * val[heroLevel]["growProp"][0].GetInt();
		spConf._physicsDefence = initProp[1].GetInt() + growRate[1].GetInt() / 10000.0f * val[heroLevel]["growProp"][1].GetInt();
		spConf._magicDefence = initProp[2].GetInt() + growRate[2].GetInt() / 10000.0f * val[heroLevel]["growProp"][2].GetInt();
		spConf._health = initProp[3].GetInt() + growRate[3].GetInt() / 10000.0f * val[heroLevel]["growProp"][3].GetInt();
		spConf._criticalHitRate = level["cri"].GetInt();
		spConf._type = level["type"].GetInt();
		spConf._addInterval = level["addInterval"].GetInt();
		spConf._addCost = level["addCost"].GetInt();
		spConf._attackSpeed = level["attackSpeed"].GetInt() / 10000.0f;
		spConf._walkSpeed = level["walkSpeed"].GetInt() / 10000.0f;
	}

	//rapidjson::Value& equipment = level["equip"];
	//for (int i = 0; i < equipment.Size(); ++i)
	//{
	//	spConf._equipmentList.push_back(equipment[i].GetInt() - 5001);
	//}

	rapidjson::Value& magic = level["skill"];
	for (int i = 0; i < magic.Size(); ++i)
	{
		spConf._magicList.push_back(magic[i].GetInt() - 2001);
	}
}

void DataInstance::magicConfigFromJson(int magicId, int magicLevel, spMagicConfig& magconf, const char* filepath)
{
	if (!FileUtils::getInstance()->isFileExist(filepath))
	{
		log("%s is not exist", filepath);
		return;
	}

	rapidjson::Document doc;
	std::string str = FileUtils::getInstance()->getStringFromFile(filepath);
	doc.Parse<rapidjson::kParseDefaultFlags>(str.c_str());

	if (doc.HasParseError())
	{
		log("%s", doc.GetParseError());
	}

	rapidjson::Value& val = doc["magic"];
	rapidjson::Value& level = val[(rapidjson::SizeType)magicId];
	val = doc["grow"];
	rapidjson::Value& initProp = level["initProp"];

	if (level.HasMember("id") && level.HasMember("name") &&
		level.HasMember("effectIndex") && level.HasMember("path") && level.HasMember("flyingSpeed") &&
		level.HasMember("flyingHeight") && level.HasMember("description") && level.HasMember("multiTarget"))
	{
		magconf._id = level["id"].GetInt() - 2001;
		magconf._name = _textModel.at(level["name"].GetString());
		magconf._description = _textModel.at(level["description"].GetString());
		magconf._level = magicLevel;
		magconf._cost = level["cost"].GetInt() / 10000.0f * val[magicLevel]["costCoin"].GetInt();
		magconf._moveDistance = level["moveDistance"].GetInt();
		magconf._effectRange = initProp[0].GetInt();
		magconf._cd = initProp[1].GetInt();
		magconf._damage = level["damage"].GetInt() / 10000.0f + level["growRate"].GetInt() / 10000.0f * val[magicLevel]["growProp"].GetInt();
		magconf._effectIndex = level["effectIndex"].GetInt();
		magconf._path = level["path"].GetBool();
		magconf._flyingSpeed = level["flyingSpeed"].GetInt() / 10000.0f;
		magconf._flyingHeight = level["flyingHeight"].GetInt() / 10000.0f;
		magconf._hitType = level["hitType"].GetInt();
		magconf._multiTarget = level["multiTarget"].GetInt();
	}
}

void DataInstance::equipConfigFromJson(int equipId, int equipLevel, spEquipmentConfig& equipConf, const char* filepath)
{
	if (!FileUtils::getInstance()->isFileExist(filepath))
	{
		log("%s is not exist", filepath);
		return;
	}

	rapidjson::Document doc;
	std::string str = FileUtils::getInstance()->getStringFromFile(filepath);
	doc.Parse<rapidjson::kParseDefaultFlags>(str.c_str());

	rapidjson::Value &val = doc["equip"];
	rapidjson::Value& level = val[(rapidjson::SizeType)equipId];
	val = doc["grow"];
	rapidjson::Value& initProp = level["initProp"];
	rapidjson::Value& growRate = level["growRate"];

	if (level.HasMember("id"))
	{
		equipConf._id = level["id"].GetInt() - 5001;
		equipConf._name = _textModel.at(level["name"].GetString());
		equipConf._level = equipLevel;
		equipConf._cost = level["costRate"].GetInt() / 10000.0f * val[equipLevel]["costGrow"].GetInt();
		equipConf._criticalHitRate = level["cri"].GetInt() / 10000.0f;
		equipConf._Attack = initProp[0].GetInt() + growRate[0].GetInt() / 10000.0f * val[equipLevel]["growProp"][0].GetInt();
		equipConf._physicsDefence = initProp[1].GetInt() + growRate[1].GetInt() / 10000.0f * val[equipLevel]["growProp"][1].GetInt();
		equipConf._magicDefence = initProp[2].GetInt() + growRate[2].GetInt() / 10000.0f * val[equipLevel]["growProp"][2].GetInt();
		equipConf._health = initProp[3].GetInt() / 10000.0f + growRate[3].GetInt() / 10000.0f * val[equipLevel]["growProp"][3].GetInt();
	}
}

int DataInstance::heroBattleQueueAddIn(int id)
{
	for (unsigned int i = 0; !isHeroInQue(id) && isHeroInQue(-1); ++i)
	{
		if (_curBattleRoleQue.at(i) == -1)
		{
			_curBattleRoleQue.at(i) = id;
			return i;
		}
		else if (getRoleConfigWithId(_curBattleRoleQue.at(i))._walkSpeed >= getRoleConfigWithId(id)._walkSpeed)
		{
			for (int j = _curBattleRoleQue.size() - 1; j > i; --j)
			{
				_curBattleRoleQue[j] = _curBattleRoleQue[j - 1];
			}
			_curBattleRoleQue[i] = id;
			return i;
		}
	}
	return -1;
}

bool DataInstance::heroRemoveFromBattleQue(int id)
{
	if (isHeroInQue(id))
	{
		for (auto it = std::find(_curBattleRoleQue.begin(), _curBattleRoleQue.end(), id); it != _curBattleRoleQue.end(); ++it)
		{
			*it = -1;
			if (*(it + 1) == -1)
				break;
			*(it) = *(it + 1);
		}
		return true;
	}
	return false;
}

bool DataInstance::loadHeroData()
{
	for (int i = 0; i < _ownedHeroList.size(); ++i)
	{
		spRoleConfig tempHeroConfig;
		heroConfigFromJson(_ownedHeroList.find(i)->first, _ownedHeroList.at(i).m_iLevel, tempHeroConfig, heroConfigPath);
		_heroModel.insert(std::pair<int, spRoleConfig>(_ownedHeroList.find(i)->first, tempHeroConfig));
		for (auto e : _ownedHeroList)
			for (int j = 0; j < e.second._equipList.size(); ++j)
				_heroModel.at(e.first)._equipmentList.push_back(e.second._equipList[j]);
	}
	return true;
}

bool DataInstance::loadEquipData()
{
	for (int i = 0; i < _ownedEquipList.size(); ++i)
	{
		spEquipmentConfig tempEquipConfig;
		equipConfigFromJson(_ownedEquipList.find(i)->first, _ownedEquipList.at(i), tempEquipConfig, equipConfigPath);
		_equipModel.insert(std::pair<int, spEquipmentConfig>(_ownedEquipList.find(i)->first, tempEquipConfig));
	}
	return true;
}

bool DataInstance::loadMagicData()
{
	for (int i = 0; i < _ownedMagicList.size(); ++i)
	{
		spMagicConfig tempMagicConfig;
		magicConfigFromJson(_ownedMagicList.find(i)->first, _ownedMagicList.at(i), tempMagicConfig, magicConfigPath);
		_magicModel.insert(std::pair<int, spMagicConfig>(_ownedMagicList.find(i)->first, tempMagicConfig));
	}
	return true;
}

bool DataInstance::loadBattleData()
{
	_heroModel.clear();
	_magicModel.clear();
	_equipModel.clear();
	_monsterModel.clear();
	for (int i = 0; i < getHeroNumInCurBattleQue(); ++i)
	{
		spRoleConfig tempHeroConfig;
		heroConfigFromJson(_curBattleRoleQue.at(i), _ownedHeroList.at(_curBattleRoleQue.at(i)).m_iLevel, tempHeroConfig, heroConfigPath);
		_heroModel.insert(std::pair<int, spRoleConfig>(_curBattleRoleQue.at(i), tempHeroConfig));
		for (auto e : _ownedHeroList)
			for (int j = 0; j < e.second._equipList.size(); ++j)
				_heroModel.at(e.first)._equipmentList.push_back(e.second._equipList[j]);

		for (int j = 0; j < tempHeroConfig._magicList.size(); ++j)
		{
			if (_magicModel.count(tempHeroConfig._magicList.at(j)))
				continue;

			spMagicConfig tempMagicConfig;
			magicConfigFromJson(tempHeroConfig._magicList.at(j), _ownedMagicList.at(tempHeroConfig._magicList.at(j)), tempMagicConfig, magicConfigPath);
			_magicModel.insert(std::pair<int, spMagicConfig>(tempHeroConfig._magicList.at(j), tempMagicConfig));
		}

		for (int k = 0; k < tempHeroConfig._equipmentList.size(); ++k)
		{
			if (_equipModel.count(tempHeroConfig._equipmentList.at(k)))
				continue;

			spEquipmentConfig tempEquipConfig;
			equipConfigFromJson(tempHeroConfig._equipmentList.at(k), _ownedEquipList.at(tempHeroConfig._equipmentList.at(k)), tempEquipConfig, equipConfigPath);
			_equipModel.insert(std::pair<int, spEquipmentConfig>(tempHeroConfig._equipmentList.at(k), tempEquipConfig));
		}
	}

	// just for passing audit
	auto& enemyQueue = _curChapterStagesInfo.at(_curLevel).m_vEnemySet;
	for (int i = 0; i < enemyQueue.size(); ++i)
	{
		spRoleConfig tempHeroConfig;
		monsConfigFromJson(enemyQueue[i].first, enemyQueue[i].second, tempHeroConfig, monsConfigPath);
		_monsterModel.insert(std::pair<int, spRoleConfig>(enemyQueue[i].first, tempHeroConfig));
	}

	return true;
}

void DataInstance::passLevelandStore()
{
	if (_levelStateSet[_curLevel] != 1)
	{
		_levelStateSet[_curLevel] = 1;
		UserDefault::getInstance()->setIntegerForKey(StringUtils::format("chapter-%d-%d", _curChapter, _curLevel).c_str(), 1);

		if (_curLevel < 4)
		{
			_levelStateSet[_curLevel + 1] = 2;
			UserDefault::getInstance()->setIntegerForKey(StringUtils::format("chapter-%d-%d", _curChapter, _curLevel + 1).c_str(), 2);
		}
		else	// pass chapter
		{
			(_chapterDownNum + 1) > _allChapterInfo.size() ? _chapterDownNum : ++_chapterDownNum;
			UserDefault::getInstance()->setIntegerForKey("chapterDown", _chapterDownNum);
		}
	}
}

void DataInstance::getChapterInfo()
{
	DataInstance::getInstance()->chapterInfoInit();
	_chapterDownNum = UserDefault::getInstance()->getIntegerForKey("chapterDown", 1);
}

void DataInstance::getLevelInfo(int chapter)
{
	DataInstance::getInstance()->stagesInfoWithChapter(chapter);
	std::string str("");
	for (int i = 0; i < sizeof(_levelStateSet) / sizeof(_levelStateSet[0]); ++i)
	{
		str = StringUtils::format("chapter-%d-%d", chapter, i).c_str();
		_levelStateSet[i] = UserDefault::getInstance()->getIntegerForKey(str.c_str(), i > 0 ? 0 : 2);
	}
}

void DataInstance::fragmentItemlistInit()
{
	if (!FileUtils::getInstance()->isFileExist(fragConfigPath))
	{
		log("%s is not exist", fragConfigPath);
		return;
	}

	rapidjson::Document doc;
	std::string str = FileUtils::getInstance()->getStringFromFile(fragConfigPath);
	doc.Parse<rapidjson::kParseDefaultFlags>(str.c_str());

	if (doc.HasParseError())
	{
		log("%s", doc.GetParseError());
	}

	auto content = doc["fragmentItem"].GetArray();

	for (int i = 0; i < content.Size(); ++i)
	{
		if (content[i].HasMember("id") && content[i].HasMember("name") && content[i].HasMember("costNum") &&
			content[i].HasMember("costType") && content[i].HasMember("fragmentType") && content[i].HasMember("exchangeNum"))
		{
			auto item = spFragment();
			item._id = content[i]["id"].GetInt();
			std::string str = content[i]["name"].GetString();
			item._name = _textModel.at(content[i]["name"].GetString());
			item.m_iCostNum = content[i]["costNum"].GetInt();
			item.m_iCostType = content[i]["costType"].GetInt();
			item.m_iExchangeNums = content[i]["exchangeNum"].GetInt();
			_allShopItemList.push_back(item);
		}
	}
}

void DataInstance::allTextInit()
{
	if (!FileUtils::getInstance()->isFileExist(textConfigPath))
	{
		log("%s is not exist", textConfigPath);
		return;
	}

	rapidjson::Document doc;
	std::string str = FileUtils::getInstance()->getStringFromFile(textConfigPath);
	doc.Parse<rapidjson::kParseDefaultFlags>(str.c_str());

	if (doc.HasParseError())
	{
		log("getParseError %s\n", doc.GetParseError());
	}

	if (doc.IsObject() && doc.HasMember("text"))
	{
		rapidjson::Value& text = doc["text"];
		for (int i = 0; i < text.Size(); ++i)
		{
			_textModel.insert(std::pair<std::string, std::string>(text[i]["key"].GetString(), text[i]["chinese"].GetString()));
		}
	}
}

void DataInstance::stagesInfoWithChapter(int id)
{
	if (!FileUtils::getInstance()->isFileExist(chapterConfigPath))
	{
		log("%s is not exist", chapterConfigPath);
		return;
	}

	rapidjson::Document doc;
	std::string str = FileUtils::getInstance()->getStringFromFile(chapterConfigPath);
	doc.Parse<rapidjson::kParseDefaultFlags>(str.c_str());

	if (doc.HasParseError())
	{
		log("getParseError %s\n", doc.GetParseError());
	}

	if (doc.IsObject() && doc.HasMember("stage"))
	{
		for (int i = 0; i < _allChapterInfo.at(id).m_vStages.size(); ++i)
		{
			rapidjson::Value& stages = doc["stage"][i];
			auto tempStageConfig = stageConfig();
			if (stages.HasMember("id") && stages.HasMember("name") && stages.HasMember("enemySet") &&
				stages.HasMember("exp") && stages.HasMember("gold") && stages.HasMember("dropSet"))
			{
				tempStageConfig.m_iId = i;
				tempStageConfig.m_sName = _textModel.at(stages["name"].GetString());
				tempStageConfig.m_iExp = stages["exp"].GetInt();
				tempStageConfig.m_iGold = stages["gold"].GetInt();

				rapidjson::Value& enemySet = stages["enemySet"];
				for (int j = 0; j < enemySet.Size(); ++j)
				{
					tempStageConfig.m_vEnemySet.push_back(std::pair<int, int>(enemySet[j]["id"].GetInt() - 3001, enemySet[j]["level"].GetInt()));
				}

				rapidjson::Value& dropSet = stages["dropSet"];
				for (int k = 0; k < dropSet.Size(); ++k)
				{
					tempStageConfig.m_vDropSet.push_back(dropSet[k].GetInt());
				}
			}
			_curChapterStagesInfo.insert(std::pair<int, stageConfig>(i, tempStageConfig));
		}
	}
}

void DataInstance::chapterInfoInit()
{
	if (!FileUtils::getInstance()->isFileExist(chapterConfigPath))
	{
		log("%s is not exist", chapterConfigPath);
		return;
	}

	rapidjson::Document doc;
	std::string str = FileUtils::getInstance()->getStringFromFile(chapterConfigPath);
	doc.Parse<rapidjson::kParseDefaultFlags>(str.c_str());

	if (doc.HasParseError())
	{
		log("getParseError %s\n", doc.GetParseError());
	}

	if (doc.IsObject() && doc.HasMember("chapter"))
	{
		rapidjson::Value& chapters = doc["chapter"];
		auto tempChapterConfig = chapterConfig();
		for (int i = 0; i < chapters.Size(); ++i)
		{
			if (chapters[i].HasMember("id") && chapters[i].HasMember("name") && chapters[i].HasMember("icon") &&
				chapters[i].HasMember("stages"))
			{
				tempChapterConfig.m_iId = i;
				tempChapterConfig.m_sIcon = chapters[i]["icon"].GetString();
				tempChapterConfig.m_sName = _textModel.at(chapters[i]["name"].GetString());

				for (int j = 0; j < chapters[i]["stages"].Size(); ++j)
				{
					tempChapterConfig.m_vStages.push_back(chapters[i]["stages"][j].GetInt() - 9001);
				}
			}

			_allChapterInfo.insert(std::pair<int, chapterConfig>(i, tempChapterConfig));
		}
	}
}

void DataInstance::topUpItemListInit()
{
	if (!FileUtils::getInstance()->isFileExist(topUpConfigPath))
	{
		log("%s is not exist", topUpConfigPath);
		return;
	}

	rapidjson::Document doc;
	std::string str = FileUtils::getInstance()->getStringFromFile(topUpConfigPath);
	doc.Parse<rapidjson::kParseDefaultFlags>(str.c_str());

	rapidjson::Value& val = doc["topUpItem"];

	for (int i = 0; i < val.Size(); ++i)
	{
		auto topItem = spTopUpItem();
		topItem.m_iId = val[i]["id"].GetInt();
		topItem.m_iCostNum = val[i]["cost"].GetInt();
		topItem.m_iType = val[i]["type"].GetInt();
		topItem._name = val[i]["name"].GetString();
		topItem._description = _textModel.at(val[i]["des"].GetString());
		_allTopItemList.push_back(topItem);
	}
}

void DataInstance::baseItemListInit()
{
	if (!FileUtils::getInstance()->isFileExist(baseItemConfigPath))
	{
		log("%s is not exist", baseItemConfigPath);
		return;
	}

	rapidjson::Document doc;
	std::string str = FileUtils::getInstance()->getStringFromFile(baseItemConfigPath);
	doc.Parse<rapidjson::kParseDefaultFlags>(str.c_str());

	if (doc.HasParseError())
	{
		log("getParseError %s\n", doc.GetParseError());
	}

	rapidjson::Value& val = doc["baseItem"];

	for (int i = 0; i < val.Size(); ++i)
	{
		auto baseItem = spBaseItem();
		baseItem.m_iId = val[i]["id"].GetInt();
		baseItem.m_iSalePrice = val[i]["salePrice"].GetInt();
		baseItem.m_iSuperposition = val[i]["superposition"].GetInt();
		baseItem.m_iRelatedId = val[i]["relatedId"].GetInt();
		baseItem.m_iExpendable = val[i]["expendable"].GetInt();
		baseItem.m_sRelatedName = val[i]["relatedName"].GetString();
		baseItem.m_sItemPic = val[i]["itemPic"].GetString();
		baseItem.m_sItemPic += ".png";
		_baseItemList.insert(std::pair<int, spBaseItem>(baseItem.m_iId, baseItem));
	}
}

void DataInstance::dropItemListInit()
{
	if (!FileUtils::getInstance()->isFileExist(dropConfigPath))
	{
		log("%s is not exist", dropConfigPath);
		return;
	}

	rapidjson::Document doc;
	std::string str = FileUtils::getInstance()->getStringFromFile(dropConfigPath);
	doc.Parse<rapidjson::kParseDefaultFlags>(str.c_str());

	rapidjson::Value& val = doc["dropItem"];

	for (int i = 0; i < val.Size(); ++i)
	{
		auto dropItem = spDropItem();
		dropItem._id = val[i]["id"].GetInt();
		dropItem._dropProb = val[i]["dropProb"].GetInt();
		dropItem._relatedId = val[i]["relatedId"].GetInt();
		_allDropItemList.insert(std::pair<int, spDropItem>(dropItem._id, dropItem));
	}
}

bool DataInstance::isHeroInQue(int id)
{
	auto it = std::find(_curBattleRoleQue.begin(), _curBattleRoleQue.end(), id);
	if (it == _curBattleRoleQue.end())
		return false;
	else
		return true;
}

spRoleConfig DataInstance::getMonsterConifgWithId(int id)
{
	CCASSERT(_monsterModel.size() > 0, "monsterConfig is out of range");
	return _monsterModel.at(id);
}

spRoleConfig DataInstance::getRoleConfigWithId(int id)
{
	CCASSERT(_heroModel.size() > 0, "heroConfig is out of range");
	return _heroModel.at(id);
}

spMagicConfig DataInstance::getMagicConfigWithId(int id)
{
	CCASSERT(_magicModel.size() > 0, "magicConfig is out of range");
	return _magicModel.at(id);
}

spEquipmentConfig DataInstance::getEquipConfigWithId(int id)
{
	CCASSERT(_equipModel.size() > 0, "equipConfig is out of range");
	return _equipModel.at(id);
}

void DataInstance::upgradeHero(int id)
{
	++_ownedHeroList.at(id).m_iLevel;
	heroConfigFromJson(id, _ownedHeroList.at(id).m_iLevel, _heroModel.at(id), heroConfigPath);
	storeOwnedHero();
}

void DataInstance::upgradeMagic(int id)
{
	_ownedMagicList.at(id)++;
	magicConfigFromJson(id, _ownedMagicList.at(id), _magicModel.at(id), magicConfigPath);
	storeOwnedMagic();
}

void DataInstance::upgradeEquip(int id)
{
	++_equipUpgradeTimes;	// record upgrade times
	UserDefault::getInstance()->setIntegerForKey("equipUpgradeTimes", _equipUpgradeTimes);

	++_ownedEquipList.at(id);
	equipConfigFromJson(id, _ownedEquipList.at(id), _equipModel.at(id), equipConfigPath);
	storeOwnedEquip();
}

void DataInstance::toggleTask(int curId, int nextId)
{
	auto it = find(_playerCurTaskList.begin(), _playerCurTaskList.end(), curId);
	*it = nextId;

	storeTaskList();
}

void DataInstance::getNewHero(int heroId, int level /* = 0 */)
{
	if (_ownedHeroList.count(heroId) != 0)
	{
		_ownedHeroList.at(heroId).m_iLevel = MIN(_ownedHeroList.at(heroId).m_iLevel + 1, 100);
		storeOwnedHero();
		_heroModel.erase(heroId);
		auto temp = spRoleConfig();
		heroConfigFromJson(heroId, _ownedHeroList.at(heroId).m_iLevel, temp, heroConfigPath);
		_heroModel.insert(std::pair<int, spRoleConfig>(heroId, temp));
		AnimateManager::getInstance()->insufficient(sameHero);
		return;
	}

	_ownedHeroList.insert(std::pair<int,spOwnedHero>(heroId, spOwnedHero()));
	storeOwnedHero();

	spRoleConfig temp;
	heroConfigFromJson(heroId, level, temp, heroConfigPath);
	_heroModel.insert(std::pair<int, spRoleConfig>(heroId, temp));

	for (auto& equip : temp._equipmentList)
	{
		spEquipmentConfig tempEqu;
		_ownedEquipList.insert(std::pair<int, int>(equip, 0));
		equipConfigFromJson(equip, 0, tempEqu, equipConfigPath);
		_equipModel.insert(std::pair<int, spEquipmentConfig>(equip, tempEqu));
	}
	storeOwnedEquip();

	for (auto& magic : temp._magicList)
	{
		spMagicConfig tempMa;
		_ownedMagicList.insert(std::pair<int, int>(magic, 0));
		magicConfigFromJson(magic, 0, tempMa, magicConfigPath);
		_magicModel.insert(std::pair<int, spMagicConfig>(magic, tempMa));
	}
	storeOwnedMagic();
}

void DataInstance::alterExp(int incremental /* = 0 */)
{
	if (incremental == 0)
		return;

	auto userData = UserDefault::getInstance();

	int currentExp = userData->getIntegerForKey("playerExp", 0);
	currentExp += incremental;
	userData->setIntegerForKey("playerExp", currentExp);
	userData->setIntegerForKey("playerLevel", currentExp / 100 + 1);		// 100 just for test
}

bool DataInstance::alterMaterial(const int MateId, int increment /* = 0 */, int sign /* = STHMINUS */)
{
	if (increment == 0)
		return false;

	switch (sign)
	{
	case 1:
		if (_ownedItemList.count(MateId))
			_ownedItemList.at(MateId) += increment;
		else
			_ownedItemList.insert(std::pair<int, int>(MateId, increment));
		break;
	case -1:
		if (_ownedItemList.count(MateId) == 0 || _ownedItemList.at(MateId) - increment < 0)
			return false;
		else
			_ownedItemList.at(MateId) -= increment;
		break;
	}

	storeOwnedItem();
	return true;
}

bool DataInstance::alterCurrency(const int index, int incremental, int sign)
{
	CCASSERT(index == 0 || index == 1, "currency is not exist");
	if (incremental == 0)
		return false;

	incremental *= sign;
	if (_currency[index] + incremental < 0)
		return false;

	_currency[index] += incremental;
	UserDefault::getInstance()->setIntegerForKey(index ? "ingot" : "coin", _currency[index]);
	Director::getInstance()->getEventDispatcher()->dispatchCustomEvent("trade");
	return true;
}

int DataInstance::getHeroNumInCurBattleQue()
{
	int num = 0;
	for (int i = 0; i < _curBattleRoleQue.size(); ++i)
	{
		if (-1 < _curBattleRoleQue.at(i))
		{
			++num;
		}
	}

	return num;
}