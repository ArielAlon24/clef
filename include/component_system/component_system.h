#ifndef COMPONENT_SYSTEM_H
#define COMPONENT_SYSTEM_H

#include "component_system/component.h"
#include "component_system/component_methods.h"

#include "components/oscillator.h"
#include "components/rack.h"
#include <stdbool.h>

static ComponentMethods COMPONENT_METHODS_MAPPING[_COMPONENT_TYPE_SIZE];

void component_system_init(void);

Component *component_init(ComponentType type, Component *parent);
void component_free(Component *self);
void component_preview(ComponentType type, Vector2 position, Vector2 size);

void component_audio_callback(Component *self, float *buffer, size_t size);
void component_midi_callback(Component *self, const MidiMessage *messages, size_t size, bool system);

void component_settings_render(Component *self, Vector2 position, Vector2 size);
void component_render(Component *self, Vector2 position, Vector2 size);
void component_rack_render(Component *self, Vector2 position, Vector2 size);

void component_mount(Component *self, Component *component);
void component_unmount(Component *self);
Component *component_current(Component *self);
void component_move_cursor(Component *self, Vector2 delta);

#endif
