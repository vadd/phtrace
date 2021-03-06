#include <stdlib.h>
#include <stdio.h>
#include "buffer.h"
#include <uuid/uuid.h>

phtrace_buffer_t phtrace_buffer;

static FILE *f;

void phtrace_buffer_allocate() {
    size_t size = 1024 * 1024 * 5;
    phtrace_buffer.data = (unsigned char *) malloc(size);
    if (!phtrace_buffer.data) {
        // TODO: find a proper way to signal about errors
        printf("failed to allocate memory for the data buffer\n");
    }

    phtrace_buffer.size = size;
    phtrace_buffer.used = 0;
}

void phtrace_buffer_free() {
    if (phtrace_buffer.data) {
        free(phtrace_buffer.data);
        phtrace_buffer.data = NULL;
    }

    phtrace_buffer.size = 0;
    phtrace_buffer.used = 0;
}

void phtrace_buffer_flush() {
    if (phtrace_buffer.used == 0) {
        return;
    }
    if (!f) {
        uuid_t uuid;
        uuid_generate(uuid);
        char uuid_str[37];
        uuid_unparse_lower(uuid, uuid_str);

        char filename[100];
        sprintf(filename, "/tmp/%s.phtrace", uuid_str);

        f = fopen(filename, "w");
    }
    fwrite(phtrace_buffer.data, 1, phtrace_buffer.used, f);
    fflush(f);

    phtrace_buffer.used = 0;
}

void phtrace_buffer_close() {
    phtrace_buffer_flush();
    if (f) {
      fclose(f);
      f = NULL;
    }
}
