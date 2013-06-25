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

void test_run(int messageSize, int runs){

  u8 m[messageSize];
  u8 c[messageSize];
  u8 d[messageSize];
  u8 k[32];
  u8 v[8];


  ECRYPT_ctx x;

  random_bytes(m,messageSize);
  random_bytes(k,32);
  random_bytes(v,8);

  ECRYPT_keysetup(&x,k,256,64);
  ECRYPT_ivsetup(&x,v);



  int loop;
  startm = clock();
  for(loop=0;loop<runs;loop++){


    ECRYPT_encrypt_bytes(&x,m,c,messageSize);

    //ECRYPT_ivsetup(&x,v);
    //ECRYPT_decrypt_bytes(&x,c,d,messageSize);


  }
  stopm = clock();
  int clocks = PRINTTIME

  printf( "---- Encryption | %f seconds | %d bytes.\n", clocks/((double)runs*CLOCKS_PER_SEC), messageSize);

  startm = clock();
  for(loop=0;loop<runs;loop++){


    ECRYPT_decrypt_bytes(&x,m,c,messageSize);

    //ECRYPT_ivsetup(&x,v);
    //ECRYPT_decrypt_bytes(&x,c,d,messageSize);


  }
  stopm = clock();
  clocks = PRINTTIME

  printf( "---- Decryption | %f seconds | %d bytes.\n", clocks/((double)runs*CLOCKS_PER_SEC), messageSize);

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

void withOutput(int messageSize,int log)
{

    u8 m[messageSize];
    u8 c[messageSize];
    u8 d[messageSize];
    u8 k[32];
    u8 v[8];

    if(log){
    ///Fill message with random bytes
    printf("Generating random message, key and IV\n");
    }
    random_bytes(m,messageSize);
    random_bytes(k,32);
    random_bytes(v,8);
    if(log)  printf("Initializing Salsa20\n");

    ECRYPT_ctx x;
    ECRYPT_keysetup(&x,k,256,64);
    ECRYPT_ivsetup(&x,v);

    if(log){
      printf("Initialized.....\n");
      printf("Encrypting\n");
    }

    START;
    ECRYPT_encrypt_bytes(&x,m,c,messageSize);
    STOP;
    PRINTTIME;

    if(log){
      printf("Encrypted\n");
      printf("Preparing for decryption\n");
    }
    ECRYPT_ivsetup(&x,v);

    if(log){
      printf("Decrypting\n");
    }
    START;
    ECRYPT_decrypt_bytes(&x,c,d,messageSize);
    STOP;
    PRINTTIME;

    if(log){
      printf("Decrypted\n");
    }
}

int main()
{
  srand(time(NULL));

  int messageSize = 1000;
  test_run(messageSize,10000);
  messageSize = 10000;
  test_run(messageSize,1000);
  messageSize = 100000;
  test_run(messageSize,100);




  return 0;
}
