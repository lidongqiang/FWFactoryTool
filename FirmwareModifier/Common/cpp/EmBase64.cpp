#include "stdafx.h"
#include "../inc/EmBase64.h"

#include <string.h>




 static const char *g_pCodes =  
     "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/=";  
   
 static const unsigned char g_pMap[256] =  
 {  
     255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,  
     255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,  
     255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,  
     255, 255, 255, 255, 255, 255, 255,  62, 255, 255, 255,  63,  
      52,  53,  54,  55,  56,  57,  58,  59,  60,  61, 255, 255,  
     255, 254, 255, 255, 255,   0,   1,   2,   3,   4,   5,   6,  
       7,   8,   9,  10,  11,  12,  13,  14,  15,  16,  17,  18,  
      19,  20,  21,  22,  23,  24,  25, 255, 255, 255, 255, 255,  
     255,  26,  27,  28,  29,  30,  31,  32,  33,  34,  35,  36,  
      37,  38,  39,  40,  41,  42,  43,  44,  45,  46,  47,  48,  
      49,  50,  51, 255, 255, 255, 255, 255, 255, 255, 255, 255,  
     255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,  
     255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,  
     255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,  
     255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,  
     255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,  
     255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,  
     255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,  
     255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,  
     255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,  
     255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,  
     255, 255, 255, 255  
 };  
  
 
 char* em::EmBase64::Encode(char *szDest, const char *szSrc)  
 {  
     int i = 0;
	 int iSrcLen = strlen(szSrc);
	 int iLoop = 3 * (iSrcLen / 3);  
     unsigned char *pLoop = (unsigned char *)szDest;
	 unsigned char *pOut = (unsigned char *)szDest;
	 unsigned char *pIn = (unsigned char *)szSrc;
	 
	 unsigned char chA = 0;  
	 unsigned char chB = 0;  
     unsigned char chC = 0;  

     if(szSrc == NULL || szDest == NULL)  
	 {
         return NULL;  
	 } 

     for(i = 0; i < iLoop; i += 3)  
     {  
         *pLoop++ = g_pCodes[pIn[0] >> 2];  
         *pLoop++ = g_pCodes[((pIn[0] & 3) << 4) + (pIn[1] >> 4)];  
         *pLoop++ = g_pCodes[((pIn[1] & 0xf) << 2) + (pIn[2] >> 6)];  
         *pLoop++ = g_pCodes[pIn[2] & 0x3f];  
         pIn += 3;  
     }  
   
     if (i < iSrcLen)  
     {  
         chA = pIn[0];  
         chB = ((i + 1) < iSrcLen) ? pIn[1] : 0;  
         chC = 0;  
   
         *pLoop++ = g_pCodes[chA >> 2];  
         *pLoop++ = g_pCodes[((chA & 3) << 4) + (chB >> 4)];  
         *pLoop++ = ((i + 1) < iSrcLen) ? g_pCodes[((chB & 0xf) << 2) + (chC >> 6)] : '=';  
         *pLoop++ = '=';
     }  
   
     *pLoop = 0; // Append NULL byte    
	 
     return szDest;  
 }  

 char* em::EmBase64::Decode(char *szDest, const char *szSrc)   
 {  
	 int iSrcLen = strlen((const char*)szSrc);
     int t = 0;
	 int x = 0;
	 int y = 0;
	 int z = 0; 
	 int g = 3; 
     unsigned char c = 0;  
	 unsigned char *pIn = (unsigned char*)szSrc;
	 unsigned char *pOut = (unsigned char*)szDest;
 
	 if(szSrc == NULL || szDest == NULL)  
	 {
         return NULL;  
	 }

     for(x = 0; x < iSrcLen; x++)  
     {  
         c = g_pMap[pIn[x]];  
         if(c == 255)
		 {
			 continue;  
		 }

         if(c == 254) 
		 { 
			 c = 0; 
			 g--; 
		 }
   
         t = (t << 6) | c;  
   
         if(++y == 4)  
         {
             pOut[z++] = (unsigned char)((t>>16)&255);  
             if(g > 1) 
			 {
				 pOut[z++] = (unsigned char)((t>>8)&255);  
			 }
             if(g > 2)
			 {
				 pOut[z++] = (unsigned char)(t&255);  
			 }
             y = t = 0;  
         }  
     }  
     return szDest;  
 }  