#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <cs50.h>

int main(int argc, char *argv[])
{
    if(argc != 2)
    {
        printf("Usage: ./recover image\n");
        return 1;
    }
	FILE *inptr = fopen(argv[1], "rb");
	if (inptr == NULL)
	{
		fprintf(stderr, "Could not open ");
    	return 2;
	}
	bool check = false;
	int block_count = 0;
	do 
	{
		unsigned char buffer[512];
		fread(buffer,1,512,inptr);
		block_count ++;
		if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff)
		{
			check = true;
			printf("Success! \n");
			printf("Blocks: %d\n", block_count);
		}
	} while (check == false);
	int imageCount = 0;
	char filename[8];
	// межа циклу
	for(int i = 0; i < 50; i++)
	{
		fseek(inptr,-512,SEEK_CUR); //перевод каретки на початок першого зображення
		
		int check2 = 0;
		sprintf(filename, "%03i.jpg", imageCount);
		FILE *outptr = fopen(filename, "wb");
		do 
		{
			unsigned char buffer[512];
			
			
			if (fread(buffer,1,512,inptr) == 512)
			{
				if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff)
				{
					check2++;
				}
				if (check2 == 1)
				{
					fwrite(buffer,1,512,outptr);
				}
			}
			else
			{
				fclose(outptr);	
				fclose(inptr);
				return 0;
			}
			
		} 
		while (check2 != 2);
		imageCount++;
		fclose(outptr);
	}
	// межа циклу
	fclose(inptr);
	return 0;
}