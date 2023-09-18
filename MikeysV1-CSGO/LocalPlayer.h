#pragma once

class Vec3;
class LocalPlayer
{
private:
	LocalPlayer();

public:
	static LocalPlayer* Get();

	Vec3* GetOrigin();
	Vec3* GetViewOffset();
	Vec3* GetAimPunchAngle();
	int* GetiShotsFired();
	int* GetHealth();
	int* GetTeam();
	void AimAt(Vec3* target, uintptr_t engineModule);
	void RecoilControlSystem(uintptr_t engineModule);
	void RecoilControlSystem(uintptr_t engineModule, Vec3 botAngles);
	float GetDistance(Vec3*);

};

