#include <stdio.h>
#include <stdlib.h>

int main()
{
    printf(".:: Chomotkar - 0.1 ::.\n");
    FILE *output_pointer;
    char line[100];
    output_pointer = popen("mkdir -p ~/chomotkar", "r");
    printf("------------------------------------------\n");
    printf("Default Folder Created.\n");
    printf("------------------------------------------\n");
    output_pointer = popen("ls -l", "r");

    while (fgets(line, 100, output_pointer))
        printf("%s", line);

    pclose(output_pointer);
    char *outfilename;
    // downloadImage();
    int result = download_url("http://picsum.photos/3840/2400/?random");

}


// gcc -o chomotkar main.c downloadHelper.c -lcurl && ./chomotkar