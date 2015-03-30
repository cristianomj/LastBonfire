#pragma once

/*
	A Tile represents a single image in a tiled background.
	Tiles can be collidable or not collidable. Tiles do not
	know where they are placed. They do know the index of
	the texture that should be used for rendering themselves
	as stored in the GameGraphics's world string table and
	it's partnered texture manager.
*/
class Tile
{
public:
	int textureID;
	bool collidable;
};