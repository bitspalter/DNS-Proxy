//////////////////////////////////////////////////////////////////////////////////
// [ DArray_Class_Source ]
//////////////////////////////////////////////////////////////////////////////////

#include "C_DArray.hpp"

//////////////////////////////////////////////////////////////////////////////////
// [ Constructor ]  
//////////////////////////////////////////////////////////////////////////////////
C_DArray::C_DArray(){
   pHead  = new S_C_DArray;
   pTail  = new S_C_DArray;
   nItems = 0;
}
//////////////////////////////////////////////////////////////////////////////////
// [ Destructor ]
//////////////////////////////////////////////////////////////////////////////////
C_DArray::~C_DArray(){
   clear();
   delete pHead;
   delete pTail;
}
//////////////////////////////////////////////////////////////////////////////////
// [ Copy Constructor ]
//////////////////////////////////////////////////////////////////////////////////
C_DArray::C_DArray(const C_DArray& rCDArray){
   S_C_DArray* pNewItem  = 0;
   S_C_DArray* prCDArray = 0;

   pHead  = new S_C_DArray;
   pTail  = new S_C_DArray;
   nItems = 0;

   for(int rnItems = 0; rnItems < rCDArray.getnItems(); rnItems++){
      if(!nItems){ // First Item
         pNewItem = new S_C_DArray;
	 
         if(!pNewItem) break;
	 
         pNewItem->pData = new C_Array;

         if(!pNewItem->pData) break;

         ///////////////////////////////////////////

         prCDArray = rCDArray.getpItem(rnItems);

         if(!prCDArray) break;

         *pNewItem->pData = *prCDArray->pData;

         ///////////////////////////////////////////

         pNewItem->pNext = pTail;
         pNewItem->pPrev = pHead;

         pHead->pNext = pNewItem;
         pTail->pPrev = pNewItem;
      }else{ // Next Item 
	
         pNewItem = new S_C_DArray;

         if(!pNewItem) break;

         pNewItem->pData = new C_Array;

         if(!pNewItem->pData) break;

         ///////////////////////////////////////////

         prCDArray = rCDArray.getpItem(rnItems);

         if(!prCDArray) break;

         *pNewItem->pData = *prCDArray->pData;

         ///////////////////////////////////////////

         pNewItem->pNext = pTail;
         pNewItem->pPrev = pTail->pPrev;

         pTail->pPrev->pNext = pNewItem;
         pTail->pPrev = pNewItem;
      }
	
      nItems++;
   }
}
//////////////////////////////////////////////////////////////////////////////////
// [ operator = ]
//////////////////////////////////////////////////////////////////////////////////
C_DArray & C_DArray::operator=(const C_DArray& rCDArray){
   if(this == &rCDArray) return(*this);

   S_C_DArray* pNewItem  = 0;
   S_C_DArray* prCDArray = 0;

   if(nItems) clear();

   for(int rnItems = 0; rnItems < rCDArray.getnItems(); rnItems++){
      if(!nItems){ // First Item
	  
         pNewItem = new S_C_DArray;

         if(!pNewItem) break;

         pNewItem->pData = new C_Array;

         if(!pNewItem->pData) break;

         ///////////////////////////////////////////

         prCDArray = rCDArray.getpItem(rnItems);

         if(!prCDArray) break;

         *pNewItem->pData = *prCDArray->pData;

         ///////////////////////////////////////////

         pNewItem->pNext = pTail;
         pNewItem->pPrev = pHead;

         pHead->pNext = pNewItem;
         pTail->pPrev = pNewItem;
      }else{ // Next Item 

         pNewItem = new S_C_DArray;

         if(!pNewItem) break;

         pNewItem->pData = new C_Array;

         if(!pNewItem->pData) break;

         ///////////////////////////////////////////

         prCDArray = rCDArray.getpItem(rnItems);

         if(!prCDArray) break;

         *pNewItem->pData = *prCDArray->pData;

         ///////////////////////////////////////////

         pNewItem->pNext = pTail;
         pNewItem->pPrev = pTail->pPrev;

         pTail->pPrev->pNext = pNewItem;
         pTail->pPrev = pNewItem;
      }
	
      nItems++;
   }

   return(*this);
}
//////////////////////////////////////////////////////////////////////////////////
// [ addItem ]
//////////////////////////////////////////////////////////////////////////////////
S_C_DArray* C_DArray::addItem(){
  
   S_C_DArray* pNewItem = 0;

   if(!nItems){ // First Item
   
      pNewItem = new S_C_DArray;

      if(!pNewItem) return(0x00);

      pNewItem->pData = new C_Array;

      if(!pNewItem->pData) return(0x00);

      pNewItem->pNext = pTail;
      pNewItem->pPrev = pHead;

      pHead->pNext = pNewItem;
      pTail->pPrev = pNewItem;
   }else{ // Next Item 
   
      pNewItem = new S_C_DArray;

      if(!pNewItem) return(0x00);

      pNewItem->pData = new C_Array;

      if(!pNewItem->pData) return(0x00);

      pNewItem->pNext = pTail;
      pNewItem->pPrev = pTail->pPrev;

      pTail->pPrev->pNext = pNewItem;
      pTail->pPrev = pNewItem;
   }
	
   nItems++;

   return(pNewItem);
}
//////////////////////////////////////////////////////////////////////////////////
// [ addItem ]
//////////////////////////////////////////////////////////////////////////////////
C_Array* C_DArray::addItem(int nItem, int cItem){
   S_C_DArray* pNewItem = 0;

   if(!nItems){ // First Item
   
      pNewItem = new S_C_DArray;

      if(!pNewItem) return(0x00);

      pNewItem->pData = new C_Array;

      if(!pNewItem->pData) return(0x00);

      ////////////////////////////////////////////////////////////

      if(pNewItem->pData->create(nItem, cItem) != 1) return(0);

      ////////////////////////////////////////////////////////////

      pNewItem->pNext = pTail;
      pNewItem->pPrev = pHead;

      pHead->pNext = pNewItem;
      pTail->pPrev = pNewItem;
   }else{ // Next Item 
   
      pNewItem = new S_C_DArray;

      if(!pNewItem) return(0x00);

      pNewItem->pData = new C_Array;

      if(!pNewItem->pData) return(0x00);

      ////////////////////////////////////////////////////////////

      if(pNewItem->pData->create(nItem, cItem) != 1) return(0);

      ////////////////////////////////////////////////////////////

      pNewItem->pNext = pTail;
      pNewItem->pPrev = pTail->pPrev;

      pTail->pPrev->pNext = pNewItem;
      pTail->pPrev = pNewItem;
   }
	
   nItems++;

   return(pNewItem->pData);
}
//////////////////////////////////////////////////////////////////////////////////
// [ getpItem ]
//////////////////////////////////////////////////////////////////////////////////
S_C_DArray* C_DArray::getpItem(int Offset) const {
   S_C_DArray* pItem = 0;

   if((!nItems) || (Offset > nItems - 1)) return(0x00);

   if(Offset <= (nItems / 2)){ // Go over Head
   
      pItem = pHead->pNext;

      for(int nItem = 0; nItem < Offset; nItem++) 
         pItem = pItem->pNext;
      
   }else{ // Go over Tail
   
      pItem = pTail->pPrev;

      for(int nItem = C_DArray::nItems - 1; nItem > Offset; nItem--) 
         pItem = pItem->pPrev;
   }

   return(pItem);
}
//////////////////////////////////////////////////////////////////////////////////
// [ getpItempData ] -return ( C_Array* )
//////////////////////////////////////////////////////////////////////////////////
C_Array* C_DArray::getpItempData(int Offset) const {
   S_C_DArray* pItem = 0;

   if((!nItems) || (Offset > nItems - 1)) return(0x00);

   if(Offset <= (nItems / 2)){ // Go over Head
   
      pItem = pHead->pNext;

      for(int nItem = 0; nItem < Offset; nItem++) 
         pItem = pItem->pNext;
      
   }else{ // Go over Tail
   
      pItem = pTail->pPrev;

      for(int nItem = C_DArray::nItems - 1; nItem > Offset; nItem--) 
	 pItem = pItem->pPrev;
   }

   return(pItem->pData);
}
//////////////////////////////////////////////////////////////////////////////////
// [ delItem ]
//////////////////////////////////////////////////////////////////////////////////
int C_DArray::delItem(int Offset){
   S_C_DArray* pItem = 0;

   if((!nItems) || (Offset > nItems - 1)) return(C_DARRAY_ERROR);

   if(Offset <= (nItems / 2)){ // Go over Head
   
      pItem = pHead->pNext;

      for(int nItem = 0; nItem < Offset; nItem++)
         pItem = pItem->pNext;

   }else{ // Go over Tail
   
      pItem = pTail->pPrev;

      for(int nItem = C_DArray::nItems - 1; nItem > Offset; nItem--)
         pItem = pItem->pPrev;
   }

   if(pItem->pData) delete pItem->pData;

   pItem->pPrev->pNext = pItem->pNext;
   pItem->pNext->pPrev = pItem->pPrev;

   delete pItem;

   nItems--;

   return(C_DARRAY_READY);
}
//////////////////////////////////////////////////////////////////////////////////
// [ clear ]
//////////////////////////////////////////////////////////////////////////////////
int C_DArray::clear(){
  
   S_C_DArray* pItem = 0;

   if(nItems){
     
      pItem = pHead->pNext;

      for(int nItems = 0; nItems < C_DArray::nItems; nItems++){
         if(pItem->pData) delete pItem->pData;

         pItem = pItem->pNext;

         if(!pItem) return(C_DARRAY_ERROR);

         if(pItem->pPrev) delete pItem->pPrev;
      }

      nItems = 0;
   }

   return(C_DARRAY_READY);
}
