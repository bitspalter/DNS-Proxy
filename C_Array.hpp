//////////////////////////////////////////////////////////////////////////////////
// [ Array_Class_Header ]
//////////////////////////////////////////////////////////////////////////////////
//
//
// [STATIC ARRAY CLASS]
//
// Last modi: 18.08.17 L.ey (µ~)
//
#ifndef _C_ARRAY_H_
 #define _C_ARRAY_H_

 #include <iostream>
 #include <string.h>
 
 using namespace std;

//////////////////////////////////////////////////////////////////////////////////

 const int C_ARRAY_READY = 0x01;
 const int C_ARRAY_ERROR = 0x00;

//////////////////////////////////////////////////////////////////////////////////
 
 template <typename T>
 class C_Array {
   
    public:
       /////////////////////////////////////////////////////////////////
       // [ Constructor ]  
       /////////////////////////////////////////////////////////////////
       C_Array(){
          status  = C_ARRAY_ERROR;
          cItem   = 0;
          nItem   = 0;
          cBuffer = 0;
          pBuffer = 0;
       }
       /////////////////////////////////////////////////////////////////
       // [ Constructor ]  
       /////////////////////////////////////////////////////////////////
       C_Array(int nItem){
          status = C_ARRAY_ERROR;
          create(nItem);
       }
       /////////////////////////////////////////////////////////////////
       // [ Copy Constructor ]  
       /////////////////////////////////////////////////////////////////
       C_Array(const C_Array& rCArray){
          status  = C_ARRAY_ERROR;
          cItem   = rCArray.getcItem();
          nItem   = rCArray.getnItem();
          cBuffer = cItem * nItem;

          if(cBuffer){
             try {
                pBuffer = new T[nItem];
             }catch(exception& e){
                cItem   = 0;
                nItem   = 0;
                cBuffer = 0;
                return;
             }
             status  = C_ARRAY_READY;
             T* pTempBuffer = rCArray.getpBuffer();
             if(pBuffer && pTempBuffer) 
                memcpy(pBuffer, pTempBuffer, cBuffer);
          }
       }
       /////////////////////////////////////////////////////////////////
       // [ Destructor ]  
       /////////////////////////////////////////////////////////////////
       ~C_Array(){
          destroy();
       }; 
       /////////////////////////////////////////////////////////////////
       // [ operator = ]  
       /////////////////////////////////////////////////////////////////
       C_Array& operator=(const C_Array& rCArray){

          if(this == &rCArray) return(*this);

          if(status != C_ARRAY_ERROR) destroy(); 

          status  = C_ARRAY_ERROR;
          cItem   = rCArray.getcItem();
          nItem   = rCArray.getnItem();
          cBuffer = cItem * nItem;

          if(cBuffer){
             try {
                pBuffer = new T[nItem];
             }catch(exception& e){
                cItem   = 0;
                nItem   = 0;
                cBuffer = 0;
                return(nullptr); 
             }
             
             status  = C_ARRAY_READY;
             
             T* pTempBuffer = rCArray.getpBuffer();
             
             if(pBuffer && pTempBuffer) 
                memcpy(pBuffer, pTempBuffer, cBuffer);
          }

          return(*this);
       }
       /////////////////////////////////////////////////////////////////
       // [ create ]  
       /////////////////////////////////////////////////////////////////
       int create(int _nItem){
  
          if(status == C_ARRAY_READY) return(C_ARRAY_ERROR);

          ////////////////////////////////
          try {
             pBuffer = new T[_nItem];
          }catch(exception& e){
             return(C_ARRAY_ERROR); 
          }
          ////////////////////////////////

          status  = C_ARRAY_READY;
          nItem   = _nItem;
          cItem   = sizeof(T);
          cBuffer = _nItem * cItem;

          memset(pBuffer, 0, cBuffer);

          return(C_ARRAY_READY);
       }
       /////////////////////////////////////////////////////////////////
       // [ destroy ]  
       /////////////////////////////////////////////////////////////////
       int destroy(){
  
          if(status == C_ARRAY_ERROR) return(C_ARRAY_ERROR); 
  
          //////////////////////////
          delete [] pBuffer;
          //////////////////////////

          status  = C_ARRAY_ERROR;

          nItem   = 0;
          cItem   = 0;
          cBuffer = 0;
          pBuffer = 0;
          
          return(C_ARRAY_READY);
       }
       /////////////////////////////////////////////////////////////////
       // [ getItem ]  
       /////////////////////////////////////////////////////////////////
       int getItem(int offset, T* pDataDst){

          if(status == C_ARRAY_ERROR) return(C_ARRAY_ERROR); 

          if(!pDataDst) return(C_ARRAY_ERROR);

          if(offset > nItem) return(C_ARRAY_ERROR);
	 
          T* pDataSrc = pBuffer;
          pDataSrc += offset;

          ////////////////////////////////////
          memcpy(pDataDst, pDataSrc, cItem);
          ////////////////////////////////////

          return(C_ARRAY_READY);
       }
       /////////////////////////////////////////////////////////////////
       // [ setItem ]  
       /////////////////////////////////////////////////////////////////
       int setItem(int offset, T* pDataSrc){

          if(!pDataSrc) return(C_ARRAY_ERROR); 

          if(offset > nItem) return(C_ARRAY_ERROR); 
	 
          T* pDataDst = pBuffer;
          pDataDst += offset;

          ////////////////////////////////////
          memcpy(pDataDst, pDataSrc, cItem);
          ////////////////////////////////////

          return(C_ARRAY_READY);
       }
       /////////////////////////////////////////////////////////////////
       // [ getpItem ]  
       /////////////////////////////////////////////////////////////////
       T* getpItem(int offset){
  
          if(status == C_ARRAY_ERROR) return((T*)(C_ARRAY_ERROR)); 

          return(pBuffer + offset);
       }

       //////////////////////////////////
       // Inline
       //
       int getStatus()  const {return(status);}

       T*  getpBuffer() const {return(pBuffer);}
       int getcBuffer() const {return(cBuffer);}

       int getnItem()   const {return(nItem);}
       int getcItem()   const {return(cItem);}
   
    private:

       int status;
       
       int cBuffer; // FullSize
       int nItem;   // Item count
       int cItem;   // Item size 
       
       T*  pBuffer; // DeltaPointer
 };

#endif // _C_ARRAY_H_
