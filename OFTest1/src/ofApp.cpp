#include "ofApp.h"
#include "game/gamescene.h"

//--------------------------------------------------------------
void ofApp::setup(){
    GameScene *rGameScene=new GameScene("data/levels/testlevel.json");
    Director::getInstance().switchScene(rGameScene);

    bgImage.loadImage("testhg.png");

}

//--------------------------------------------------------------
void ofApp::update(){
    Director::getInstance().update();
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofDrawCircle(100.0,100.0,20.0);
    //bgImage.draw(0.0,0.0);
    Director::getInstance().draw();

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    GameScene *rGameScene=(GameScene*)Director::getInstance().getCurrentScene();
    if (rGameScene) {
        rGameScene->keyPressed(key);
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    GameScene *rGameScene=(GameScene*)Director::getInstance().getCurrentScene();
    if (rGameScene) {
        rGameScene->keyReleased(key);
    }
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){

}
