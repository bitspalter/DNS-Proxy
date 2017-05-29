//////////////////////////////////////////////////////////////////////////////////
// [ Whitelist_Class_Header ]
//////////////////////////////////////////////////////////////////////////////////
//
//
// Last modi: 20.03.17 L.ey (µ~)
//
//
//
#ifndef _C_BLACKLIST_H_
   #define _C_BLACKLIST_H_

   #include <stdio.h>
   #include <fstream>
   #include <iostream>
   #include <sstream>

   using namespace std;

   #include "C_DArray.hpp"

   typedef unsigned char  UCHAR;
   typedef unsigned short WORD;
   typedef unsigned int   DWORD;
   
//////////////////////////////////////////////////////////////////////////////////
// CONSTANT
//////////////////////////////////////////////////////////////////////////////////

   const int C_BLIST_READY  = 0x01;
   const int C_BLIST_ERROR  = 0x00;

   const char BLPATH[] = "blacklist.txt";

//////////////////////////////////////////////////////////////////////////////////
// CLASS
//////////////////////////////////////////////////////////////////////////////////

   class C_Blacklist{

      public:

         C_Blacklist(){bBlackList = false;};  
        ~C_Blacklist(){};  

         //////////////////////////////////

         int read();
         int add(const char* pSite);
         int del(const char* pSite);

	 //////////////////////////////////
	 
	 C_DArray CDA_BlackList;
	 
      protected:

         fstream fBlackList;
         bool    bBlackList;
   };

#endif // _C_BLACKLIST_H_