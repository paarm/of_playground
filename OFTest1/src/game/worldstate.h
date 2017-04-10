#pragma once
#include <vector>
#include "../engine/node.h"
#include "gamedirector.h"

using namespace std;


static bool intersects(const ofRectangle& rectangleBase, const ofRectangle& rectangle, ofRectangle& intersection)
{
    // Rectangles with negative dimensions are allowed, so we must handle them correctly

    // Compute the min and max of the first rectangle on both axes
    float r1MinX = std::min(rectangleBase.x, (rectangleBase.x + rectangleBase.width));
    float r1MaxX = std::max(rectangleBase.x, (rectangleBase.x + rectangleBase.width));
    float r1MinY = std::min(rectangleBase.y, (rectangleBase.y + rectangleBase.height));
    float r1MaxY = std::max(rectangleBase.y, (rectangleBase.y + rectangleBase.height));

    // Compute the min and max of the second rectangle on both axes
    float r2MinX = std::min(rectangle.x, (rectangle.x + rectangle.width));
    float r2MaxX = std::max(rectangle.x, (rectangle.x + rectangle.width));
    float r2MinY = std::min(rectangle.y, (rectangle.y + rectangle.height));
    float r2MaxY = std::max(rectangle.y, (rectangle.y + rectangle.height));

    // Compute the intersection boundaries
    float interLeft   = std::max(r1MinX, r2MinX);
    float interTop    = std::max(r1MinY, r2MinY);
    float interRight  = std::min(r1MaxX, r2MaxX);
    float interBottom = std::min(r1MaxY, r2MaxY);

    // If the intersection is valid (positive non zero area), then there is an intersection
    if ((interLeft < interRight) && (interTop < interBottom))
    {
        intersection = ofRectangle(interLeft, interTop, interRight - interLeft, interBottom - interTop);
        return true;
    }
    else
    {
        intersection = ofRectangle(0, 0, 0, 0);
        return false;
    }
}


class ObjectBase {
private:
protected:
	Node2d		*mNode2d;
    ofRectangle mLocation;
public:
    ofRectangle getLocalBounds() {
		if (mNode2d) {
			return mNode2d->getLocalBounds();
		}
        return ofRectangle();
	}
    ofRectangle getGlobalBounds() {
		if (mNode2d) {
			return mNode2d->getGlobalBounds();
		}
        return ofRectangle();
	}
#if 0	
	sf::FloatRect& getLocation() {
		if (mNode2d) {
			mLocation.left=mNode2d->getX();
			mLocation.top=mNode2d->getY();
			mLocation.width=mNode2d->getWidth();
			mLocation.height=mNode2d->getHeight();
		} else {
			mLocation.left=0.0;
			mLocation.top=0.0;
			mLocation.width=0.0;
			mLocation.height=0.0;
		}
		return mLocation;
	}
#endif	
};

class FixedObject : public ObjectBase {
private:
public:
	FixedObject(Node2d *rNode2d) {
		mNode2d=rNode2d;
	}
	void setNode2d(Node2d* rNode2d) {
		mNode2d=rNode2d;
	}
};

enum class Gravity {
	Bottom,
	Top
};

enum class Direction {
	Right,
	Left
};

class ObjectMoveable : public ObjectBase {
private:
	Gravity			mGravity=Gravity::Bottom;
	Direction		mDirection=Direction::Right;
	bool 			mTouchGround=false;
	float			mLastDeltaX=0.0;
	float			mLastDeltaY=0.0;
public:	
	void setNode2d(Node2d* rNode2d) {
		mNode2d=rNode2d;
        if (rNode2d) {
            if (rNode2d->getFlipX()) {
                mDirection=Direction::Left;
            }
            if (rNode2d->getFlipY()) {
                mGravity=Gravity::Top;
            }
        }
	}

