#pragma once
#include <Windows.h>
namespace offsets
{
	constexpr auto UWORLD = 0xf86ae30;
	constexpr auto PERSISTENT_LEVEL = 0x38;
	constexpr auto OWNING_GAME_INSTANCE = 0x1c8;
	constexpr auto LOCAL_PLAYERS = 0x40;
	constexpr auto PLAYER_CONTROLLER = 0x38;
	constexpr auto PLAYER_ID = 0x2a4;
	constexpr auto PAWN_PRIVATE = 0x320;
	constexpr auto PLAYER_NAME_PRIVATE = 0x3a0;
	constexpr auto PLAYER_STATE = 0x2a8;
	constexpr auto ROOT_COMPONENT = 0x1a8;
	constexpr auto PLAYER_CAMERA_MANAGER = 0x360;
	constexpr auto RELATIVE_LOCATION = 0x140;
	constexpr auto RELATIVE_ROTATION = 0x158;
	constexpr auto ACKNOWLEDGED_PAWN = 0x350;
	constexpr auto GNAMES = 0xf612900;

}