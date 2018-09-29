#ifndef HASHBLOCK_H
#define HASHBLOCK_H

#include "sph_blake.h"
#include "sph_bmw.h"
#include "sph_groestl.h"
#include "sph_jh.h"
#include "sph_keccak.h"
#include "sph_skein.h"



void Hash9(void *state, const void *init)
{
    sph_blake256_context     ctx_blake;
    sph_bmw256_context       ctx_bmw;
    sph_groestl256_context   ctx_groestl;
    sph_jh256_context        ctx_jh;
    sph_keccak256_context    ctx_keccak;
    sph_skein256_context     ctx_skein;
    static unsigned char pblank[1];

	
    uint32_t mask = 8;
    uint32_t zero = 0;

	//these uint256 in the c++ source of the client are backed by an array of uint32
    uint32_t hashA[8], hashB[8];	
	
/*	
	int ii=0;
	printf("Start: ");
	for (ii=0; ii < 80; ii++)
	{
		printf ("%.2x",((uint8_t*)init)[ii]);
	};
	printf ("\n");
*/	
	
    sph_blake256_init(&ctx_blake);
    sph_blake256 (&ctx_blake, init, 80);
    sph_blake256_close (&ctx_blake, hashA);	 //0
	
/*	
	printf("bla256: ");
	for (ii=0; ii < 64; ii++)
	{
		printf ("%.2x",((uint8_t*)hashA)[ii]);
	};
	printf ("\n");
*/	
	
    sph_bmw256_init(&ctx_bmw);
    sph_bmw256 (&ctx_bmw, hashA, 64);    //0
    sph_bmw256_close(&ctx_bmw, hashB);   //1

/*
	printf("bmw256: ");
	for (ii=0; ii < 64; ii++)
	{
		printf ("%.2x",((uint8_t*)hashB)[ii]);
	};
	printf ("\n");	
*/
	
    if ((hashB[0] & mask) != zero)   //1
    {
        sph_groestl256_init(&ctx_groestl);
        sph_groestl256 (&ctx_groestl, hashB, 32); //1
        sph_groestl256_close(&ctx_groestl, hashA); //2
    }
    else
    {
        sph_skein256_init(&ctx_skein);
        sph_skein256 (&ctx_skein, hashB, 32); //1
        sph_skein256_close(&ctx_skein, hashA); //2
    }

/*	
	printf("1stcon: ");
	for (ii=0; ii < 64; ii++)
	{
		printf ("%.2x",((uint8_t*)hashA)[ii]);
	};
	printf ("\n");
*/	
	
    sph_groestl256_init(&ctx_groestl);
    sph_groestl256 (&ctx_groestl, hashA, 32); //2
    sph_groestl256_close(&ctx_groestl, hashB); //3

    sph_jh256_init(&ctx_jh);
    sph_jh256 (&ctx_jh, hashB, 64); //3
    sph_jh256_close(&ctx_jh, hashA); //4

    if ((hashA[0] & mask) != zero) //4
    {
        sph_blake256_init(&ctx_blake);
        sph_blake256 (&ctx_blake, hashA, 32); //
        sph_blake256_close(&ctx_blake, hashB); //5
    }
    else
    {
        sph_bmw256_init(&ctx_bmw);
        sph_bmw256 (&ctx_bmw, hashA, 64); //4
        sph_bmw256_close(&ctx_bmw, hashB);   //5
    }
    
    sph_keccak256_init(&ctx_keccak);
    sph_keccak256 (&ctx_keccak,hashB, 32); //5
    sph_keccak256_close(&ctx_keccak, hashA); //6

    sph_skein256_init(&ctx_skein);
    sph_skein256 (&ctx_skein, hashA, 32); //6
    sph_skein256_close(&ctx_skein, hashB); //7

    if ((hashB[0] & mask) != zero) //7
    {
        sph_keccak256_init(&ctx_keccak);
        sph_keccak256 (&ctx_keccak, hashB, 32); //
        sph_keccak256_close(&ctx_keccak, hashA); //8
    }
    else
    {
        sph_jh256_init(&ctx_jh);
        sph_jh256 (&ctx_jh, hashB, 64); //7
        sph_jh256_close(&ctx_jh, hashA); //8
    }

/*	
	printf("result: ");
	for (ii=0; ii < 64; ii++)
	{
		printf ("%.2x",((uint8_t*)hashA)[ii]);
	};
	printf ("\n");	
*/
    //return hash[8].trim256(); //8
	memcpy(state, hashA, 32);
	
/*	
	printf("result: ");
	for (ii=0; ii < 32; ii++)
	{
		printf ("%.2x",((uint8_t*)state)[ii]);
	};
	printf ("\n");	
*/	
}



void Hash6(void *state, const void *init)

{
    sph_blake256_context     ctx_blake;
    sph_bmw256_context       ctx_bmw;
    sph_groestl256_context   ctx_groestl;
    sph_jh256_context        ctx_jh;
    sph_keccak256_context    ctx_keccak;
    sph_skein256_context     ctx_skein;
    //static unsigned char pblank[1];

    char hashA[8], hashB[8];

    sph_blake256_init(&ctx_blake);
    sph_blake256 (&ctx_blake, init, 80);
//    sph_blake256_close(&ctx_blake, (void*)(&hashA));
    sph_blake256_close (&ctx_blake, hashA);

    sph_bmw256_init(&ctx_bmw);
    sph_bmw256 (&ctx_bmw, (const void*)(hashA), 64);
    sph_bmw256_close(&ctx_bmw, (void*)(hashB));

    sph_groestl256_init(&ctx_groestl);
    sph_groestl256 (&ctx_groestl, (const void*)(hashB), 32);
    sph_groestl256_close(&ctx_groestl, (void*)(hashA));

    sph_jh256_init(&ctx_jh);
    sph_jh256 (&ctx_jh, (const void*)(hashA), 64);
    sph_jh256_close(&ctx_jh, (void*)(hashB));

    sph_keccak256_init(&ctx_keccak);
    sph_keccak256 (&ctx_keccak, (const void*)(hashB), 32);
    sph_keccak256_close(&ctx_keccak, (void*)(hashA));

    sph_skein256_init(&ctx_skein);
    sph_skein256 (&ctx_skein, (const void*)(hashA), 32);
    sph_skein256_close(&ctx_skein, (void*)(hashB));

    memcpy(state, hashB, 32);
};


#endif // HASHBLOCK_H
