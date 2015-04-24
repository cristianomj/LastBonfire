#include "psti_VS\stdafx.h"
#include "sssf\game\Game.h"
#include "sssf\game\WStringTable.h"
#include "sssf\gsm\sprite\AnimatedSpriteType.h"
#include "psti\PoseurSpriteTypesImporter.h"
#include "tinyxml\tinystr.h"
#include "tinyxml\tinyxml.h"
#include "xmlfi\XMLFileImporter.h"
#include "sssf\graphics\GameGraphics.h"
#include "sssf\graphics\TextureManager.h"

/*
	This method loads all the sprite types found in the provided sprite type list file
	into the game state manager, including their images.
*/
bool PoseurSpriteTypesImporter::loadSpriteTypes(Game *game, wstring spriteTypesListFileName)
{
	int slashIndex = spriteTypesListFileName.rfind('/');
	dir = string(spriteTypesListFileName.begin(), spriteTypesListFileName.end()).substr(0, slashIndex+1);
	const char *spriteTypesListFile = newCharArrayFromWstring(spriteTypesListFileName);
	bool success = loadSpriteTypesListInfo(spriteTypesListFile);
	if (!success) return false;
	for (unsigned int i = 0; i < spriteTypes.size(); i++)
	{
		success = loadSpriteTypeInfo(spriteTypes[i]);
		if (!success) return false;
	}

	TextureManager *tm = game->getGraphics()->getWorldTextureManager();
	WStringTable *wStringTable = tm->getWStringTable();

	// NOW LET'S USE ALL THE INFO WE'VE LOADED 
	// LET'S START BY LOADING THE TEXTURES INTO THE WORLD TEXTURE MANAGER
	for (unsigned int i = 0; i < spriteTypes.size(); i++)
	{
		string spriteType = spriteTypes[i];
		unsigned int offset = wStringTable->getNumWStringsInTable();
		map<int, string> images = spriteTypesImages[spriteType];
		for (unsigned int j = 0; j < images.size(); j++)
		{
			string imageToLoad = images[j];
			wstring wImageToLoad(imageToLoad.begin(), imageToLoad.end());
			tm->loadTexture(wImageToLoad);
		}

		AnimatedSpriteType *ast = new AnimatedSpriteType();
		unsigned int spriteTypeId = game->getGSM()->getSpriteManager()->addSpriteType(ast);
		ast->setSpriteTypeID(spriteTypeId);
		Dimensions dim = spriteTypesDimensions[spriteType];
		ast->setTextureSize(dim.width, dim.height);
		
		map<string, vector<Pose>> animations = spriteTypesAnimationsLists[spriteType];
		map<string, vector<Pose>>::iterator it = animations.begin();
		while (it != animations.end())
		{
			string key = it->first;
			wstring wKey(key.begin(), key.end());
			ast->addAnimationSequence(wKey);
			vector<Pose> poseList = it->second;
			vector<Pose>::iterator poseIt = poseList.begin();
			while (poseIt != poseList.end())
			{
				Pose pose = *poseIt;
				ast->addAnimationFrame(wKey, pose.imageId + offset - 1, pose.duration);
				poseIt++;
			}				
			it++;
		}
	}


	return true;
}

bool PoseurSpriteTypesImporter::loadSpriteTypeInfo(string spriteTypeName)
{
	string fullPath = dir + spriteTypeName;
	string spriteTypeFile = spriteTypeName + XML_FILE_EXTENSION;
	string spriteTypePath = fullPath + "/" + spriteTypeFile;
	const char *path = newCharArrayFromString(spriteTypePath);
	TiXmlDocument doc(path);
	bool loadOkay = doc.LoadFile();
	if (loadOkay)
	{
		TiXmlElement *pElem = doc.FirstChildElement();
		string eName;
		if (pElem)
		{
			eName = pElem->Value();
			if (strcmp(eName.c_str(), SPRITE_TYPE_ELEMENT.c_str()) == 0)
			{
				Dimensions dim;
				const TiXmlNode *node = pElem->FirstChild();
				while (node)
				{
					string eName = node->Value();
					if (strcmp(eName.c_str(), SPRITE_WIDTH_ELEMENT.c_str()) == 0)
					{
						stringstream ss;
						ss << node->FirstChild()->Value();
						ss >> dim.width;
					}
					else if (strcmp(eName.c_str(), SPRITE_HEIGHT_ELEMENT.c_str()) == 0)
					{
						stringstream ss;
						ss << node->FirstChild()->Value();
						ss >> dim.height;
						spriteTypesDimensions[spriteTypeName] = dim;
					}
					else if (strcmp(eName.c_str(), IMAGES_LIST_ELEMENT.c_str()) == 0)
					{
						loadImagesListInfo(node, fullPath, spriteTypeName);
					}
					else if (strcmp(eName.c_str(), ANIMATIONS_LIST_ELEMENT.c_str()) == 0)
					{
						map<string, vector<Pose>> animationsLists;
						loadAnimationsListsInfo(node, spriteTypeName);
					}
					node = node->NextSibling();
				}
			}
			else
				return false;
		}
		else
			return false;
	}
	return true;
}

