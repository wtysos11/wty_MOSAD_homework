#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"
#include "Monster.h"
#include <cstdio>
#include <string>
#include <cmath>
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

	//加载地图
	/*
	TMXTiledMap* tmx = TMXTiledMap::create("map.tmx");
	tmx->setPosition(visibleSize.width / 2, visibleSize.height / 2);
	tmx->setAnchorPoint(Vec2(0.5, 0.5));
	tmx->setScale(Director::getInstance()->getContentScaleFactor());
	this->addChild(tmx, 0);
	*/

	deadState = false;
	monsterKill = 0;
	//创建一张贴图
	auto texture = Director::getInstance()->getTextureCache()->addImage("$lucia_2.png");
	//从贴图中以像素单位切割，创建关键帧
	auto frame0 = SpriteFrame::createWithTexture(texture, CC_RECT_PIXELS_TO_POINTS(Rect(0, 0, 113, 113)));
	//使用第一帧创建精灵
	player = Sprite::createWithSpriteFrame(frame0);
	player->setPosition(Vec2(origin.x + visibleSize.width / 2,
		origin.y + visibleSize.height / 2));
	addChild(player, 3);

	for (unsigned int i = 0; i < 15; i++)
	{
		auto fac = Factory::getInstance();
		
		float x = random(origin.x, visibleSize.width);
		float y = random(origin.y, visibleSize.height);
		//随机点离角色太近，跳掉
		if (fabs(x - player->getPositionX()) < 10 || fabs(y - player->getPositionY()) < 10)
		{
			i--;
			continue;
		}
		auto monster = fac->createMonster();
		monster->setPosition(x, y);
		addChild(monster, 3);

	}

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
	attack.reserve(17);
	for (int i = 0; i < 17; i++) {
		auto frame = SpriteFrame::createWithTexture(texture, CC_RECT_PIXELS_TO_POINTS(Rect(113 * i, 0, 113, 113)));
		attack.pushBack(frame);
	}

	// 可以仿照攻击动画
	// 死亡动画(帧数：22帧，高：90，宽：79）
	auto texture2 = Director::getInstance()->getTextureCache()->addImage("$lucia_dead.png");

	dead.reserve(22);
	for (int i = 0; i < 22; i++)
	{
		auto frame = SpriteFrame::createWithTexture(texture2, CC_RECT_PIXELS_TO_POINTS(Rect(79 * i, 0, 79, 90)));
		dead.pushBack(frame);
	}
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
		if (deadState)
			return;

		cid = 'W';
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
		if (deadState)
			return;

		cid = 'S';
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
		if (deadState)
			return;

		cid = 'A';
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

		if (lastCid != 'A')
		{
			player->setFlipX(true);
		}
		lastCid = 'A';
	});
	moveLeft->setPosition(25, 50);
	menuItems.pushBack(moveLeft);

	itemlabel = Label::createWithTTF("D", "fonts/arial.ttf", 36);
	auto moveRight = MenuItemLabel::create(itemlabel, [&](cocos2d::Ref* pSender) {
		if (deadState)
			return;

		cid = 'D';
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

		if (lastCid != 'D')
		{
			player->setFlipX(false);
		}
		lastCid = 'D';

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
		if (!actioning && !deadState)
		{
			Animation *deadAnimation = Animation::createWithSpriteFrames(dead);
			deadAnimation->setDelayPerUnit(0.1f);
			deadAnimation->setRestoreOriginalFrame(true);
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
		if (!actioning && !deadState)
		{
			Animation *attackAnimation = Animation::createWithSpriteFrames(attack);
			attackAnimation->setDelayPerUnit(0.1f);
			attackAnimation->setRestoreOriginalFrame(true);
			Animate* attacking = Animate::create(attackAnimation);

			auto factory = Factory::getInstance();
			Rect playerRect = player->getBoundingBox();
			Rect attackRect = Rect(playerRect.getMinX() - 40, playerRect.getMinY(), playerRect.getMaxX() - playerRect.getMinX() + 80, playerRect.getMaxY() - playerRect.getMinY());
			auto atkMonster = factory->collider(attackRect);
			if (atkMonster != NULL)
			{
				factory->removeMonster(atkMonster);
				monsterKill++;
				kill->setString(std::to_string(monsterKill));
			}

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

	kill = Label::createWithTTF(std::to_string(monsterKill), "fonts/arial.ttf", 36);
	kill->setPosition(origin.x + visibleSize.width / 2, origin.y + visibleSize.height - 75);
	this->addChild(kill);
	schedule(schedule_selector(HelloWorld::updateCounter), 1.0f, 150, 0);
	schedule(schedule_selector(HelloWorld::hitByMonster), 0.2f, 750, 0);
	schedule(schedule_selector(HelloWorld::monsterChase), 3.0f, 50, 0);//3s怪物动一次


    return true; 
}

void HelloWorld::updateCounter(float dt)
{
	if (deadState)
	{
		return;
	}
	dtime--;
	time->setString(std::to_string(dtime));
}

//怪物追逐
void HelloWorld::monsterChase(float dt)
{
	if (deadState)
	{
		return;
	}
	auto factory = Factory::getInstance();
	factory->moveMonster(player->getPosition(), 1.0f);
}
void HelloWorld::hitByMonster(float dt)
{
	auto factory = Factory::getInstance();
	Sprite* collision = factory->collider(player->getBoundingBox());//collision为碰撞到主角的怪物
	if (collision != NULL)
	{
		factory->removeMonster(collision);//移除怪物对象
		pT->setPercentage(pT->getPercentage() - 10);//人物血条下降
	}

	//死亡
	if (pT->getPercentage() <= 0 && !deadState)
	{
		Animation *deadAnimation = Animation::createWithSpriteFrames(dead);
		deadAnimation->setDelayPerUnit(0.1f);
		Animate* dying = Animate::create(deadAnimation);
		player->runAction(dying);
		deadState = true;
		removeChild(time);
		Label* gameOver = Label::createWithTTF("Game Over!", "fonts/arial.ttf", 36);
		gameOver->setPosition(origin.x + visibleSize.width / 2, origin.y + visibleSize.height - 30);
	}
}