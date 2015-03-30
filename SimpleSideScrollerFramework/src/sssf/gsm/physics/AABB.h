#pragma once

#include "sssf_VS\stdafx.h"

class AABB
{
protected:
	float centerX;
	float centerY;
	float width;
	float height;

public:
	AABB()	{}
	~AABB()	{}

	// INLINED METHODS	

	void incCenterX(float xInc)			{ centerX += xInc;				}
	void incCenterY(float yInc)			{ centerY += yInc;				}
	float getWidth()					{ return width;					}
	float getHeight()					{ return height;				}
	float getCenterX()					{ return centerX;				}
	float getCenterY() 					{ return centerY;				}
	void setWidth(float initWidth)		{ width = initWidth;			}
	void setHeight(float initHeight)	{ height = initHeight;			}
	void setCenterX(float initCenterX)	{ centerX = initCenterX;		}
	void setCenterY(float initCenterY)	{ centerY = initCenterY;		}
	float getLeft()						{ return centerX - (width/2.0f);	}
	float getRight()					{ return centerX + (width/2.0f);	}
	float getTop()						{ return centerY - (height/2.0f);	}
	float getBottom()					{ return centerY + (height/2.0f);	}
	bool  overlaps(AABB *testAABB);
	bool  overlapsX(AABB *testAABB);
	bool  overlapsY(AABB *testAABB);
};