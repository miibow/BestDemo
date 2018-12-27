#include "BackLayer.h"
#include "ui/UIWidget.h"
#include "Data/DataInstance.h"

USING_NS_CC;
using namespace ui;

BackLayer::BackLayer()
{
}

BackLayer::~BackLayer()
{
	_eventDispatcher->removeEventListener(_tradeListener);
}

bool BackLayer::init()
{
	if (!Layer::init())
		return false;

	Size winsize = VisibleRect::getVisibleRect().size;
	this->setContentSize(winsize);
	this->setLocalZOrder(2);

	// decoration on back ground
	Button* backBtn = Button::create("instance/backBtn.png", "instance/backBtn.png");
	backBtn->addTouchEventListener(CC_CALLBACK_2(BackLayer::ButtonCallBack, this));
	backBtn->setPosition(Vec2(winsize.width * 0.8749f, winsize.height * 0.9389f));
	this->addChild(backBtn);

	// left frame
	ImageView* leftFrame = ImageView::create("HeartBeat/img-01.png");
	leftFrame->setScale9Enabled(true);
	leftFrame->setContentSize(Size(winsize.width * 0.1499f, winsize.height * 0.04f));
	leftFrame->setPosition(Vec2(winsize.width * 0.2514f, winsize.height * 0.9292f));
	this->addChild(leftFrame);
	Sprite* coinIcon = Sprite::create("instance/coin.png");
	coinIcon->setScale(0.8f);
	coinIcon->setPosition(winsize.width * 0.1998f, winsize.height * 0.9289f);
	this->addChild(coinIcon);
	Label* coinNum = Label::createWithTTF("0", "fonts/BenMoJingYuan-2.ttf", 25);
	coinNum->setPosition(winsize.width * 0.2759f, winsize.height * 0.9270f);
	coinNum->setName("coin");
	this->addChild(coinNum);

	// right frame
	ImageView* rightFrame = static_cast<ImageView*>(leftFrame->clone());
	rightFrame->setPosition(Vec2(winsize.width * 0.7498f, winsize.height * 0.9292f));
	this->addChild(rightFrame);
	Sprite* ingotIcon = Sprite::create("instance/ingot.png");
	ingotIcon->setScale(0.8f);
	ingotIcon->setPosition(winsize.width * 0.6991f, winsize.height * 0.9298f);
	this->addChild(ingotIcon);
	Label* ingotnum = Label::createWithTTF("0", "fonts/BenMoJingYuan-2.ttf", 25);
	ingotnum->setPosition(winsize.width * 0.7732f, winsize.height * 0.9270f);
	ingotnum->setName("ingot");
	this->addChild(ingotnum);

	_tradeListener = EventListenerCustom::create("trade", CC_CALLBACK_1(BackLayer::flush, this));
	_eventDispatcher->addEventListenerWithFixedPriority(_tradeListener, 1);

	return true;
}

void BackLayer::onEnter()
{
	Layer::onEnter();

	flush(nullptr);
}

void BackLayer::flush(cocos2d::EventCustom*)
{
	// currency
	auto coin = static_cast<Label*>(this->getChildByName("coin"));
	coin->setString(StringUtils::toString(DataInstance::getInstance()->_currency[0]));
	auto ingot = static_cast<Label*>(this->getChildByName("ingot"));
	ingot->setString(StringUtils::toString(DataInstance::getInstance()->_currency[1]));
}

void BackLayer::ButtonCallBack(Ref* pSender, ui::Widget::TouchEventType type)
{
	// here are audio
	auto button = static_cast<Button*>(pSender);
	switch (type)
	{
	case Widget::TouchEventType::BEGAN:
		AudioManager::clickEffect();
		button->setScale(0.98f);
		break;
	case Widget::TouchEventType::ENDED:
		button->setScale(1.0f);
		Director::getInstance()->getInstance()->popScene();
		break;
	case Widget::TouchEventType::CANCELED:
		button->setScale(1.0f);
		break;
	}
}