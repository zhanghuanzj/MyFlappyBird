#include "Bird.h"

USING_NS_CC ;
using namespace std;
SpriteFrameCache *SFCache = SpriteFrameCache::getInstance();

Bird::Bird()
{
	bird->createWithSpriteFrameName("bird0_0.png");
}

Bird::Bird(BIRD_KINDS kind)
{
	auto birdName = __String::createWithFormat("bird%d.0",kind);
	bird->createWithSpriteFrameName(birdName->getCString());
}

Sprite* Bird::getBird()
{
	return bird;
}