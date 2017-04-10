#include "level.h"

void Level::clearLevel() {
	mLevelName="";
	mObjectDescMap.clear();
	mLayerList.clear();
	mTextureManager.reset();
	mLevelObjectNameNbPool=0;
}

void Level::setLevelName(const string &rLevelName) {
	mLevelName=rLevelName;
}

string& Level::getLevelName() {
	return mLevelName;
}

ObjectDesc& Level::getOrAddObjectDesc(const string& rId) {
	ObjectDesc &rObjectDescExist=getObjectDescByName(rId);
	if (!rObjectDescExist.isValid() && !rId.empty()) {
		return mObjectDescMap.emplace(rId, rId).first->second;
		//auto y=x.first;
		//return y->second;
	}
	return rObjectDescExist;
}

bool Level::isObjectDescAvailable(const string &rId) {
	bool rv=false;
	if (mObjectDescMap.find(rId)!=mObjectDescMap.end()) {
		rv=true;
	}
	return rv;
}

ObjectDesc& Level::getObjectDescByName(const string &rId) {
	auto it=mObjectDescMap.find(rId);
	if (it!=mObjectDescMap.end()) {
		return it->second;
	}
	return mEmptyObjectDesc;
}

void Level::setAssetPath(const string &rAssetPath) {
	mAssetPath=rAssetPath;
}
string &Level::getAssetPath() {
	return mAssetPath;
}

Layer& Level::getOrAddLayer(const string &rLayerName) {
	Layer &existLayer=getLayerByName(rLayerName);
	if (!rLayerName.empty() && !existLayer.isValid()) {
		mLayerList.emplace_back(rLayerName);
		return mLayerList.back();
	}
	return existLayer;
}

Layer& Level::getLayerByName(const string &rLayerName) {
	auto foundIterator=find_if(mLayerList.begin(), mLayerList.end(), [&rLayerName] (Layer& r) {
		//cout <<"Lambda Layer Name: " << r.getLayerName() << "Name: "<< rLayerName <<endl;
		return rLayerName==r.getLayerName();
	});
	if (foundIterator!=mLayerList.end()) { 
		return *foundIterator;
	}
	return mEmptyLayer;
}

int Level::getLayerCount() {
	return mLayerList.size();
}

Layer& Level::getLayerFromIndex(int rIndex) {
	if (rIndex>=0 && rIndex<(int)mLayerList.size()) {
		return mLayerList[rIndex];
	}
	return mEmptyLayer;
}

int	Level::getNextLevelObjectNameNb() {
	return mLevelObjectNameNbPool++;
}

