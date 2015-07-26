#include "GameOver.h"


USING_NS_CC;
using namespace std;

Scene* GameOver::createScene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = GameOver::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

// on "init" you need to initialize your instance
bool GameOver::init()
{
	//////////////////////////////
	// 1. super init first
	if ( !Layer::init() )
	{
		return false;
	}
	log("GameStart init");
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	// add a "close" icon to exit the progress. it's an autorelease object
	auto closeItem = MenuItemImage::create(
		"CloseNormal.png",
		"CloseSelected.png",
		CC_CALLBACK_1(GameOver::menuCloseCallback, this));

	closeItem->setPosition(Vec2(origin.x + visibleSize.width - closeItem->getContentSize().width/2 ,
		origin.y + closeItem->getContentSize().height/2));

	// create menu, it's an autorelease object
	auto menu = Menu::create(closeItem, NULL);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, 1);

	// create and initialize a label
	auto label = Label::createWithTTF("Flappy Bird", "fonts/Marker Felt.ttf", 32);
	label->setPosition(Vec2(origin.x + visibleSize.width/2,origin.y + visibleSize.height - 80));
	this->addChild(label, 3);


	//initialize the SpriteFrameCache
	//SpriteFrameCache *frameCache = SpriteFrameCache::getInstance();
	//frameCache->addSpriteFramesWithFile("birdSprites.plist");

	auto ready = Sprite::createWithSpriteFrameName("text_ready.png");
	ready->setPosition(visibleSize.width/2,visibleSize.height/2+100);
	addChild(ready,1);

	auto tutorial = Sprite::createWithSpriteFrameName("tutorial.png");
	tutorial->setPosition(visibleSize.width/2,visibleSize.height/2-10);
	addChild(tutorial,1);

	auto ok = Sprite::createWithSpriteFrameName("button_ok.png");
	auto ok_item = MenuItemSprite::create(ok,ok,nullptr,CC_CALLBACK_1(GameOver::Game_Begin,this));
	auto mu = Menu::create(ok_item,0);
	mu->setPosition(visibleSize.width/2,visibleSize.height/2-80);
	addChild(mu,2);

	auto bg = Sprite::createWithSpriteFrameName("bg_day.png");
	bg->setPosition(visibleSize.width/2,visibleSize.height/2);
	addChild(bg);

	Bird *bird = new Bird(static_cast<BIRD_KINDS>(random(0,2)));
	bird->getBird()->setPosition(visibleSize.width/2-45,visibleSize.height/2);
	addChild(bird->getBird(),2);
	bird->bird_Fly_Spot();


	//draw the background





	return true;
}

void GameOver::onEnter()
{
	Layer::onEnter();
	log("gamestart onEnter");
}

void GameOver::onExit()
{
	Layer::onExit();
	log("gamestart onExit");
}

void GameOver::cleanup()
{
	Layer::cleanup();
	log("gamestart cleanup");

}
void GameOver::Game_Begin(Ref *pSender)
{
	//auto sc = GameStart::createScene();
	//Director::getInstance()->replaceScene(sc);
}



void GameOver::menuCloseCallback(Ref* pSender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
	MessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
	return;
#endif

	Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif
}
