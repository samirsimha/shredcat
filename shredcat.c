#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

void cat(FILE *f, long size)
{
    fseek(f, 0, SEEK_SET);
    char buf[size];
    while (fgets(buf, sizeof(buf), f) != NULL)
    {
        printf("%s", buf);
    }
    printf("\n");
    fclose(f);
}

void encrypt(FILE *f, long size)
{
    char *buf = malloc(size);
    if (buf == NULL)
    {
        perror("allocation error");
        fclose(f);
        return;
    }
    srand(time(NULL));
    for (int i = 0; i < size; i++)
    {
        buf[i] = rand() % 256;
    }
    fseek(f, 0, SEEK_SET);
    size_t written = fwrite(buf, sizeof(char), size, f);
    if (written != size)
    {
        perror("encryption failed");
    }

    free(buf);
    fclose(f);
}

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        printf("usage:\n shredcat <file>\n shredcat -s <file>\n");
        return 1;
    }

    if (argc == 2)
    {
        FILE *f = fopen(argv[1], "r");
        if (f == NULL)
        {
            perror("file could not be opened");
            return (1);
        }
        fseek(f, 0, SEEK_END);
        long size = ftell(f);
        if (size > 0)
        {
            cat(f, size);
        }
        else
        {
            printf("file is empty, nothing to display\n");
            fclose(f);
        }
    }

    if (argc == 3)
    {
        if (strcmp(argv[1], "-e") == 0)
        {
            FILE *f = fopen(argv[2], "rb+");
            if (f == NULL)
            {
                perror("file could not be opened");
                return (1);
            }
            fseek(f, 0, SEEK_END);
            long size = ftell(f);
            if (size > 0)
            {
                encrypt(f, size);
                printf("file encrypted");
            }
            else
            {
                printf("file is empty, nothing to encrypt\n");
                fclose(f);
            }
        }

        if (strcmp(argv[1], "-s") == 0)
        {
            FILE *f = fopen(argv[2], "rb+");
            if (f == NULL)
            {
                perror("file could not be opened");
                return (1);
            }
            fseek(f, 0, SEEK_END);
            long size = ftell(f);
            if (size > 0)
            {
                encrypt(f, size);
                if (remove(argv[2]) == 0)
                {
                    printf("file shredded\n");
                }
                else
                {
                    perror("file could not be deleted");
                }
            }
            else
            {
                printf("file is empty, nothing to shred\n");
                fclose(f);
            }
        }
    }

    return 0;
}
