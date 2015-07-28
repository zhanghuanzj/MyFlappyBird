#ifndef __GAMERUN_SCENE_H__
#define __GAMERUN_SCENE_H__

#include "cocos2d.h"
#include "SimpleAudioEngine.h"
#include "Bird.h"
#include <vector>
#include "GameOver.h"


class GameRun : public cocos2d::Layer
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
	virtual void onEnter();
	virtual void onExit();
    virtual void cleanup();

    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
    
    // implement the "static create()" method manually
    CREATE_FUNC(GameRun);


	virtual bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unused_event);
	virtual void onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *unused_event);
	virtual void onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *unused_event);

private:
	void update(float delta);
	void update_bird();
	void update_pipes();
	void update_bg();
	void update_land();
	int height_float();
	bool Game_over();
	bool isin_pipes();
	void draw_score();

	Bird *myBird;
	std::vector<cocos2d::Node*> vec_pipes;
	cocos2d::EventListenerPhysicsContact *listener;
};

#endif // __GAMERUN_SCENE_H__
