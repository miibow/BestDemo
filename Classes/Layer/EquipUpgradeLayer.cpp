#include "EquipUpgradeLayer.h"
#include "GameScene.h"
#include "Data/DataInstance.h"
#include "MyUI/HeroInUpgradeQue.h"
#include "MyUI/EquipIcon.h"
#include "Resource/Resource.h"

USING_NS_CC;
using namespace ui;

EquipUpgradeLayer::EquipUpgradeLayer()
{
}


EquipUpgradeLayer::~EquipUpgradeLayer()
{
}

bool EquipUpgradeLayer::init()
{
	if (!Layer::init())
		return false;

	Size winsize = VisibleRect::getVisibleRect().size;
	auto dataMap = FileUtils::getInstance()->getValueMapFromFile(langConfigPath);
	this->setContentSize(winsize);

	// bg
	Scale9Sprite* attrBg = Scale9Sprite::create(Rect(10, 40, 44, 66), "instance/img-02.png");
	attrBg->setContentSize(Size(winsize.width * 0.305f, winsize.height * 0.86f));
	attrBg->setPosition(Vec2(winsize.width * 0.7219f, winsize.height * 0.475f));
	this->addChild(attrBg);

	// name text
	Sprite* attrTextBg = Sprite::create();
	Label* attrText = Label::createWithTTF(dataMap["equipAttribute"].asString(), "fonts/HanYiChangLiSongKeBenJianTi-2.ttf", 28);
	attrText->setTextColor(Color4B(218, 184, 91, 255));
	attrText->enableShadow(Color4B::BLACK, Size(1, -1));
	attrText->enableOutline(Color4B(175, 100, 60, 255), 1);
	attrText->setPosition(attrTextBg->getContentSize().width * 0.5f, attrTextBg->getContentSize().height * 0.48f);
	attrTextBg->setPosition(winsize.width * 0.722f, winsize.height * 0.8713f);
	attrTextBg->addChild(attrText);
	this->addChild(attrTextBg);

	// add decorator for name
	ImageView* decoratorL = ImageView::create("instance/LD.png");
	decoratorL->setScaleX(0.9f);
	decoratorL->setPosition(Vec2(0.624f*winsize.width, 0.8713f * winsize.height));
	this->addChild(decoratorL);
	ImageView* decoratorR = ImageView::create("instance/LD.png");
	decoratorR->setScale(-0.9f);
	decoratorR->setPosition(Vec2(0.820f*winsize.width, 0.8713f * winsize.height));
	this->addChild(decoratorR);

	// upgrade button
	Button* button = Button::create("instance/button.png", "instance/button.png");
	button->setName("button");
	button->setPosition(Vec2(winsize.width * 0.7219f, winsize.height * 0.1174f));
	button->setScale(0.7f);
	button->addTouchEventListener(CC_CALLBACK_2(EquipUpgradeLayer::upgradeEquipCallBack, this));
	Label* buttonTitle = Label::createWithTTF(dataMap["upgrade"].asString(), "fonts/HanYiChangLiSongKeBenJianTi-2.ttf", 40);
	buttonTitle->setTextColor(Color4B(67, 125, 158, 255));
	buttonTitle->setPosition(button->getContentSize().width * 0.5f, button->getContentSize().height * 0.55f);
	button->addChild(buttonTitle);
	this->addChild(button);

	// add attribute title text
	Text* attLeft = Text::create(dataMap["curAttr"].asString(), "fonts/WeiRuanYaHei-1.ttf", 22);
	attLeft->setTextColor(Color4B(128, 82, 65, 255));
	attLeft->setPosition(Vec2(winsize.width * 0.635f, winsize.height * 0.5225f));
	this->addChild(attLeft);
	Text* attRight = static_cast<Text*>(attLeft->clone());
	attRight->setString(dataMap["nextAttr"].asString());
	attRight->setPosition(Vec2(winsize.width * 0.809f, winsize.height * 0.5225f));
	this->addChild(attRight);

	// add equip name
	Text* equipName = Text::create("", "fonts/WeiRuanYaHei-1.ttf", 25);
	equipName->setTextHorizontalAlignment(TextHAlignment::LEFT);
	equipName->setPosition(Vec2(winsize.width * 0.752f, winsize.height * 0.5855f));
	equipName->setTextColor(Color4B(183, 50, 12, 255));
	equipName->setName("equipName");
	this->addChild(equipName);

	// add equip level
	Text* equipLevel = Text::create("", "fonts/WeiRuanYaHei-1.ttf", 23);
	equipLevel->setPosition(Vec2(winsize.width * 0.655f, winsize.height * 0.5855f));
	equipLevel->setTextHorizontalAlignment(TextHAlignment::LEFT);
	equipLevel->setTextColor(Color4B(128, 82, 65, 255));
	equipLevel->setName("equipLevel");
	this->addChild(equipLevel);

	// add equip frame
	Sprite* equipFrame = Sprite::create("instance/equipFrame.png");
	equipFrame->setPosition(winsize.width * 0.72f, winsize.height * 0.725f);
	this->addChild(equipFrame);

	// add equip icon
	Sprite* equipIcon = Sprite::create();
	//equipIcon->setPosition(winsize.width * 0.72f, winsize.height * 0.725f);
	equipIcon->setScale(1.2f);
	equipIcon->setName("equipIcon");
	ClippingNode* clip = ClippingNode::create();
	clip->setName("clip");
	clip->setAlphaThreshold(0.5f);
	clip->setContentSize(equipIcon->getContentSize());
	clip->setPosition(Vec2(winsize.width * 0.72f, winsize.height * 0.725f));
	clip->setStencil(equipIcon);
	equipIcon->setPosition(clip->getContentSize() / 2);
	clip->addChild(equipIcon);
	_glisten = Sprite::create("instance/glisten.png");
	_glisten->setScaleX(2);
	_glisten->setPosition(Vec2(-_glisten->getContentSize().width, 0));
	_glisten->retain();
	clip->addChild(_glisten);
	this->addChild(clip);

	// add ingot
	Sprite* ingot = Sprite::create("instance/coin.png");
	ingot->setPosition(winsize.width * 0.70f, winsize.height * 0.18f);
	this->addChild(ingot);
	Label* ingotNum = Label::createWithTTF("0", "fonts/BenMoJingYuan-2.ttf", 23);
	ingotNum->setTextColor(Color4B(112, 82, 65, 255));
	ingotNum->setPosition(winsize.width * 0.74f, winsize.height * 0.177f);
	ingotNum->setName("upgradeCost");
	this->addChild(ingotNum);

	// add green arrow
	Sprite* greenArrow = Sprite::create("instance/greenArrow.png");
	greenArrow->setPosition(winsize.width * 0.72f, winsize.height * 0.365f);
	this->addChild(greenArrow);

	// add attribute context
	for (int i = 0; i < MAXATTRNUM; ++i)
	{
		float weightY = 0.4755f - i * 0.045f;
		Label* text = Label::createWithTTF(dataMap[StringUtils::format("attr%d", i).c_str()].asString(), "fonts/WeiRuanYaHei-1.ttf", 20);
		text->setTextColor(Color4B(132, 102, 85, 255));
		text->setPosition(winsize.width * 0.6038f, winsize.height * weightY);
		this->addChild(text);

		Label* value = Label::createWithTTF("0", "fonts/WeiRuanYaHei-1.ttf", 20);
		value->setTextColor(Color4B(168, 83, 63, 255));
		value->setPosition(winsize.width * 0.6661f, winsize.height * (weightY - 0.003f));
		value->setName(StringUtils::format("curAttr%d", i));
		this->addChild(value);

		Label* text2 = Label::createWithTTF(dataMap[StringUtils::format("attr%d", i).c_str()].asString(), "fonts/WeiRuanYaHei-1.ttf", 20);
		text2->setTextColor(Color4B(132, 102, 85, 255));
		text2->setPosition(winsize.width * 0.7738f, winsize.height * weightY);
		this->addChild(text2);

		Label* value2 = Label::createWithTTF("0", "fonts/WeiRuanYaHei-1.ttf", 20);
		value2->setPosition(winsize.width * 0.8361f, winsize.height * (weightY - 0.003f));
		value2->setTextColor(Color4B(63, 168, 88, 255));
		value2->setName(StringUtils::format("nextAttr%d", i));
		this->addChild(value2);
	}

	return true;
}

