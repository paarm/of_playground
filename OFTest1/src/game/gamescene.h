#pragma once
#include "../engine/node.h"
#include "leveldesc/levelloader.h"
#include "worldstate.h"

class GameScene : public Node {
private:
	LevelLoader mLevelLoader;
	WorldState  mWorldState;
	Node2d *background=nullptr;
    bool keyLeft=false;
    bool keyRight=false;
    bool keyUp=false;
    bool keyDown=false;
    bool keySpace=false;
protected:
	void init(const string& rLevelName);
	//virtual void doDestroy() override;
	virtual void doUpdate(float) override; 
public:
	GameScene(const string &rLevelName);
    void keyPressed(int key);
    void keyReleased(int key);

	using Node::Node;
};
