#include "HelloWorldScene.h"

const int velocity_y = -900;
const int BIRD = 0;
const int BG_0 = 1;
const int BG_1 = 2;
const int LAND_0 = 3;
const int LAND_1 = 4;
const int PIPE_UP = 5;
const int PIPE_DOWN = 6;
const int SHAKE_NODE = 7;
const int NUMBER_NODE = 8;
const int per_move = 1;
const int offset_bg = 1;
const int offset_land = 2;
const int per_move_land = 2;
const int pipe_width = 52;
const int pipe_height = 320;
const int pipe_distance = 100;
const int pipe_count = 2;

int score = 0;
bool GAME_OVER = false;
bool TRANS_COUNT = false;
bool IS_ING = false;
bool FLAG = false;
USING_NS_CC;
using namespace std;

Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::createWithPhysics();
	//scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
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
	log("HelloWorld init..........");
	log("GAME_OVER is %d",GAME_OVER);
	GAME_OVER = false;
	TRANS_COUNT = false;
	score = 0;
	IS_ING = false;
	FLAG = false;
	
    if ( !Layer::init() )
    {
        return false;
    }

    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

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

    // create and initialize a label
    auto label = Label::createWithTTF("Flappy Bird", "fonts/Marker Felt.ttf", 24);
    
    // position the label on the center of the screen
    label->setPosition(Vec2(origin.x + visibleSize.width/2,origin.y + visibleSize.height - label->getContentSize().height));

    // add the label as a child to this layer
    this->addChild(label, 3);


	//draw the background
	auto background_0 = Sprite::createWithSpriteFrameName("bg_day.png");
	auto background_1 = Sprite::createWithSpriteFrameName("bg_day.png");
	background_0->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));
	background_1->setPosition(Vec2(visibleSize.width*3/2 + origin.x-offset_bg, visibleSize.height/2 + origin.y));
    this->addChild(background_0,0,BG_0);
	this->addChild(background_1,0,BG_1);


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
	this->addChild(myBird->getBird(),2,BIRD);
	


	//create the land
	auto land = Sprite::createWithSpriteFrameName("land.png");								   //loading Sprites
	auto land_1 = Sprite::createWithSpriteFrameName("land.png");
	land_1->setPosition(visibleSize.width*3/2-offset_land,land_1->getContentSize().height/2);  //setLocation
	land->setPosition(visibleSize.width/2,land->getContentSize().height/2);
	auto land_body = PhysicsBody::createBox(land->getContentSize());						   //create the PhysicsBody
	auto land_1_body = PhysicsBody::createBox(land_1->getContentSize());
	land_body->setLinearDamping(500);
	land_1_body->setLinearDamping(500);
	land_body->setContactTestBitmask(0x10);													   //setContactBitmask
	land_1_body->setContactTestBitmask(0x10);
	land_body->setDynamic(false);															   //fix the body
	land_1_body->setDynamic(false);
	
	land->setPhysicsBody(land_body);														   //add body to the sprite
	land_1->setPhysicsBody(land_1_body);
	this->addChild(land,2,LAND_0);															   //add sprite to the layer
	this->addChild(land_1,2,LAND_1);

	//create the pipes
	for (int i=0;i<pipe_count;i++)
	{
		auto pipe_up = Sprite::createWithSpriteFrameName("pipe_up.png");
		auto pipe_down = Sprite::createWithSpriteFrameName("pipe_down.png");
		pipe_down->setPosition(0,visibleSize.height/2+(pipe_distance+pipe_height)/2);
		pipe_up->setPosition(0,visibleSize.height/2-(pipe_distance+pipe_height)/2);
		auto pipe_up_body = PhysicsBody::createBox(pipe_up->getContentSize());
		auto pipe_down_body = PhysicsBody::createBox(pipe_down->getContentSize());
		pipe_up_body->setContactTestBitmask(0x01);
		pipe_down_body->setContactTestBitmask(0x01);
		pipe_up_body->setDynamic(false);
		pipe_down_body->setDynamic(false);
		pipe_up->setPhysicsBody(pipe_up_body);
		pipe_down->setPhysicsBody(pipe_down_body);
		auto pipe_pair = Node::create();
		pipe_pair->addChild(pipe_up,0,PIPE_UP);
		pipe_pair->addChild(pipe_down,0,PIPE_DOWN);
		pipe_pair->setPosition(i*(pipe_width+visibleSize.width)/2+pipe_width/2+visibleSize.width,height_float());
		this->addChild(pipe_pair,1);
		vec_pipes.push_back(pipe_pair);
	}
	
	auto number_node = Node::create();
	auto num_sprite = Sprite::createWithSpriteFrameName("font_048.png");
	num_sprite->setPosition(visibleSize.width/2,visibleSize.height*4/5);
	number_node->addChild(num_sprite);
	addChild(number_node,4,NUMBER_NODE);


	//set the scheduleUpdate
	this->scheduleUpdate();

	//set touch enable
	setTouchEnabled(true);
	setTouchMode(Touch::DispatchMode::ONE_BY_ONE);

    return true;
}

