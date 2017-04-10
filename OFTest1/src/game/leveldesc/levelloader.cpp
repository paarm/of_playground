#include "levelloader.h"

using namespace std;
bool LevelLoader::loadLevel(const string &rLevelName) {
	bool rv=false;

	mLevel.clearLevel();
	rv=parse(rLevelName);
	
#if 0
	for (int y=0, countY=GameDirector::getInstance().getBlockCountY();y<countY;y++) {
		Row &rRow=mLevel.addRow();
		for (int x=0, countX=GameDirector::getInstance().getBlockCountX();x<countX;x++) {
			rRow.addColumn();
		}
	}
	rv=parse(rLevelName);
	mLevel.clearLevel();
	mLevel.setAssetPath("assets/");
	mLevel.setLevelName(rLevelName);
	mLevel.setBackgroundImageName("");
#endif
	return rv;
}

Level& LevelLoader::getLevel() {
	return mLevel;
}


void LevelLoader::addLevelToNode(Node &rParentNode) {
	Level &rLevel=getLevel();
	for (int i=0, count=rLevel.getLayerCount();i<count;i++) {
		Layer &rLayer=rLevel.getLayerFromIndex(i);
		if (rLayer.isValid()) {
			Node2d *rLayerNode=(Node2d*)rParentNode.addNode(new Node2d(rLayer.getLayerName(), 0,0));
			for (int o=0, counto=rLayer.getLayerObjectCount();o<counto;o++) {
				LayerObject& rLayerObject=rLayer.getLayerObjectByIndex(o);
				if (rLayerObject.isValid()) {
					ObjectDesc &rObjectDesc=rLevel.getObjectDescByName(rLayerObject.getObjectDescId());
					if (rObjectDesc.isValid()) {
						TextureFrame *rTextureFrame=rLevel.getTextureManager().getTextureFrame(rObjectDesc.getDefaultFrame());
						Node2d *rNode2d=nullptr;
						if (rTextureFrame) {
							string name=rLayerObject.getLayerObjectName();
							rNode2d=(Node2d*)rLayerNode->addNode(new Node2d(name, rTextureFrame, rLayerObject.getPosX(), rLayerObject.getPosY()));
							if (rLayerObject.getRotation()!=0.0) {
								rNode2d->setRotation(rLayerObject.getRotation());
							}
							rNode2d->setFlipX(rLayerObject.getFlipX());
							rNode2d->setFlipY(rLayerObject.getFlipY());
							rNode2d->setOriginFactor(rLayerObject.getOriginFactorX(), rLayerObject.getOriginFactorY());
							rNode2d->setHint(rObjectDesc.getObjectHint());
							int acount=rObjectDesc.getObjectSequenceCount();
							for (int a=0;a<acount;a++) {
								ObjectSequence &rObjectSequence=rObjectDesc.getObjectSequenceByIndex(a);
								if (rObjectSequence.isValid()) {
									FrameSequence *rFrameSequence=rLevel.getTextureManager().getFrameSequence(rObjectSequence.getSequenceName());
									if (rFrameSequence) {
										rNode2d->addNode(new FramePlayer(rObjectSequence.getSequenceName(), rFrameSequence,(float)rObjectSequence.getFrameDelayInMs()));
										if (rObjectDesc.getDefaultSequenceName()==rObjectSequence.getSequenceName()) {
											rNode2d->activateFramePlayer(rObjectDesc.getDefaultSequenceName());
										}						
									}
								}
							}
						}
					}
				}
			}
		}
	}
	
}

bool LevelLoader::parse(const string &rLevelName) {
	bool rv=true;
	cout << "Loading Level " << rLevelName << endl;

    char cCurrentPath[FILENAME_MAX];
    getcwd(cCurrentPath, sizeof(cCurrentPath));

    cout << "Current working dir: " << cCurrentPath <<endl;

	ifstream rFileStream(rLevelName);
	string rFileString((istreambuf_iterator<char>(rFileStream)), istreambuf_iterator<char>());
	//cout << rFileString <<endl;

	JSONValue *rJSONValue=JSON::Parse(rFileString.c_str());
	if (rJSONValue && rJSONValue->IsObject()) {
		rv=parseRoot(rJSONValue);
	} 
	return rv;
}

