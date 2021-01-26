#include "time.h"
#include "debugPrintf.h"

static double g_lastUpdateTime;
static double g_currentTime;
static double scale;

void InitTime(double currentTime) {
	g_lastUpdateTime = currentTime;
	g_currentTime = currentTime;
	scale = 1;
}

void UninitTime() {

}

void UpdateTime(double currentTime) {
	g_lastUpdateTime = g_currentTime;
	g_currentTime = currentTime;

	// DebugPrintf("%lf\n", GetDeltaTime());

}

double GetDeltaTime() {

	return  (g_currentTime - g_lastUpdateTime)*scale;
}

void SetTimeScale(double s) {
	scale = s;
}