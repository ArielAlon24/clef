#include <assert.h>
#include <stdbool.h>
#include "component_system/component_system.h"
#include "component_system/component.h"
#include "component_system/component_methods.h"

#include "components/oscillator.h"
#include "components/rack.h"
#include "components/envelope.h"

#define COMPONENT_METHOD_CALL(type, method, ...) \
    ComponentMethods methods = COMPONENT_METHODS_MAPPING[(type)]; \
    assert(methods.method != NULL); \
    return methods.method(__VA_ARGS__);

void component_system_init(void) {
    COMPONENT_METHODS_MAPPING[COMPONENT_OSCILLATOR] = oscillator_methods;
    COMPONENT_METHODS_MAPPING[COMPONENT_RACK] = rack_methods;
    COMPONENT_METHODS_MAPPING[COMPONENT_ENVELOPE] = envelope_methods;
}

Component *component_init(ComponentType type, Component *parent) {
    COMPONENT_METHOD_CALL(type, init, parent);
}

void component_free(Component *self) {
    COMPONENT_METHOD_CALL((self->type), free, self);
}

void component_preview(ComponentType type, Vector2 position, Vector2 size) {
    COMPONENT_METHOD_CALL(type, preview, position, size);
}

void component_audio_callback(Component *self, float *buffer, size_t size) {
    COMPONENT_METHOD_CALL((self->type), audio_callback, self, buffer, size);
}
void component_midi_callback(Component *self, const MidiMessage *messages, size_t size, bool system) {
    COMPONENT_METHOD_CALL((self->type), midi_callback, self, messages, size, system);
}

void component_settings_render(Component *self, Vector2 position, Vector2 size) {
    COMPONENT_METHOD_CALL((self->type), settings_render, self, position, size);
}
void component_render(Component *self, Vector2 position, Vector2 size) {
    COMPONENT_METHOD_CALL((self->type), render, self, position, size);
}

void component_rack_render(Component *self, Vector2 position, Vector2 size) {
    COMPONENT_METHOD_CALL((self->type), rack_render, self, position, size);
}

void component_mount(Component *self, Component *component) {
    COMPONENT_METHOD_CALL((self->type), mount, self, component);
}

void component_unmount(Component *self) {
    COMPONENT_METHOD_CALL((self->type), unmount, self);
}

Component *component_current(Component *self) {
    COMPONENT_METHOD_CALL((self->type), current, self);
}

void component_move_cursor(Component *self, Vector2 delta) {
    COMPONENT_METHOD_CALL((self->type), move_cursor, self, delta);
}
