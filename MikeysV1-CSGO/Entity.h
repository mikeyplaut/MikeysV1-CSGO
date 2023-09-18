#pragma once

class Vec3;
class Entity
{
private:
	Entity();

public:
	static int* GetMaxEntity();
	static Entity* GetEntity(int index);

	int* GetHealth();
	int* GetTeam();
	bool* isDormant();
	Vec3* GetOrigin();
	Vec3* GetViewOffset();
	Vec3* GetBonePos(int boneID);

};
