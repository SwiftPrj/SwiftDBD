#include "offsets.h"
#include <cmath>
#include "SDK.h"
// ###################################################### Some useful stuff (I was lazy) ###################################################### \\


std::vector<EntityList> entityList;

HWND hwnd = NULL;
HWND hwnd_active = NULL;
HWND OverlayWindow = NULL;
auto CrosshairColor = D2D1::ColorF(0, 100, 255, 255);
auto Color = D2D1::ColorF(255, 0, 0, 255);

FCameraCacheEntry cameraCache;
int actorsCount;
uintptr_t actorsArray;
std::string nameS;
SDK sdk;
void ESPLoop() {

	hwnd = FindWindowA("UnrealWindow", NULL); 
	OverlayWindow = FindWindow("CEF-OSC-WIDGET", "NVIDIA GeForce Overlay");
	hwnd_active = GetForegroundWindow();

	if (hwnd_active == hwnd) {

		ShowWindow(OverlayWindow, SW_SHOW);
	}
	else
	{
		ShowWindow(OverlayWindow, SW_HIDE);
	}
}

void RenderMenu()
{
	if (showmenu && rendering)
	{
		if (esp)
			g_overlay->draw_text(5, 20, D2D1::ColorF(0, 255, 0, 255), "F1 ESP : ON");
		else
			g_overlay->draw_text(5, 20, D2D1::ColorF(255, 0, 0, 255), "F1 ESP : OFF");

		if (espBox)
			g_overlay->draw_text(5, 40, D2D1::ColorF(0, 255, 0, 255), "F4 ESP Box : ON");
		else
			g_overlay->draw_text(5, 40, D2D1::ColorF(255, 0, 0, 255), "F4 ESP Box : OFF");

		if (name)
			g_overlay->draw_text(5, 60, D2D1::ColorF(0, 255, 0, 255), "F5 NAME : ON");
		else
			g_overlay->draw_text(5, 60, D2D1::ColorF(255, 0, 0, 255), "F5 NAME: OFF");

		if (mate)
			g_overlay->draw_text(5, 80, D2D1::ColorF(0, 255, 0, 255), "F6 TEAM : ON");
		else
			g_overlay->draw_text(5, 80, D2D1::ColorF(255, 0, 0, 255), "F6 TEAM : OFF");
	}
}

inline PVOID BaseThread2()
{
	while (true)
	{
		std::vector<EntityList> tmpList;
		cameraCache = sdk.GetUWorld()->GetGameInstance()->GetLocalPlayer()->GetPlayerController()->GetCameraManager()->GetCameraCachePrivate();
		actorsCount = sdk.GetUWorld()->GetPersistentLevel()->GetActorCount();
		actorsArray = sdk.GetUWorld()->GetPersistentLevel()->GetActorArray();
		auto fov = cameraCache.POV.FOV;
		std::cout << fov << std::endl;
		uintptr_t playerStateLocal = sdk.GetUWorld()->GetGameInstance()->GetLocalPlayer()->GetPlayerController()->GetPlayerState()->Address;
		for (int i = 0; i < actorsCount; i++)
		{
			UDBDPlayer player;
			uintptr_t actor = read<uintptr_t>(actorsArray + i * 0x8);
			if (!actor)
				continue;
			player.instance = actor;
			player.objectId = read<int>(actor + 0x18);
			if (GetNameById(player.objectId).find("BP_CamperMale") != std::string::npos || GetNameById(player.objectId).find("BP_CamperFemale") != std::string::npos || GetNameById(player.objectId).find("BP_Slasher") != std::string::npos)
			{
				player.PlayerState = read<uintptr_t>(actor + offsets::PLAYER_STATE);
				player.Pawn = read<uintptr_t>(actor + offsets::ACKNOWLEDGED_PAWN);
				player.root_component = read<uintptr_t>(player.instance + offsets::ROOT_COMPONENT);
				player.origin = read<FVector>(player.root_component + offsets::RELATIVE_LOCATION);
				FVector tempVec;
				tempVec.X = player.origin.X;
				tempVec.Y = player.origin.Y;
				tempVec.Z = player.origin.Z + 125;
				player.TopLocation = tempVec;
				float DistM = 250;

				if (player.PlayerState == playerStateLocal)
					continue;


				if (GetNameById(player.objectId).find("BP_CamperMale") != std::string::npos)
					nameS = "MaleSurvivor";

				if (GetNameById(player.objectId).find("BP_CamperFemale") != std::string::npos)
					nameS = "FemaleSurvivor";

				if (GetNameById(player.objectId).find("BP_Slasher") != std::string::npos)
					nameS = "Killer";

				EntityList Entity{ };
				Entity.instance = player.instance;
				Entity.objectId = player.objectId;
				Entity.PlayerState = player.PlayerState;
				Entity.health = player.health;
				Entity.root_component = player.root_component;
				Entity.origin = player.origin;
				Entity.TopLocation = player.TopLocation;
				Entity.dist = DistM;
				Entity.name = nameS;
				tmpList.push_back(Entity);
			}
			else {
				continue;
			}
		}			
		entityList = tmpList;
		Sleep(0);

	}
}

void RenderESP()
{
	if (esp)
	{
		Vector3 Screen, head;
		auto EntityList_Copy = entityList;
		for (int i = 0; i < EntityList_Copy.size(); i++)
		{
			auto Entity = EntityList_Copy[i];
			vec2 Screen = W2S(Entity.origin, cameraCache.POV);
			vec2 head = W2S(Entity.TopLocation, cameraCache.POV);
			if (!mate & Entity.name == "MaleSurvivor" || !mate & Entity.name == "FemaleSurvivor")
				continue;

			if (Screen.x && head.x)
			{
				if (espBox)
					g_overlay->draw_boxnew(Screen.x, Screen.y, 25, 50, Color);
					
				if (name)
				{
					if (Entity.name == "Killer")
						g_overlay->draw_text(Screen.x - 50, Screen.y + 50, D2D1::ColorF(255, 0, 0, 255), Entity.name.c_str());
					else
						g_overlay->draw_text(Screen.x - 50, Screen.y + 50, D2D1::ColorF(0, 255, 0, 255), Entity.name.c_str());
				}
			}

		}
	}
}