#include "app.h"
#include "ring_buffer.h"

int main(void) {
    app_init();
    app_run();
    app_free();
}
