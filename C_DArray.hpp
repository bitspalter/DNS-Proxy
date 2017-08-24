//////////////////////////////////////////////////////////////////////////////////
// [ DArray_Class_Header ]
//////////////////////////////////////////////////////////////////////////////////
//
// [DYNAMIC ARRAY CLASS]
//
// Last modi: 18.08.17 L.ey (µ~)
//
#ifndef _C_DARRAY_H_
 #define _C_DARRAY_H_

  #include "C_Array.hpp"

//////////////////////////////////////////////////////////////////////////////////

 const int C_DARRAY_READY = 0x01;
 const int C_DARRAY_ERROR = 0x00;
 
//////////////////////////////////////////////////////////////////////////////////
 
 template <typename T>
 struct S_C_DArray {
    S_C_DArray<T>* pNext;
    S_C_DArray<T>* pPrev;
    C_Array<T>*    pData;
 };

//////////////////////////////////////////////////////////////////////////////////
 
 template <typename T>
 class C_DArray {
     
    public:
        
       /////////////////////////////////////////////////////////////////
       // [ Constructor ]  
       /////////////////////////////////////////////////////////////////
       C_DArray(){
          this->status = C_DARRAY_ERROR;
          try {
             this->pHead  = new S_C_DArray<T>;
             this->pTail  = new S_C_DArray<T>;
          }catch(exception& e){
             return;
          }
          this->status = C_DARRAY_READY;
          this->nItems = 0;
       }
       /////////////////////////////////////////////////////////////////
       // [ Copy Constructor ]  
       /////////////////////////////////////////////////////////////////
       C_DArray(const C_DArray<T>& rCDArray){
          S_C_DArray<T>* pNewItem  = 0;
          S_C_DArray<T>* prCDArray = 0;

          this->status = C_DARRAY_ERROR;
          
          try {
             this->pHead  = new S_C_DArray<T>;
             this->pTail  = new S_C_DArray<T>;
          }catch(exception& e){
             return;   
          }
          
          this->nItems = 0;

          for(int rnItems = 0; rnItems < rCDArray.getnItems(); rnItems++){
             if(!this->nItems){ // First Item
                 
                try {
                   pNewItem        = new S_C_DArray<T>;
                   pNewItem->pData = new C_Array<T>;
                }catch(exception& e){
                   return;   
                }
                
                ///////////////////////////////////////////

                if(!(prCDArray = rCDArray.getpItem(rnItems))) break;

                *pNewItem->pData = *prCDArray->pData;

                ///////////////////////////////////////////

                pNewItem->pNext = this->pTail;
                pNewItem->pPrev = this->pHead;

                this->pHead->pNext = pNewItem;
                this->pTail->pPrev = pNewItem;
                
             }else{ // Next Item 
	
                try {
                   pNewItem        = new S_C_DArray<T>;
                   pNewItem->pData = new C_Array<T>;
                }catch(exception& e){
                   return;   
                }

                ///////////////////////////////////////////

                if(!(prCDArray = rCDArray.getpItem(rnItems))) break;

                *pNewItem->pData = *prCDArray->pData;

                ///////////////////////////////////////////

                pNewItem->pNext = this->pTail;
                pNewItem->pPrev = this->pTail->pPrev;

                this->pTail->pPrev->pNext = pNewItem;
                this->pTail->pPrev        = pNewItem;
             }
	
             this->nItems++;
          }
          
          this->status = C_DARRAY_READY;
       }
       /////////////////////////////////////////////////////////////////
       // [ Destructor ]  
       /////////////////////////////////////////////////////////////////
      ~C_DArray(){
          clear();
          delete this->pHead;
          delete this->pTail;
       }
       /////////////////////////////////////////////////////////////////
       // [ operator = ]  
       /////////////////////////////////////////////////////////////////
       C_DArray& operator=(const C_DArray& rCDArray){
          if(this == &rCDArray) return(*this);
          
          S_C_DArray<T>* pNewItem  = 0;
          S_C_DArray<T>* prCDArray = 0;

          if(this->nItems) clear();

          for(int rnItems = 0; rnItems < rCDArray.getnItems(); rnItems++){
             if(!this->nItems){ // First Item
	  
                try {
                   pNewItem        = new S_C_DArray<T>;
                   pNewItem->pData = new C_Array<T>;
                }catch(exception& e){
                   return(nullptr);   
                }

                ///////////////////////////////////////////

                if(!(prCDArray = rCDArray.getpItem(rnItems))) break;

                *pNewItem->pData = *prCDArray->pData;

                ///////////////////////////////////////////

                pNewItem->pNext = this->pTail;
                pNewItem->pPrev = this->pHead;

                this->pHead->pNext = pNewItem;
                this->pTail->pPrev = pNewItem;
                
             }else{ // Next Item 

                try {
                   pNewItem        = new S_C_DArray<T>;
                   pNewItem->pData = new C_Array<T>;
                }catch(exception& e){
                   return(nullptr);   
                }

                ///////////////////////////////////////////

                if(!(prCDArray = rCDArray.getpItem(rnItems))) break;

                *pNewItem->pData = *prCDArray->pData;

                ///////////////////////////////////////////

                pNewItem->pNext = this->pTail;
                pNewItem->pPrev = this->pTail->pPrev;

                this->pTail->pPrev->pNext = pNewItem;
                this->pTail->pPrev        = pNewItem;
             }
                
             this->nItems++;
          }

          return(*this);
       }
       /////////////////////////////////////////////////////////////////
       // [ addItem ]  
       /////////////////////////////////////////////////////////////////
       S_C_DArray<T>* addItem(){
  
          S_C_DArray<T>* pNewItem = 0;

          if(!this->nItems){ // First Item
   
             try {
                pNewItem        = new S_C_DArray<T>;
                pNewItem->pData = new C_Array<T>;
             }catch(exception& e){
                return(nullptr);   
             }

             pNewItem->pNext = this->pTail;
             pNewItem->pPrev = this->pHead;

             this->pHead->pNext = pNewItem;
             this->pTail->pPrev = pNewItem;
             
          }else{ // Next Item 
   
             try {
                pNewItem        = new S_C_DArray<T>;
                pNewItem->pData = new C_Array<T>;
             }catch(exception& e){
                return(nullptr);   
             }

             pNewItem->pNext = this->pTail;
             pNewItem->pPrev = this->pTail->pPrev;

             this->pTail->pPrev->pNext = pNewItem;
             this->pTail->pPrev        = pNewItem;
          }
	
          this->nItems++;

          return(pNewItem);
       }
       /////////////////////////////////////////////////////////////////
       // [ addItem ]  
       /////////////////////////////////////////////////////////////////
       C_Array<T>* addItem(int nItem){
          S_C_DArray<T>* pNewItem = 0;

          if(!this->nItems){ // First Item

             try {
                pNewItem        = new S_C_DArray<T>;
                pNewItem->pData = new C_Array<T>;
             }catch(exception& e){
                return(nullptr);   
             }

             ////////////////////////////////////////////////////////////

             if(pNewItem->pData->create(nItem) != C_ARRAY_READY) 
                return(nullptr);

             ////////////////////////////////////////////////////////////

             pNewItem->pNext = this->pTail;
             pNewItem->pPrev = this->pHead;

             this->pHead->pNext = pNewItem;
             this->pTail->pPrev = pNewItem;
             
          }else{ // Next Item 

             try {
                pNewItem        = new S_C_DArray<T>;
                pNewItem->pData = new C_Array<T>;
             }catch(exception& e){
                return(nullptr);   
             }

             ////////////////////////////////////////////////////////////

             if(pNewItem->pData->create(nItem) != C_ARRAY_READY)
                return(nullptr);

             ////////////////////////////////////////////////////////////

             pNewItem->pNext = this->pTail;
             pNewItem->pPrev = this->pTail->pPrev;

             this->pTail->pPrev->pNext = pNewItem;
             this->pTail->pPrev        = pNewItem;
          }
	
          this->nItems++;

          return(pNewItem->pData);
       }
       /////////////////////////////////////////////////////////////////
       // [ getpItem ]  
       /////////////////////////////////////////////////////////////////
       S_C_DArray<T>* getpItem(int Offset) const {
          S_C_DArray<T>* pItem = 0;

          if((!this->nItems) || (Offset > this->nItems - 1)) return(nullptr);

          if(Offset <= (this->nItems >> 1)){ // Go over Head
              
             if(!(pItem = this->pHead->pNext)) 
                return(nullptr);
              
             for(int nItem = 0; nItem < Offset; nItem++) 
                pItem = pItem->pNext;
             
          }else{ // Go over Tail
              
             if(!(pItem = this->pTail->pPrev)) 
                return(nullptr);
             
             for(int nItem = this->nItems - 1; nItem > Offset; nItem--) 
                pItem = pItem->pPrev;
          }

          return(pItem);
       }
       /////////////////////////////////////////////////////////////////
       // [ getpItempData ]  
       /////////////////////////////////////////////////////////////////
       C_Array<T>* getpItempData(int Offset) const {
          S_C_DArray<T>* pItem = 0;

          if((!this->nItems) || (Offset > this->nItems - 1)) return(nullptr);

          if(Offset <= (this->nItems >> 1)){ // Go over Head
              
             if(!(pItem = this->pHead->pNext)) 
                return(nullptr);
             
             for(int n = 0; n < Offset; n++) 
                pItem = pItem->pNext;
      
          }else{ // Go over Tail
              
             if(!(pItem = this->pTail->pPrev)) 
                return(nullptr);
             
             for(int n = this->nItems - 1; n > Offset; n--) 
	            pItem = pItem->pPrev;
          }

          return(pItem->pData);
       }
       /////////////////////////////////////////////////////////////////
       // [ delItem ]  
       /////////////////////////////////////////////////////////////////
       int delItem(int Offset){
          S_C_DArray<T>* pItem = 0;
          
          if((!this->nItems) || (Offset > this->nItems - 1)) return(C_DARRAY_ERROR);
          
          if(Offset <= (this->nItems >> 1)){ // Go over Head
              
             if(!(pItem = this->pHead->pNext)) 
                return(C_DARRAY_ERROR);
             
             for(int n = 0; n < Offset; n++)
                pItem = pItem->pNext;

          }else{ // Go over Tail
              
             if(!(pItem = this->pTail->pPrev)) 
                return(C_DARRAY_ERROR);
             
             for(int n = this->nItems - 1; n > Offset; n--)
                pItem = pItem->pPrev;
          }

          if(pItem->pData) delete pItem->pData;

          pItem->pPrev->pNext = pItem->pNext;
          pItem->pNext->pPrev = pItem->pPrev;

          delete pItem;

          this->nItems--;

          return(C_DARRAY_READY);
       }
       /////////////////////////////////////////////////////////////////
       // [ clear ]  
       /////////////////////////////////////////////////////////////////
       int clear(){
          S_C_DArray<T>* pItem = 0;
          
          if(this->nItems){
              
             if(!(pItem = this->pHead->pNext)) 
                return(C_DARRAY_ERROR);
             
             for(int n = 0; n < this->nItems; n++){
                if(pItem->pData) delete pItem->pData;
                
                if(!(pItem = pItem->pNext)) 
                   return(C_DARRAY_ERROR);
                
                if(pItem->pPrev) delete pItem->pPrev;
             }
             this->nItems = 0;
          }
          return(C_DARRAY_READY);
       }
       /////////////////////////////////////////////////////////////////
       // [ getnItems ]  
       /////////////////////////////////////////////////////////////////
       int getnItems() const {return(this->nItems);}

    private:
     
       int nItems;
       int status;
       
       S_C_DArray<T>* pHead;
       S_C_DArray<T>* pTail;
 };

#endif // _C_DARRAY_H_
