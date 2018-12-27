#include "BackPackLayer.h"
#include "Layer/BackLayer.h"
#include "Manager/AnimateManager.h"
#include "Resource/Resource.h"

BackPackLayer::BackPackLayer()
{
}

BackPackLayer::~BackPackLayer()
{
	DataInstance::getInstance()->_allShopItemList.clear();
	DataInstance::getInstance()->_allShopItemList.shrink_to_fit();
}

Scene* BackPackLayer::createScene()
{
	auto scene = Scene::create();
	Layer* select = BackPackLayer::create();
	Sprite* bg = Sprite::create("img/BG.jpg");
	bg->setPosition(VisibleRect::getVisibleRect().size / 2);
	scene->addChild(bg);
	scene->addChild(select);
	return scene;
}

bool BackPackLayer::init()
{
	if (!LayerColor::init())
		return false;

	this->setContentSize(VisibleRect::getVisibleRect().size);
	this->setColor(Color3B::BLACK);
	this->setOpacity(128);
	this->addChild(BackLayer::create());
	DataInstance::getInstance()->fragmentItemlistInit();

	return true;
}

void BackPackLayer::onEnter()
{
	LayerColor::onEnter();

	Size winsize = VisibleRect::getVisibleRect().size;
	auto dataMap = FileUtils::getInstance()->getValueMapFromFile("utf8.txt");
	DataInstance::getInstance()->getChapterInfo();

	// decoration on back ground
	Scale9Sprite* bg = Scale9Sprite::create("instance/protrait-frame.png");
	bg->setCapInsets(Rect(20, 20, 386, 592));
	bg->setContentSize(Size(0.771f * winsize.width, 0.924f * winsize.height));
	bg->setPosition(winsize.width * 0.4979f, winsize.height * 0.491f);
	this->addChild(bg);

	// decoration on back ground
	ImageView* border = ImageView::create("instance/img-05.png");
	border->setPosition(Vec2(winsize.width * 0.5f, winsize.height * 0.5f));
	border->setScale9Enabled(true);
	border->setContentSize(Size(0.821f * winsize.width, 0.978f * winsize.height));
	this->addChild(border);

	// add title
	Text* exchange = Text::create(dataMap["exchange"].asString(), "fonts/BenMoJingYuan-2.ttf", 30);
	exchange->setTextColor(Color4B(154, 109, 83, 255));
	exchange->enableOutline(Color4B::WHITE);
	exchange->setPosition(Vec2(border->getContentSize().width / 2, border->getContentSize().height * 0.94f));
	border->addChild(exchange);

	// add list view
	ListView* list = ListView::create();
	list->setContentSize(Size(winsize.width * 0.74f, winsize.height * 0.82f));
	list->setPosition(Vec2(winsize.width * 0.1271f, winsize.height * 0.0675f));
	list->setBounceEnabled(true);
	list->setScrollBarOpacity(0);
	this->addChild(list);

	// model item
	Layout* rowModel = Layout::create();
	rowModel->setContentSize(Size(list->getContentSize().width, list->getContentSize().height * 0.25f));

	// add layout
	for (int i = 0; i < (DataInstance::getInstance()->_allShopItemList.size() + 3) / 4; ++i)
	{
		Layout* row = (Layout*)rowModel->clone();
		for (int j = i * 4; j < DataInstance::getInstance()->_allShopItemList.size() && j < (i + 1) * 4; ++j)
		{

		}
		list->addChild(row);
	}
}

void BackPackLayer::ButtonCallBack(Ref* pSender, ui::Widget::TouchEventType type)
{
	// here are audio
	auto button = static_cast<Button*>(pSender);
	switch (type)
	{
	case Widget::TouchEventType::BEGAN:
		AudioManager::clickEffect();
		button->setScale(0.56f);
		break;
	case Widget::TouchEventType::ENDED:
		button->setScale(0.58f);
		if (!DataInstance::getInstance()->alterMaterial(static_cast<shopItem*>(button->getParent())->m_iItemId, static_cast<shopItem*>(button->getParent())->m_iItemCost, STHMINUS))
			AnimateManager::getInstance()->insufficient(exchangeFail);
		else
		{
			auto id = static_cast<shopItem*>(button->getParent())->m_iItemId - 1001;
			auto itemConfig = DataInstance::getInstance()->_allShopItemList.at(id);
			auto ownItemList = DataInstance::getInstance()->_ownedItemList;
			static_cast<shopItem*>(button->getParent())->itemCost->setString(StringUtils::format("%2d/%2d", ownItemList.count(id) ? ownItemList.at(id) : 0, itemConfig.m_iCostNum));

			AnimateManager::getInstance()->insufficient(exchangeObject);
			DataInstance::getInstance()->getNewHero(id, 0);
		}
		break;
	case Widget::TouchEventType::CANCELED:
		button->setScale(0.58f);
		break;
	}
}