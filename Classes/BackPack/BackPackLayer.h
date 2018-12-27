#ifndef __BACKPACKLAYER_H__
#define __BACKPACKLAYER_H__

#include "Common.h"
#include "ui/CocosGUI.h"
#include "ui/UIWidget.h"
#include "Data/DataInstance.h"
#include "Resource/Resource.h"

USING_NS_CC;
using namespace ui;

class BackPackLayer : public cocos2d::LayerColor
{
public:
	BackPackLayer();
	~BackPackLayer();
	static cocos2d::Scene* createScene();

	virtual void onEnter();

	void ButtonCallBack(cocos2d::Ref*,cocos2d::ui::Widget::TouchEventType);

	CREATE_FUNC(BackPackLayer);
	bool init();

};

#endif