bool LevelLoader::parseRoot(JSONValue *rJSONValueParent) {
	bool rv=false;
	do {
		if (!rJSONValueParent->IsObject()) {
			cout << "Level root is no object" << endl;
			break;
		}
		// Header
		JSONValue *rHeader=rJSONValueParent->Child(L"Header");
		if (!rHeader) {
			cout << "Level Header not found" << endl;
			break;
		} 
		if (!parseHeader(rHeader)) {
			break;
		}
		// Textures
		JSONValue *rTextures=rJSONValueParent->Child(L"Textures");
		if (!rTextures) {
			cout << "Level Textures definition not found" << endl;
			break;
		} 
		if (!parseTextures(rTextures)) {
			break;
		}
		// Textures AutoFramesAndSequences		
		JSONValue *rAutoFramesAndSequences=rJSONValueParent->Child(L"AutoFramesAndSequences");
		if (rAutoFramesAndSequences) {
			if (!parseAutoFramesAndSequences(rAutoFramesAndSequences)) {
				break;
			}
		}
		// Frames for Textures FramesForTexture		
		JSONValue *rFramesForTexture=rJSONValueParent->Child(L"FramesForTextures");
		if (rFramesForTexture) {
			if (!parseFramesForTextures(rFramesForTexture)) {
				break;
			}
		}
		// Frames for FramesForSequences		
		JSONValue *rFramesForSequences=rJSONValueParent->Child(L"FramesForSequences");
		if (rFramesForSequences) {
			if (!parseFramesForSequences(rFramesForSequences)) {
				break;
			}
		}
		// ObjectDescriptors
		JSONValue *rObjectDescriptors=rJSONValueParent->Child(L"ObjectDescriptors");
		if (!rObjectDescriptors) {
			cout << "Level ObjectDescriptors definition not found" << endl;
			break;
		} 
		if (!parseObjectDescriptors(rObjectDescriptors)) {
			break;
		}
		// Level
		JSONValue *rLevel=rJSONValueParent->Child(L"Level");
		if (!rLevel) {
			cout << "Level definition not found" << endl;
			break;
		} 
		if (!parseLevel(rLevel)) {
			break;
		}
		rv=true;
	} while(false);
	return rv;
}


bool LevelLoader::parseHeader(JSONValue *rJSONValueParent) {
	bool rv=false;
	do {
		if (!rJSONValueParent->IsObject()) {
			cout << "Level Header is no object" << endl;
			break;
		}

		string t=extractString(rJSONValueParent, L"LevelName");
		if (t.empty()) {
			cout << "LevelName not found or empty" << endl;
			break;
		}
		mLevel.setLevelName(t);

		t=extractString(rJSONValueParent, L"AssetPath");
		if (t.empty()) {
			cout << "AssetPath not found or empty" << endl;
			break;
		}
		mLevel.setAssetPath(t);
		rv=true;
	} while(false);
	return rv;
}

bool LevelLoader::parseTextures(JSONValue *rJSONValueParent) {
	bool rv=false;

	do {
		if (!rJSONValueParent->IsArray()) {
			cout << "Level Textures is no array" << endl;
			break;
		}
		const JSONArray &rJSONArray=rJSONValueParent->AsArray();
		for (auto *rJSONValue : rJSONArray) {
			string rTextureName=extractString(rJSONValue, L"TextureName");
			if (rTextureName.empty()) {
				cout << "Textures.TextureName not found or empty" << endl;
				continue;
			}
			string rTextureFile=extractString(rJSONValue, L"TextureFile");
			if (rTextureName.empty()) {
				cout << "Textures.TextureFile not found or empty" << endl;
				continue;
			}
			getLevel().getTextureManager().loadTexture(mLevel.getAssetPath()+rTextureFile, rTextureName);
		}
		rv=true;
	} while(false);
	return rv;
}

bool LevelLoader::parseAutoFramesAndSequences(JSONValue *rJSONValueParent) {
	bool rv=false;

	do {
		if (!rJSONValueParent->IsArray()) {
			cout << "Level AutoFramesAndSequences is no array" << endl;
			break;
		}
		const JSONArray &rJSONArray=rJSONValueParent->AsArray();
		for (auto *rJSONValue : rJSONArray) {
			string rTextureName=extractString(rJSONValue, L"TextureName");
			if (rTextureName.empty()) {
				cout << "Textures.TextureName not found or empty" << endl;
				continue;
			}
			string rSequenceName=extractString(rJSONValue, L"SequenceName");
			if (rSequenceName.empty()) {
				cout << "AutoFramesAndSequences.SequenceName not found or empty" << endl;
				continue;
			}
			int rFrameCountX=(int)extractNumber(rJSONValue, L"FrameCountX");
			int rFrameCountY=(int)extractNumber(rJSONValue, L"FrameCountY");

			getLevel().getTextureManager().createAutomaticFramesAndSequence(rTextureName, rSequenceName, rFrameCountX, rFrameCountY);
		}
		rv=true;
	} while(false);
	return rv;
}

