#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef uint8_t BYTE;

int main(int argc, char *argv[])
{
    int filecount = 0;
    bool writeflag = false;
    const int BLOCK_SIZE = 512;
    BYTE buffer[512];
    FILE *file = fopen(argv[1], "r");
    FILE *image;
    if (file == NULL)
    {
        printf("Enter file name\n");
        return 1;
    }
    while (fread(&buffer, 1, BLOCK_SIZE, file) == BLOCK_SIZE)
    {
        char name[8];
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && buffer[3] <= 0xef && buffer[3] >= 0xe0)
        {
            if (filecount != 0)
            {
                fclose(image);
            }
            sprintf(name, "%03i.jpg", filecount);
            image = fopen(name, "w");
            filecount++;
            writeflag = true;
        }
        if (writeflag)
        {
            fwrite(&buffer, BLOCK_SIZE, 1, image);
        }
    }
    fclose(image);
    fclose(file);
}