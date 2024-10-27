#ifndef AUDIO_METER_H
#define AUDIO_METER_H

#include "raylib.h"
#include "sample_buffer.h"

void dB_meter_render(SampleBuffer *sample_buffer, Vector2 position, Vector2 size);

#endif
