#pragma once
#include <fstream>

void load_cfg();
void default_cfg();
void change_cfg(float master_volume, float sfx_volume, float music_volume);
void abort_cfg();
void apply_cfg(std::ifstream& config);