	bool moveLeftRight(float rDeltaTime, float rSpeedPerSecond, vector<FixedObject>& rFixedObjectList) {
		bool intersected=false;
		mLastDeltaX=0.0;
		if (mNode2d) {
			if (mTouchGround) {
				float rDeltaX=(float)rSpeedPerSecond*rDeltaTime/(float)1000.0;
				if (rDeltaX>GameDirector::getInstance().getBlockSizeX()-2.0) {
					// limit move to something lower thatn 32 so that it cannot breat trought a full block
					rDeltaX=GameDirector::getInstance().getBlockSizeX()-2.0;
				}
				if (mDirection==Direction::Left) {
					rDeltaX=-rDeltaX;
                }
				if (rDeltaX!=0.0) {
					mLastDeltaX=rDeltaX;

                    ofRectangle oldPos=getGlobalBounds();
                    //oldPos.setX(oldPos.getX()+2.0);
                    //oldPos.width-=-4.0;
                    ofRectangle newPos=oldPos;
                    ofRectangle intersectionRect;
                    newPos.x=(newPos.x+getLastDeltaX());
					for(FixedObject& rFixedObject: rFixedObjectList) {
                        if (intersects(rFixedObject.getGlobalBounds(),newPos,intersectionRect)) {
							intersected=true;
							if (getDirection()==Direction::Left) {
                                mLastDeltaX+=intersectionRect.width;
							} else {
								mLastDeltaX-=intersectionRect.width;
							}
                            newPos.x=oldPos.x;
                            newPos.x=(newPos.x+getLastDeltaX());
						}
                        if ((mDirection==Direction::Left && mLastDeltaX>=0.0) || (mDirection==Direction::Right && mLastDeltaX<=0.0)) {
                            mLastDeltaX=0.0;
                            // ok nothing more to correct - dont need to check other static objects
                            break;
                        }
#if 0
                        if (mLastDeltaX==0.0) {
							// ok nothing more to correct - dont need to check other static objects
							break;
						}
#endif
					}
					mNode2d->setPositionRelative(mLastDeltaX, 0.0);
					if (mNode2d->getX()<0.0) {
						mNode2d->setPosition(0.0, mNode2d->getY());
						intersected=true;
					} else if (mNode2d->getX()+mNode2d->getWidth()>GameDirector::getInstance().getVirtualScreenSizeX()) {
						mNode2d->setPosition(GameDirector::getInstance().getVirtualScreenSizeX()-mNode2d->getWidth(), mNode2d->getY());
						intersected=true;
					}
				}
			}
		}
		return intersected;
	}

	void setPositionRelative(float rDeltaX, float rDeltaY) {
		if (mNode2d) {
			mNode2d->setPositionRelative(rDeltaX, rDeltaY);
		}
	}

	void setPosition(float rX, float rY) {
		if (mNode2d) {
			mNode2d->setPosition(rX, rY);
		}
	}

