#pragma once
#include "pch.h"
#include "Entity.h"
#include "LocalPlayer.h"
#include "Vec3.h"
#include "csgo.hpp"

using namespace hazedumper::signatures;
using namespace hazedumper::netvars;

Entity* GetClosestFOVEnemy(static uintptr_t engineModule)
{
	LocalPlayer* localPlayer = LocalPlayer::Get();

	float closestDistance = INT_MAX, closestFOV = INT_MAX;
	double PI = 3.14159265358;
	int bestIndex = -1;

	for (int i = 1; i < *Entity::GetMaxEntity(); i++)
	{
		Entity* currentEntity = Entity::GetEntity(i);

		if (!currentEntity || !(*(uintptr_t*)currentEntity) || (uintptr_t)currentEntity == (uintptr_t)localPlayer)
			continue;

		if (*currentEntity->GetTeam() == *localPlayer->GetTeam())
			continue;

		if (*currentEntity->GetHealth() < 1 || *localPlayer->GetHealth() < 1)
			continue;

		if (*currentEntity->isDormant())
			continue;

		static Vec3* viewAngles = (Vec3*)(*(uintptr_t*)(engineModule + dwClientState) + dwClientState_ViewAngles);
		Vec3 origin = *localPlayer->GetOrigin();
		Vec3 viewOffset = *localPlayer->GetViewOffset();
		Vec3* myPos = &(origin + viewOffset);
		Vec3* target = currentEntity->GetBonePos(8);
		Vec3 newAngles = CalcAngle(Vec3(myPos->x, myPos->y, myPos->z), Vec3(target->x, target->y, target->z));
	
		Vec3 currentFOV = NormalizeDiff(Vec3(*viewAngles), Vec3(newAngles));

		if (currentFOV.y < closestFOV)
		{
			closestFOV = currentFOV.y;
			bestIndex = i;
		}
	}

	if (bestIndex == -1)
		return NULL;

	return Entity::GetEntity(bestIndex);
}

Entity* GetClosestEnemy()
{
	LocalPlayer* localPlayer = LocalPlayer::Get();

	float closestDistance = INT_MAX;
	int closestDistanceIndex = -1;

	for (int i = 1; i < *Entity::GetMaxEntity(); i++)
	{
		Entity* currentEntity = Entity::GetEntity(i);

		if (!currentEntity || !(*(uintptr_t*)currentEntity) || (uintptr_t)currentEntity == (uintptr_t)localPlayer)
			continue;

		if (*currentEntity->GetTeam() == *localPlayer->GetTeam())
			continue;

		if (*currentEntity->GetHealth() < 1 || *localPlayer->GetHealth() < 1)
			continue;

		if (*currentEntity->isDormant())
			continue;

		float currentDistance = localPlayer->GetDistance(currentEntity->GetOrigin());
		if (currentDistance < closestDistance)
		{
			closestDistance = currentDistance;
			closestDistanceIndex = i;
		}
	}

	if (closestDistanceIndex == -1)
		return NULL;

	return Entity::GetEntity(closestDistanceIndex);
}
void Run()
{
	static uintptr_t engineModule = (uintptr_t)GetModuleHandleA("engine.dll");
	while (GetAsyncKeyState(VK_XBUTTON1))
	{
		Entity* closestEnemy = GetClosestFOVEnemy(engineModule);

		if (closestEnemy)
		{
			LocalPlayer::Get()->AimAt(closestEnemy->GetBonePos(8), engineModule);
		}
	}

	//rcs
	LocalPlayer::Get()->RecoilControlSystem(engineModule);
}
