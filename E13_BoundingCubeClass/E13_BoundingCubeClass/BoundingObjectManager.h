#pragma once
#include "RE\ReEngAppClass.h"
#include <SFML\Graphics.hpp>
#include "MyBoundingCubeClass.h"
#include <cstring>

class BoundingObjectManager : public ReEngAppClass
{
public:
	typedef ReEngAppClass super;
	static BoundingObjectManager* instance;
	static BoundingObjectManager* GetInstance();
	static void ReleaseInstance();
private:
	BoundingObjectManager();
	~BoundingObjectManager();
	void AddObject(String);
};

