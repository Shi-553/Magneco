// --------------------------------------------------------------
//  ŽžŠÔ‚Å‚·[time.h]
// 
//							Author:Kanna Noda
//----------------------------------------------------------------

#include "time.h"
#include "debugPrintf.h"

static double g_lastUpdateTime;
static double g_currentTime;


void InitTime(double currentTime) {
	g_lastUpdateTime = currentTime;
	g_currentTime = currentTime;

}

void UninitTime() {

}

void UpdateTime(double currentTime) {
	g_lastUpdateTime = g_currentTime;
	g_currentTime = currentTime;

	// DebugPrintf("%lf\n", GetDeltaTime());

}

double GetDeltaTime() {

	return  g_currentTime - g_lastUpdateTime;
}
