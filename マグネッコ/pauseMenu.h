#pragma once 
#include <functional>

typedef std::function<void(bool)> SwitchPauseEvent;

void InitPauseMenu();
void UninitPauseMenu();
void DrawPauseMenu();

void UpdatePauseMenu();

bool IsPause();
int AddSwitchPauseEvent(const SwitchPauseEvent& e);
void RemoveSwitchPauseEvent(int i);