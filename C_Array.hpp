//////////////////////////////////////////////////////////////////////////////////
// [ Array_Class_Header ]
//////////////////////////////////////////////////////////////////////////////////
//
//
// [STATIC ARRAY CLASS]
//
// Last modi: 24.03.17 L.ey (µ~)
//
#ifndef _C_ARRAY_H_
 #define _C_ARRAY_H_

 #include <iostream>
 #include <string.h>
 
 using namespace std;

//////////////////////////////////////////////////////////////////////////////////

 const int C_ARRAY_READY = 0x01;
 const int C_ARRAY_ERROR = 0x00;
 
 const int C_ARRAY_SUB_PTR   = 0x0100;
 const int C_ARRAY_SUB_EXIST = 0x0200;
 const int C_ARRAY_SUB_OVER  = 0x0300;

//////////////////////////////////////////////////////////////////////////////////

 class C_Array {
   
    public:

       C_Array();
       C_Array(int nItem, int cItem);
       C_Array(const C_Array&);
      ~C_Array(); 
       C_Array& operator=(const C_Array&);
 
       int create(int nItem, int cItem);
       int destroy();
 
       // (!) offset[0] = Item nr.1
       //
       int getItem(int offset, void* pDataDst); 
       int setItem(int offset, void* pDataSrc);

       char* getpItem(int offset);

       //////////////////////////////////
       // Inline
       //
       int   getStatus()  const {return(status);}

       char* getpBuffer() const {return(pBuffer);}
       int   getcBuffer() const {return(cBuffer);}

       int   getnItem()   const {return(nItem);}
       int   getcItem()   const {return(cItem);}
   
    private:

       int   status;
       
       int   cBuffer;    // FullSize
       int   nItem;      // Item count
       int   cItem;      // Item size 
       
       char* pBuffer;    // DeltaPointer
 };

#endif // _C_ARRAY_H_
