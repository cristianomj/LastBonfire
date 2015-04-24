#pragma once

#include "psti_VS\stdafx.h"
#include "tinyxml\tinystr.h"
#include "tinyxml\tinyxml.h"
#include "sssf\game\Game.h"
#include "xmlfi\XMLFileImporter.h"

// XML TEXT
static const string		XML_FILE_EXTENSION			= ".xml";
static const string		SPRITE_TYPE_LIST_ELEMENT	= "sprite_type_list";
static const string		SPRITE_TYPE_ELEMENT			= "sprite_type";
static const string		SPRITE_WIDTH_ELEMENT		= "width";
static const string		SPRITE_HEIGHT_ELEMENT		= "height";
static const string		IMAGES_LIST_ELEMENT			= "images_list";
static const string		IMAGE_FILE_ELEMENT			= "image_file";
static const string		ID_ATT						= "id";
static const string		FILE_NAME_ATT				= "file_name";
static const string		ANIMATIONS_LIST_ELEMENT		= "animations_list";
static const string		ANIMATION_STATE_ELEMENT		= "animation_state";
static const string		STATE_ELEMENT				= "state";
static const string		ANIMATION_SEQUENCE_ELEMENT	= "animation_sequence";
static const string		POSE_ELEMENT				= "pose";
static const string		IMAGE_ID_ATT				= "image_id";
static const string		DURATION_ATT				= "duration";

struct Dimensions
{
	int width;
	int height;
};

struct Pose
{
	int imageId;
	int duration;
};

class PoseurSpriteTypesImporter : XMLFileImporter
{
protected:
	vector<string> spriteTypes;
	map<string, Dimensions> spriteTypesDimensions;
	map<string, map<int, string>> spriteTypesImages;
	map<string, map<string, vector<Pose>>> spriteTypesAnimationsLists;
	string dir;

public:
	PoseurSpriteTypesImporter() {}
	~PoseurSpriteTypesImporter(){}
	bool loadSpriteTypes(Game *game, wstring spriteTypesListFileName);

	// THESE ARE ALL THE HELPER METHODS
private:
	bool loadSpriteTypesListInfo(const char spriteTypesListFileName[]);
	bool loadSpriteTypeInfo(string spriteTypeName);
	bool loadImagesListInfo(const TiXmlNode *node, const string fullPath, const string spriteTypeName);
	bool loadAnimationsListsInfo(const TiXmlNode *node, string spriteTypeName);
	bool loadAnimationStateInfo(const TiXmlNode *animationStateNode, map<string, vector<Pose>> *animationStates, string state);
};