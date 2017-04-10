#include "ofMain.h"
#include "ofApp.h"
#include "engine/director.h"
#include "game/gamedirector.h"

//========================================================================
int main( ){
    Director::getInstance().getInstance().initialize(GameDirector::getInstance().getVirtualScreenSizeX(), GameDirector::getInstance().getVirtualScreenSizeY());

    // this kicks off the running of my app
    // can be OF_WINDOW or OF_FULLSCREEN
    // pass in width and height too:
    ofRunApp(new ofApp());
}
