#include "node.h"

Node2d::Node2d(const string &name, int x, int y) : Node(name) {
	setPosition((float)x,(float)y);
	setScale(1.0,1.0);
	//setTextureFrame(rTextureFrame);
}  

Node2d::Node2d(const string &name, TextureFrame *rTextureFrame, int x, int y) : Node(name) {
	setPosition((float)x,(float)y);
	setScale(1.0,1.0);
	setTextureFrame(rTextureFrame);
}  

Node2d::Node2d(const string &name, TextureFrame *rTextureFrame, int x, int y, int w, int h) : Node2d(name, rTextureFrame, x, y) {
	setScale((float)w/(float)mTextureFrame->mFrame.dx,(float)h/(float)mTextureFrame->mFrame.dy);
}

void Node2d::setTextureFrame(TextureFrame *rTextureFrame) {
	mTextureFrame=rTextureFrame;
}

void Node2d::drawInternal(const ofMatrix4x4& parentTransform) {
    ofMatrix4x4 combinedTransform=getTransform();
    ofPushMatrix();
    if (mTextureFrame) {
		int x=mTextureFrame->mFrame.x;
		int y=mTextureFrame->mFrame.y;
		int dx=mTextureFrame->mFrame.dx;
		int dy=mTextureFrame->mFrame.dy;
        //mTextureFrame->mSDL_Texture->mirror(mFlipX, mFlipY);
        ofTranslate(mX,mY);
        if (mFlipX || mFlipY) {
            ofTranslate(mFlipX?getWidth():0.0,mFlipY?getHeight():0.0);
            ofScale(mFlipX?-1.0:1.0, mFlipY?-1.0:1.0);
            //ofTranslate(0.0,0.0);
        }


#if 0
        if (mFlipX) {
			x=x+dx;
			dx=-dx;
		}
		if (mFlipY) {
			y=y+dy;
			dy=-dy;
		}
#endif
        //setTextureRect(x, y, dx, dy);
        ofMatrix4x4 combinedTransform = parentTransform * getTransform();
        // todo set matrix
        mTextureFrame->mSDL_Texture->drawSubsection(0,0,0,dx,dy,x,y);
    }
    //target.draw(mSprite, parentTransform);
	// draw the childs with the combinedTransform matrix as parent matrix
    Node::drawInternal(combinedTransform);
    ofPopMatrix();
}

#if 0
void Node2d::setTextureRect(float x, float y, float dx, float dy) {
    mTextureRectX=x;
    mTextureRectY=y;
    mTextureRectDX=dx;
    mTextureRectDY=dy;
}
#endif

void Node2d::updateInternal(float deltaTime) {
	if (mFramePlayer) {
		mFramePlayer->updateFramePlayer(deltaTime);
		setTextureFrame(mFramePlayer->getCurrentTextureFrame());
	}
	Node::updateInternal(deltaTime);
}

void Node2d::activateFramePlayer(const string &name, float rDelayInMs, PlayerType playerType) {
	Node *n=searchNode(name);
	if (n && n->getNodeType()==NodeType::FramePlayer) {
		mFramePlayer=(FramePlayer*)n;
		if (rDelayInMs!=-1.0) {
			mFramePlayer->setDelayInMs(rDelayInMs);
		}
        if (playerType!=PlayerType::Nonex) {
			mFramePlayer->setPlayerType(playerType);
		}
		mFramePlayer->resetPlayer();
	}
}


bool Node2d::isFramePlayerFinish() {
	bool rv=true;
	if (mFramePlayer) {
		rv=mFramePlayer->isFinish();
	}
	return rv;
}
void Node2d::deactivateFramePlayer() {
	mFramePlayer=nullptr;
}

void Node2d::setPositionRelative(float distanceX, float distanceY) {
    mX+=distanceX;
    mY+=distanceY;
}


void Node2d::setPosition(float rX, float rY) {
    mX=rX;
    mY=rY;
}

float Node2d::getX() {
    return mX;
}

float Node2d::getY() {
    return mY;
}

int Node2d::getHeight() {
	if (mTextureFrame) {
        return (int)(mTextureFrame->mFrame.dx*mScaleX);
	}
	return 0;
}

int Node2d::getWidth() {
	if (mTextureFrame) {
        return (int)(mTextureFrame->mFrame.dy*mScaleY);
	}
	return 0;
}

void Node2d::setScale(float rScaleX, float rScaleY) {
    mScaleX=rScaleX;
    mScaleY=rScaleY;
}

void Node2d::scaleRelative(float rScaleX, float rScaleY) {
    mScaleX+=rScaleX;
    mScaleY+=rScaleY;
}

void Node2d::setRotation(float rAngle) {
    mRotate=rAngle;
}

void Node2d::setOriginMiddle() {
    mOriginX=getWidth()/2;
    mOriginY=getHeight()/2;
}

void Node2d::setOriginFactor(float rOriginFactorX, float rOriginFactorY) {
    mOriginX=getWidth()*rOriginFactorX;
    mOriginY=getHeight()*rOriginFactorY;
}
float Node2d::getAngle() {
    return mRotate;
}

void Node2d::setRotationRelative(float rAngle) {
    mRotate+=rAngle;
    if (mRotate>360.0) {
        mRotate-=360.0;
    } else if (mRotate<0.0) {
        mRotate+=360.0;
    }
}

void Node2d::setFlipX(bool rFlipX) {
	mFlipX=rFlipX;
}

void Node2d::setFlipY(bool rFlipY) {
	mFlipY=rFlipY;
}

bool Node2d::getFlipX() {
    return mFlipX;
}

bool Node2d::getFlipY() {
    return mFlipY;
}

ofRectangle Node2d::getLocalBounds() {
    ofRectangle r(0,0,getWidth(), getHeight());
    return r;
}

ofRectangle Node2d::getGlobalBounds() {
    ofRectangle r(mX,mY,getWidth(), getHeight());
    return r;
    // TODO return mSprite.getGlobalBounds();
}
