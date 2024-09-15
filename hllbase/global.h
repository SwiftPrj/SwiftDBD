#include "overlay.h"
#include "driver.h"

bool esp = false;
bool espBox = false;
bool enemyESP = false;
bool name = false;
bool mate = false;

bool showmenu = true;
bool rendering = true;
int frame = 0;
FOverlay* g_overlay;

float ScreenCenterX = 0;
float ScreenCenterY = 0;

float FOV = 40;