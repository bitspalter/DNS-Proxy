//////////////////////////////////////////////////////////////////////////////////
// [ DArray_Class_Header ]
//////////////////////////////////////////////////////////////////////////////////
//
// [DYNAMIC ARRAY CLASS]
//
// Last modi: 24.03.17 L.ey (µ~)
//
#ifndef _C_DARRAY_H_
 #define _C_DARRAY_H_

  #include "C_Array.hpp"

//////////////////////////////////////////////////////////////////////////////////

 const int C_DARRAY_READY = 0x01;
 const int C_DARRAY_ERROR = 0x00;
 
//////////////////////////////////////////////////////////////////////////////////

 struct S_C_DArray {
    S_C_DArray* pNext;
    S_C_DArray* pPrev;
    C_Array*    pData;
 };

//////////////////////////////////////////////////////////////////////////////////

 class C_DArray {
    public:

       C_DArray();
       C_DArray(const C_DArray&);
      ~C_DArray();  
       C_DArray& operator=(const C_DArray&);

       S_C_DArray*  addItem();
       C_Array*     addItem(int nItem, int cItem);

       S_C_DArray*  getpItem(int Offset) const;
       C_Array*     getpItempData(int Offset) const;
     
       int  delItem(int Offset);
       int  clear();

       int  getnItems() const {return(nItems);}

    private:
     
       int nItems;

       S_C_DArray* pHead;
       S_C_DArray* pTail;
 };

#endif // _C_DARRAY_H_
