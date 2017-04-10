#pragma once
#include <string>
#include <vector>
#include <algorithm>
#include <iostream>
#include <ofMain.h>

#include <math.h>
#include "texturemanager.h"
//#include "fontmanager.h"


enum class NodeType {
	Node=0,
	Sprite,
	Text,
	FramePlayer,
	Root
};

class NodeBase {
public:
	virtual NodeType getNodeType()=0;
	virtual void setTextureFrame(TextureFrame *rTextureFrame)=0;
protected:
};

class Node : NodeBase {
private:
	Node 	*mParentNode=nullptr;
	string 	mName;
	string 	mHint="";
	bool 	mScheduledUpdate=false;
	void 	initChildNodeList();
protected:
	vector<Node*> *mChildNodes=nullptr;
	vector<Node*> *mChildToDelete=nullptr;

    virtual void	drawInternal(const ofMatrix4x4& parentTransform);
	virtual void 	updateInternal(float deltaTime);

	virtual void doUpdate(float deltaTime);
	virtual void doDestroy();
public:
	Node(const string &name);
	Node();
	virtual ~Node();
	Node* addNode(Node *n);
	void deleteMeScheduled();
	void scheduleChildToDelete(Node *rToDeleteNode);
	void deleteNode(Node *n);
	void deleteChilds();
	Node* searchNode(const string&name, bool searchInSub=false);
    const vector<Node*>* getChildNodeList();
	void debugPrint();
	void scheduleUpdate(bool rScheduleUpdate) {
		mScheduledUpdate=rScheduleUpdate;
	}
	void setHint(const string&rHint) {mHint=rHint;}
	string& getHint() {return mHint;}
	virtual NodeType getNodeType() override { return NodeType::Node;}
	virtual void setTextureFrame(TextureFrame *rTextureFrame) override {}
};

enum class PlayerType {
    Nonex,
	ForwardLoop,
	BackwardLoop,
	Forward,
	Backward
};

class FramePlayer;


class NodeRoot : public Node {
private:
public:
	NodeRoot(const string& rName) : Node(rName) {}
	virtual NodeType getNodeType() override { return NodeType::Root;}
    void	executeDraw(const ofMatrix4x4& parentTransform) {
        drawInternal(parentTransform);
	}
	void 	executeUpdate(float deltaTime) {
		updateInternal(deltaTime);
	}
};
#if 0
class NodeText : public Node {
private:
	sf::Text		mText;
	FontContext		*mFontContext=nullptr;
protected:
	//virtual void updateInternal(float deltaTime) override;
    virtual void drawInternal(const ofMatrix4x4& parentTransform) override;
public:
	NodeText(const string &name, FontContext *rFontContext, int x, int y);
	void setText(const string &rText) {
		mText.setString(rText);
	}

	float getX();
	float getY();
	//int	getWidth();
	//int	getHeight();
	void setPosition(float, float);
	void setPositionRelative(float distanceX, float distanceY);
	void setScale(float, float);
	void scaleRelative(float, float);
	void setRotation(float);
	void setRotationRelative(float rAddAngle);
	float getAngle();


