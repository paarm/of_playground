#if 0
#pragma once

#include "ofApp.h"
#include <map>
#include <iostream>

using namespace std;
struct FontContext {
    sf::Font mFont;
};

class FontManager {
private:
    map<string, FontContext*> mFontContextMap;
public:
    static FontManager& getInstance() {
        static FontManager mInstance;
        return mInstance;
    }
    ~FontManager() {
        for(const auto &it : mFontContextMap) {
            std::cout << "Font destroyed: " << it.first << std::endl;
            delete it.second;
        }
        mFontContextMap.clear();
    }

    FontContext* getFont(const string& rAlias) {
        const auto& itAlias=mFontContextMap.find(rAlias);
        if (itAlias!=mFontContextMap.end()) {
            return itAlias->second;
        }
        return nullptr;
    }

    FontContext* addFont(const string& rPath, const string &rAlias) {
        FontContext*rv=getFont(rAlias);
        if (rv==nullptr) {
            FontContext* rFontContext=new FontContext();
            if (rFontContext->mFont.loadFromFile(rPath)) {
                mFontContextMap[rAlias]=rFontContext;
                rv=rFontContext;
            } else {
                delete rFontContext;
            }
        }
        return rv;
    }
};
#endif
