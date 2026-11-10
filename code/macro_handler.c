#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "macro_handler.h"

#define MAX_MACROS 100  // defines maximum number of macros we can use 
#define MAX_ARGS 10    // max number of argumentffds 
#define MAX_LINE 512   // size of line max 
#define MAX_OUTPUT 4096 //

struct Macro
{
    char name[50];
    char value[256];
    char args[MAX_ARGS][50];
    int arg_count;
    int is_function;
};

/* Append string safely */
void append_str(char *dest, const char *src, int size)
{
    int len = strlen(dest);

    if(len < size - 1)
        strncat(dest, src, size - len - 1); //conacatinate 
}

/* Find macro */
int find_macro(struct Macro macros[], int count, char *word)
{
    int i;

    for(i = 0; i < count; i++)
    {
        if(strcmp(macros[i].name, word) == 0)
            return i;
    }

    return -1;
}

/* Extract function arguments */
int get_arguments(const char *line, int pos,char args[MAX_ARGS][100],int *argc, int *end_pos)// line -> original input line ,pos->position of opening paranthesis ,args-> store actual arguments ,argc->return number of arguments ,end_pos-> return closing parenthesis position
{
    int depth = 1;// inside outer opening parentheseis  tracks nested paranthese 
    int i = pos + 1; // posi in the input line skip opening paranthese  
    int j = 0; // pos in curruent argument 
    *argc = 0; // initialize acaller argument counter 

    while(line[i] != '\0' && depth > 0)
    {
        char ch = line[i];

        if(ch == '(')
        {
            depth++;
        }
        else if(ch == ')')
        {
            depth--;

            if(depth == 0)
                break;
        }

        if(ch == ',' && depth == 1)
        {
            args[*argc][j] = '\0';
            (*argc)++;

            if(*argc >= MAX_ARGS)
                return 1;

            j = 0;
        }
        else
        {
            if(j < 99)
                args[*argc][j++] = ch;
        }

        i++;
    }

    if(depth != 0)
        return 1;

    if(j > 0 || *argc > 0)
    {
        args[*argc][j] = '\0';
        (*argc)++;
    }

    *end_pos = i;

    return 0;
}

/* Replace formal parameters with actual arguments */
void expand_function(struct Macro *macro,
                     char actual[MAX_ARGS][100],
                     char *result)
{
    int i = 0;
    char word[100];
    int j, k;

    while(macro->value[i] != '\0')
    {
        if(isalpha((unsigned char)macro->value[i]) ||
           macro->value[i] == '_')
        {
            j = 0;

            while((isalnum((unsigned char)macro->value[i]) ||
                   macro->value[i] == '_') && j < 99)
            {
                word[j++] = macro->value[i++];
            }

            word[j] = '\0';

            int found = 0;

            for(k = 0; k < macro->arg_count; k++)
            {
                if(strcmp(word, macro->args[k]) == 0)
                {
                    append_str(result, actual[k], MAX_OUTPUT);
                    found = 1;
                    break;
                }
            }

            if(!found)
                append_str(result, word, MAX_OUTPUT);
        }
        else
        {
            int len = strlen(result);

            if(len < MAX_OUTPUT - 1)
            {
                result[len] = macro->value[i];
                result[len + 1] = '\0';
            }

            i++;
        }
    }
}

/* Main macro processing function */
int process_macros(const char *input, const char *output)
{
    FILE *fp1, *fp2;

    struct Macro macros[MAX_MACROS];

    int count = 0;
    int i, j, index;

    char line[MAX_LINE];
    char word[100];
    char result[MAX_OUTPUT];

    fp1 = fopen(input, "r");

    if(fp1 == NULL)
        return 1;

    fp2 = fopen(output, "w");

    if(fp2 == NULL)
    {
        fclose(fp1);
        return 1;
    }

    while(fgets(line, sizeof(line), fp1) != NULL)
    {
        /* Detect macro definition */

        if(strncmp(line, "#define", 7) == 0)
        {
            if(count >= MAX_MACROS)
            {
                fclose(fp1);
                fclose(fp2);
                return 1;
            }

            char *p = line + 7;

            while(isspace((unsigned char)*p))
                p++;

            /* Extract macro name */

            i = 0;

            while((isalnum((unsigned char)*p) || *p == '_') && i < 49) // extract micro name 
            {
                macros[count].name[i++] = *p++;  // store macro naeme 
            }

            macros[count].name[i] = '\0';   

            macros[count].arg_count = 0; // for object like 
            macros[count].is_function = 0; // for function type macr0 

            // Function-like macro 

            if(*p == '(')
            {
                macros[count].is_function = 1;
                p++;

                while(*p != ')' && *p != '\0')
                {
                    while(isspace((unsigned char)*p) || *p == ',') // extract replacment values 
                        p++;

                    if(*p == ')' || *p == '\0')
                        break;

                    i = 0;

                    while((isalnum((unsigned char)*p) || *p == '_') && i < 49)
                    {
                        macros[count].args[macros[count].arg_count][i++] = *p++;
                    }

                    macros[count].args[macros[count].arg_count][i] = '\0';

                    if(macros[count].arg_count < MAX_ARGS)
                        macros[count].arg_count++;

                    while(isspace((unsigned char)*p))
                        p++;
                }

                if(*p == ')')
                    p++;
            }

            /* Extract replacement value */

            while(isspace((unsigned char)*p))
                p++;
            strncpy(macros[count].value, p,
                    sizeof(macros[count].value) - 1);
            macros[count].value[
                sizeof(macros[count].value) - 1] = '\0';
            macros[count].value[
                strcspn(macros[count].value, "\n")] = '\0';
            count++;
            continue;
        }
        /* Process normal lines */
        result[0] = '\0';
        i = 0;
        while(line[i] != '\0')
        {
            if(isalpha((unsigned char)line[i]) ||
               line[i] == '_')
            {
                /* Extract identifier */
                j = 0;
                while((isalnum((unsigned char)line[i]) ||
                       line[i] == '_') && j < 99)
                {
                    word[j++] = line[i++];
                }
                word[j] = '\0';

                index = find_macro(macros, count, word);
                if(index == -1)
                {
                    append_str(result, word, MAX_OUTPUT);
                }
                else if(macros[index].is_function == 0)
                {
                    /* Object-like macro */
                    append_str(result, macros[index].value,
                               MAX_OUTPUT);
                }
                else
                {
                    /* Function-like macro */
                    int pos = i;
                    while(isspace((unsigned char)line[pos]))
                        pos++;
                    if(line[pos] == '(')
                    {
                        char actual[MAX_ARGS][100] = {{0}};
                        int argc, end_pos;
                        if(get_arguments(line, pos, actual,
                                         &argc, &end_pos) == 0 &&
                           argc == macros[index].arg_count)
                        {
                            char expanded[MAX_OUTPUT] = "";
                            expand_function(&macros[index],
                                            actual, expanded);
                            append_str(result, expanded, MAX_OUTPUT);

                            i = end_pos + 1;
                        }
                        else
                        {
                            append_str(result, word, MAX_OUTPUT);
                        }
                    }
                    else
                    {
                        append_str(result, word, MAX_OUTPUT);
                    }
                }
            }
            else
            {
                int len = strlen(result);

                if(len < MAX_OUTPUT - 1)
                {
                    result[len] = line[i];
                    result[len + 1] = '\0';
                }
                i++;
            }
        }
        fputs(result, fp2);
    }
    fclose(fp1);
    fclose(fp2);
    return 0;
}
