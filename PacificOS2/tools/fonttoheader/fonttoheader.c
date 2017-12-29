#include <stdio.h>
#include <stdint.h>
#include <stdio.h>

int main(int argc, char *const argv[])
{
    FILE *fp = fopen("font.psfu", "r");

    if (fp == NULL)
    {
    	perror("Error opening file!");
    	return 0;
    }

    int32_t magic1 = fgetc(fp);
    int32_t magic2 = fgetc(fp);
    int32_t magic3 = fgetc(fp);
    int32_t magic4 = fgetc(fp);

    uint32_t version = fgetc(fp);
    uint32_t headersize = fgetc(fp);
    uint32_t flags = fgetc(fp);
    uint32_t length = fgetc(fp);
    uint32_t charsize = fgetc(fp);
    uint32_t width = fgetc(fp);
    uint32_t height = fgetc(fp);

	printf("Width: %i \n", charsize);
	printf("Height: %i \n", height);

}