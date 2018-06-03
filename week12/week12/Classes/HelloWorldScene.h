#pragma once
#include "cocos2d.h"
using namespace cocos2d;

class HelloWorld : public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();
	void updateCounter(float dt);
	void hitByMonster(float dt);
	void monsterChase(float dt);
	//void goForward(cocos2d::Ref* pSender);
    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);
private:
	bool actioning = false;
	cocos2d::Sprite* player;
	cocos2d::Vector<SpriteFrame*> attack;
	cocos2d::Vector<SpriteFrame*> dead;
	cocos2d::Vector<SpriteFrame*> run;
	cocos2d::Vector<SpriteFrame*> idle;
	cocos2d::Size visibleSize;
	cocos2d::Vec2 origin;
	cocos2d::Label* time;
	cocos2d::Label* kill;
	int dtime;
	cocos2d::ProgressTimer* pT;
	bool deadState;
	char cid;
	char lastCid;
	int monsterKill;//击杀怪物的数量
};