bool LevelLoader::parseFramesForTextures(JSONValue *rJSONValueParent) {
	bool rv=false;

	do {
		if (!rJSONValueParent->IsArray()) {
			cout << "Level FramesForTextures is no array" << endl;
			break;
		}
		const JSONArray &rJSONArray=rJSONValueParent->AsArray();
		for (auto *rJSONValue : rJSONArray) {
			string rTextureName=extractString(rJSONValue, L"TextureName");
			if (rTextureName.empty()) {
				cout << "FramesForTextures.TextureName not found or empty" << endl;
				continue;
			}
			string rFrameName=extractString(rJSONValue, L"FrameName");
			if (rFrameName.empty()) {
				cout << "FramesForTextures.rFrameName not found or empty" << endl;
				continue;
			}
			int rSourceX=(int)extractNumber(rJSONValue, L"SourceX");
			int rSourceY=(int)extractNumber(rJSONValue, L"SourceY");
			int rWidth=(int)extractNumber(rJSONValue, L"Width");
			int rHeight=(int)extractNumber(rJSONValue, L"Height");

			getLevel().getTextureManager().addFrameToTexture(rTextureName, rFrameName, rSourceX, rSourceY, rWidth, rHeight);
		}
		rv=true;
	} while(false);
	return rv;
}

bool LevelLoader::parseFramesForSequences(JSONValue *rJSONValueParent) {
	bool rv=false;

	do {
		if (!rJSONValueParent->IsArray()) {
			cout << "Level FramesForSequences is no array" << endl;
			break;
		}
		const JSONArray &rJSONArray=rJSONValueParent->AsArray();
		for (auto *rJSONValue : rJSONArray) {
			string rSequenceName=extractString(rJSONValue, L"SequenceName");
			if (rSequenceName.empty()) {
				cout << "FramesForSequences.SequenceName not found or empty" << endl;
				continue;
			}
			JSONValue *rFrames=rJSONValue->Child(L"Frames");
			if (rFrames && rFrames->IsArray()) {
				const JSONArray &rJSONArrayFrames=rFrames->AsArray();
				for (auto *rJSONValueFrame : rJSONArrayFrames) {
					if (!rJSONValueFrame->IsObject()) {
						cout << "FramesForSequences.Frames.Frame not an object" << endl;
						continue;
					}
					string rFrameName=extractString(rJSONValueFrame, L"FrameName");
					if (rFrameName.empty()) {
						cout << "FramesForSequences.Frames.Frame.FrameName not found or empty" << endl;
						continue;
					}
					TextureFrame *rTextureFrame=getLevel().getTextureManager().getTextureFrame(rFrameName);
					if (!rTextureFrame) {
						cout << "FramesForSequences.Frames.Frame.FrameName not exists in TextureManager. FrameName: "<< rFrameName << endl;
						continue;
					}
					getLevel().getTextureManager().addFrameSequence(rSequenceName)->addTextureFrame(rTextureFrame);
				}
			}
		}
		rv=true;
	} while(false);
	return rv;
}

