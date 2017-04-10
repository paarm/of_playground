#include "director.h"

Director::Director() {
	std::cout << "Director created" << std::endl;
	mRootScene.scheduleUpdate(true);
}

Director::~Director() {
	mRootScene.deleteChilds();
	std::cout << "Director destroyed" << std::endl;
}

void Director::initialize(int rVirtualScreenResolutionX, int rVirtualScreenResolutionY) {
    ofSetupOpenGL(rVirtualScreenResolutionX,rVirtualScreenResolutionY,OF_WINDOW);			// <-------- setup the GL context
    mIsInitialized=true;
}

void Director::switchScene(Node *n) {
	mStageScene=n;
}

Node* Director::getCurrentScene() {
    return mCurrentScene;
}

void Director::update() {
    if (mIsInitialized) {
        if (mStageScene) {
            mRootScene.deleteChilds();
            mRootScene.addNode(mStageScene);
            mCurrentScene=mStageScene;
            mStageScene=nullptr;
        }

        mRootScene.executeUpdate(ofGetLastFrameTime()*1000.0);
        //WindowHandle.mSFL_Window->clear();
        //mWindowHandle.mSFL_Window->display();
    }
}

void Director::draw() {
    if (mIsInitialized) {
        mRootScene.executeDraw(ofMatrix4x4());

    }
}

#if 0

		bool quit=false;
		mIsRunning=true;
		while (!quit) {
 			clock.tick();
			sf::Event e;
			while (mWindowHandle.mSFL_Window->pollEvent(e)) {
				if (e.type == sf::Event::Closed) {
					mWindowHandle.mSFL_Window->close();
				}
			}
			if (mStageScene) {
				mRootScene.deleteChilds();
				mRootScene.addNode(mStageScene);
				mStageScene=nullptr;
			}
			mRootScene.executeUpdate(clock.getDelta());
			mWindowHandle.mSFL_Window->clear();
			mRootScene.executeDraw(*mWindowHandle.mSFL_Window, sf::Transform::Identity);
			mWindowHandle.mSFL_Window->display();
			
			if (quit==true) {
				break;
			} else {
				quit=(!mWindowHandle.mSFL_Window->isOpen());
			}
		}
		mRootScene.deleteChilds();
		mIsRunning=false;
	}
}

#endif
