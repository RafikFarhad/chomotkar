#include <stdio.h>
#include <stdlib.h>

int main()
{
    printf(".:: Chomotkar - 1.0 ::.\n");
    // FILE *output_pointer;
    // char line[100];
    // output_pointer = popen("ls -l", "r");

    // while (fgets(line, 100, output_pointer))
    //     printf("%s", line);

    // pclose(output_pointer);
    char *outfilename;
    // downloadImage();
    download_url("http://picsum.photos/3840/2400/?random");

}


// gcc -o chomotkar main.c downloadHelper.c -lcurl && ./chomotkar