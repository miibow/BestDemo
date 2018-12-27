#include "LifeBar.h"

USING_NS_CC;

LifeBar::LifeBar() :
	_lifebar(nullptr)
{
}


LifeBar::~LifeBar()
{
}

LifeBar* LifeBar::createWithType(int type)
{
	LifeBar* scene = new(std::nothrow) LifeBar();
	if (scene != nullptr && scene->initWithType(type))
	{
		scene->autorelease();
	}
	else {
		delete scene;
		scene = nullptr;
	}
	return scene;
}

bool LifeBar::initWithType(int type)
{
	if (!Sprite::initWithFile("lifebar/lifebg.png"))
		return false;
	
	_lifebar = ProgressTimer::create(Sprite::create(StringUtils::format("lifebar/life-%02d.png", type)));
	_lifebar->setType(ProgressTimer::Type::BAR);
	_lifebar->setMidpoint(Vec2(1, 0));
	_lifebar->setBarChangeRate(Vec2(1, 0));
	_lifebar->setPercentage(100);
	
	_lifebar->setPosition(this->getContentSize() / 2);
	this->addChild(_lifebar);

	return true;
}

void LifeBar::setPer(float percentage)
{
	//_lifebar->setPercentage(percentage);
	ProgressTo* pt = ProgressTo::create(0.08f, percentage);
	_lifebar->runAction(pt);
}