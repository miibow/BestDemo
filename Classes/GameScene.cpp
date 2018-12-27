#include "GameScene.h"
#include "Map/BaseMap.h"
#include "Data/DataInstance.h"
#include "Resource/Resource.h"

USING_NS_CC;

GameScene::GameScene()
{
	for (int i = 0; i < RESMAGICANIMATIONCOUNT; ++i)
	{
		AnimateManager::getInstance()->cacheAnimation(magicAnimationArray[i] + ".plist", magicAnimationArray[i].substr(magicAnimationArray[i].find("/") + 1) + "-%02d.png", 0.1f, 1, StringUtils::format("magicEffect%d", i).c_str());
	}
}

GameScene::~GameScene()
{
	for (int i = 0; i < RESMAGICANIMATIONCOUNT; ++i)
	{
		AnimateManager::getInstance()->removeAnimation(StringUtils::format("magicEffect%d", i).c_str());
	}

	AudioPlayer->stopAllEffects();
	AudioPlayer->playBackgroundMusic("sound/instanceBGM1");
}

GameScene* GameScene::createWithProperty(int level)
{
	GameScene* scene = new(std::nothrow) GameScene();
	if (scene != nullptr && scene->iniWithProperty(level))
	{
		scene->autorelease();
	}
	else
	{
		delete scene;
		scene = nullptr;
	}
	return scene;
}

bool GameScene::iniWithProperty(int level)
{
	if (!Scene::init())
		return false;

	Size winsize = VisibleRect::getVisibleRect().size;

	DataInstance::getInstance()->_curLevel = level;

	// role manager 
	_roleManager = RoleManager::create();
	this->addChild(_roleManager, 0);

	// level map
	BaseMap* gameMap = BaseMap::createWithId(level);
	_roleManager->addChild(gameMap, 0, "Map");

	return true;
}

void GameScene::onEnter()
{
	Scene::onEnter();
	AudioPlayer->playBackgroundMusic("sound/battleEffect1.mp3", true);
}
