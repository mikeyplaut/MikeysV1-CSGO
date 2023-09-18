#pragma once
#include "pch.h"

#include "Entity.h"
#include "Vec3.h"
#include "csgo.hpp"

using namespace hazedumper::netvars;
using namespace hazedumper::signatures;


Entity* Entity::GetEntity(int index)
{
	static uintptr_t moduleBase = (uintptr_t)GetModuleHandleA("client.dll");
	static uintptr_t entityList = moduleBase + dwEntityList;

	return (Entity*)(entityList + index * 0x10);
}

int* Entity::GetMaxEntity()
{
	static uintptr_t moduleBase = (uintptr_t)GetModuleHandleA("engine.dll");
	return (int*)(*(uintptr_t*)(moduleBase + dwClientState) + dwClientState_MaxPlayer);
}

Vec3* Entity::GetOrigin()
{
	return (Vec3*)(*(uintptr_t*)this + m_vecOrigin);
}

Vec3* Entity::GetViewOffset()
{
	return (Vec3*)(*(uintptr_t*)this + m_vecViewOffset);
}

Vec3* Entity::GetBonePos(int boneID)
{
	uintptr_t boneMatrix = *(uintptr_t*)(*(uintptr_t*)this + m_dwBoneMatrix);
	static Vec3 bonePos;
	bonePos.x = *(float*)(boneMatrix + 0x30 * boneID + 0x0C);
	bonePos.y = *(float*)(boneMatrix + 0x30 * boneID + 0x1C);
	bonePos.z = *(float*)(boneMatrix + 0x30 * boneID + 0x2C);
	return &bonePos;
}

int* Entity::GetHealth()
{
	return (int*)(*(uintptr_t*)this + m_iHealth);
}

int* Entity::GetTeam()
{
	return (int*)(*(uintptr_t*)this + m_iTeamNum);
}

bool* Entity::isDormant()
{
	return (bool*)(*(uintptr_t*)this + m_bDormant);
}