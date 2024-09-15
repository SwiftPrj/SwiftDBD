#pragma once
#include <Windows.h>
#include <TlHelp32.h>
#include <cstdint>
#include "global.h"
#include "offsets.h" 
#include "structs.h"
#include "driver.h"
class UWorld;
class UPersistentLevel;
class UGameInstance;
class UPlayer;
class APawn;
class APlayerState;
class APlayerController;
class USceneComponent;
class APlayerCameraManager;

class UObject
{
public:
    static uintptr_t UWorldAddress; 

    static void Initialize()
    {
        UWorldAddress = read<uintptr_t>(mem::process_base + offsets::UWORLD);
    }
};


uintptr_t UObject::UWorldAddress = 0;



class UPersistentLevel
{
public:
    uintptr_t Address;

    UPersistentLevel(uintptr_t address) : Address(address) {}

    int GetActorCount()
    {
        return read<int>(Address + 0xa8);
    }
    uintptr_t GetActorArray()
    {
        return read<uintptr_t>(Address + 0xa0);
    }
};

class USceneComponent
{
public:
    uintptr_t Address;
    USceneComponent(uintptr_t address) : Address(address) {}

    FVector GetRelativeLocation()
    {
        return read<FVector>(Address + offsets::RELATIVE_LOCATION);
    }

    FRotator GetRelativeRotation()
    {
        return read<FRotator>(Address + offsets::RELATIVE_ROTATION);
    }

    FVector GetRelativeScale()
    {
        return read<FVector>(Address + 0x170);
    }
};

class APawn
{
public:
    uintptr_t Address;
    APawn(uintptr_t address) : Address(address) {}

    USceneComponent* GetRootComponent()
    {
        auto address = read<uintptr_t>(Address + offsets::ROOT_COMPONENT);
        return new USceneComponent(address);
    }
};

class APlayerState
{
public:
    uintptr_t Address;
    APlayerState(uintptr_t address) : Address(address) {}

    int GetPlayerId()
    {
        return read<int>(Address + offsets::PLAYER_ID);
    }
    APawn* GetPawnPrivate()
    {
        auto address = read<uintptr_t>(Address + offsets::PAWN_PRIVATE);
        return new APawn(address);
    }
    
};

class APlayerCameraManager {
public:
    uintptr_t Address;
    APlayerCameraManager(uintptr_t address) : Address(address) {}
    float GetDefaultFOV()
    {
        return read<float>(Address + 0x2bc);
    }
    float GetDefaultOrthoWidth()
    {
        return read<float>(Address + 0x2c4);
    }
    float GetDefaultAspectRatio()
    {
        return read<float>(Address + 0x2cc);
    }
    FCameraCacheEntry GetCameraCache()
    {
        return read<FCameraCacheEntry>(Address + 0x340);
    }
    FCameraCacheEntry GetCameraCachePrivate()
    {
        return read<FCameraCacheEntry>(Address + 0x22d0);
    }

};

class APlayerController
{
public:
    uintptr_t Address;
    APlayerController(uintptr_t address) : Address(address) {}
    APlayerState* GetPlayerState()
    {
        auto address = read<uintptr_t>(Address + offsets::PLAYER_STATE);
        return new APlayerState(address);
    }
    APawn* GetAcknowledgedPawn()
    {
        auto address = read<uintptr_t>(Address + offsets::ACKNOWLEDGED_PAWN);
        return new APawn(address);
    }
    APlayerCameraManager* GetCameraManager()
    {
        auto address = read<uintptr_t>(Address + offsets::PLAYER_CAMERA_MANAGER);
        return new APlayerCameraManager(address);
    }

};

class UPlayer
{
public:
    uintptr_t Address;
    UPlayer(uintptr_t address) : Address(address) {}
    APlayerController* GetPlayerController()
    {
        auto address = read<uintptr_t>(Address + offsets::PLAYER_CONTROLLER);
        return new APlayerController(address);
    }


};

class UGameInstance
{
public:
    uintptr_t Address;

    UGameInstance(uintptr_t address) : Address(address) {}
    uintptr_t GetLocalPlayers()
    {
        return read<uintptr_t>(Address + offsets::LOCAL_PLAYERS);
    }
    UPlayer* GetLocalPlayer()
    {
        uintptr_t playerAddress = read<uintptr_t>(GetLocalPlayers());

        return new UPlayer(playerAddress);
    }
};

class UWorld
{
public:
    uintptr_t Address;

    UWorld(uintptr_t address) : Address(address) {}

    UPersistentLevel* GetPersistentLevel()
    {
        uintptr_t persistentLevelAddress = read<uintptr_t>(Address + offsets::PERSISTENT_LEVEL);
        return new UPersistentLevel(persistentLevelAddress);
    }
    UGameInstance* GetGameInstance()
    {
        uintptr_t owningGameInstanceAddress = read<uintptr_t>(Address + offsets::OWNING_GAME_INSTANCE);
        return new UGameInstance(owningGameInstanceAddress);
    }

};

class SDK
{
public:
    SDK() = default;

    UWorld* GetUWorld()
    {
        uintptr_t uworldAddress = read<uintptr_t>(mem::process_base + offsets::UWORLD);
        if (uworldAddress)
        {
            return new UWorld(uworldAddress);
        }
    }
};
