#define _CRT_SECURE_NO_WARNINGS

#include "InputLogger.h"
#include "keyboard.h"
#include "mouse.h"
#include "debugPrintf.h"

#define KEY_CONVERSIOINS_MAX (16)
#define AXIS_CONVERSIOINS_MAX (8)



typedef struct KeyConversion {
	VirtualKey virtualKey;
	int realKey;
};

typedef struct AxisConversion {
	VirtualAxis virtualAxis;
	int realAxis;
};

enum LoggerMode
{
	LOGGER_NORMAL,
	LOGGER_RECORD,
	LOGGER_TRACE,
};

static LoggerMode inputloggerMode = LOGGER_NORMAL;

typedef unsigned short InputLoggerType;

static InputLoggerType current = 0;
static InputLoggerType previous = 0;
static InputLoggerType trigger = 0;
static InputLoggerType release = 0;

static InputLoggerType* recordCurrent = NULL;

static int currentAxis[AXIS_CONVERSIOINS_MAX] = {};
static int previousAxis[AXIS_CONVERSIOINS_MAX] = {};

static  int* recordCurrentAxis[AXIS_CONVERSIOINS_MAX] = {};

static int recordFrame = 0;
static int recordTraceFrame = 0;

static char* recordFilename = NULL;
static int gRecordframeMax = 0;


static KeyConversion KeyConversions[KEY_CONVERSIOINS_MAX]{
	{MYVK_UP,KK_W},
	{MYVK_DOWN,KK_S},
	{MYVK_LEFT,KK_A},
	{MYVK_RIGHT,KK_D},
	{MYVK_SHOT,KK_ENTER},
	{MYVK_SHOT,MOUSE_LEFT},
	{MYVK_SHOT2,MOUSE_RIGHT},
	{MYVK_SHOT2,KK_SPACE},
	{MYVK_ENTER,KK_ENTER},
};

static AxisConversion AxisConversions[KEY_CONVERSIOINS_MAX]{
	{MYVA_X,MOUSE_X},
	{MYVA_Y,MOUSE_Y},
};

void InitInputLogger() {
	current = previous = trigger = release = 0;
	ZeroMemory(currentAxis,sizeof(currentAxis));
	ZeroMemory(previousAxis,sizeof(previousAxis));

	inputloggerMode = LOGGER_NORMAL;
	recordCurrent = NULL;
	for (size_t i = 0; i < AXIS_CONVERSIOINS_MAX; i++)
	{
		free(recordCurrentAxis[i]);
		recordCurrentAxis[i] = NULL;
	}
	recordFrame = 0;
	recordTraceFrame = 0;

	const char f[] = "keylogger.dat";
	SetRecordFilename(f, sizeof(f));
}


void UninitInputLogger() {

	free(recordCurrent);
	recordCurrent = NULL;

	for (size_t i = 0; i < AXIS_CONVERSIOINS_MAX; i++)
	{
		free(recordCurrentAxis[i]);
		recordCurrentAxis[i] = NULL;
	}
	free(recordFilename);
	recordFilename = NULL;
}

void UpdateInputLogger() {

	previous = current;
	for (size_t i = 0; i < AXIS_CONVERSIOINS_MAX; i++) {
		previousAxis[i] = currentAxis[i];
	}

	if (inputloggerMode == LOGGER_NORMAL || inputloggerMode == LOGGER_RECORD) {
		current = 0;

		for (int i = 0; i < KEY_CONVERSIOINS_MAX; i++)
		{
			if (KeyConversions[i].realKey == 0)
				continue;

			if (KeyConversions[i].realKey < KK_MAX) {
				if (Keyboard_IsKeyDown((Keyboard_Keys)KeyConversions[i].realKey)) {
					current |= 1u << KeyConversions[i].virtualKey;
				}
			}
			else if (KeyConversions[i].realKey < MOUSE_BUTTONS_MAX) {
				if (Mouse_IsKeyDown((Mouse_Buttons)KeyConversions[i].realKey)) {
					current |= 1u << KeyConversions[i].virtualKey;
				}
			}
		}

		for (int i = 0; i < KEY_CONVERSIOINS_MAX; i++)
		{
			if (AxisConversions[i].realAxis == 0)
				continue;

			if (AxisConversions[i].realAxis < MOUSE_AXIS_MAX) {
				currentAxis[(int)AxisConversions[i].virtualAxis] = Mouse_GetAxis((Mouse_Axis)AxisConversions[i].realAxis);

			}
		}

		if (inputloggerMode == LOGGER_RECORD) {
			recordCurrent[recordFrame] = current;
			for (size_t i = 0; i < AXIS_CONVERSIOINS_MAX; i++)
			{
				recordCurrentAxis[i][recordFrame] = currentAxis[i];

			}
			recordFrame++;
			if (recordFrame >= gRecordframeMax) {
				RecordEnd();
			}
		}
	}

	else if (inputloggerMode == LOGGER_TRACE) {
		current = recordCurrent[recordTraceFrame];

		for (size_t i = 0; i < AXIS_CONVERSIOINS_MAX; i++){
			currentAxis[i] = recordCurrentAxis[i][recordTraceFrame];
		}

		recordTraceFrame++;

		if (recordTraceFrame >= recordFrame) {
			TraceEnd();
		}
	}
	trigger = (previous ^ current) & current;
	release = (previous ^ current) & previous;
}

