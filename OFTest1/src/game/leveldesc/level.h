#pragma once

#include <vector>
#include "objectdesc.h"
#include "layer.h"
#include <iostream>
#include <algorithm>
#include "../../engine/texturemanager.h"

using namespace std;
class Level {
private:
	string 		mAssetPath="assets/";
	string 		mLevelName="";
	map<string, ObjectDesc> mObjectDescMap;
	vector<Layer> mLayerList;
	Layer		mEmptyLayer{""};
	ObjectDesc	mEmptyObjectDesc{""};
	TextureManager mTextureManager;
	int			mLevelObjectNameNbPool=0;
public:
	void setAssetPath(const string &rAssetPath);
	string &getAssetPath();
	void setBackgroundImageName(const string &rBackgroundImageName);
	string &getBackgroundImageName();
	ObjectDesc& getOrAddObjectDesc(const string &rId);
	ObjectDesc& getObjectDescByName(const string &rId);
	bool isObjectDescAvailable(const string &rId);

	Layer& getOrAddLayer(const string &rLayerName);
	Layer& getLayerByName(const string &rLayerName);
	int getLayerCount();
	Layer& getLayerFromIndex(int rIndex);
	TextureManager& getTextureManager() {
		return mTextureManager;
	}
	int	getNextLevelObjectNameNb();
	void clearLevel();
	void setLevelName(const string &rLevelName);
	string& getLevelName();
};