bool LevelLoader::parseObjectDescriptors(JSONValue *rJSONValueParent) {
	bool rv=false;

	do {
		if (!rJSONValueParent->IsArray()) {
			cout << "Level ObjectDescriptors is no array" << endl;
			break;
		}
		const JSONArray &rJSONArray=rJSONValueParent->AsArray();
		for (auto *rJSONValue : rJSONArray) {
			string rObjectDescId=extractString(rJSONValue, L"ObjectDescId");
			if (rObjectDescId.empty()) {
				cout << "ObjectDescriptors.Id not found or empty" << endl;
				continue;
			}
			string rObjectHint=extractString(rJSONValue, L"ObjectHint");

			string rDefaultFrame=extractString(rJSONValue, L"DefaultFrame");
			//if (rDefaultFrame.empty()) {
			//	cout << "ObjectDescriptors.DefaultFrame not found or empty" << endl;
			//	continue;
			//}
			string rDefaultSequence=extractString(rJSONValue, L"DefaultSequence");
			//if (rDefaultSequence.empty()) {
			//	cout << "ObjectDescriptors.DefaultSequence not found or empty" << endl;
			//	continue;
			//}
			ObjectDesc &rObjectDesc=mLevel.getOrAddObjectDesc(rObjectDescId);
			if (!rObjectDesc.isValid()) {
				cout << "ObjectDescriptors.Id not found or empty" << endl;
				continue;
			}
			rObjectDesc.setObjectHint(rObjectHint);
			rObjectDesc.setDefaultFrame(rDefaultFrame);

			JSONValue *rObjectSequences=rJSONValue->Child(L"ObjectSequences");
			if (rObjectSequences && rObjectSequences->IsArray()) {
				const JSONArray &rJSONArrayS=rObjectSequences->AsArray();
				for (auto *rJSONValueS : rJSONArrayS) {
					string rSequenceName=extractString(rJSONValueS, L"SequenceName");
					if (rSequenceName.empty()) {
						cout << "ObjectDescriptors.ObjectSequences.SequenceName not found or empty" << endl;
						continue;
					}
			
					int rFrameDelayInMs=(int)extractNumber(rJSONValueS, L"FrameDelayInMs");
					ObjectSequence &rObjectSequence=rObjectDesc.getOrAddObjectSequence(rSequenceName);
					if (rObjectSequence.isValid()) {
						rObjectSequence.setFrameDelayInMs(rFrameDelayInMs);
					}
				}
			}
			if (!rDefaultSequence.empty()) {
				if (rObjectDesc.getObjectSequenceByName(rDefaultSequence).isValid()) {
					rObjectDesc.setDefaultSequence(rDefaultSequence);
				} else {
					cout << "ObjectDescriptors.ObjectSequences.DefaultSequence "<< rDefaultSequence <<" not available" << endl;
				}
			}
		}
		rv=true;
	} while(false);
	return rv;
}

