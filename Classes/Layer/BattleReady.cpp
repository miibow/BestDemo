#include "BattleReady.h"
#include "GameScene.h"
#include "Data/DataInstance.h"
#include "MyUI/HeroCandidate.h"
#include "MyUI/HeroInBattleQue.h"
#include "Layer/BackLayer.h"
#include "Layer/GuideLayer.h"
#include "Resource/Resource.h"

USING_NS_CC;
using namespace ui;

BattleReady::BattleReady() :
	_displayQueListener(nullptr),
	_displayCandidateListener(nullptr)
{
}


BattleReady::~BattleReady()
{
}

bool BattleReady::init()
{
	if (!LayerColor::init())
		return false;

	this->setContentSize(VisibleRect::getVisibleRect().size);
	this->setColor(Color3B::BLACK);
	this->setOpacity(128);
	this->addChild(BackLayer::create());

	if (DataInstance::getInstance()->_isFirstPlay)
		this->addChild(GuideLayer::getInstance());

	return true;
}

void BattleReady::onEnter()
{
	LayerColor::onEnter();

	Size winsize = VisibleRect::getVisibleRect().size;
	auto dataMap = FileUtils::getInstance()->getValueMapFromFile(langConfigPath);

	// decoration on back ground
	Scale9Sprite* bg = Scale9Sprite::create("instance/protrait-frame.png");
	bg->setCapInsets(Rect(20, 20, 386, 592));
	bg->setContentSize(Size(0.771f * winsize.width, 0.924f * winsize.height));
	bg->setPosition(winsize.width * 0.4979f, winsize.height * 0.491f);
	this->addChild(bg);
	// border on back ground
	ImageView* border = ImageView::create("instance/img-05.png");
	border->setPosition(winsize / 2);
	border->setScale9Enabled(true);
	border->setContentSize(Size(0.821f * winsize.width, 0.978f * winsize.height));
	this->addChild(border);
	//Sprite* battleText = Sprite::create("instance/battleReady.png");
	//battleText->setPosition(border->getContentSize().width / 2, border->getContentSize().height * 0.94f);
	//border->addChild(battleText);
	Label* TopTitle = Label::create(dataMap["title2"].asString(), "fonts/WeiRuanYaHei-1.ttf", 30);
	TopTitle->setPosition(Vec2(winsize.width * 0.5f, winsize.height * 0.935f));
	TopTitle->setTextColor(Color4B(155, 89, 46, 255));
	TopTitle->enableOutline(Color4B(253, 247, 225, 255), 1);
	this->addChild(TopTitle);

	// add listView bg
	Sprite* lbg = Sprite::create("instance/img-02.png");
	lbg->setScaleX(0.9f);
	lbg->setPosition(Vec2(winsize.width * 0.72f, winsize.height * 0.48f));
	this->addChild(lbg);

	// Create the list view ex
	ListView* listView = ListView::create();
	// set list view ex direction
	listView->setDirection(ui::ScrollView::Direction::VERTICAL);
	listView->setBounceEnabled(true);
	listView->setScrollBarOpacity(0);
	listView->setContentSize(Size(0.26f * winsize.width, 0.80f * winsize.height));
	listView->setPosition(Vec2(winsize.width * 0.60f, winsize.height * 0.08f));
	listView->setScrollBarPositionFromCorner(Vec2(2, 1));
	listView->setScrollBarColor(Color3B(179, 120, 79));
	listView->setName("listView");
	this->addChild(listView);

	// create model
	Layout* default_item = Layout::create();
	default_item->setContentSize(Size(0.15f * winsize.width, 0.2f * winsize.height));

	// set model
	listView->setItemModel(default_item);
	listView->setGravity(ListView::Gravity::CENTER_VERTICAL);

	auto heroList = DataInstance::getInstance()->_ownedHeroList;
	auto it = heroList.begin();
	for (int i = 0; i < (heroList.size() + 3) / 3; ++i)
	{
		Widget* item = default_item->clone();
		int NumsPerLine = 0;
		for (; it != heroList.end() && NumsPerLine < 3; ++it, ++NumsPerLine)
		{
			HeroCandidate* hero = HeroCandidate::createWithId(it->first);
			hero->setPosition(item->getContentSize().width / 2 - hero->getContentSize().width * 0.4f + hero->getContentSize().width * 1.1f * NumsPerLine,
				item->getContentSize().height / 2);
			hero->setTag(it->first);
			item->addChild(hero);
		}

		listView->pushBackCustomItem(item);
		item->setName(StringUtils::format("item%d", i));
	}

	/* add hero icon in queue */
	// back ground image
	auto spBgi = Scale9Sprite::create("instance/img-03.png");
	spBgi->setCapInsets(Rect(50, 50, 4, 4));
	spBgi->setContentSize(Size(winsize.width * 0.4223f, winsize.height * 0.824f));
	spBgi->setPosition(winsize.width * 0.3570f, winsize.height* 0.483f);
	this->addChild(spBgi);

	// add attackValue bar
	Sprite* attackBg = Sprite::create("instance/attackValue.png");
	attackBg->setPosition(winsize.width * 0.3570f, winsize.height * 0.7986f);
	this->addChild(attackBg);
	Label* valueText = Label::createWithTTF(dataMap["attack"].asString(), "fonts/WeiRuanYaHei-1.ttf", 30);
	valueText->setTextColor(Color4B(231, 157, 101, 255));
	valueText->enableOutline(Color4B(254, 254, 223, 255), 1);
	valueText->setPosition(winsize.width * 0.3572f, winsize.height * 0.818f);
	this->addChild(valueText);
	Label* valueNum = Label::createWithCharMap("img/number4.png", 27, 32, '0');
	valueNum->setPosition(winsize.width * 0.3573f, winsize.height * 0.7509f);
	valueNum->setName("attackValue");
	this->addChild(valueNum);

	// battle queue 
	for (int i = 0; i < MAXHERONUMS; ++i)
	{

		Sprite* bg = Sprite::create("instance/img-06.png");
		Sprite* plus = Sprite::create("instance/plus.png");
		plus->setPosition(bg->getContentSize().width* 0.7f, bg->getContentSize().height * 0.5f);
		float x = i % 2 ? winsize.width * 0.455f : winsize.width * 0.255f;
		float y = winsize.height * (0.55f - 0.185f * (i / 2));
		_posArray[i] = Vec2(x, y);
		bg->setPosition(x, y);
		bg->addChild(plus);
		this->addChild(bg);
		if (DataInstance::getInstance()->isHeroInQue(DataInstance::getInstance()->_curBattleRoleQue.at(i)))
		{
			HeroInBattleQue* battleHero = HeroInBattleQue::create();
			battleHero->setPosition(x, y);
			this->addChild(battleHero, 1, i + 1000);
		}
	}

	// add listener
	_displayQueListener = EventListenerCustom::create("heroBattle", CC_CALLBACK_1(BattleReady::displayBattleQueWindow, this));
	_displayCandidateListener = EventListenerCustom::create("heroCandidate", CC_CALLBACK_1(BattleReady::displayCandidateHero, this));
	_eventDispatcher->addEventListenerWithFixedPriority(_displayQueListener, 1);
	_eventDispatcher->addEventListenerWithFixedPriority(_displayCandidateListener, 1);

	// display
	displayWhenEnter();
}

