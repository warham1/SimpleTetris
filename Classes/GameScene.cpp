#include "GameScene.h"

USING_NS_CC;

Scene* GameScene::createScene()
{
	Scene* scene = Scene::create();

	auto layer = GameScene::create();
	scene->addChild(layer);

	return scene;
}

bool GameScene::init()
{
	if (!Layer::init())
	{
		return false;
	}

	auto listener = EventListenerKeyboard::create();
	listener->onKeyPressed = CC_CALLBACK_2(GameScene::onKeyPressed, this);
	listener->onKeyReleased = CC_CALLBACK_2(GameScene::onKeyReleased, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	line = 0;
	level = 1;

	isLeft = false;
	isRight = false;
	isDown = false;

	lines = LabelTTF::create(StringUtils::format("line = %d", line), "arial", 40.0f);
	lines->setColor(Color3B::WHITE);
	lines->setPosition(Point(80, 800));

	this->addChild(lines);

	makeField();

	startGame();

	return true;
}

void GameScene::onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event)
{
	switch (keyCode)
	{
		case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
			isLeft = true;
			break;
		case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
			isRight = true;
			break;
		case EventKeyboard::KeyCode::KEY_UP_ARROW:
			tapTurnRight();
			break;
		case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
			isDown = true;
			break;
		case EventKeyboard::KeyCode::KEY_SPACE:
			moveUnderMax();
			break;

	}
}

void GameScene::onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event)
{
	switch (keyCode)
	{
	case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
		isLeft = false;
		break;
	case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
		isRight = false;
		break;
	case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
		isDown = false;
		break;
	}
}

void GameScene::makeField()
{
	Size winSize = Director::sharedDirector()->getWinSize();

	for (int i = 0; i < FIELD_HEIGHT; i++) {
		for (int j = 0; j <= FIELD_WIDTH_RIGHT_INDEX - FIELD_WIDTH_LEFT_INDEX; j++) {
			auto b = Sprite::create("res/block_w.png");
			b->setAnchorPoint(Point(0, 0));
			b->setPosition(Point(b->getContentSize().width * j + 164, b->getContentSize().height * i + 84));
			
			this->addChild(b);
		}
	}

}

void GameScene::tapTurnRight()
{
	game->turnRight();
	moveChunk();
}

void GameScene::startGame()
{
	game = new Game();
	this->scheduleOnce(schedule_selector(GameScene::makeAndMoveChunk), 1);
	this->schedule(schedule_selector(GameScene::update), 0.07f);
}

void GameScene::update(float delta)
{
	if (isLeft)
	{
		game->moveLeft();
		moveChunk();
	}
	else if (isRight)
	{
		game->moveRight();
		moveChunk();
	}
	else if (isDown)
	{
		moveUnder(0);
	}
}

void GameScene::makeChunk()
{
	int number = lastBlockNumber + 1;
	int numbers[] = { number, number + 1, number + 2, number + 3 };
	game->makeChunk(CHUNK_START_X, CHUNK_START_Y, numbers);

	Size winSize = Director::sharedDirector()->getWinSize();

	for (int i = 0; i < CHUNK_HEIGHT; i++) {
		for (int j = 0; j < CHUNK_WIDTH; j++) {
			if (game->chunk->blocks[i][j] != NULL) {
				auto b = Sprite::create("res/block_b.png");
				int x = CHUNK_START_X - FIELD_WIDTH_LEFT_INDEX + j;
				int y = (FIELD_HEIGHT - 1) - (CHUNK_START_Y + i);
				b->setPosition(Point(b->getContentSize().width * x + 180, b->getContentSize().height * y + 100));
				b->setTag(number);
				number++;
				this->addChild(b);
			}
		}
	}

	lastBlockNumber += 4;
}
void GameScene::moveUnderMax()
{
	while (game->canMoveUnder())
	{
		game->moveUnder();
		moveChunk();
	}

	game->copyBlocks();
	this->unschedule(schedule_selector(GameScene::moveUnder));
	deleteLines();
	if (!resetIfGameover())
	{
		this->scheduleOnce(schedule_selector(GameScene::makeAndMoveChunk), 1);
	}
	else
	{
		CCLOG("gameover");
	}
}


void GameScene::moveUnder(float delta)
{
	if (game->canMoveUnder()) {
		game->moveUnder();
		moveChunk();
	}
	else {
		game->copyBlocks();
		this->unschedule(schedule_selector(GameScene::moveUnder));
		deleteLines();
		if (!resetIfGameover())
		{
			this->scheduleOnce(schedule_selector(GameScene::makeAndMoveChunk), float(1.0f / float(level)));
		}
		else
		{
			CCLOG("gameover");
		}
	}
}

void GameScene::moveChunk()
{
	Size winSize = Director::sharedDirector()->getWinSize();

	for (int i = 0; i < CHUNK_HEIGHT; i++) {
		for (int j = 0; j < CHUNK_WIDTH; j++) {
			if (game->chunk->blocks[i][j] != NULL) {
				Block* b = game->chunk->blocks[i][j];
				int number = b->getNumber();
				auto l = (Sprite*)this->getChildByTag(number);
				int x = game->chunk->posX - FIELD_WIDTH_LEFT_INDEX + j;
				int y = (FIELD_HEIGHT - 1) - (game->chunk->posY + i);
				
				l->setPosition(Point(l->getContentSize().width * x + 180, l->getContentSize().height * y + 100));

			}
		}
	}
}

void GameScene::makeAndMoveChunk(float delta)
{
	makeChunk();
	this->schedule(schedule_selector(GameScene::moveUnder), float(0.25f / (float)(level)));
}

void GameScene::deleteLines()
{
	if (game->checkDeletableLines())
	{
		line++;
		level = line / 5 + 1;
		lines->setString(StringUtils::format("line = %d", line));

		for (int i = 0; i < FIELD_HEIGHT; i++)
		{
			if (game->deletableLines[i])
			{
				for (int j = 0; j < FIELD_WIDTH; j++)
				{
					if (game->field->blocks[i][j] != NULL)
					{
						Block* b = game->field->blocks[i][j];
						int number = b->getNumber();

						this->removeChildByTag(number);
					}
				}
			}
		}

		game->deleteDeletableLines();

		for (int i = 0; i < FIELD_HEIGHT; i++) {
			for (int j = 0; j < FIELD_WIDTH; j++) {
				if (game->field->blocks[i][j] != NULL) {
					Block* b = game->field->blocks[i][j];
					int number = b->getNumber();
					auto l = (Sprite*)this->getChildByTag(number);
					int x = j - FIELD_WIDTH_LEFT_INDEX;
					int y = (FIELD_HEIGHT - 1) - i;
					MoveTo* action = MoveTo::create(0.2, Point(l->getContentSize().width * x + 180, l->getContentSize().height * y + 100));
					l->runAction(action);
				}
			}
		}
	}
}

bool GameScene::resetIfGameover()
{
	for (int i = 0; i < FIELD_HEIGHT_UPPER_INDEX; i++) {
		for (int j = 0; j < FIELD_WIDTH; j++) {
			if (game->field->blocks[i][j] != NULL) {
				return true;
			}
		}
	}
	return false;
}