void EquipUpgradeLayer::onEnter()
{
	Layer::onEnter();
}

void EquipUpgradeLayer::upgradeEquipCallBack(cocos2d::Ref* pSender, cocos2d::ui::Widget::TouchEventType type)
{
	auto sender = static_cast<Button*>(pSender);
	switch (type)
	{
	case cocos2d::ui::Widget::TouchEventType::BEGAN:
		AudioManager::clickEffect();
		sender->setScale(0.68f);
		break;
	case cocos2d::ui::Widget::TouchEventType::MOVED:
		break;
	case cocos2d::ui::Widget::TouchEventType::ENDED:
		sender->setScale(0.7f);
		if (DataInstance::getInstance()->_ownedEquipList.at(_equipId) >= MAXEQUIPLEVEL)
		{
			AnimateManager::getInstance()->insufficient(levelObject);
		}
		else if (DataInstance::getInstance()->alterCurrency(0, this->_upgradeCost))
		{
			AudioPlayer->playEffect("sound/swordUpgrade0.mp3");
			upgradeEquip();
		}
		else {
			AnimateManager::getInstance()->insufficient(currencyObject);
		}
		break;
	case cocos2d::ui::Widget::TouchEventType::CANCELED:
		sender->setScale(0.7f);
		break;
	}
}

void EquipUpgradeLayer::upgradeEquip()
{
	// flow light
	auto index = MIN(_equipId, DataInstance::getInstance()->_ownedEquipList.size() - 1);
	DataInstance::getInstance()->upgradeEquip(index);

	_glisten->setPosition(Vec2(-_glisten->getContentSize().width, 0));
	CallFunc* flush = CallFunc::create([this, index]()
	{
		flushEquipAttr(index);
	});
	_glisten->runAction(Sequence::create(MoveTo::create(1.2f, Vec2(_glisten->getContentSize().width, 0)), flush, nullptr));
}

