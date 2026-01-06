#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

/*
 * Simple C Preprocessor for "Zig-like" Platform Tags.
 *
 * Syntax:
 *   @windows {
 *       // windows specific code
 *   }
 *
 *   @linux {
 *       // linux specific code
 *   }
 *
 *   @arm { ... }
 *
 * Output:
 *   #if defined(_WIN32)
 *       // windows specific code
 *   #endif
 *
 *   #if defined(__linux__)
 *      // linux specific code
 *   #endif
 */

typedef struct {
    const char *tag;
    const char *macro;
} PlatformMap;

static const PlatformMap MAPPINGS[] = {
    {"windows", "_WIN32"},
    {"linux", "__linux__"},
    {"unix", "__unix__"},
    {"macos", "__APPLE__"},
    {"apple", "__APPLE__"},
    {"android", "__ANDROID__"},
    {"x86", "__i386__"},
    {"x64", "__x86_64__"},
    {"amd64", "__x86_64__"},
    {"arm", "__arm__"},
    {"aarch64", "__aarch64__"},
    {"debug", "DEBUG"},
    {NULL, NULL}
};

const char *get_macro_for_tag(const char *tag) {
    for (int i = 0; MAPPINGS[i].tag != NULL; i++) {
        if (strcmp(MAPPINGS[i].tag, tag) == 0) {
            return MAPPINGS[i].macro;
        }
    }
    return NULL;
}

void process_file(FILE *in, FILE *out) {
    int ch;
    char buffer[256];
    int buf_idx = 0;

    while ((ch = fgetc(in)) != EOF) {
        if (ch == '@') {
            // Potential tag start
            buf_idx = 0;
            // Read the tag name
            while ((ch = fgetc(in)) != EOF && isalnum(ch)) {
                if (buf_idx < (int)sizeof(buffer) - 1) {
                    buffer[buf_idx++] = (char)ch;
                }
            }
            buffer[buf_idx] = '\0';

            const char *macro = get_macro_for_tag(buffer);
            
            // Skip whitespace after tag to find '{'
            while (ch != EOF && isspace(ch)) {
                ch = fgetc(in);
            }

            if (macro && ch == '{') {
                // It is a valid block!
                fprintf(out, "#if defined(%s)", macro);
                
                // Track brace depth to find the matching closing '}'
                int depth = 1;
                while (depth > 0 && (ch = fgetc(in)) != EOF) {
                    if (ch == '{') depth++;
                    else if (ch == '}') depth--;

                    if (depth > 0) {
                        fputc(ch, out);
                    }
                }
                fprintf(out, "\n#endif");
            } else {
                // Not a known tag or malformed, print everything back as is
                fprintf(out, "@%s", buffer);
                if (ch != EOF) fputc(ch, out);
            }
        } else {
            fputc(ch, out);
        }
    }
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <input_file>\n", argv[0]);
        return 1;
    }

    FILE *in = fopen(argv[1], "r");
    if (!in) {
        perror("Failed to open input file");
        return 1;
    }

    // We output to stdout, which can be redirected
    process_file(in, stdout);

    fclose(in);
    return 0;
}
