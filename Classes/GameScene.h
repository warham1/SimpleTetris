#ifndef __tetris__GameScene__
#define __tetris__GameScene__

#include "Game.h"
#include "cocos2d.h"

USING_NS_CC;

class GameScene : public cocos2d::Layer
{
private:
	::Game* game;
	int lastBlockNumber = 0;

	int level;
	int line;

	bool isLeft;
	bool isRight;
	bool isDown;
	LabelTTF* lines;

	void makeField();

	void update(float delta);

	void tapMoveLeft();
	void tapMoveRight();
	void tapTurnLeft();
	void tapTurnRight();

	void startGame();

	void makeChunk();
	void moveChunk();
	void makeAndMoveChunk(float delta);

	void moveUnder(float delta);
	void moveUnderMax();

	void deleteLines();

	bool resetIfGameover();

public:
	static cocos2d::Scene* createScene();

	virtual bool init();

	void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event)override;
	void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event)override;


	CREATE_FUNC(GameScene);
};

#endif