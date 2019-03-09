#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <curl/curl.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdlib.h>
#include <time.h>

typedef unsigned long uint64_t;

int LOADER = 0;
typedef struct
{
    char dnld_remote_fname[4096];
    char dnld_url[4096];
    FILE *dnld_stream;
    FILE *dbg_stream;
    uint64_t dnld_file_sz;
} dnld_params_t;

static int get_oname_from_cd(char const *const cd, char *oname)
{
    char const *const cdtag = "Content-disposition:";
    char const *const key = "filename=";
    int ret = 0;
    char *val = NULL;

    /* Example Content-Disposition: filename=name1367; charset=funny; option=strange */

    /* If filename is present */
    val = strcasestr(cd, key);
    if (!val)
    {
        printf("No key-value for \"%s\" in \"%s\"", key, cdtag);
        goto bail;
    }

    /* Move to value */
    val += strlen(key);

    /* Copy value as oname */
    while (*val != '\0' && *val != ';')
    {
        //printf (".... %c\n", *val);
        *oname++ = *val++;
    }
    *oname = '\0';

bail:
    return ret;
}

static int get_oname_from_url(char const *url, char *oname)
{
    int ret = 0;
    char const *u = url;

    /* Remove "http(s)://" */
    u = strstr(u, "://");
    if (u)
    {
        u += strlen("://");
    }

    u = strrchr(u, '/');

    /* Remove last '/' */
    u++;

    /* Copy value as oname */
    while (*u != '\0')
    {
        //printf (".... %c\n", *u);
        *oname++ = *u++;
    }
    *oname = '\0';

    return ret;
}

size_t dnld_header_parse(void *hdr, size_t size, size_t nmemb, void *userdata)
{
    const size_t cb = size * nmemb;
    const char *hdr_str = hdr;
    dnld_params_t *dnld_params = (dnld_params_t *)userdata;
    char const *const cdtag = "Content-disposition:";
    return cb;
}

FILE *get_dnld_stream(char const *const fname)
{
    FILE *fp = fopen(fname, "wb");
    if (!fp)
    {
        printf("Could not create file %s\n", fname);
    }

    return fp;
}

size_t write_cb(void *buffer, size_t sz, size_t nmemb, void *userdata)
{
    int ret = 0;
    dnld_params_t *dnld_params = (dnld_params_t *)userdata;

    if (LOADER%5==0)
    {
        printf(".");
    }
    LOADER++;
    if (!dnld_params->dnld_stream)
    {
        dnld_params->dnld_stream = get_dnld_stream(dnld_params->dnld_remote_fname);
    }

    ret = fwrite(buffer, sz, nmemb, dnld_params->dnld_stream);
    if (ret == (sz * nmemb))
    {
        dnld_params->dnld_file_sz += ret;
    }
    return ret;
}

int download_url(char const *const url, char *fileName)
{
    printf("Downloading Wallpaper ...\n");
    CURL *curl;
    int ret = -1;
    LOADER = 0;
    CURLcode cerr = CURLE_OK;
    dnld_params_t dnld_params;

    memset(&dnld_params, 0, sizeof(dnld_params));
    strncpy(dnld_params.dnld_url, url, strlen(url));

    char *fileNameLocal = dnld_params.dnld_remote_fname;
    while (*fileName != '\0')
    {
        *fileNameLocal = *fileName;
        fileName++;
        fileNameLocal++;
    }
    fileNameLocal = '\0';

    curl = curl_easy_init();
    if (!curl)
    {
        goto bail;
    }

    cerr = curl_easy_setopt(curl, CURLOPT_URL, url);
    if (cerr)
    {
        printf("%s: failed with err %d\n", "URL", cerr);
        goto bail;
    }

    cerr = curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    if (cerr)
    {
        printf("%s: failed with err %d\n", "FOLLOW LOCATION", cerr);
        goto bail;
    }

    cerr = curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, dnld_header_parse);
    if (cerr)
    {
        printf("%s: failed with err %d\n", "HEADER", cerr);
        goto bail;
    }

    cerr = curl_easy_setopt(curl, CURLOPT_HEADERDATA, &dnld_params);
    if (cerr)
    {
        printf("%s: failed with err %d\n", "HEADER DATA", cerr);
        goto bail;
    }

    cerr = curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_cb);
    if (cerr)
    {
        printf("%s: failed with err %d\n", "WR CB", cerr);
        goto bail;
    }

    cerr = curl_easy_setopt(curl, CURLOPT_WRITEDATA, &dnld_params);
    if (cerr)
    {
        printf("%s: failed with err %d\n", "WR Data", cerr);
        goto bail;
    }

    cerr = curl_easy_perform(curl);
    if (cerr != CURLE_OK)
    {
        fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(cerr));
    }
    fclose(dnld_params.dnld_stream);

    /* always cleanup */
    curl_easy_cleanup(curl);
    ret = dnld_params.dnld_file_sz;
    printf("\n");
    return ret;
bail:
    return -1;
}