bool HelloWorld::Game_over()
{
	//log("GameOver.................................%d",GAME_OVER);
	//if (!GAME_OVER)
	//{
	//	log("access-------------------------");
	//	for (int i=0;i<pipe_count;i++)
	//	{
	//		vec_pipes[0]->getChildByTag(PIPE_UP)->getPhysicsBody()->setEnable(false);
	//		vec_pipes[0]->getChildByTag(PIPE_DOWN)->getPhysicsBody()->setEnable(false);
	//	}
	//	GAME_OVER = true;
	//	return true;
	//}
	//auto velocity_bird = myBird->getBird()->getPhysicsBody()->getVelocity();
	//log("the velocity is %d,  %d",velocity_bird.x,velocity_bird.y);
	//if (velocity_bird.x==0&&velocity_bird.y==0)    //When the velocity of bird is zero 
	//{
	log("begin to trans");
	auto sc = GameOver::createScene();
	auto resc = TransitionFade::create(3,sc);
	Director::getInstance()->replaceScene(resc);
	//}
	return true;
}


void HelloWorld::onEnter()
{
	log("Helloworld onEnter");
	Layer::onEnter();
	listener = EventListenerPhysicsContact::create();
	/*listener->onContactBegin = CC_CALLBACK_0(HelloWorld::Game_over,this);*/
	listener->onContactBegin=[this](PhysicsContact & contact)->bool{
		this->getChildByTag(BIRD);
		auto SpriteA = (Sprite*)contact.getShapeA()->getBody()->getNode();
		auto SpriteB = (Sprite*)contact.getShapeB()->getBody()->getNode();

		if (SpriteA->getTag()==3||SpriteA->getTag()==4||SpriteB->getTag()==3||SpriteB->getTag()==4)
		{
			if (!TRANS_COUNT)
			{
				Game_over();
				TRANS_COUNT = true;
			}
		}
		else if (!GAME_OVER)
		{
			GAME_OVER = true;
			for (int i=0;i<pipe_count;i++)
			{
				vec_pipes[i]->getChildByTag(PIPE_UP)->getPhysicsBody()->setEnable(false);
				vec_pipes[i]->getChildByTag(PIPE_DOWN)->getPhysicsBody()->setEnable(false);
			}
		}
		return true;
	};

	//listener->onContactPreSolve = [](PhysicsContact & contact,PhysicsContactPreSolve& solve){
	//	log("preSolve");
	//	return true;
	//};
	//listener->onContactPostSolve = [](PhysicsContact & contact,const PhysicsContactPostSolve& solve){
	//	log("postSolve");
	//};
	//listener->onContactSeperate = [](PhysicsContact& contact){
	//	log("contact seperate");
	//};
	Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(listener,1);


}

void HelloWorld::onExit()
{
	Layer::onExit();
	Director::getInstance()->getEventDispatcher()->removeEventListener(listener);
	log("Exit");
}

void HelloWorld::cleanup()
{
	Layer::cleanup();
	/*CCNotificationCenter::sharedNotificationCenter()->removeAllObservers(this);*/
	log("HelloWorld cleanup");
}
//update the bird
void HelloWorld::update_bird()
{
	auto velocity_bird = myBird->getBird()->getPhysicsBody()->getVelocity();
	int angle = -velocity_bird.y*0.1-20;
	if (angle>90)
	{
		angle = 90;
	}
	myBird->getBird()->setRotation(angle);
	

}