bool PoseurSpriteTypesImporter::loadAnimationsListsInfo(const TiXmlNode *node, string spriteTypeName)
{
	const TiXmlNode *animationStateNode = node->FirstChild();
	map<string, vector<Pose>> animationStates;
	while (animationStateNode)
	{
		// FIRST GET THE STATE NAME
		const TiXmlNode *stateNode = animationStateNode->FirstChild();
		const TiXmlElement *stateElement = stateNode->ToElement();
		string state = stateElement->FirstChild()->Value();
		bool success = loadAnimationStateInfo(stateNode, &animationStates, state);
		if (!success) return false;
		animationStateNode = animationStateNode->NextSibling();
	}
	spriteTypesAnimationsLists[spriteTypeName] = animationStates;
	return true;
}

bool PoseurSpriteTypesImporter::loadAnimationStateInfo( const TiXmlNode *animationStateNode,
														map<string, vector<Pose>> *animationStates,
														string state)
{
	// THESE ARE THE POSES WE'LL BE LOADING
	vector<Pose> poses;
	
	// THEN THE ANIMATION SEQUENCE FOR THAT STATE
	const TiXmlNode *animationSequenceNode = animationStateNode->NextSibling();
	const TiXmlNode *poseNode = animationSequenceNode->FirstChild();
	while (poseNode)
	{
		const TiXmlElement *poseElement = poseNode->ToElement();
		Pose poseToAdd;
		poseToAdd.imageId = extractIntAtt(poseElement, IMAGE_ID_ATT);
		poseToAdd.duration = extractIntAtt(poseElement, DURATION_ATT);
		poses.push_back(poseToAdd);
		poseNode = poseNode->NextSibling();
	}

	// ADD THIS ANIMATION STATE
	(*animationStates)[state] = poses;

	return true;
}

bool PoseurSpriteTypesImporter::loadSpriteTypesListInfo(const char spriteTypesListFileName[])
{
	TiXmlDocument doc(spriteTypesListFileName);
	bool loadOkay = doc.LoadFile();
	if (loadOkay)
	{
		TiXmlElement *pElem = doc.FirstChildElement();
		string eName;
		if (pElem)
		{
			eName = pElem->Value();
			if (strcmp(eName.c_str(), SPRITE_TYPE_LIST_ELEMENT.c_str()) == 0)
			{
				// GET THE NAMES OF ALL THE SPRITE TYPES
				const TiXmlElement *spriteTypeListElement = pElem->ToElement();

				// NOW GET THE SPRITE TYPES
				const TiXmlNode *spriteTypeNode = spriteTypeListElement->FirstChild();
				int spriteTypeCounter = 0;
				
				while (spriteTypeNode)
				{
					const TiXmlNode *dataNode = spriteTypeNode->FirstChild();
					const char *spriteType = dataNode->Value();
					string spriteTypeStr(spriteType);
					spriteTypes.push_back(spriteTypeStr);
					spriteTypeNode = spriteTypeNode->NextSibling();
				}
			}
			else
			{
				return false;
			}
		}
	}
	else
	{
		return false;
	}
	return true;
}

bool PoseurSpriteTypesImporter::loadImagesListInfo(	const TiXmlNode *node, const string fullPath, const string spriteTypeName)
{
	const TiXmlNode *imageNode = node->FirstChild();
	map<int, string> imageFiles;
	while (imageNode)
	{
		const TiXmlElement *imageElement = imageNode->ToElement();
		int id = extractIntAtt(imageElement, ID_ATT);
		string fileName = extractCharAtt(imageElement, FILE_NAME_ATT);
		fileName = fullPath + "/" + fileName;
		imageFiles[id] = fileName;
		imageNode = imageNode->NextSibling();
	}
	spriteTypesImages[spriteTypeName] = imageFiles;
	return true;
}