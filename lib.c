#include <stdio.h>
#include <curl/curl.h>
#include <string.h>
#include <stdlib.h>
int shorten(int argc, char *argv[])
{
  if (argc != 4) {
   printf("Invalid number of arguments. Please specify arguments in the order key, url.\n");
   exit(1);
  } 
  CURL *curl;
  CURLcode res;
 
  curl_global_init(CURL_GLOBAL_DEFAULT);
 
  curl = curl_easy_init();
  if(curl) {
    char sendurl[255];
    
    char key[255];
    char shortenurl[255];
    strcpy(key, argv[2]);
    strcpy(shortenurl, argv[3]);
    sprintf(sendurl, "https://api.awau.moe/shorten/polr?action=shorten&key=%s&url=%s", key, shortenurl);
    curl_easy_setopt(curl, CURLOPT_URL, sendurl); 
    /* Perform the request, res will get the return code */ 
    res = curl_easy_perform(curl);
    /* Check for errors */ 
    if(res != CURLE_OK)
      fprintf(stderr, "curl_easy_perform() failed: %s\n",
              curl_easy_strerror(res));
 
    /* always cleanup */ 
    curl_easy_cleanup(curl);
  }
 
  curl_global_cleanup();
  printf("\n"); 
  return 0;
}

int upload(int argc, char *argv[])
{
  CURL *curl;
  CURLcode res;
  char sendurl[255];
  char key[255];
  char filepath[255];
  strcpy(key, argv[1]);
  strcpy(filepath, argv[2]);
  printf("%s", filepath);
  sprintf(sendurl, "https://api.awau.moe/upload/pomf?key=%s", key);
  struct curl_httppost *formpost=NULL;
  struct curl_httppost *lastptr=NULL;
  struct curl_slist *headerlist=NULL;
  static const char buf[] = "Expect:";

  curl_global_init(CURL_GLOBAL_ALL);

  /* Fill in the file upload field */
  curl_formadd(&formpost,
               &lastptr,
               CURLFORM_COPYNAME, "files[]",
               CURLFORM_FILE, filepath,
               CURLFORM_END);
  curl = curl_easy_init();
  /* initialize custom header list (stating that Expect: 100-continue is not
     wanted */
  headerlist = curl_slist_append(headerlist, buf);
  if(curl) {
    /* what URL that receives this POST */
    curl_easy_setopt(curl, CURLOPT_URL, sendurl);
    if((argc == 2) && (!strcmp(argv[1], "noexpectheader")))
      /* only disable 100-continue header if explicitly requested */
      curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headerlist);
    curl_easy_setopt(curl, CURLOPT_HTTPPOST, formpost);

    /* Perform the request, res will get the return code */
    res = curl_easy_perform(curl);
    /* Check for errors */
    if(res != CURLE_OK)
      fprintf(stderr, "curl_easy_perform() failed: %s\n",
              curl_easy_strerror(res));

    /* always cleanup */
    curl_easy_cleanup(curl);

    /* then cleanup the formpost chain */
    curl_formfree(formpost);
    /* free slist */
    curl_slist_free_all(headerlist);
  }
  return 0;
}


int main(int argc, char *argv[])
{
  if (argc == 1) {
    printf("Usage owo [flags] key url/filepath\n -l     Shorten provided URL\n");
    exit(0);
  }
  if (argc == 3) {
    upload(argc, argv);
  }
  if (!strcmp(argv[1], "-l")) 
  {
    shorten(argc, argv);
  }
}




