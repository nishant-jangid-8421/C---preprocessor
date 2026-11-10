#include <stdio.h>
#include <string.h>
#include "file_inclusion.h"

int process_includes(const char *input, const char *output)
{
    FILE *fp1, *fp2, *fp3;
    char line[512];    // store each line 
    char filename[256]; // store file name 
    char *start, *end;// to point to headerfile names 
    int ch;
    fp1 = fopen(input, "r"); // opem input file      // file name received from main.c 
    if(fp1 == NULL)
    {
        printf("Cannot open input file\n");
        return 1;
    }
    fp2 = fopen(output, "w"); // temp1.c 
    if(fp2 == NULL)
    {
        fclose(fp1);
        printf("Cannot create output file\n");
        return 1;
    }
    while(fgets(line, sizeof(line), fp1) != NULL)
    {
        if(strncmp(line, "#include", 8) == 0) // compare first #include that is 8 bytes 
        {
            // Check for system header 
            start = strchr(line, '<');   // 
            if(start != NULL)
            {
                fputs(line, fp2); //copy full line to temp2 file 
                continue;
            }
            // Check for local header 
            start = strchr(line, '"');
            if(start == NULL)
            {
                fputs(line, fp2);
                continue;
            }
            end = strchr(start + 1, '"');// search from start +1 
            if(end == NULL)
            {
                fputs(line, fp2);
                continue;
            }
            // Extract filename  
            long unsigned int  len = end - start - 1;
            if(len == 0 || len >= sizeof(filename))
            {
                fclose(fp1);
                fclose(fp2);
                return 1;
            }
            strncpy(filename, start + 1, len);
            filename[len] = '\0';
            // Open header file 
            fp3 = fopen(filename, "r");
            if(fp3 == NULL)
            {
                printf("Cannot open header: %s\n", filename);

                fclose(fp1);
                fclose(fp2);
                return 1;
            }
            // Copy header contents 
            while((ch = fgetc(fp3)) != EOF)
            {
                fputc(ch, fp2);
            }
            fclose(fp3);
        }
        else
        {
            fputs(line, fp2);
        }
    }
    fclose(fp1);
    fclose(fp2);

    return 0;
}
