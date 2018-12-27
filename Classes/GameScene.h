#ifndef __GAMESCENE_H__
#define __GAMESCENE_H__

#include "Common.h"
#include "Manager/RoleManager.h"

class GameScene : public cocos2d::Scene
{
public:
	GameScene();
	~GameScene();

	static GameScene* createWithProperty(int level = 0);
	bool iniWithProperty(int level);

	virtual void onEnter() override;

private:
	RoleManager * _roleManager;

};

#endif