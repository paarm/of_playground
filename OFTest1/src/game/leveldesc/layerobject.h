#pragma once
#include <string>
#include "objectdesc.h"

using namespace std;
class LayerObject {
private:
	string 	mLayerObjectName;
	string 	mObjectDescId;
	int		mPosX=0;
	int		mPosY=0;
	float	mRotation=0;
	float	mOriginFactorX=0.0;
	float	mOriginFactorY=0.0;
	bool 	mFlipX=false;
	bool	mFlipY=false;
public:
	LayerObject(const string &rLayerObjectName) {
		mLayerObjectName=rLayerObjectName;
	}
	bool isValid() {
		return !mLayerObjectName.empty();
	}
	void setName(const string &rName) {
		mLayerObjectName=rName;
	}
	const string& getLayerObjectName() {
		return mLayerObjectName;
	}
	void setObjectDescId(const string &rObjectDescId) {
		mObjectDescId=rObjectDescId;
	}
	const string& getObjectDescId() {
		return mObjectDescId;
	}
	void setPosX(int rPosX) {
		mPosX=rPosX;
	}
	void setRotation(float rRotation) {
		mRotation=rRotation;
	}
	void setOriginFactorX(float rOriginFactorX) {
		mOriginFactorX=rOriginFactorX;
	}
	void setOriginFactorY(float rOriginFactorY) {
		mOriginFactorY=rOriginFactorY;
	}
	int getPosX() {
		return mPosX;
	}
	void setPosY(int rPosY) {
		mPosY=rPosY;
	}
	int getPosY() {
		return mPosY;
	}
	float getRotation() {
		return mRotation;
	}
	float getOriginFactorX() {
		return mOriginFactorX;
	}
	float getOriginFactorY() {
		return mOriginFactorY;
	}
	void setFlipX(bool rFlipX) {
		mFlipX=rFlipX;
	}
	void setFlipY(bool rFlipY) {
		mFlipY=rFlipY;
	}
	bool getFlipX() {
		return mFlipX;
	}
	bool getFlipY() {
		return mFlipY;
	}
};