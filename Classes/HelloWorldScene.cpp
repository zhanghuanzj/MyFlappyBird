#include "HelloWorldScene.h"

const int velocity_y = -900;

USING_NS_CC;

Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::createWithPhysics();
	/*scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);*/
	scene->getPhysicsWorld()->setGravity(Vec2(0,velocity_y));
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    auto closeItem = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));
    
	closeItem->setPosition(Vec2(origin.x + visibleSize.width - closeItem->getContentSize().width/2 ,
                                origin.y + closeItem->getContentSize().height/2));

    // create menu, it's an autorelease object
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);

    /////////////////////////////
    // 3. add your codes below...

    // add a label shows "Hello World"
    // create and initialize a label
    
    auto label = Label::createWithTTF("Flappy Bird", "fonts/Marker Felt.ttf", 24);
    
    // position the label on the center of the screen
    label->setPosition(Vec2(origin.x + visibleSize.width/2,
                            origin.y + visibleSize.height - label->getContentSize().height));

    // add the label as a child to this layer
    this->addChild(label, 1);


	//initialize the SpriteFrameCache
	SpriteFrameCache *frameCache = SpriteFrameCache::getInstance();
	frameCache->addSpriteFramesWithFile("birdSprites.plist");

	//draw the background
	auto background = Sprite::createWithSpriteFrameName("bg_day.png");
	background->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));
    this->addChild(background,0);

	//create the world bound
	auto worldBody = PhysicsBody::createEdgeBox(visibleSize,PHYSICSBODY_MATERIAL_DEFAULT,2.0f);
	auto edgeNode = Node::create();
	edgeNode->setPhysicsBody(worldBody);
	edgeNode->setPosition(Vec2(visibleSize.width/2,visibleSize.height/2));
	addChild(edgeNode);

	//create a bird
	myBird =  new Bird((BIRD_KINDS)random(0,2));
	myBird->getBird()->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));
	myBird->getBird()->getPhysicsBody()->setGravityEnable(true);
	this->addChild(myBird->getBird());
	/*myBird->bird_Fly();*/

	//set the scheduleUpdate
	this->scheduleUpdate();

	setTouchEnabled(true);
	setTouchMode(Touch::DispatchMode::ONE_BY_ONE);

    return true;
}

void HelloWorld::update(float delta)
{
	log("update is coming");
	auto velocity_bird = myBird->getBird()->getPhysicsBody()->getVelocity();
	int angle = -velocity_bird.y*0.1-20;
	if (angle>90)
	{
		angle = 90;
	}
	myBird->getBird()->setRotation(angle);
}
bool HelloWorld::onTouchBegan(Touch *touch, Event *unused_event)
{
	myBird->bird_Fly();
	log("bird fly");
	return true;     
}

void HelloWorld::onTouchMoved(Touch *touch, Event *unused_event)
{
	log("just move");
}
void HelloWorld::onTouchEnded(Touch *touch, Event *unused_event)
{
	/*myBird->bird_Stop();*/
	log("bird stop");
	log("the velocity is %f\n",myBird->getBird()->getPhysicsBody()->getVelocity().y);
}
void HelloWorld::menuCloseCallback(Ref* pSender)
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
