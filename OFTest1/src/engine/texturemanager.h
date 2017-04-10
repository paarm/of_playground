#pragma once
#include <string>
#include <iostream>
#include <map>
#include <vector>
#include "ofMain.h"
#include <algorithm>

struct Frame {
    int x;
    int y;
    int dx;
    int dy;
};

struct TextureFrame {
	string		mFrameName;
	Frame 		mFrame;
    ofImage   *mSDL_Texture;
public:
	bool 		isValid() {
		return !mFrameName.empty();
	}
	string&		getFrameName() {
		return mFrameName;
	}
};

class FrameSequence {
private:
    vector<TextureFrame*> mTextureFrame;

public:
    FrameSequence() {}
    void addTextureFrame(TextureFrame *f) {
		TextureFrame* rTextureFrameExist=getTextureFrameByName(f->getFrameName());
		if (!rTextureFrameExist) {
			mTextureFrame.push_back(f);
		}
	}
    TextureFrame* getTextureFrameByName(const string& rFrameName) {
		auto it=find_if(mTextureFrame.begin(), mTextureFrame.end(), [&rFrameName](TextureFrame* r) {
			return r->getFrameName()==rFrameName;
		});
		if (it!=mTextureFrame.end()) {
			return *it;
		}
		return nullptr;
    }

    TextureFrame* getTextureFrame(int rIndex) {
        return mTextureFrame[rIndex];
    }
	int countFrames () {
        return mTextureFrame.size();
    }
};


class TextureManager {
private:
    map<string, ofImage   *> mTextureMap;
	map<string, string> mAliasMap;
	map<string, TextureFrame>mFrameMap;
    map<string,	FrameSequence> mFrameSequenceMap;
	
	string* getPathNameFromAlias(const string *alias);
	void keepSourceInTextureRange(Frame &rFrame, int textX, int textY) {
		rFrame.x=max(0,rFrame.x);
		rFrame.x=min(textX,rFrame.x);
		rFrame.y=max(0,rFrame.y);
		rFrame.y=min(textY,rFrame.y);
		// todo: proof dx, dy
	}	
public:
	TextureManager();
	~TextureManager();
	TextureManager(const TextureManager&) = delete;
	void operator=(const TextureManager&) = delete;
	void reset();
    ofImage  * loadTexture(const string &pathName, const string &alias="");
    ofImage  * getTexture(const string &name);
	TextureFrame *getTextureFrame(const string &frameName);
	FrameSequence *createAutomaticFramesAndSequence(const string &textureName, const string &rSequenceName, int xFrameCount, int yFrameCount=1);
    FrameSequence *addAutomaticFramesToFrameSequence(const string& rSequenceName, int count);
	int addFramesAutomaticallyToTexture(const string &textureName, const string &rSequenceName, int xFrameCount, int yFrameCount=1);
	TextureFrame *addFrameToTexture(const string &textureName, const string &frameName, int x, int y, int dx, int dy);
    FrameSequence *addFrameSequence(const string& rSequenceName);
    bool isFrameSequenceExist(const string &rSequenceName);
    FrameSequence *getFrameSequence(const string& rSequenceName);
};
