#ifndef __LIFEBAR_H__
#define __LIFEBAR_H__

#include "Common.h"
//#include "ui/UILoadingBar.h"

class LifeBar : public cocos2d::Sprite
{
public:
	LifeBar();
	~LifeBar();

	static LifeBar*createWithType(int type);
	bool initWithType(int type);

	void setPer(float);

private:
	cocos2d::ProgressTimer* _lifebar;

};


#endif