	virtual NodeType getNodeType() override { return NodeType::Text;}
};
#endif
class Node2d : public Node {
private:
	TextureFrame 	*mTextureFrame=nullptr;
	FramePlayer  	*mFramePlayer=nullptr;
	bool			mFlipX=false;
	bool			mFlipY=false;
    float           mX=0;
    float           mY=0;
    float           mScaleX=1.0;
    float           mScaleY=1.0;
    float           mOriginX=0;
    float           mOriginY=0;
    float           mRotate=0;
    ofMatrix4x4     mTransform;
#if 0
    int             mTextureRectX;
    int             mTextureRectY;
    int             mTextureRectDX;
    int             mTextureRectDY;
#endif
protected:
	virtual void updateInternal(float deltaTime) override;
    virtual void drawInternal(const ofMatrix4x4& parentTransform) override;
public:
	Node2d(const string &name, int x, int y);
	Node2d(const string &name, TextureFrame *rTextureFrame, int x, int y);
	Node2d(const string &name, TextureFrame *rTextureFrame, int x, int y, int w, int h);
	float getX();
	float getY();
	int	getWidth();
	int	getHeight();
	void setPosition(float, float);
	void setPositionRelative(float distanceX, float distanceY);
	void setScale(float, float);
	void scaleRelative(float, float);
	void setRotation(float);
	void setRotationRelative(float rAddAngle);
	float getAngle();
	void setOriginMiddle();
	void setOriginFactor(float rOriginFactorX, float rOriginFactorY);
	virtual NodeType getNodeType() override { return NodeType::Sprite;}
	void setTextureFrame(TextureFrame*) override;
    void activateFramePlayer(const string &name, float rDelayInMs=-1.0, PlayerType playerType=PlayerType::Nonex);
	bool isFramePlayerFinish();
	void deactivateFramePlayer();
	void setFlipX(bool rFlipX);
	void setFlipY(bool rFlipY);
    bool getFlipX();
    bool getFlipY();
    ofMatrix4x4 & getTransform() {
        return mTransform;
    }

    ofRectangle getLocalBounds();
    ofRectangle getGlobalBounds();
};


class FramePlayer : public Node {
private:
	PlayerType		mPlayerType;
	int				mCurrentFrame=0;
	float			mDelayInMs;
	FrameSequence 	*mFrameSequence;
	TextureFrame	*mCurrentTextureFrame=nullptr;
	float 			mCurrentDelayTime=0.0;
	bool			mIsFinish=false;
public:
	FramePlayer(const string& name, FrameSequence *rFrameSequence, float rDelayInMs, PlayerType playerType=PlayerType::ForwardLoop) : Node(name) {
		mPlayerType=playerType;
		mFrameSequence=rFrameSequence;
		mDelayInMs=rDelayInMs;
		mCurrentTextureFrame=mFrameSequence->getTextureFrame(mCurrentFrame);
		resetPlayer();
	}	
	void resetPlayer() {
		if (mPlayerType==PlayerType::Backward) {
			mCurrentFrame=getLastFrameIndex();
		} else {
			mCurrentFrame=0;
		}
		mIsFinish=false;
		mCurrentDelayTime=0.0;
	}
	void setPlayerType(PlayerType rPlayerType) {
		mPlayerType=rPlayerType;
	}
	void setDelayInMs(float rDelayInMs) {
		mDelayInMs=rDelayInMs;
	}

	void addFrame() {
		if (mCurrentFrame<mFrameSequence->countFrames()-1) {
			mCurrentFrame++;
		} else {
			mIsFinish=true;
		}
	}
	void subFrame() {
		if (mCurrentFrame>0) {
			mCurrentFrame--;
		} else {
			mIsFinish=true;
		}
	}
	void addFrameLoop() {
		mCurrentFrame++;
		if (mCurrentFrame>=mFrameSequence->countFrames()) {
			mCurrentFrame=0;
		}
	}
	void subFrameLoop() {
		mCurrentFrame--;
		if (mCurrentFrame<0) {
			mCurrentFrame=mFrameSequence->countFrames()-1;
		}
	}
	int getLastFrameIndex() {
		return mFrameSequence->countFrames()-1;
	}
	void updateFramePlayer(float deltaTime) {
		if (!mIsFinish) {
			mCurrentDelayTime+=deltaTime;
			if (mCurrentDelayTime>mDelayInMs) {
				mCurrentDelayTime=0.0;
				if (mPlayerType==PlayerType::ForwardLoop) {
					addFrameLoop();
				} else if (mPlayerType==PlayerType::BackwardLoop) {
					subFrameLoop();
				} else if (mPlayerType==PlayerType::Forward) {
					addFrame();
				} else if (mPlayerType==PlayerType::Backward) {
					subFrame();
				}
			}
		} 
	}
	bool isFinish() {
		return mIsFinish;
	}
	TextureFrame* getCurrentTextureFrame() {
		return mFrameSequence->getTextureFrame(mCurrentFrame);
	}
	virtual NodeType getNodeType() override { return NodeType::FramePlayer;}
	
};