	bool moveTopBottom(float rDeltaTime, float rSpeedPerSecond, vector<FixedObject>& rFixedObjectList) {
		bool intersected=false;
		if (mNode2d) {
			//if (!mTouchGround) {
				float rDeltaY=(float)rSpeedPerSecond*rDeltaTime/(float)1000.0;
				if (rDeltaY>GameDirector::getInstance().getBlockSizeY()-2.0) {
					// limit move to something lower thatn 32 so that it cannot breat trought a full block
					rDeltaY=GameDirector::getInstance().getBlockSizeY()-2.0;
				}
				// move to top direction?
				if (mGravity==Gravity::Top) {
					rDeltaY=-rDeltaY;
				}
				// anyone moved to top or bottom?
				if (rDeltaY!=0.0) {
					mLastDeltaY=rDeltaY;

                    ofRectangle oldPos=getGlobalBounds();
                    //oldPos.x+=4.0;
                    //oldPos.width-=8.0;
                    ofRectangle newPos=oldPos;
                    ofRectangle intersectionRect;
                    newPos.y=(newPos.y+getLastDeltaY());
					// check all static objects for collision now
					for(FixedObject& rFixedObject: rFixedObjectList) {
                        if (intersects(rFixedObject.getGlobalBounds(),newPos,intersectionRect)) {
                            // ok have collision - remove the overlap from the move delta and continue with other static objects
							intersected=true;
							if (getGravity()==Gravity::Top) {
								mLastDeltaY+=intersectionRect.height;
							} else {
								mLastDeltaY-=intersectionRect.height;
							}
							// continue with a fresh position from the corrected move delta
                            newPos.y=oldPos.y;
                            newPos.y=(newPos.y+getLastDeltaY());
						}
                        if ((mGravity==Gravity::Top && mLastDeltaY>=0.0) || (mGravity==Gravity::Bottom && mLastDeltaY<=0.0)) {
                            mLastDeltaY=0.0;
                            // ok nothing more to correct - dont need to check other static objects
                            break;
                        }
#if 0
                        if (mLastDeltaY==0.0) {
							// ok nothing more to correct - dont need to check other static objects
							break;
						}
#endif
					}
					mNode2d->setPositionRelative(0.0, mLastDeltaY);

					if (!intersected) {
						// rollover to top or bottom of the screen if required
						if (mGravity==Gravity::Top && mNode2d->getY()<0.0-mNode2d->getHeight()) {
							mNode2d->setPosition(mNode2d->getX(), GameDirector::getInstance().getVirtualScreenSizeY()+mNode2d->getHeight());
						} else if (mGravity==Gravity::Bottom && mNode2d->getY()>GameDirector::getInstance().getVirtualScreenSizeY()) {
							mNode2d->setPosition(mNode2d->getX(), 0.0-mNode2d->getHeight());
						}
					}
					if (!intersected) {
						setGroundTouched(false);
					} else {
						// touched, so can move to right or left from now on
						setGroundTouched(true);
					}
				}
			//}
		}
		return intersected;
	}

	bool changeGravityToTop() {
		bool rGravityChanged=false;
		if (mNode2d && mTouchGround && mGravity==Gravity::Bottom) {
			mNode2d->setFlipY(true);
			mGravity=Gravity::Top;
			mTouchGround=false;
			rGravityChanged=true;
		}
		return rGravityChanged;
	}
	bool changeGravityToBottom() {
		bool rGravityChanged=false;
		if (mNode2d && mTouchGround && mGravity==Gravity::Top) {
			mNode2d->setFlipY(false);
			mGravity=Gravity::Bottom;
			mTouchGround=false;
			rGravityChanged=true;
		}
		return rGravityChanged;
	}
	void setGroundTouched(bool rGroundTouched) {
		mTouchGround=rGroundTouched;
	}
	bool isGroundTouched() {
		return mTouchGround;
	}
	void setDirection(Direction rDirection) {
		mDirection=rDirection;
	}
	void switchDirection() {
		if (mDirection==Direction::Left) {
			mDirection=Direction::Right;
            mNode2d->setFlipX(false);
		} else {
			mDirection=Direction::Left;
            mNode2d->setFlipX(true);
        }
	}
	Direction getDirection() {
		return mDirection;
	}
	Gravity getGravity() {
		return mGravity;
	}
	float getLastDeltaX() {
		return mLastDeltaX;
	}
	float getLastDeltaY() {
		return mLastDeltaY;
	}
	void resetMoveDeltas() {
		mLastDeltaX=0.0;
		mLastDeltaY=0.0;
	}

};

class EnemyObject : public ObjectMoveable {
private:
public:
	EnemyObject(Node2d *rNode2d) {
        setNode2d(rNode2d);
    }
};

class JumpObject : public ObjectMoveable {
private:
	bool 	mPickable=false;
public:
	JumpObject(Node2d *rNode2d) {
        setNode2d(rNode2d);
    }
	void setPickable(bool rPickable) {
		mPickable=rPickable;
	}
	bool isPickable() {
		return mPickable;
	}
};

