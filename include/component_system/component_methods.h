#ifndef COMPONENT_METHODS
#define COMPONENT_METHODS

#include "component_system/component.h"
#include "midi/midi.h"
#include "raylib.h"
#include <stddef.h>

/* Lifetime methods */
typedef Component *(*ComponentInit)(Component *parent);
typedef void (*ComponentFree)(Component *self);
typedef void (*ComponentPreview)(Vector2 position, Vector2 size);

/* Callback methods */
typedef void (*ComponentAudioCallback)(Component *self, float *buffer, size_t size);
typedef void (*ComponentMidiCallback)(Component *self, MidiMessage *messages, size_t size, bool system);

/* Rendering methods */
typedef void (*ComponentSettingsRender)(Component *self, Vector2 position, Vector2 size);
typedef void (*ComponentRender)(Component *self, Vector2 position, Vector2 size);

/* Container related methods */
typedef void (*ComponentMount)(Component *self, Component *component);
typedef void (*ComponentUnmount)(Component *self);
typedef Component *(*ComponentCurrent)(Component *self);
typedef void (*ComponentMoveCursor)(Component *self, Vector2 delta);

typedef struct {
    ComponentInit init;
    ComponentFree free;
    ComponentPreview preview;
    ComponentAudioCallback audio_callback;
    ComponentMidiCallback midi_callback;
    ComponentSettingsRender settings_render;
    ComponentRender render;
    ComponentMount mount;
    ComponentUnmount unmount;
    ComponentCurrent current;
    ComponentMoveCursor move_cursor;
} ComponentMethods;

#endif