void EquipUpgradeLayer::flushEquipAttr(int equipId)
{
	_equipId = equipId;
	if (_equipId < 0 || _equipId + 1 > DataInstance::getInstance()->_equipModel.size())
		return;

	auto tempEquipConfig = DataInstance::getInstance()->_equipModel.at(_equipId);

	{
		auto clip = static_cast<Sprite*>(this->getChildByName("clip"));
		static_cast<Sprite*>(clip->getChildByName("equipIcon"))->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(StringUtils::format("equipIcon%02d.png", _equipId)));
		MoveBy* floating = MoveBy::create(1.0f, Vec2(0, -3.0f));
		if (clip->getNumberOfRunningActions() == 0)
		{
			clip->setPosition(VisibleRect::getVisibleRect().size.width * 0.72f, VisibleRect::getVisibleRect().size.height * 0.725f);
			clip->runAction(RepeatForever::create(Sequence::create(floating, floating->reverse(), nullptr)));
		}
	}
	static_cast<Text*>(this->getChildByName("equipName"))->setString(tempEquipConfig._name);
	static_cast<Text*>(this->getChildByName("equipLevel"))->setString(StringUtils::format("Lv.%d", tempEquipConfig._level));
	static_cast<Label*>(this->getChildByName("upgradeCost"))->setString(StringUtils::toString(tempEquipConfig._cost));
	_upgradeCost = tempEquipConfig._cost;

	// current attribute
	if (tempEquipConfig._Attack > 0)
	{
		static_cast<Label*>(this->getChildByName("curAttr0"))->setString(StringUtils::toString(tempEquipConfig._Attack));
		static_cast<Label*>(this->getChildByName("curAttr1"))->setString(StringUtils::toString(tempEquipConfig._Attack));
	}
	else
	{
		static_cast<Label*>(this->getChildByName("curAttr0"))->setString("--");
		static_cast<Label*>(this->getChildByName("curAttr1"))->setString("--");
	}
	if(tempEquipConfig._physicsDefence)
		static_cast<Label*>(this->getChildByName("curAttr2"))->setString(StringUtils::toString(tempEquipConfig._physicsDefence));
	else
		static_cast<Label*>(this->getChildByName("curAttr2"))->setString("--");
	if (tempEquipConfig._magicDefence)
		static_cast<Label*>(this->getChildByName("curAttr3"))->setString(StringUtils::toString(tempEquipConfig._magicDefence));
	else
		static_cast<Label*>(this->getChildByName("curAttr3"))->setString("--");
	if(tempEquipConfig._health)
		static_cast<Label*>(this->getChildByName("curAttr4"))->setString(StringUtils::toString(tempEquipConfig._health));
	else
		static_cast<Label*>(this->getChildByName("curAttr4"))->setString("--");
	if (tempEquipConfig._criticalHitRate)
		static_cast<Label*>(this->getChildByName("curAttr5"))->setString(StringUtils::toString(tempEquipConfig._criticalHitRate));
	else
		static_cast<Label*>(this->getChildByName("curAttr5"))->setString("--");

	// next attribute
	int level = MIN(tempEquipConfig._level + 1, MAXEQUIPLEVEL);
	DataInstance::getInstance()->equipConfigFromJson(_equipId, level, tempEquipConfig, equipConfigPath);
	if (tempEquipConfig._Attack > 0)
	{
		static_cast<Label*>(this->getChildByName("nextAttr0"))->setString(StringUtils::toString(tempEquipConfig._Attack));
		static_cast<Label*>(this->getChildByName("nextAttr1"))->setString(StringUtils::toString(tempEquipConfig._Attack));
	}
	else
	{
		static_cast<Label*>(this->getChildByName("nextAttr0"))->setString("--");
		static_cast<Label*>(this->getChildByName("nextAttr1"))->setString("--");
	}
	if(tempEquipConfig._physicsDefence)
		static_cast<Label*>(this->getChildByName("nextAttr2"))->setString(StringUtils::toString(tempEquipConfig._physicsDefence));
	else
		static_cast<Label*>(this->getChildByName("nextAttr2"))->setString("--");
	if (tempEquipConfig._magicDefence)
		static_cast<Label*>(this->getChildByName("nextAttr3"))->setString(StringUtils::toString(tempEquipConfig._magicDefence));
	else
		static_cast<Label*>(this->getChildByName("nextAttr3"))->setString("--");
	if(tempEquipConfig._health)
		static_cast<Label*>(this->getChildByName("nextAttr4"))->setString(StringUtils::toString(tempEquipConfig._health));
	else
		static_cast<Label*>(this->getChildByName("nextAttr4"))->setString("--");
	if (tempEquipConfig._criticalHitRate)
		static_cast<Label*>(this->getChildByName("nextAttr5"))->setString(StringUtils::toString(tempEquipConfig._criticalHitRate));
	else
		static_cast<Label*>(this->getChildByName("nextAttr5"))->setString("--");
}

void EquipUpgradeLayer::onExit()
{

	Layer::onExit();
}