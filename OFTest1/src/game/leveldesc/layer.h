#pragma once
#include <vector>
#include <map>
#include "layerobject.h"
#include <algorithm>
using namespace std;

class Layer {
private:
	string  mLayerName;
	int     mZOrder=0;
	vector<LayerObject> mLayerObjectList;
	LayerObject	mEmptyLayerObject{""};
public:
	Layer(const string& rLayerName) {
		mLayerName=rLayerName;
	}
	bool isValid() {
		return !mLayerName.empty();
	}
	const string& getLayerName() {
		return mLayerName;
	}
	void setZOrder(int rZOrder) {
		mZOrder=rZOrder;
	}
	int getZOrder() {
		return mZOrder;
	}

	LayerObject& getOrAddLayerObject(string &rLayerObjectName) {
		LayerObject &rLayerObjectExist=getLayerObjectByName(rLayerObjectName);
		if (!rLayerObjectName.empty() && !rLayerObjectExist.isValid()) {
			mLayerObjectList.emplace_back(rLayerObjectName);
			return mLayerObjectList.back();
		}
		return rLayerObjectExist;
	}

	LayerObject& getLayerObjectByName(string &rLayerObjectName) {
		auto r=find_if(mLayerObjectList.begin(), mLayerObjectList.end(), [&rLayerObjectName] (LayerObject &r) {
			return r.getLayerObjectName()==rLayerObjectName;
		});
		if (r!=mLayerObjectList.end()) {
			return *r;
		}
		return mEmptyLayerObject;
	}

	int getLayerObjectCount() {
		return mLayerObjectList.size();
	}

	LayerObject& getLayerObjectByIndex(int rIndex) {
		LayerObject& rv=mEmptyLayerObject;
		if ((int)mLayerObjectList.size()>rIndex && rIndex>=0) {
			rv=mLayerObjectList[rIndex];
		}
		return rv;
	}
};

