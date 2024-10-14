#include <assert.h>
#include "rack/component.h"
#include "rack/component_handler.h"
#include "components/oscillator.h"

void component_handler_init() {
    library_component_table[COMPONENT_OSCILLATOR].factory = oscillator_init;
    library_component_table[COMPONENT_OSCILLATOR].preview_callback = oscillator_preview;
}

Component *component_create(ComponentType type) {
    LibraryComponent library_component = library_component_table[type];
    assert(library_component.factory != NULL);
    return (*library_component.factory)();
}

void component_preview(ComponentType type, Vector2 position) {
    LibraryComponent library_component = library_component_table[type];
    assert(library_component.preview_callback != NULL);
    return (*library_component.preview_callback)(position);
}
