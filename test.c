#include <stdio.h>
#include <openssl/md5.h>
#include "ecrypt-sync.h"
#include <time.h>
#include <stdint.h>


clock_t startm, stopm;
#define START if ( (startm = clock()) == -1) {printf("Error calling clock");}
#define STOP if ( (stopm = clock()) == -1) {printf("Error calling clock");}
#define PRINTTIME (stopm-startm);



void random_bytes(u8* buffer,size_t size){
  size_t i;
  for(i=0; i < size; i++)
      buffer[i] = rand() % 255;
}


char* line;
char *vmpeak;
size_t len;

int getUsage(){
  FILE* f;
  len = 128;

  f=fopen("/proc/self/status", "r");
  if (!f) return 0;

  while(getline(&line,&len,f)!=NULL){
    if (strncmp(line, "VmHWM:", 6)==0)
    {
      vmpeak = strdup(&line[7]);
      printf("---- Ram usage | %s", vmpeak);
      return 1;
      break;
    }
  }
  fclose(f);
  return 1;
}

void test_run(int messageSize, int runs){

  u8 k[32];
  u8 v[8];


  ECRYPT_ctx x;

  random_bytes(k,32);
  random_bytes(v,8);

  ECRYPT_keysetup(&x,k,256,64);
  ECRYPT_ivsetup(&x,v);

  getUsage();


  u8 m[messageSize];
  random_bytes(m,messageSize);
  u8 c[messageSize];

  int loop;
  startm = clock();
  for(loop=0;loop<runs;loop++){


    ECRYPT_encrypt_bytes(&x,m,c,messageSize);

    //ECRYPT_ivsetup(&x,v);
    //ECRYPT_decrypt_bytes(&x,c,d,messageSize);


  }
  getUsage();
  stopm = clock();


  u8 d[messageSize];
  int clocks = PRINTTIME

  printf( "---- Encryption | %f miliseconds | %d bytes.\n", clocks/((double)runs*CLOCKS_PER_SEC/1000), messageSize);

  startm = clock();
  for(loop=0;loop<runs;loop++){


    ECRYPT_decrypt_bytes(&x,m,c,messageSize);

    //ECRYPT_ivsetup(&x,v);
    //ECRYPT_decrypt_bytes(&x,c,d,messageSize);


  }
  stopm = clock();
  clocks = PRINTTIME

  printf( "---- Decryption | %f miliseconds | %d bytes.\n", clocks/((double)runs*CLOCKS_PER_SEC/1000), messageSize);

}

void checkSame(u8 *d,u8 *m,int messageSize){
    int i;
    int err=0;
    for (i = 0;i < messageSize;++i)
      if (d[i] != m[i]) err++;
    if (err>0){
      printf("ERROR\n");
    }
}

int main()
{
  srand(time(NULL));

  printf("Usage before runing.\n");
  getUsage();

  int messageSize = 100;
  test_run(messageSize,10000);
  messageSize = 1000;
  test_run(messageSize,1000);
  messageSize = 10000;
  test_run(messageSize,100);




  return 0;
}