class PlayerObject : public ObjectMoveable {
private:
	JumpObject 	*mJumpObjectPicked=nullptr;
	float		mPickedTimeDelayMs=0.0;
public:
	void setPickedGravity(JumpObject *rJumpObject) {
		if (rJumpObject) {
			mJumpObjectPicked=rJumpObject;
			rJumpObject->setPosition(-1000.0,-1000.0);
		} else {
			mJumpObjectPicked=nullptr;
		}
		mPickedTimeDelayMs=300.0;
	}
	void updatePickedDelta(float rDeltaTime) {
		if (mPickedTimeDelayMs>0.0) {
			mPickedTimeDelayMs-=rDeltaTime;
			if (mPickedTimeDelayMs<0.0) {
				mPickedTimeDelayMs=0.0;
			}
		}
	}
	bool isPickedDelayTimeExpired() {
		return mPickedTimeDelayMs<=0.0;
	}
	JumpObject*	getPickedGravity() {
		return mJumpObjectPicked;
	}
};

class WorldState {
private:
	vector<FixedObject> mFixedObjectList;
	vector<EnemyObject> mEnemyObjectList;
	vector<JumpObject> mJumpObjectList;
	PlayerObject 		mPlayerObject;
    ofRectangle		mIntersectionRect;
public:
	void setPlayerObject(Node2d *rNode2d) {
		mPlayerObject.setNode2d(rNode2d);
	}
	void addFixedObject(Node2d *rNode2d) {
		mFixedObjectList.emplace_back(rNode2d);
	}
	void addEnemyObject(Node2d *rNode2d) {
		mEnemyObjectList.emplace_back(rNode2d);
	}
	void addJumpObject(Node2d *rNode2d, bool rPickable) {
		mJumpObjectList.emplace_back(rNode2d);
		mJumpObjectList.back().setPickable(rPickable);
	}

    bool intersectWithFixedObject(ofRectangle &rTestRect, ofRectangle &intersectionRect) {
		bool rv=false;
		for(FixedObject& rFixedObject: mFixedObjectList) {
            if (intersects(rFixedObject.getGlobalBounds(),rTestRect,intersectionRect)) {
				rv=true;
			}
		}
		return rv;
	}

