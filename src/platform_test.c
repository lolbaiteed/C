#include <stdio.h>

void print_platform_info() {
    printf("Running on: ");

    #if defined(_WIN32)
        printf("Windows\n");
    
#endif

    #if defined(__linux__)
        printf("Linux\n");
    
#endif
    
    #if defined(__ANDROID__)
        printf("Android\n");
    
#endif

    #if defined(__APPLE__)
        printf("MacOS\n");
    
#endif
}

