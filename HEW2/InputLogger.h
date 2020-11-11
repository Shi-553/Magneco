#ifndef _INPUT_LOGGER_H
#define _INPUT_LOGGER_H

typedef enum VirtualKey {
	MYVK_UP,
	MYVK_DOWN,
	MYVK_LEFT,
	MYVK_RIGHT,
	MYVK_ENTER,
	MYVK_BEACON,
	MYVK_SHOT2,
	MYVK_MAX
};

typedef enum VirtualAxis {
	MYVA_X ,
	MYVA_Y ,
	MYVA_MAX
};

void InitInputLogger();
void UninitInputLogger();
void UpdateInputLogger();

bool PressInputLogger(VirtualKey key);
bool TriggerInputLogger(VirtualKey key);
bool ReleaseInputLogger(VirtualKey key);
int GetInputLoggerAxis(VirtualAxis axis);
int GetInputLoggerAxisAmount(VirtualAxis axis);

void RecordStart( int frameMax);
void RecordEnd();

void RecordLoad();
void TraceStart();
void TraceEnd();
bool IsTrace();


void SetRecordFilename(const char* f, size_t size);
#endif // !_INPUT_LOGGER_H
