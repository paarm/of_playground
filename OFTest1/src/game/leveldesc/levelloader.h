#pragma once

#include "level.h"
#include "../../thirdparty/simplejson/JSONValue.h"
#include "../../engine/node.h"
#include "../../engine/texturemanager.h"
#include <string>
#include <fstream>
#include <streambuf>


class LevelLoader {
private:
	Level mLevel;
	bool parse(const string &rLevelName);
	bool parseRoot(JSONValue *rJSONValueParent);
	bool parseHeader(JSONValue *rJSONValueParent);
	bool parseTextures(JSONValue *rJSONValueParent);
	bool parseAutoFramesAndSequences(JSONValue *rJSONValueParent);
	bool parseFramesForTextures(JSONValue *rJSONValueParent);
	bool parseFramesForSequences(JSONValue *rJSONValueParent);
	bool parseObjectDescriptors(JSONValue *rJSONValueParent);
	bool parseLevel(JSONValue *rJSONValueParent);
	const string extractString(JSONValue *rJSONValueParent, const wstring &rName);
	float extractNumber(JSONValue *rJSONValueParent, const wstring &rName);
	bool extractNumberExist(JSONValue *rJSONValueParent, const wstring &rName, float *out);
	bool extractNumberExistAsInt(JSONValue *rJSONValueParent, const wstring &rName, int *out);
	bool extractBool(JSONValue *rJSONValueParent, const wstring &rName);
public:
	bool loadLevel(const string &rLevelName);
	Level &getLevel();
	void addLevelToNode(Node &rParentNode);
	

};
