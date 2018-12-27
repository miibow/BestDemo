#include "AppDelegate.h"
#include "SimpleAudioEngine.h"
#include "LoadingScene.h"
#include "Data/DataInstance.h"

using namespace CocosDenshion;
USING_NS_CC;

static cocos2d::Size designResolutionSize = cocos2d::Size(480, 320);
static cocos2d::Size smallResolutionSize = cocos2d::Size(480, 320);
static cocos2d::Size mediumResolutionSize = cocos2d::Size(1024, 768);
static cocos2d::Size largeResolutionSize = cocos2d::Size(2048, 1536);

AppDelegate::AppDelegate() {
	auto data = DataInstance::getInstance();
	if (data->_isFirstPlay)
	{
		remove(UserDefault::getInstance()->getXMLFilePath().c_str());
		remove((FileUtils::getInstance()->getWritablePath() + userHeroData).c_str());
		remove((FileUtils::getInstance()->getWritablePath() + userEquipData).c_str());
		remove((FileUtils::getInstance()->getWritablePath() + userMagicData).c_str());
		remove((FileUtils::getInstance()->getWritablePath() + userItemData).c_str());
	}
}

AppDelegate::~AppDelegate() 
{
	
}

//if you want a different context,just modify the value of glContextAttrs
//it will takes effect on all platforms
void AppDelegate::initGLContextAttrs()
{
    //set OpenGL context attributions,now can only set six attributions:
    //red,green,blue,alpha,depth,stencil
    GLContextAttrs glContextAttrs = {8, 8, 8, 8, 24, 8};

    GLView::setGLContextAttrs(glContextAttrs);
}

// If you want to use packages manager to install more packages, 
// don't modify or remove this function
static int register_all_packages()
{
    return 0; //flag for packages manager
}

bool AppDelegate::applicationDidFinishLaunching() {
    // initialize director
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if(!glview) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) || (CC_TARGET_PLATFORM == CC_PLATFORM_MAC) || (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)
        glview = GLViewImpl::createWithRect("BattleCard", Rect(0, 0, designResolutionSize.width, designResolutionSize.height));
#else
        glview = GLViewImpl::create("BattleCard");
#endif
        director->setOpenGLView(glview);
    }

    // turn on display FPS
    director->setDisplayStats(false);

    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0f / 60);

#if _DEBUG
	glview->setFrameSize(1334, 750);
#endif

    // Set the design resolution
	glview->setDesignResolutionSize(1334, 750, ResolutionPolicy::SHOW_ALL);

    register_all_packages();

    // create a scene. it's an autorelease object
	auto scene = Scene::create();

	srand(time(0));
    // run
    director->runWithScene(LoadingScene::create());
	//director->runWithScene(scene);

    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();

    // if you use SimpleAudioEngine, it must be pause
     SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();

    // if you use SimpleAudioEngine, it must resume here
     SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}
