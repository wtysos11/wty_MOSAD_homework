#include "GameScene.h"

USING_NS_CC;

Scene* GameSence::createScene()
{
	return GameSence::create();
}

// on "init" you need to initialize your instance
bool GameSence::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Scene::init())
	{
		return false;
	}


	//add touch listener
	EventListenerTouchOneByOne* listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	listener->onTouchBegan = CC_CALLBACK_2(GameSence::onTouchBegan, this);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);


	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	//draw image
	auto background = Sprite::create("level-background-0.jpg");
	background->setPosition(visibleSize.width / 2, visibleSize.height / 2);   //����λ��  
	background->setAnchorPoint(Point(0.5, 0.5));    //����ê��  
													//������������ʵ�ʴ�С  
	Size visibleSize_back = background->getContentSize();  //��ȡ�����С(����ͼƬ��ʱ�򣬾����СΪͼƬ��С)  
														   //����ʵ�ʿ�Ⱥ��ܿ�ȣ������������ű���  
	float scaleX = (float)visibleSize.width / (float)visibleSize_back.width;
	float scaleY = (float)visibleSize.height / (float)visibleSize_back.height;
	background->setScale(scaleX, scaleY);    //����  
	this->addChild(background, 1);

	auto label = MenuItemLabel::create(Label::createWithTTF("Shoot", "fonts/Marker Felt.ttf", 64),
											this, 
											menu_selector(GameSence::ShootCallback));
	label->setPosition(700, 480);
	Menu* pMenu = Menu::create(label, NULL);
	pMenu->setPosition(Vec2(origin.x, origin.y));
	this->addChild(pMenu, 50);

	this->mouseLayer = Layer::create();
	mouseLayer->setAnchorPoint(Point(0, 0));
	mouseLayer->setPosition(origin.x, origin.y + visibleSize.height / 2);
	this->addChild(mouseLayer, 10);

	this->stoneLayer = Layer::create();
	stoneLayer->setAnchorPoint(Point(0, 0));
	this->addChild(stoneLayer, 10);

	this->stone = Sprite::create("stone.png");
	stone->setPosition(Vec2(560, 480));
	stoneLayer->addChild(stone, 10);

	//��������Ķ�����Դ
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("level-sheet.plist");
	char totalFrames = 8;
	char frameName[20];
	Animation* mouseAnimation = Animation::create();
	for (int i = 0; i < totalFrames; i++)
	{
		sprintf(frameName,"gem-mouse-%d.png",i);
		mouseAnimation->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(frameName));
	}
	mouseAnimation->setDelayPerUnit(0.1);
	AnimationCache::getInstance()->addAnimation(mouseAnimation, "mouseAnimation");

	this->mouse = Sprite::createWithSpriteFrameName("gem-mouse-0.png");
	Animate* mouseAnimate = Animate::create(AnimationCache::getInstance()->getAnimation("mouseAnimation"));
	mouse->runAction(RepeatForever::create(mouseAnimate));
	mouse->setPosition(origin.x+visibleSize.width/2,0);
	mouseLayer->addChild(mouse, 10);

	return true;
}
/*
����¼�
���֮������������
1.�����ƶ�
2.������������
*/
bool GameSence::onTouchBegan(Touch *touch, Event *unused_event) {

	auto location = touch->getLocation();
	auto MousePos = mouse->getPosition()+mouseLayer->getPosition();

	auto moveTo = MoveBy::create(1, Vec2(location.x- MousePos.x,location.y- MousePos.y)); // 1s moveby
	mouse->runAction(moveTo);

	auto cheese = Sprite::create("cheese.png");
	cheese->setPosition(location);
	cheese->runAction(FadeOut::create(3.0f));//3s fade out
	this->addChild(cheese, 100);

	return true;
}

/*
�����ʱ��
�����λ�÷���һ����ʯ
����ʯͷ�������λ��
����ȡ��ԭ�ж������������һ���ط�
*/
void GameSence::ShootCallback(cocos2d::Ref* pSender)
{
	auto stone2 = Sprite::create("stone.png");
	stone2->setPosition(Vec2(560, 480));
	this->stoneLayer->addChild(stone2, 10);

	auto MousePos = this->mouse->getPosition() + this->mouseLayer->getPosition();
	stone2->runAction(Sequence::create(MoveTo::create(1, MousePos),FadeOut::create(1), nullptr));

	auto gem = Sprite::create("diamond.png");
	gem->setPosition(MousePos);
	this->addChild(gem, 20);

	Size visibleSize = Director::getInstance()->getVisibleSize();
	srand((unsigned)time(nullptr));
	int x = visibleSize.width;
	int y = visibleSize.height;
	this->mouse->runAction(MoveTo::create(2.0f,Vec2(rand() % x - this->mouseLayer->getPosition().x, rand() % y - this->mouseLayer->getPosition().y)));
}