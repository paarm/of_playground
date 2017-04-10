#if 0
#include "node.h"

NodeText::NodeText(const string &name, FontContext *rFontContext, int x, int y) : Node(name) {
	setPosition((float)x,(float)y);
	setScale(1.0,1.0);
    mText.setString("");
    //mText.setFillColor(sf::Color::Red);
    mText.setCharacterSize(24); // in pixels, not points!
	mText.setFont(rFontContext->mFont);
    mFontContext=rFontContext;
}

void NodeText::drawInternal(sf::RenderTarget& target, const sf::Transform& parentTransform) {
	//mText.setTextureRect(sf::IntRect(mTextureFrame->mFrame.x, mTextureFrame->mFrame.y, mTextureFrame->mFrame.dx, mTextureFrame->mFrame.dy));
	sf::Transform combinedTransform = parentTransform * mText.getTransform();
	target.draw(mText, parentTransform);
	Node::drawInternal(target, combinedTransform);
}


void NodeText::setPositionRelative(float distanceX, float distanceY) {
	mText.move(sf::Vector2f(distanceX,distanceY));
}


void NodeText::setPosition(float rX, float rY) {
	mText.setPosition(sf::Vector2f(rX,rY));
}

float NodeText::getX() {
	return mText.getPosition().x;
}

float NodeText::getY() {
	return mText.getPosition().y;
}
#if 0
int NodeText::getHeight() {
	return mTextureFrame->mFrame.dx*mText.getScale().x;
}

int NodeText::getWidth() {
	return mTextureFrame->mFrame.dy*mText.getScale().y;
}
#endif

void NodeText::setScale(float rScaleX, float rScaleY) {
	mText.setScale(sf::Vector2f(rScaleX, rScaleY));
}

void NodeText::scaleRelative(float rScaleX, float rScaleY) {
	mText.scale(sf::Vector2f(rScaleX, rScaleY));
}

void NodeText::setRotation(float rAngle) {
	mText.setRotation(rAngle);
}

float NodeText::getAngle() {
	return mText.getRotation();
}

void NodeText::setRotationRelative(float rAngle) {
	mText.rotate(rAngle);
}
#endif
