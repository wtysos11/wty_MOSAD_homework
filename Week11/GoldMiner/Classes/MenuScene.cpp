#include "MenuScene.h"
#include "SimpleAudioEngine.h"
#include "GameScene.h"

USING_NS_CC;

Scene* MenuScene::createScene()
{
    return MenuScene::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

// on "init" you need to initialize your instance
bool MenuScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Scene::init() )
    {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto bg_sky = Sprite::create("menu-background-sky.jpg");
	bg_sky->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y + 150));
	this->addChild(bg_sky, 0);

	auto bg = Sprite::create("menu-background.png");
	bg->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y - 60));
	this->addChild(bg, 0);

	auto miner = Sprite::create("menu-miner.png");
	miner->setPosition(Vec2(150 + origin.x, visibleSize.height / 2 + origin.y - 60));
	this->addChild(miner, 1);

	auto leg = Sprite::createWithSpriteFrameName("miner-leg-0.png");
	Animate* legAnimate = Animate::create(AnimationCache::getInstance()->getAnimation("legAnimation"));
	leg->runAction(RepeatForever::create(legAnimate));
	leg->setPosition(110 + origin.x, origin.y + 102);
	this->addChild(leg, 1);

	auto goldtext = Sprite::create("gold-miner-text.png");
	goldtext->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height - 100));
	this->addChild(goldtext, 2);

	auto gold = Sprite::create("menu-start-gold.png");
	gold->setPosition(Vec2(origin.x + visibleSize.width - 180, origin.y + 160));
	this->addChild(gold, 1);

	MenuItemImage *pChangeScene = MenuItemImage::create(
		"start-0.png",
		"start-1.png",
		this,
		menu_selector(MenuScene::startMenuCallback));

	pChangeScene->setPosition(Vec2(origin.x + visibleSize.width - 180,origin.y + 210));
	Menu* pMenu = Menu::create(pChangeScene, NULL);
	pMenu->setPosition(Vec2(origin.x,origin.y));
	this->addChild(pMenu, 2);

    return true;
}



void MenuScene::startMenuCallback(cocos2d::Ref* pSender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT) || (CC_TARGET_PLATFORM == CC_PLATFORM_WP8)  
	CCMessageBox("You pressed the close button. Windows Store Apps do not implement a close button.", "Alert");
#else  
	//CCTransitionMoveInL为左进入特效，0.4f为耗时，越少越快，可以为3.0f等，HelloWorld::scene()就是要切换到的场景  
	CCDirector::sharedDirector()->replaceScene(TransitionFade::create(3.0f, GameSence::create()));
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)  
	exit(0);
#endif  
#endif  
}