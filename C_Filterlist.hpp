//////////////////////////////////////////////////////////////////////////////////
// [ Filterlist_Class_Header ]
//////////////////////////////////////////////////////////////////////////////////
//
//
// Last modi: 19.08.17 L.ey (µ~)
//
//
//
#ifndef _C_FILTERLIST_H_
 #define _C_FILTERLIST_H_

 #include <stdio.h>
 #include <fstream>
 #include <iostream>
 #include <sstream>
 #include <string>

 using namespace std;

 #include "C_DArray.hpp"

//////////////////////////////////////////////////////////////////////////////////
// CONSTANT
//////////////////////////////////////////////////////////////////////////////////

 const int C_FLIST_READY = 0x01;
 const int C_FLIST_ERROR = 0x00;
 
 const int C_FLIST_MAX_DNS = 0xFF;

//////////////////////////////////////////////////////////////////////////////////
// CLASS
//////////////////////////////////////////////////////////////////////////////////

 class C_Filterlist{

    public:

       C_Filterlist(){};  
      ~C_Filterlist(){};  

       //////////////////////////////////

       int init(const char* pFile);
      
       int read();
       int add(const char* pSite);
       int del(const char* pSite);

       ////////////////////////////////

       C_DArray<char> CDA_FilterList;

    protected:

       string  sFilterlist;
       fstream fFilterlist;
       bool    bFilterlist;
 };

#endif // _C_FILTERLIST_H_
