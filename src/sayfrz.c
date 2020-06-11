#include <stdio.h>
#include <stdlib.h>
#include <string.h>


void PrintProgressBar(char *outBuffer, int bufferLength, float progressValue)
{
    int nFill = (float)bufferLength * progressValue;
    for(int i = 0; i < nFill; i++)
        outBuffer[i] = '#';
    for(int i = nFill; i < bufferLength; i++)
        outBuffer[i] = ' ';

    for(int j = 0; j < 80; ++j)
        printf("\b");
    printf("[%s] %.2f%%", outBuffer, progressValue*100);
}

int FileXorSingleKey(
    FILE *infile,
    int key, 
    FILE *outfile)
{
    if(!infile)
    {
        printf("In-file not found.\n");
        return 1;
    }
    if(!outfile)
    {
        printf("Out-file not found.\n");
        return 1;
    }

    char outbuffer[61];
    int filesize;

    fseek(infile, 0, SEEK_END);
    filesize = ftell(infile);
    fseek(infile, 0, SEEK_SET);

    int c;
    for(int i = 0; (c = fgetc(infile)) != EOF; ++i)
    {
        if(i%10000 == 0)
        {
            float progress = (float)(i+1)/(float)filesize;
            PrintProgressBar(outbuffer, 60, progress);
        }

        if(fputc(c^key, outfile) == EOF)
        {
            return 1;
        }
    }
    PrintProgressBar(outbuffer, 60, 1.00f);
    printf("\n");
    
    return 0;
}


int main(int argc, char **argv)
{
    if(argc < 3)
    {
        printf("USAGE:\n");
        printf(  "\tsayfrz.exe filename key\n");
        printf("\n\tfilename: path of the input file\n");
        printf(  "\tkey     : decimal value for key\n");
        return 0;
    }
    
    char *sInfile = argv[1];
    int key = atoi(argv[2]);
    char *sOutfile = (char *)malloc((strlen(sInfile)+4) * sizeof(char)); // Allah da C'nin belasını versin..
    strcpy(sOutfile, sInfile);
    strcat(sOutfile, ".out");

    printf("File name : %s\n", sInfile);
    printf("Key       : %d\n", key);
    printf("Out-file  : %s\n\n", sOutfile);

    FILE *in = fopen(sInfile, "rb");
    FILE *out = fopen(sOutfile, "wb");

    int error = FileXorSingleKey(in, key, out);
    
    if(error)
    {
        printf("Failure\n");
    }
    else
    {
        printf("Success\n");
    }
    
    if(in)  fclose(in);
    if(out) fclose(out);
    free(sOutfile);
    return 0;
}