	void update(float deltaTime, bool keyLeft, bool keyRight, bool keyUp, bool keyDown, bool keyPick) {
		if (keyLeft || keyRight) {
			if (keyLeft) {
                if (mPlayerObject.getDirection()==Direction::Right) {
                    mPlayerObject.switchDirection();
                }
			}
			if (keyRight) {
                if (mPlayerObject.getDirection()==Direction::Left) {
                    mPlayerObject.switchDirection();
                }
            }
			mPlayerObject.moveLeftRight(deltaTime, GameDirector::getInstance().getPlayerSpeedLeftRightPerSecond(), mFixedObjectList);
		}
		if (keyUp) {
			mPlayerObject.changeGravityToTop();
		} else if (keyDown) {
			mPlayerObject.changeGravityToBottom();
		}
		mPlayerObject.moveTopBottom(deltaTime, GameDirector::getInstance().getPlayerSpeedTopBottomPerSecond(), mFixedObjectList);
		mPlayerObject.resetMoveDeltas();
		

		// pick up a gravity block or lay it down
		mPlayerObject.updatePickedDelta(deltaTime);
		if (keyPick && mPlayerObject.isGroundTouched()) {
            ofRectangle rPlayerPos=mPlayerObject.getGlobalBounds();
			if (mPlayerObject.getGravity()==Gravity::Bottom) {
                rPlayerPos.y+=5.0;
			} else {
                rPlayerPos.y-=5.0;
			}
			if (mPlayerObject.isPickedDelayTimeExpired()) {
				if (!mPlayerObject.getPickedGravity()) {
					for (JumpObject &rJumpObject : mJumpObjectList) {
						// only allowed to pick a gravity block if it is allowed to pick this block
						if (rJumpObject.isPickable()) {
                            ofRectangle intersectionRect;
                            ofRectangle rJumpObjectLocation=rJumpObject.getGlobalBounds();
							float rJumpObjectWidht=rJumpObject.getLocalBounds().width;
                            if (intersects(rJumpObjectLocation, rPlayerPos,intersectionRect)) {
                                if (intersectionRect.width>rJumpObjectWidht*0.6) {
									// player stands on a gravity block at least 60 percent -> pick it up
									mPlayerObject.setPickedGravity(&rJumpObject);
								}
							}
						}
					}
				} else {
					JumpObject *rJumpObject=mPlayerObject.getPickedGravity();
					if (rJumpObject) {
                        ofRectangle rJumpObjLocals=mPlayerObject.getLocalBounds();
                        ofRectangle rPlayerPos=mPlayerObject.getGlobalBounds();
						// init pos x with the middle of the players x location
                        int rNewPosX=(int)(rPlayerPos.x+rPlayerPos.width/2.0);
						// snap to grid 
						rNewPosX-=rNewPosX%(int)rJumpObjLocals.width;
						float rNewPosY=0.0;
						if (mPlayerObject.getGravity()==Gravity::Bottom) {
							// pos y is next to the bottom of the player
                            rNewPosY=rPlayerPos.y+rPlayerPos.height;
						} else {
							// pox y is next to the y location of the player minus the gravity block y size 
                            rNewPosY=rPlayerPos.y-rJumpObjLocals.height;
						}
                        rJumpObjLocals.x=(float)rNewPosX;
                        rJumpObjLocals.y=rNewPosY;
                        ofRectangle intersectionRect;
						// check if there is really a fixed object behind the new location of the gravity block
						if (intersectWithFixedObject(rJumpObjLocals, intersectionRect)) {
							rJumpObject->setPosition(rNewPosX,rNewPosY);
							mPlayerObject.setPickedGravity(nullptr);
						}
					}
				}
			}
		}

		// move enemies
		for (EnemyObject &rEnemyObject : mEnemyObjectList) {
			bool hadLeftRightCollision=rEnemyObject.moveLeftRight(deltaTime, GameDirector::getInstance().getEnemyLeftRightSpeedPerSecond(), mFixedObjectList);
            //if (hadLeftRightCollision) {
            //	rEnemyObject.switchDirection();
            //}
			bool hadTopBottomCollision=rEnemyObject.moveTopBottom(deltaTime, GameDirector::getInstance().getEnemyTopBottomSpeedPerSecond(), mFixedObjectList);
            if (rEnemyObject.isGroundTouched()) {
                if (hadLeftRightCollision) {
                    rEnemyObject.switchDirection();
                }
            }


            rEnemyObject.resetMoveDeltas();
			// test jump objects
			if (rEnemyObject.isGroundTouched()) {
				for (JumpObject &rJumpObject : mJumpObjectList) {
                    ofRectangle rEnemyPos=rEnemyObject.getGlobalBounds();
                    ofRectangle intersectionRect;
					if (rEnemyObject.getGravity()==Gravity::Bottom) {
                        rEnemyPos.y+=5.0;
					} else {
                        rEnemyPos.y-=5.0;
					}
				
					float rJumpObjectWidht=rJumpObject.getLocalBounds().width;
					float rEnemyObjectWidth=rEnemyObject.getLocalBounds().width;
                    ofRectangle rJumpObjectLocation=rJumpObject.getGlobalBounds();
                    if (intersects(rJumpObjectLocation, rEnemyPos,intersectionRect)) {
						if (intersectionRect.width>rJumpObjectWidht*0.9) {
                            float snappedX=rJumpObjectLocation.x+rJumpObjectWidht/2.0-rEnemyObjectWidth/2.0;
                            if (snappedX!=rEnemyPos.x) {
                                rEnemyObject.setPositionRelative(snappedX-rEnemyPos.x, 0.0);
							}
							
							if (rEnemyObject.getGravity()==Gravity::Bottom) {
								rEnemyObject.changeGravityToTop();
							} else {
								rEnemyObject.changeGravityToBottom();
							}
							break;
						}
					}
				}
			}
		}
	}
};
