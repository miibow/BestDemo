#ifndef __BATTLEREADY_H__
#define __BATTLEREADY_H__

#include "Common.h"
#include "ui/CocosGUI.h"

class BattleReady : public cocos2d::LayerColor
{
public:
	BattleReady();
	~BattleReady();

	virtual void onEnter();
	virtual void onExit();

	CREATE_FUNC(BattleReady);
	bool init();

private:
	void displayWhenEnter();

	void displayCandidateHero(cocos2d::EventCustom*);
	void displayBattleQueWindow(cocos2d::EventCustom*);

	void showAttackValue();

private:
	int _attackValue = 0;

	cocos2d::EventListenerCustom* _displayQueListener;
	cocos2d::EventListenerCustom* _displayCandidateListener;

	cocos2d::Vec2 _posArray[6];
};

#endif