#ifndef __GAMESTART_SCENE_H__
#define __GAMESTART_SCENE_H__ 

#include "cocos2d.h"
#include "Bird.h"
#include "GameRun.h"

class GameStart : public cocos2d::Layer
{
public:
	static cocos2d::Scene* createScene();

	virtual bool init();
	virtual void onEnter();
	virtual void onExit();
	virtual void cleanup();
	//virtual void onEnter();
	//virtual void onExit();

	// a selector callback
	void menuCloseCallback(cocos2d::Ref* pSender);
	void Game_Begin(cocos2d::Ref* pSender);
	// implement the "static create()" method manually
	CREATE_FUNC(GameStart);
};
#endif