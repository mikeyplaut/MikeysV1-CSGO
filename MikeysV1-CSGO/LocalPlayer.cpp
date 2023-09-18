#include "pch.h"
#include "LocalPlayer.h"
#include "Vec3.h"
#include "csgo.hpp"

using namespace hazedumper::netvars;
using namespace hazedumper::signatures;


LocalPlayer* LocalPlayer::Get()
{
	static uintptr_t clientModule = (uintptr_t)(GetModuleHandleA("client.dll"));
	static LocalPlayer* localPlayer = (LocalPlayer*)(clientModule + dwLocalPlayer);

	return localPlayer;
}

Vec3* LocalPlayer::GetOrigin()
{
	return (Vec3*)(*(uintptr_t*)this + m_vecOrigin);
}

Vec3* LocalPlayer::GetViewOffset()
{
	return (Vec3*)(*(uintptr_t*)this + m_vecViewOffset);
}

Vec3* LocalPlayer::GetAimPunchAngle()
{
	return (Vec3*)(*(uintptr_t*)this + m_aimPunchAngle);
}

int* LocalPlayer::GetiShotsFired()
{
	return (int*)(*(uintptr_t*)this + m_iShotsFired);
}
int* LocalPlayer::GetHealth()
{
	return (int*)(*(uintptr_t*)this + m_iHealth);
}

int* LocalPlayer::GetTeam()
{
	return (int*)(*(uintptr_t*)this + m_iTeamNum);
}

float LocalPlayer::GetDistance(Vec3* other)
{
	Vec3* myPos = GetOrigin();
	Vec3 delta = Vec3(other->x - myPos->x, other->y - myPos->y, other->z - myPos->z);

	return sqrt(delta.x * delta.x + delta.y * delta.y + delta.z * delta.z);
}

void LocalPlayer::AimAt(Vec3* target, uintptr_t engineModule)
{
	static Vec3* viewAngles = (Vec3*)(*(uintptr_t*)(engineModule + dwClientState) + dwClientState_ViewAngles);

	Vec3 origin = *GetOrigin();
	Vec3 viewOffset = *GetViewOffset();
	Vec3* myPos = &(origin + viewOffset);
	Vec3 newAngles = CalcAngle(Vec3(myPos->x, myPos->y, myPos->z), Vec3(target->x, target->y, target->z));
	float pitch = newAngles.x;
	float yaw = newAngles.y;

	if (pitch >= -89 && pitch <= 89 && yaw >= -180 && yaw <= 180)
	{
		Vec3 currentFOV = NormalizeDiff(Vec3(*viewAngles), Vec3(newAngles));

		//aim
		if (currentFOV.y > 180 )
		{
			float smoothing = 23.0f;

			while (currentFOV.y > 1)
			{
				if (*GetiShotsFired() > 1)
				{
					Vec3* smoothAngles = viewAngles;

					if ((yaw - smoothAngles->y) > 180)
						smoothAngles->y -= (yaw - smoothAngles->y) / smoothing;
					else
						smoothAngles->y += (yaw - smoothAngles->y) / smoothing;

					smoothAngles->x = newAngles.x;
					RecoilControlSystem(engineModule, *smoothAngles);

				}
				else
				{
					if ((yaw - viewAngles->y) > 180)
						viewAngles->y -= (yaw - viewAngles->y) / smoothing;
					else
						viewAngles->y += (yaw - viewAngles->y) / smoothing;

					//pitch
					if((pitch - viewAngles->x) > 89)
						viewAngles->x -= (pitch - viewAngles->x) / smoothing;
					else
						viewAngles->x += (pitch - viewAngles->x) / smoothing;
				}

				Sleep(1);

				currentFOV = NormalizeDiff(Vec3(*viewAngles), Vec3(newAngles));

			}
		}
		else
		{
			if(*GetiShotsFired() > 1)
				RecoilControlSystem(engineModule, newAngles);
			else
			{
				viewAngles->x = pitch;
				viewAngles->y = yaw;
			}
		}
	}
}

void LocalPlayer::RecoilControlSystem(uintptr_t engineModule)
{
	static Vec3* viewAngles = (Vec3*)(*(uintptr_t*)(engineModule + dwClientState) + dwClientState_ViewAngles);
	int* iShotsFired = GetiShotsFired();
	Vec3* aimPunchAngle = GetAimPunchAngle();
	static Vec3 oPunch{ 0,0,0 };

	//rcs
	Vec3 punchAngle = *aimPunchAngle * 2;
	if (*iShotsFired > 1)
	{
		//calc rcs
		Vec3 newAngle = *viewAngles + oPunch - punchAngle;
		//normalize
		newAngle.RecoilNormalize();
		//set
		*viewAngles = newAngle;
	}
	//fix
	oPunch = punchAngle;
}

void LocalPlayer::RecoilControlSystem(uintptr_t engineModule, Vec3 botAngles)
{
	static Vec3* viewAngles = (Vec3*)(*(uintptr_t*)(engineModule + dwClientState) + dwClientState_ViewAngles);
	int* iShotsFired = GetiShotsFired();
	Vec3 aimPunchAngle = *GetAimPunchAngle();

	//rcs
	Vec3 punchAngle = aimPunchAngle  * 2;
	if (*iShotsFired > 1)
	{
		//calc rcs
		Vec3 newAngle = botAngles - punchAngle;
		//normalize
		newAngle.RecoilNormalize();
		//set
		*viewAngles = newAngle;
	}
}
