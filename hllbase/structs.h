#pragma once
#include <math.h>
#include "vecmath.h"
#include "d3d9.h"

#define UCONST_Pi			3.1415926
#define URotationToRadians  UCONST_Pi / 180

struct FVector
{
	double X;
	double Y;
	double Z;
};

struct FRotator
{
	double Pitch;
	double Yaw;
	double Roll;
};

struct FMinimalViewInfo
{
    FVector Location; 
    FRotator Rotation; 
    float FOV; 
};
struct FCameraCacheEntry
{
    float Timestamp; 
    char pad_4[0xc]; 
    FMinimalViewInfo POV; 
};

typedef struct _EntityList
{
    uintptr_t instance;
    uintptr_t mesh;
    uintptr_t root_component;
    uintptr_t instigator;
    uintptr_t PlayerState;
    uintptr_t Pawn;
	FVector TopLocation;
	FVector bone_origin;
    std::string name;
    FVector origin;
    float health;
    float dist;
    int objectId;
    int team;
}EntityList;


class UDBDPlayer {
private:
public:
    uintptr_t instance;
    uintptr_t mesh;
    uintptr_t root_component;
    uintptr_t OutlineComponent;
    uintptr_t instigator;
    uintptr_t PlayerState;
    uintptr_t Pawn;
	FVector TopLocation;
    std::string name;
    int objectId;
	FVector tempOrigin;
    FVector origin;
    float health;
    float dist;
    int team;
};


FVector RotationToVector(FRotator R)
{
	FVector Vec;
	float fYaw = R.Yaw * URotationToRadians;
	float fPitch = R.Pitch * URotationToRadians;
	float CosPitch = cos(fPitch);
	Vec.X = cos(fYaw) * CosPitch;
	Vec.Y = sin(fYaw) * CosPitch;
	Vec.Z = sin(fPitch);

	return Vec;
}

float Size(FVector& v)
{
	return sqrt(v.X * v.X + v.Y * v.Y + v.Z * v.Z);
}

void Normalize(FVector& v)
{
	float size = Size(v);

	if (!size)
		v.X = v.Y = v.Z = 1;
	else
	{
		v.X /= size;
		v.Y /= size;
		v.Z /= size;
	}
}

FVector VectorSubtract(FVector va, FVector vb);

void GetAxes(FRotator R, FVector& X, FVector& Y, FVector& Z)
{
	X = RotationToVector(R);
	Normalize(X);
	R.Yaw += 89.8f;
	FRotator R2 = R;
	R2.Pitch = 0.0f;
	Y = RotationToVector(R2);
	Normalize(Y);
	Y.Z = 0.0f;
	R.Yaw -= 89.8f;
	R.Pitch += 89.8f;
	Z = RotationToVector(R);
	Normalize(Z);
}

FVector VectorSubtract(FVector va, FVector vb)
{
	FVector out;

	out.X = va.X - vb.X;
	out.Y = va.Y - vb.Y;
	out.Z = va.Z - vb.Z;

	return out;
}

float inline Dot(const FVector& V1, const FVector& V2)
{
	return (V1.X * V2.X + V1.Y * V2.Y + V1.Z * V2.Z);
}

vec2 W2S(FVector Location, FMinimalViewInfo cam_cache)
{
	vec2 Return;

	FVector AxisX, AxisY, AxisZ, Delta, Transformed;
	GetAxes(cam_cache.Rotation, AxisX, AxisY, AxisZ);

	Delta = VectorSubtract(Location, cam_cache.Location);
	Transformed.X = Dot(Delta, AxisY);
	Transformed.Y = Dot(Delta, AxisZ);
	Transformed.Z = Dot(Delta, AxisX);

	if (Transformed.Z < 1.00f)
		Transformed.Z = 1.00f;

	Return.x = ScreenCenterX + Transformed.X * (ScreenCenterX / tan(cam_cache.FOV * UCONST_Pi / 360.0f)) / Transformed.Z;
	Return.y = ScreenCenterY + -Transformed.Y * (ScreenCenterX / tan(cam_cache.FOV * UCONST_Pi / 360.0f)) / Transformed.Z;

	return Return;
}

static std::string GetNameById(uint32_t actor_id)
{
	char pNameBuffer[256];
	int TableLocation = (unsigned int)(actor_id >> 0x10);
	uint16_t RowLocation = (unsigned __int16)actor_id;
	uint64_t GNameTable = mem::process_base + offsets::GNAMES;

	uint64_t TableLocationAddress = read<uint64_t>(GNameTable + 0x10 + TableLocation * 0x8) + (unsigned __int32)(4 * RowLocation);

	uint64_t sLength = (unsigned __int64)(read<uint16_t>(TableLocationAddress + 4)) >> 1;

	if (sLength < 128)
	{
		mem::read_physical((PVOID*)(TableLocationAddress + 6), pNameBuffer, sLength);
		return std::string(pNameBuffer);
	}
	return std::string("NULL");
}