bool PressInputLogger(VirtualKey key) {
	return current & (1u << (int)key);
}
bool TriggerInputLogger(VirtualKey key) {
	return trigger & (1u << (int)key);
}
bool ReleaseInputLogger(VirtualKey key) {
	return release & (1u << (int)key);
}
int GetInputLoggerAxis(VirtualAxis axis) {
	return currentAxis[(int)axis];
}
int GetInputLoggerAxisAmount(VirtualAxis axis) {
	return  currentAxis[(int)axis]- previousAxis[(int)axis];
}


void RecordStart(int recordFrameMax) {
	gRecordframeMax = recordFrameMax;
	free(recordCurrent);

	recordCurrent = (InputLoggerType*)malloc(sizeof(InputLoggerType) * recordFrameMax);
	if (recordCurrent == NULL) {
		throw  "Out of memory";
		return;
	}


	for (size_t i = 0; i < AXIS_CONVERSIOINS_MAX; i++)
	{
		free(recordCurrentAxis[i]);

		recordCurrentAxis[i] = (int*)malloc(sizeof(recordCurrentAxis[i]) * recordFrameMax);
		if (recordCurrentAxis[i] == NULL) {
			throw  "Out of memory";
			return;
		}
	}

	inputloggerMode = LOGGER_RECORD;
	recordFrame = 0;
}

void RecordEnd() {
	FILE* fp = fopen(recordFilename, "wb");
	fwrite(&recordFrame, sizeof(recordFrame), 1, fp);

	fwrite(recordCurrent, sizeof(InputLoggerType), recordFrame, fp);
	for (size_t i = 0; i < AXIS_CONVERSIOINS_MAX; i++)
	{
		fwrite(recordCurrentAxis[i], sizeof(recordCurrentAxis[i]), recordFrame, fp);
	}
	fclose(fp);

	inputloggerMode = LOGGER_NORMAL;
}

void RecordLoad() {

	FILE* fp = fopen(recordFilename, "rb");
	fread(&recordFrame, sizeof(recordFrame), 1, fp);


	free(recordCurrent);

	recordCurrent = (InputLoggerType*)malloc(sizeof(InputLoggerType) * recordFrame);
	if (recordCurrent == NULL) {
		throw  "Out of memory";
		return;
	}
	for (size_t i = 0; i < AXIS_CONVERSIOINS_MAX; i++)
	{
		free(recordCurrentAxis[i]);

		recordCurrentAxis[i] = (int*)malloc(sizeof(recordCurrentAxis[i]) * recordFrame);
		if (recordCurrentAxis[i] == NULL) {
			throw  "Out of memory";
			return;
		}
	}

	fread(recordCurrent, sizeof(InputLoggerType), recordFrame, fp);

	for (size_t i = 0; i < AXIS_CONVERSIOINS_MAX; i++)
	{
		fread(recordCurrentAxis[i], sizeof(recordCurrentAxis[i]), recordFrame, fp);
	}
	fclose(fp);
}

void TraceStart() {
	recordTraceFrame = 0;
	inputloggerMode = LOGGER_TRACE;
}

void TraceEnd() {
	recordTraceFrame = 0;
	inputloggerMode = LOGGER_NORMAL;
}

bool IsTrace() {
	return (inputloggerMode == 2);
}

void SetRecordFilename(const char* f, size_t size) {
	if (size <= 0) {
		return;
	}

	free(recordFilename);
	recordFilename = (char*)malloc(size + 1);

	if (recordFilename == NULL) {
		throw  "Out of memory";
		return;
	}

	for (size_t i = 0; i < size; i++)
	{
		recordFilename[i] = f[i];
	}
	recordFilename[size] = '\0';
}