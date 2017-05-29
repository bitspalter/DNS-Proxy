//////////////////////////////////////////////////////////////////////////////////
// [ Whitelist_Class_Header ]
//////////////////////////////////////////////////////////////////////////////////
//
//
// Last modi: 20.03.17 L.ey (µ~)
//
//
//
#ifndef _C_WHITELIST_H_
  #define _C_WHITELIST_H_

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

   const int C_WLIST_READY  = 0x01;
   const int C_WLIST_ERROR  = 0x00;

   const char WLPATH[] = "whitelist.txt";

//////////////////////////////////////////////////////////////////////////////////
// CLASS
//////////////////////////////////////////////////////////////////////////////////

   class C_Whitelist{

      public:

         C_Whitelist(){bWhiteList = false;};  
        ~C_Whitelist(){};  

         //////////////////////////////////

         int read();
         int add(const char* pSite);
         int del(const char* pSite);

	 ////////////////////////////////
	 
	 C_DArray CDA_WhiteList;
	 
      protected:

         fstream fWhiteList;
         bool    bWhiteList;
 };

#endif // _C_WHITELIST_H_