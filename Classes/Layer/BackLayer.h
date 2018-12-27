#ifndef __BACKLAYER_H__
#define __BACKLAYER_H__

#include "Common.h"
#include "ui/CocosGUI.h"

class BackLayer : public cocos2d::Layer
{
public:
	BackLayer();
	~BackLayer();

	virtual void onEnter();

	void flush(cocos2d::EventCustom*);
	void ButtonCallBack(cocos2d::Ref*,cocos2d::ui::Widget::TouchEventType);

	CREATE_FUNC(BackLayer);
	bool init();

	cocos2d::EventListenerCustom* _tradeListener;
};

#endif