void BattleReady::displayWhenEnter()
{
	for (int i = 0; i < MAXHERONUMS; ++i)
	{
		auto tag = DataInstance::getInstance()->_curBattleRoleQue.at(i);
		if (tag == -1)
			continue;
		static_cast<HeroInBattleQue*>(this->getChildByTag(1000 + i))->setInQueState(DataInstance::getInstance()->_curBattleRoleQue.at(i));

		auto hero = static_cast<HeroCandidate*>(this->getChildByName("listView")->getChildByName(StringUtils::format("item%d", tag / 3))->getChildByTag(tag));
		hero->setGrayState();
	}
	showAttackValue();
}

void BattleReady::displayCandidateHero(cocos2d::EventCustom* event)
{
	auto tag = *static_cast<int*>(event->getUserData());
	auto hero = static_cast<HeroCandidate*>(this->getChildByName("listView")->getChildByName(StringUtils::format("item%d", tag / 3))->getChildByTag(tag));
	hero->setNormalState();
}

void BattleReady::displayBattleQueWindow(cocos2d::EventCustom* event)
{

	int* buff = static_cast<int*>(event->getUserData());
	Vec2 pos = _posArray[buff[0]];
	if (buff[1])
	{
		HeroInBattleQue* heroIcon = HeroInBattleQue::create();
		heroIcon->setOpacity(0);
		heroIcon->setScale(0.1f);
		heroIcon->setPosition(buff[2], buff[3]);
		FadeIn* fadeIn = FadeIn::create(0.12f);
		MoveTo* move = MoveTo::create(0.18f, _posArray[buff[0]]);
		ScaleTo* scale = ScaleTo::create(0.15f, 1.0f);
		heroIcon->runAction(Spawn::create(fadeIn, move, scale, nullptr));
		heroIcon->setInQueState(DataInstance::getInstance()->_curBattleRoleQue.at(buff[0]));
		this->addChild(heroIcon);
		for (int i = MAXHERONUMS - 1; i > buff[0]; --i)
		{
			if (DataInstance::getInstance()->_curBattleRoleQue.at(i) == -1)
				continue;
			this->getChildByTag(1000 + i - 1)->setTag(1000 + i);
		}
		heroIcon->setTag(1000 + buff[0]);
	}
	else {
		this->getChildByTag(1000 + buff[0])->removeFromParent();
		for (int i = buff[0]; i < MAXHERONUMS; ++i)
		{
			if (DataInstance::getInstance()->_curBattleRoleQue.at(i) == -1)
				break;
			this->getChildByTag(1000 + i + 1)->setTag(1000 + i);
		}
	}

	// add: index plus 1 , remove: index
	for (int i = buff[1] ? buff[0] + 1 : buff[0]; i < MAXHERONUMS; ++i)
	{
		if (DataInstance::getInstance()->_curBattleRoleQue.at(i) == -1)
			continue;
		static_cast<HeroInBattleQue*>(this->getChildByTag(1000 + i))->runAction(MoveTo::create(0.2f, _posArray[i]));
	}

	showAttackValue();

	scheduleOnce([this](float)
	{
		for (int i = 0; i < MAXHERONUMS; ++i)
		{
			auto a = DataInstance::getInstance()->_curBattleRoleQue;
			auto tag = DataInstance::getInstance()->_curBattleRoleQue.at(i);
			if (tag == -1)
				continue;
			static_cast<HeroInBattleQue*>(this->getChildByTag(1000 + i))->setPosition(_posArray[i]);
		}
	}, 0.2f, "flushBattleQue");
}

void BattleReady::showAttackValue()
{
	float curAttack = _attackValue;
	_attackValue = 0;
	for (int i = 0; i < DataInstance::getInstance()->getHeroNumInCurBattleQue(); ++i)
	{
		//if (DataInstance::getInstance()->_curBattleRoleQue.at(i) == -1)
			//continue;
		_attackValue += DataInstance::getInstance()->_heroModel.at(DataInstance::getInstance()->_curBattleRoleQue.at(i))._Attack;
	}

	auto Floating = ActionFloat::create(0.5f, curAttack, _attackValue,
		[this](float dt) {static_cast<Label*>(this->getChildByName("attackValue"))->setString(StringUtils::format("%d", int(dt))); }
	);

	this->runAction(Floating);
}

void BattleReady::onExit()
{
	_eventDispatcher->removeEventListener(_displayQueListener);
	_eventDispatcher->removeEventListener(_displayCandidateListener);

	LayerColor::onExit();
}
