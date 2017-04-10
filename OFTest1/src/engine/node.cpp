#include "node.h"

Node::Node(const string &name) {
	mName=name;
	cout << "Create Node: " << mName << endl;
}

Node::Node() : mName("") {
	cout << "Create Node: " << mName << endl;
}

Node::~Node() {
	cout << "Node destroyed: " << mName << endl;
	deleteChilds();
}

void Node::deleteChilds() {
	if (mChildNodes) {
		for(Node* n : *mChildNodes) {
			n->doDestroy();
			delete n;
		}
		delete mChildNodes;
		mChildNodes=nullptr;
	}
}

void Node::initChildNodeList() {
	if (!mChildNodes) {
		mChildNodes=new vector<Node*>();
	}
}

Node* Node::addNode(Node *n) {
	if (n) {
		n->mParentNode=this;
		initChildNodeList();
		mChildNodes->push_back(n);
	}
	return n;
}

void Node::deleteMeScheduled() {
	if (mParentNode) {
		mParentNode->scheduleChildToDelete(this);
	}
}

void Node::scheduleChildToDelete(Node *rToDeleteNode) {
	if (rToDeleteNode && mChildNodes) {
		Node *n=searchNode(rToDeleteNode->mName, false);
		if (n) {
			if (!mChildToDelete) {
				mChildToDelete=new vector<Node*>();
			}
			mChildToDelete->push_back(n);
		} 
	}
}

void Node::deleteNode(Node *n) {
	if (mChildNodes) {
		mChildNodes->erase(std::remove(std::begin(*mChildNodes), std::end(*mChildNodes), n), std::end(*mChildNodes));
		n->doDestroy();
		delete n;
	}
}

Node* Node::searchNode(const string &name, bool searchInSub) {
	Node *rv=nullptr;
	if (mChildNodes) {
		for(Node *n: *mChildNodes) {
			if (n->mName==name) {
				rv=n;
				break;
			}
		}
		if (!rv && searchInSub) {
			for(Node *n: *mChildNodes) {
				rv=n->searchNode(name, searchInSub);
				if (rv) {
					break;
				}
			}	
		}
	}
	return rv;
}

const vector<Node*>* Node::getChildNodeList() {
    return mChildNodes;
}


void Node::debugPrint() {
	cout << mName << endl;
	if (mChildNodes) {
		for(Node *n : *mChildNodes) {
			n->debugPrint();
			n->debugPrint();
		}
	}
}

void Node::doUpdate(float deltaTime) {}

void Node::updateInternal(float deltaTime) {
	if (mChildNodes && mChildToDelete && mChildToDelete->size()>0) {
		for (auto n : *mChildToDelete) {
			deleteNode(n);
		}
		mChildToDelete->clear();
	}
	if (mScheduledUpdate) {
		doUpdate(deltaTime);
	}
	if (mChildNodes) {
		for(Node *n : *mChildNodes) {
			n->updateInternal(deltaTime);
		}
	}
}

void Node::drawInternal(const ofMatrix4x4& parentTransform) {
	if (mChildNodes) {
		for(const auto &n: *mChildNodes) {
            n->drawInternal(parentTransform);
		}
	}
}

void Node::doDestroy() {}