bool LevelLoader::parseLevel(JSONValue *rJSONValueParent) {
	bool rv=false;

	do {
		if (!rJSONValueParent->IsObject()) {
			cout << "Level is no Object" << endl;
			break;
		}
		JSONValue *rLayers=rJSONValueParent->Child(L"Layers");
		if (!rLayers || !rLayers->IsArray()) {
			cout << "Level.Layers not found or not an Array" << endl;
			break;;
		}
		const JSONArray &rJSONArray=rLayers->AsArray();
		for (auto *rJSONValue : rJSONArray) {
			string rLayerName=extractString(rJSONValue, L"LayerName");
			if (rLayerName.empty()) {
				cout << "Lebel.Layers.LayerName not found or empty" << endl;
				continue;
			}
			int rZOrder=(int)extractNumber(rJSONValue, L"ZOrder");
			int rGridX=(int)extractNumber(rJSONValue, L"GridX");
			int rGridY=(int)extractNumber(rJSONValue, L"GridY");
			if (rGridX<1) {
				rGridX=1;
			}
			if (rGridY<1) {
				rGridY=1;
			}
			if (mLevel.getLayerByName(rLayerName).isValid()) {
				cout << "Level.Layers.LayerName duplicate LayerName. LayerName: " << rLayerName << endl;
				continue;
			}

			Layer &rLayer=mLevel.getOrAddLayer(rLayerName);
			if (!rLayer.isValid()) {
				cout << "Level.Layers.LayerName could not constructed. LayerName not found or empty" << endl;
				continue;
			}
			JSONValue *rLayerObjects=rJSONValue->Child(L"LayerObjects");
			if (!rLayerObjects || !rLayerObjects->IsArray()) {
				cout << "Level.Layer.LayerObjects not found or not an Array" << endl;
				break;;
			}
			
			const JSONArray &rLayerObjectsArray=rLayerObjects->AsArray();
			for (auto *rJSONValueLayerObject : rLayerObjectsArray) {
				string rObjectName=extractString(rJSONValueLayerObject, L"ObjectName");
				if (rObjectName.empty()) {
					rObjectName="__"+to_string(mLevel.getNextLevelObjectNameNb());
					//cout << "Level.Layer.LayerObjects.ObjectName not found or empty" << endl;
					//continue;
				}
				string rObjectDescId=extractString(rJSONValueLayerObject, L"ObjectDescId");
				if (rObjectDescId.empty()) {
					cout << "Level.Layer.LayerObjects.ObjectDescId not found or empty" << endl;
					continue;
				}
				if (!mLevel.isObjectDescAvailable(rObjectDescId)) {
					cout << "Level.Layer.LayerObjects.ObjectDescId " << rObjectDescId <<" is not available" << endl;
					continue;
				}
				int rGridPosX=0;
				int rGridPosY=0;
				int rPosX=0;
				int rPosY=0;
				bool rGridPosXFound=extractNumberExistAsInt(rJSONValueLayerObject, L"GridPosX", &rGridPosX);
				bool rGridPosYFound=extractNumberExistAsInt(rJSONValueLayerObject, L"GridPosY", &rGridPosY);
				bool rPosXFound=extractNumberExistAsInt(rJSONValueLayerObject, L"PosX", &rPosX);
				bool rPosYFound=extractNumberExistAsInt(rJSONValueLayerObject, L"PosY", &rPosY);
				int rRepeatX=extractNumber(rJSONValueLayerObject, L"RepeatX");
				if (rRepeatX<=0) {
					rRepeatX=1;
				}
				if (!rGridPosXFound && !rPosXFound) {
					cout << "Level.Layer.LayerObjects.PosX or GridPosX not found" << endl;
					continue;
				}
				if (!rGridPosYFound && !rPosYFound) {
					cout << "Level.Layer.LayerObjects.PosY or GridPosY not found" << endl;
					continue;
				}
				int realPosX=0;
				if (rPosXFound) {
					realPosX=rPosX;	
				} else {
					realPosX=rGridPosX*rGridX;
				}
				int realPosY=0;
				if (rPosYFound) {
					realPosY=rPosY;	
				} else {
					realPosY=rGridPosY*rGridY;
				}
				float rRotation=0;
				if (extractNumberExist(rJSONValueLayerObject, L"Rotation", &rRotation)) {
					if (rRotation<0 || rRotation>=360) {
						rRotation=0;
					}
				}
				float rOriginFactorX=0.0;
				float rOriginFactorY=0.0;
				extractNumberExist(rJSONValueLayerObject, L"OriginFactorX", &rOriginFactorX);
				extractNumberExist(rJSONValueLayerObject, L"OriginFactorY", &rOriginFactorY);
				bool rFlipX=extractBool(rJSONValueLayerObject, L"FlipX");
				bool rFlipY=extractBool(rJSONValueLayerObject, L"FlipY");

				if (rLayer.getLayerObjectByName(rObjectName).isValid()) {
					cout << "Level.Layer.LayerObjects duplicate ObjectName. ObjectName: " << rObjectName << endl;
					continue;
				}
				for(int r=0;r<rRepeatX;r++) {
					string rObjectNameReal=rObjectName;
					if (r>0) {
						rObjectNameReal+=".r"+to_string(r);
						realPosX+=rGridX;
					}
					LayerObject& rLayerObject=rLayer.getOrAddLayerObject(rObjectNameReal);
					if (rLayerObject.isValid()) {
						rLayerObject.setObjectDescId(rObjectDescId);
						rLayerObject.setPosX(realPosX);
						rLayerObject.setPosY(realPosY);
						rLayerObject.setRotation(rRotation);
						rLayerObject.setOriginFactorX(rOriginFactorX);
						rLayerObject.setOriginFactorY(rOriginFactorY);
						rLayerObject.setFlipX(rFlipX);
						rLayerObject.setFlipY(rFlipY);
					}
				}
			}
		}
		rv=true;
	} while(0);
	return rv;
}


const string LevelLoader::extractString(JSONValue *rJSONValueParent, const wstring &rName) {
	JSONValue *v=rJSONValueParent->Child(rName.c_str());
	if (v && v->IsString()) {
		return string(v->AsStringPtr()->begin(), v->AsStringPtr()->end());
	}
	return "";
}

float LevelLoader::extractNumber(JSONValue *rJSONValueParent, const wstring &rName) {
	JSONValue *v=rJSONValueParent->Child(rName.c_str());
	if (v && v->IsNumber()) {
		return (float)v->AsNumber();
	}
	return 0.0;
}

bool LevelLoader::extractNumberExistAsInt(JSONValue *rJSONValueParent, const wstring &rName, int *out) {
	float tmp=0.0;
	(*out)=0;
	if (extractNumberExist(rJSONValueParent, rName, &tmp)) {
		(*out)=(int)tmp;
		return true;
	}
	return false;
}

bool LevelLoader::extractNumberExist(JSONValue *rJSONValueParent, const wstring &rName, float *out) {
	(*out)=0;
	JSONValue *v=rJSONValueParent->Child(rName.c_str());
	if (v && v->IsNumber()) {
		(*out)=(float)v->AsNumber();
		return true;
	}
	return false;
}

bool LevelLoader::extractBool(JSONValue *rJSONValueParent, const wstring &rName) {
	JSONValue *v=rJSONValueParent->Child(rName.c_str());
	if (v && v->IsBool()) {
		return v->AsBool();
	}
	return false;
}
