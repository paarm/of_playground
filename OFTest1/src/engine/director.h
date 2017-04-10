#pragma once
#include "ofApp.h"
#include "texturemanager.h"
#include "node.h"

class Director {
private:
	//SDL_Renderer 	*mSDL_Renderer=nullptr;
	bool 			mIsInitialized=false;
	bool			mIsRunning=false;
	NodeRoot		mRootScene {"Root scene"};
	Node			*mStageScene=nullptr;
	Node			*mCurrentScene=nullptr;
	//CustomMouseIcon mCustomMouseIcon;
	Director();
	Director(const Director&) = delete;
	Director& operator=(const Director&)=delete;
public:
	static Director& getInstance() {
		static Director instance; 
		return instance;
	}
	~Director();
	void initialize(int rVirtualScreenResolutionX, int rVirtualScreenResolutionY);
    void update();
    void draw();
    void switchScene(Node *n);
    Node* getCurrentScene();
    //void runWithNode(Node *n);
	//void setMousePointer(SDL_Texture *rSDL_Texture, MousePointerAlignment rMousePointerAlignment=MousePointerAlignment::LeftTop);
};
