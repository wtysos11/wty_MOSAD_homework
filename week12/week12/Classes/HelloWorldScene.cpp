#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"
#include <cstdio>
#include <string>
#pragma execution_character_set("utf-8")

USING_NS_CC;

Scene* HelloWorld::createScene()
{
    return HelloWorld::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Scene::init() )
    {
        return false;
    }

    visibleSize = Director::getInstance()->getVisibleSize();
    origin = Director::getInstance()->getVisibleOrigin();

	//创建一张贴图
	auto texture = Director::getInstance()->getTextureCache()->addImage("$lucia_2.png");
	//从贴图中以像素单位切割，创建关键帧
	auto frame0 = SpriteFrame::createWithTexture(texture, CC_RECT_PIXELS_TO_POINTS(Rect(0, 0, 113, 113)));
	//使用第一帧创建精灵
	player = Sprite::createWithSpriteFrame(frame0);
	player->setPosition(Vec2(origin.x + visibleSize.width / 2,
		origin.y + visibleSize.height / 2));
	addChild(player, 3);

	//hp条
	Sprite* sp0 = Sprite::create("hp.png", CC_RECT_PIXELS_TO_POINTS(Rect(0, 320, 420, 47)));
	Sprite* sp = Sprite::create("hp.png", CC_RECT_PIXELS_TO_POINTS(Rect(610, 362, 4, 16)));

	//使用hp条设置progressBar
	pT = ProgressTimer::create(sp);
	pT->setScaleX(90);
	pT->setAnchorPoint(Vec2(0, 0));
	pT->setType(ProgressTimerType::BAR);
	pT->setBarChangeRate(Point(1, 0));
	pT->setMidpoint(Point(0, 1));
	pT->setPercentage(100);
	pT->setPosition(Vec2(origin.x + 14 * pT->getContentSize().width, origin.y + visibleSize.height - 2 * pT->getContentSize().height));
	addChild(pT, 1);
	sp0->setAnchorPoint(Vec2(0, 0));
	sp0->setPosition(Vec2(origin.x + pT->getContentSize().width, origin.y + visibleSize.height - sp0->getContentSize().height));
	addChild(sp0, 0);

	// 静态动画
	idle.reserve(1);
	idle.pushBack(frame0);

	// 攻击动画
	attack.reserve(18);
	for (int i = 0; i < 17; i++) {
		auto frame = SpriteFrame::createWithTexture(texture, CC_RECT_PIXELS_TO_POINTS(Rect(113 * i, 0, 113, 113)));
		attack.pushBack(frame);
	}
	attack.pushBack(frame0);

	// 可以仿照攻击动画
	// 死亡动画(帧数：22帧，高：90，宽：79）
	auto texture2 = Director::getInstance()->getTextureCache()->addImage("$lucia_dead.png");

	dead.reserve(23);
	for (int i = 0; i < 22; i++)
	{
		auto frame = SpriteFrame::createWithTexture(texture2, CC_RECT_PIXELS_TO_POINTS(Rect(79 * i, 0, 79, 90)));
		dead.pushBack(frame);
	}
	dead.pushBack(frame0);
	// 运动动画(帧数：8帧，高：101，宽：68）
	auto texture3 = Director::getInstance()->getTextureCache()->addImage("$lucia_forward.png");
	
	run.reserve(8);
	for (int i = 3; i < 8; i++)
	{
		auto frame = SpriteFrame::createWithTexture(texture3, CC_RECT_PIXELS_TO_POINTS(Rect(68 * i, 0, 68, 101)));
		run.pushBack(frame);
	}
	for(int i = 0;i<3;i++)
	{
		auto frame = SpriteFrame::createWithTexture(texture3, CC_RECT_PIXELS_TO_POINTS(Rect(68 * i, 0, 68, 101)));
		run.pushBack(frame);
	}
	//创建菜单
	Vector<MenuItem*> menuItems;
	//前后左右
	
	auto itemlabel = Label::createWithTTF("W", "fonts/arial.ttf", 36);
	auto moveForward = MenuItemLabel::create(itemlabel, [&](cocos2d::Ref* pSender) {
		Animation *runAnimation = Animation::createWithSpriteFrames(run);
		runAnimation->setDelayPerUnit(0.05f);
		Animate* running = Animate::create(runAnimation);
		if (player->getPositionY() + 40 < origin.y + visibleSize.height)
		{
			player->runAction(Spawn::create(MoveBy::create(0.133f, Vec2(0, 40)), running, nullptr));
		}
		else
		{
			player->runAction(running);
		}
		
	});
	moveForward->setPosition(62.5, 100);
	menuItems.pushBack(moveForward);

	itemlabel = Label::createWithTTF("S", "fonts/arial.ttf", 36);
	auto moveBackward = MenuItemLabel::create(itemlabel, [&](cocos2d::Ref* pSender) {
		Animation *runAnimation = Animation::createWithSpriteFrames(run);
		runAnimation->setDelayPerUnit(0.05f);
		Animate* running = Animate::create(runAnimation);
		if (player->getPositionY() - 40 > origin.y)
		{
			player->runAction(Spawn::create(MoveBy::create(0.133f, Vec2(0, -40)), running, nullptr));
		}
		else
		{
			player->runAction(running);
		}

	});
	moveBackward->setPosition(62.5, 50);
	menuItems.pushBack(moveBackward);

	itemlabel = Label::createWithTTF("A", "fonts/arial.ttf", 36);
	auto moveLeft = MenuItemLabel::create(itemlabel, [&](cocos2d::Ref* pSender) {
		Animation *runAnimation = Animation::createWithSpriteFrames(run);
		runAnimation->setDelayPerUnit(0.05f);
		Animate* running = Animate::create(runAnimation);
		if (player->getPositionX() - 40 > origin.x)
		{
			player->runAction(Spawn::create(MoveBy::create(0.133f, Vec2(-40,0)), running, nullptr));
		}
		else
		{
			player->runAction(running);
		}

	});
	moveLeft->setPosition(25, 50);
	menuItems.pushBack(moveLeft);

	itemlabel = Label::createWithTTF("D", "fonts/arial.ttf", 36);
	auto moveRight = MenuItemLabel::create(itemlabel, [&](cocos2d::Ref* pSender) {
		Animation *runAnimation = Animation::createWithSpriteFrames(run);
		runAnimation->setDelayPerUnit(0.05f);
		Animate* running = Animate::create(runAnimation);
		if (player->getPositionX() + 40 < origin.x + visibleSize.width)
		{
			player->runAction(Spawn::create(MoveBy::create(0.133f, Vec2(40, 0)), running, nullptr));
		}
		else
		{
			player->runAction(running);
		}

	});
	moveRight->setPosition(100, 50);
	menuItems.pushBack(moveRight);
	/*
	auto itemlabel = Label::createWithTTF("W", "fonts/arial.ttf", 36);
	auto item3 = MenuItemLabel::create(itemlabel, CC_CALLBACK_1(HelloWorld::item_call, this));
	item3->setPosition(visibleSize.width/2-50,visibleSize.height/2);
	item3->setTag(3);
	*/

	//两个动画
	itemlabel = Label::createWithTTF("X", "fonts/arial.ttf", 36);
	auto deadAnimate = MenuItemLabel::create(itemlabel, [&](cocos2d::Ref* pSender) {
		if (!actioning)
		{
			Animation *deadAnimation = Animation::createWithSpriteFrames(dead);
			deadAnimation->setDelayPerUnit(0.1f);
			Animate* dying = Animate::create(deadAnimation);
			player->runAction(Sequence::create(dying, CallFunc::create([&]() {
				actioning = false;
			}), nullptr));
			pT->setPercentage(pT->getPercentage() - 10);
			actioning = true;
		}
	});
	deadAnimate->setPosition(origin.x+visibleSize.width - 20, 100);
	menuItems.pushBack(deadAnimate);
	
	itemlabel = Label::createWithTTF("Y", "fonts/arial.ttf", 36);
	auto attackAnimate = MenuItemLabel::create(itemlabel, [&](cocos2d::Ref* pSender) {
		if (!actioning)
		{
			Animation *attackAnimation = Animation::createWithSpriteFrames(attack);
			attackAnimation->setDelayPerUnit(0.1f);
			Animate* attacking = Animate::create(attackAnimation);
			player->runAction(Sequence::create(attacking, CallFunc::create([&]() {
				actioning = false;
			}), nullptr));
			pT->setPercentage(pT->getPercentage() + 10);
			actioning = true;
		}
	});
	attackAnimate->setPosition(origin.x + visibleSize.width - 70, 50);
	menuItems.pushBack(attackAnimate);

	auto menu = Menu::createWithArray(menuItems);
	menu->setPosition(origin.x, origin.y);
	this->addChild(menu, 1);

	dtime = 150;
	time = Label::createWithTTF(std::to_string(dtime), "fonts/arial.ttf", 36);
	time->setPosition(origin.x + visibleSize.width / 2 , origin.y + visibleSize.height - 30);
	this->addChild(time);
	schedule(schedule_selector(HelloWorld::updateCounter), 1.0f, 150, 0);

    return true; 
}
/*
void HelloWorld::goForward(cocos2d::Ref* pSender)
{
	Animation *runAnimation = Animation::createWithSpriteFrames(run);
	runAnimation->setDelayPerUnit(0.03f);
	Animate* running = Animate::create(runAnimation);

	player->runAction(Spawn::create(MoveBy::create(0.133f,Vec2(0,40)),running,nullptr));
}*/


void HelloWorld::updateCounter(float dt)
{
	dtime--;
	time->setString(std::to_string(dtime));
}