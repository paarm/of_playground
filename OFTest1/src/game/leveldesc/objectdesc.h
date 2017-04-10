#pragma once
#include <string>
#include <map>
#include <vector>
#include <algorithm>

using namespace std;

class ObjectSequence {
private:
	string		mSequenceName;
	int			mFrameDelayInMs;
public:
	ObjectSequence(const string& rSequenceName) {
		mSequenceName=rSequenceName;
	}
	bool isValid() {
		return !mSequenceName.empty();
	}
	string& getSequenceName() {
		return mSequenceName;
	}
	int getFrameDelayInMs() {
		return mFrameDelayInMs;
	}
	void setFrameDelayInMs(int rFrameDelayInMs) {
		mFrameDelayInMs=rFrameDelayInMs;
	}
};

class ObjectDesc {
private:
	string 		mId;
	string		mObjectHint="";
	string		mDefaultFrame;
	string 		mDefaultSequenceName;
	vector<ObjectSequence> mObjectSequenceList;
	ObjectSequence mEmptyObjectSequence={""};
public:
	ObjectDesc(const string& rId) {
		mId=rId;
	}
	bool isValid() {
		return !mId.empty();
	}
	void setObjectHint(string rObjectHint) {
		mObjectHint=rObjectHint;
	}
	void setDefaultFrame(string &rDefaultFrame) {
		mDefaultFrame=rDefaultFrame;
	}
	void setDefaultSequence(string &rDefaultSequenceName) {
		mDefaultSequenceName=rDefaultSequenceName;
	}
	ObjectSequence& getOrAddObjectSequence(const string &rSequenceName) {
		ObjectSequence& rObjectSequenceExist=getObjectSequenceByName(rSequenceName);
		if (!rSequenceName.empty() && !rObjectSequenceExist.isValid()) {
			mObjectSequenceList.emplace_back(rSequenceName);
			return mObjectSequenceList.back();
		}
		return rObjectSequenceExist;
	}

	ObjectSequence &getObjectSequenceByName(const string &sequenceName) {
		auto it=std::find_if(mObjectSequenceList.begin(), mObjectSequenceList.end(), [&sequenceName](ObjectSequence &r) {
			return r.getSequenceName()==sequenceName;
		});
		if (it!=mObjectSequenceList.end()) {
			return *it;
		}
		return mEmptyObjectSequence;
	}

	ObjectSequence &getObjectSequenceByIndex(int rIndex) {
		if (mObjectSequenceList.size()>rIndex && rIndex>=0) {
			return mObjectSequenceList.at(rIndex);
		}
		return mEmptyObjectSequence;
	}

	int getObjectSequenceCount() {
		return mObjectSequenceList.size();
	}

	string& getId() {
		return mId;
	}
	string& getObjectHint() {
		return mObjectHint;
	}
	string& getDefaultFrame() {
		return mDefaultFrame;
	}
	string& getDefaultSequenceName() {
		return mDefaultSequenceName;
	}
};