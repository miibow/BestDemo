#ifndef __EQUIPUPGRADELAYER_H__
#define __EQUIPUPGRADELAYER_H__

#include "Common.h"
#include "ui/CocosGUI.h"

class EquipUpgradeLayer : public cocos2d::Layer
{
public:
	EquipUpgradeLayer();
	~EquipUpgradeLayer();

	virtual void onEnter() override;
	virtual void onExit() override;

	void flushEquipAttr(int equip);
	void upgradeEquip();

	void upgradeEquipCallBack(cocos2d::Ref*, cocos2d::ui::Widget::TouchEventType type);

	CREATE_FUNC(EquipUpgradeLayer);
	bool init();

private:
	int _equipId = -1;
	int _upgradeCost = 0;

private:
	cocos2d::Sprite* _glisten;

};

#endif