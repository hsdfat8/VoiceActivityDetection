///////////////////////////////////////////////
//
// **************************
//
// Project/Software name: X-Phone
// Author: "Van Gegel" <gegelcopy@ukr.net>
//
// THIS IS A FREE SOFTWARE  AND FOR TEST ONLY!!!
// Please do not use it in the case of life and death
// This software is released under GNU LGPL:
//
// * LGPL 3.0 <http://www.gnu.org/licenses/lgpl.html>
//
// You’re free to copy, distribute and make commercial use
// of this software under the following conditions:
//
// * You have to cite the author (and copyright owner): Van Gegel
// * You have to provide a link to the author’s Homepage: <http://torfone.org>
//
///////////////////////////////////////////////

//This is a main procedure of PairPhone testing software
//one-thread implementation as a infinite loop contained procedures for:
//-receiving baseband, demodulating, decrypting, decompressing, playing over earphones (RX)
//-recording voice from mike, compressing, encrypting, modulating, sending baseband into line (TX)
//-scan keyboard, processing. Suspending thread if no job (CTR)
//---------------------------------------------------------------------------
#ifdef _WIN32

#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <basetsd.h>
#include <stdint.h>
#include <windows.h>
#include <time.h>
#include <conio.h>
#include <string.h>
#include "memory.h"
#include "math.h"

#else //linux

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "memory.h"
#include "math.h"

#include <sys/time.h>

#endif

#include "vad/vad2.h"   //low-level alsa/wave audio
vadState2 vad; //Voice Active Detector state
static numfr;
static short spbuf[748];  
vadState2 vad; //Voice Active Detector state
int main(int argc, char *argv[])
{
   int i = 0;
   int j;
  FILE *fin, *fout;

  printf("-----------------------------------------------------\r\n");
  printf("   		EDABK PairPhone TX \r\n\n");
  printf("-----------------------------------------------------\r\n");
	printf("Running VAD with: \nFile in \t:%s \nFile out\t:%s\n\n\n", argv[1], argv[2]);

  fin = fopen(argv[1], "rb");
  fout = fopen(argv[2], "wb");
  short a = 0;
  printf("-----------------------------------------------------\r\n");
  printf("   		EDABK PairPhone VAD \r\n\n");
  printf("-----------------------------------------------------\r\n");

    numfr++;
    
    //  printf("\n\nFRAME %d\n",numf);
    while(1)
    {
      j = fread(spbuf, sizeof(short), 540, fin);
      if (j < 540)
      {
          printf("Done VAD!\n");
          return 0;
      }
      
      //check for we have enough grabbed samples for processing
      // PROCESS IF GOT ENOUGH 540 SAMPLES
      i = vad2(spbuf + 10, &vad); //check frame is speech (by VAD)
            i += vad2(spbuf + 100, &vad);
            i += vad2(spbuf + 190, &vad);
            i += vad2(spbuf + 280, &vad);
            i += vad2(spbuf + 370, &vad)*2;
            i += vad2(spbuf + 410, &vad)*2;
            i += vad2(spbuf + 460, &vad)*4;

      if (i > 6) //frame is voices: compress it
          {
              a = 1;
          }
          // VAD FRAME
          else //unvoiced frame: sync packet will be send
          {
              a = 0; //or set silence flag for control blocks
          }    
      // printf("Frame: %d   a = %d   i=%d\n",numfr++,a,i);    
      fwrite(&a, sizeof(short), 1, fout);  
    }
}
//---------------------------------------------------------------------------
