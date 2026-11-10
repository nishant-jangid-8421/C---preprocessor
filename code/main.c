#include <stdio.h>
#include <string.h>

#include "comment_removal.h"
#include "file_inclusion.h"
#include "macro_handler.h"

int main(int argc, char *argv[])
{
    char output[512];
    char *dot, *slash;

    if(argc != 2)
    {
        printf("Usage: ./mypreprocessor input.c\n");
        return 1;
    }

    snprintf(output, sizeof(output), "%s", argv[1]); // copy file name in output

    dot = strrchr(output, '.');//last .
    slash = strrchr(output, '/');// last slash

    if(dot != NULL && (slash == NULL || dot > slash))
        *dot = '\0'; // replace dot terminate string file name

    strncat(output, ".i", sizeof(output) - strlen(output) - 1);//put .i instead of \0

    // Step 1: Process header inclusion 
    if(process_includes(argv[1], "temp1.c") != 0)
    {
        printf("Header inclusion failed!\n");
        return 1;
    }

    printf("Header inclusion successful!\n");

    // Step 2: Remove comments 
    if(remove_comments("temp1.c", "temp2.c") != 0)
    {
        printf("Comment removal failed!\n");
        remove("temp1.c");
        return 1;
    }

    printf("Comment removal successful!\n");

    // Step 3: Process macros 
    if(process_macros("temp2.c", output) != 0)
    {
        printf("Macro processing failed!\n");
        remove("temp1.c");
        remove("temp2.c");
        return 1;
    }

    printf("Macro processing successful!\n");

    remove("temp1.c");
    remove("temp2.c");

    printf("Preprocessing completed!\n");
    printf("Output file: %s\n", output);

    return 0;
}