//update the background
void HelloWorld::update_bg()
{
	Size visibleSize = Director::getInstance()->getVisibleSize();
	auto backGround_0 = this->getChildByTag(BG_0);
	auto backGround_1 = this->getChildByTag(BG_1);
	auto position_0 = backGround_0->getPosition();
	auto position_1 = backGround_1->getPosition();
	if (position_0.x<=-visibleSize.width/2+offset_bg*2)
	{
		position_0.x = position_1.x+visibleSize.width-offset_bg;
	}
	if (position_1.x<=-visibleSize.width/2+offset_bg*2)
	{

		position_1.x = position_0.x+visibleSize.width-offset_bg;
	}
	backGround_0->setPosition(position_0.x-per_move,position_0.y);
	backGround_1->setPosition(position_1.x-per_move,position_1.y);
}
	//update the land
void HelloWorld::update_land()
{
	Size visibleSize = Director::getInstance()->getVisibleSize();
	auto land_0 = this->getChildByTag(LAND_0);
	auto land_1 = this->getChildByTag(LAND_1);
	auto land_position_0 = land_0->getPosition();
	auto land_position_1 = land_1->getPosition();
	if (land_position_0.x<=-visibleSize.width/2+offset_land*2)
	{
		land_position_0.x = land_position_1.x+visibleSize.width-offset_land;
	}
	if (land_position_1.x<=-visibleSize.width/2+offset_land*2)
	{

		land_position_1.x = land_position_0.x+visibleSize.width-offset_land;
	}
	land_0->setPosition(land_position_0.x-per_move_land,land_position_0.y);
	land_1->setPosition(land_position_1.x-per_move_land,land_position_1.y);
}

int HelloWorld::height_float()
{
	int float_heght = rand()%200;
	return float_heght-50;
}

//update the pipes
void HelloWorld::update_pipes()
{
	Size visibleSize = Director::getInstance()->getVisibleSize();
	for ( int i=0;i<pipe_count;i++)
	{
		auto pipe_position = vec_pipes[i]->getPosition();
		if (pipe_position.x<=-pipe_width)
		{
			pipe_position.x = visibleSize.width+pipe_width/2+per_move_land;
			pipe_position.y = height_float();
		}
		vec_pipes[i]->setPosition(pipe_position.x-per_move_land,pipe_position.y);
	}
}

bool HelloWorld::isin_pipes()
{
	int bird_x = myBird->getBird()->getPosition().x;
	for ( int i=0;i<pipe_count;i++)
	{
		int pipe_x = vec_pipes[i]->getPosition().x;
		if (abs(pipe_x-bird_x)<=26)
		{
			return true;
		}
	}
	return false;
}

void HelloWorld::draw_score()
{
	auto visibleSize = Director::getInstance()->getVisibleSize();
	auto number_node = this->getChildByTag(NUMBER_NODE);
	if (number_node!=nullptr)
	{
		number_node->removeAllChildren();  //release the pre number sprites
	}
	int score_now = score;
	int positionX = 0;
	int mod_num = 0;
	int index = 0;
	while (score_now)
	{
		mod_num = score_now%10;
		log("The num is ",48+mod_num);
		__String *s = __String::createWithFormat("font_0%d.png",48+mod_num);
		log("%s",s->getCString());
		auto num_sprite = Sprite::createWithSpriteFrameName(s->getCString());
		num_sprite->setPosition(Vec2(-index*24,0));
		number_node->addChild(num_sprite);
		index++;
		score_now/=10;
	}
	number_node->setPosition(visibleSize.width/2+index*12,visibleSize.height*4/5);
}
void HelloWorld::update(float delta)
{
	log("update is coming");
	if (GAME_OVER)
	{
		return;
	}
	else
	{
		auto visibleSize = Director::getInstance()->getVisibleSize();
		IS_ING = isin_pipes();
		if (IS_ING&&!FLAG)           //between the pipes
		{
			FLAG = true;
		}
		else if (!IS_ING&&FLAG)      //out the pipes add 1
		{
			FLAG = false;
			score++;
			log("OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO");
			draw_score();
		}
		update_bird();
		update_bg();
		update_land();
		update_pipes();
	}

}
bool HelloWorld::onTouchBegan(Touch *touch, Event *unused_event)
{
	myBird->bird_Fly();
	return true;     
}

void HelloWorld::onTouchMoved(Touch *touch, Event *unused_event)
{
	/*log("just move");*/
}
void HelloWorld::onTouchEnded(Touch *touch, Event *unused_event)
{
	/*myBird->bird_Stop();*/
	/*log("bird stop");*/
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
