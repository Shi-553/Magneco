#include "keyboard.h"
#include "mouse.h"

#include "player.h"
#include "debugPrintf.h"
#include "InputLogger.h"

void InitGameOperation() {
	const char f[] = "logger.dat";
	SetRecordFilename(f, sizeof(f));

	//RecordStart(60*60);

	//RecordLoad();
	//TraceStart();
}

void UpdateGameOperation() {
}

void UninitGameOperation() {
		//RecordEnd();
	
}