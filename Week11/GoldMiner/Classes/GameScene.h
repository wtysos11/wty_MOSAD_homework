#pragma once
#include <stdio.h>
#include "cocos2d.h"
USING_NS_CC;

class GameSence : public cocos2d::Scene
{
public:
	static cocos2d::Scene* createScene();

	virtual bool init();

	virtual bool onTouchBegan(Touch *touch, Event *unused_event);
	void ShootCallback(cocos2d::Ref* pSender);
	//virtual void shootMenuCallback(Ref* pSender);

	CREATE_FUNC(GameSence);

private:
	Sprite * mouse;

	Sprite* stone;

	Layer* mouseLayer;

	Layer* stoneLayer;

};