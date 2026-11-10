#include <stdio.h>
#include "comment_removal.h"

int remove_comments(const char *input, const char *output)
{
    int ch, next;
    FILE *fp1, *fp2;

    fp1 = fopen(input, "r");

    if(fp1 == NULL)
        return 1;

    fp2 = fopen(output, "w");

    if(fp2 == NULL)
    {
        fclose(fp1);
        return 1;
    }

    while((ch = fgetc(fp1)) != EOF)
    {
        if(ch == '/')
        {
            next = fgetc(fp1);

            /* Single-line comment */
            if(next == '/')
            {
                while((ch = fgetc(fp1)) != EOF && ch != '\n')
                    ;

                if(ch == '\n')
                    fputc('\n', fp2);
            }

            /* Multi-line comment */
            else if(next == '*')
            {
                int prev = 0;

                while((ch = fgetc(fp1)) != EOF)
                {
                    if(ch == '\n')
                        fputc('\n', fp2);

                    if(prev == '*' && ch == '/')
                        break;

                    prev = ch;
                }
            }

            /* Normal division operator */
            else
            {
                fputc('/', fp2);

                if(next != EOF)
                    ungetc(next, fp1);
            }
        }
        else
        {
            fputc(ch, fp2);
        }
    }

    fclose(fp1);
    fclose(fp2);

    return 0;
}
