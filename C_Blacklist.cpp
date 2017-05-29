//////////////////////////////////////////////////////////////////////////////////
// [ Whitelist_Class_Source ]
//////////////////////////////////////////////////////////////////////////////////

#include "C_Blacklist.hpp"

//////////////////////////////////////////////////////////////////////////////////
// [ read ] Sichern !!!
//////////////////////////////////////////////////////////////////////////////////
int C_Blacklist::read(){

   CDA_BlackList.clear();

   fBlackList.open(BLPATH, ios_base::in | ios_base::out | ios_base::app);
   if(fBlackList.is_open()) bBlackList = true;
   
   char zeile[201];
   memset(zeile, 0, sizeof(zeile));

   while(fBlackList.getline(zeile, 200)){
      if(strlen(zeile)){
         C_Array* pData = CDA_BlackList.addItem(1, strlen(zeile) + 1);
         memcpy(pData->getpBuffer(), zeile, strlen(zeile) + 1);
      }
   }
   fBlackList.close();
   bBlackList = false;

   return(C_BLIST_READY);
}
//////////////////////////////////////////////////////////////////////////////////
// [ add ]
//////////////////////////////////////////////////////////////////////////////////
int C_Blacklist::add(const char* pSite){
  
   if(!pSite) return(C_BLIST_ERROR);

   long cfff = strlen(pSite);
   
   if(!cfff) return(C_BLIST_ERROR);

   long pPoint[10];
   long cPoint = 0;

   memset(pPoint, 0, sizeof(pPoint));

   for(long n = cfff; n > 0; n--){
      switch(pSite[n]){
         case 46:{
            pPoint[cPoint++] = n; break;
         }
      }
   }

   char test[1024];
   memset(test, 0, 1024);

   if(cPoint > 1)
      memcpy(test, &pSite[pPoint[1]] + 1, cfff - pPoint[1] - 1);
   else
   if(cPoint > 0)
      memcpy(test, pSite, cfff);

   ////////////////////////////////////////////
   // Test ob schon vorhanden
   bool bLegal = true;

   for(DWORD nItem = 0; nItem < CDA_BlackList.getnItems(); nItem++){
      C_Array* pData = CDA_BlackList.getpItempData(nItem);
      if(!strcmp(pData->getpBuffer(), test)){
         bLegal = false;
         break;
      }
   }

   ////////////////////////////////////////////////////////////////////
   // Speichert neuen Eintrag in CDA_BlackList und fBlackList
   if(bLegal){
      C_Array* pData = CDA_BlackList.addItem(1, strlen(test) + 1); // + ZERO
      memcpy(pData->getpBuffer(), test, strlen(test) + 1);         // + ZERO

      fBlackList.open(BLPATH, ios_base::in | ios_base::out | ios_base::trunc);
      if(fBlackList.is_open()) bBlackList = true;
   
      for(DWORD nItem = 0; nItem < CDA_BlackList.getnItems(); nItem++){
         C_Array* pData = CDA_BlackList.getpItempData(nItem);
         fBlackList.write(pData->getpBuffer(), pData->getcBuffer() - 1); // - ZERO
         fBlackList << endl;
      }

      fBlackList.close();
      bBlackList = false;
   }

   return(C_BLIST_READY);
}
//////////////////////////////////////////////////////////////////////////////////
// [ del ]
//////////////////////////////////////////////////////////////////////////////////
int C_Blacklist::del(const char* pSite){
  
   if(!pSite) return(C_BLIST_ERROR);
  
   long cfff = strlen(pSite);
   
   if(!cfff) return(C_BLIST_ERROR);

   long pPoint[10];
   long cPoint = 0;

   memset(pPoint, 0, sizeof(pPoint));

   for(long n = cfff; n > 0; n--){
      switch(pSite[n]){
         case 46:{
            pPoint[cPoint++] = n; break;
         }
      }
   }

   char test[1024];
   memset(test, 0, 1024);

   if(cPoint > 1)
      memcpy(test, &pSite[pPoint[1]] + 1, cfff - pPoint[1] - 1);
   else
   if(cPoint > 0)
      memcpy(test, &pSite, cfff);

   ////////////////////////////////////////////
   // Test ob vorhanden
   bool bLegal = true;
   DWORD nItem = 0;
   for(nItem = 0; nItem < CDA_BlackList.getnItems(); nItem++){
      C_Array* pData = CDA_BlackList.getpItempData(nItem);
      if(!strcmp(pData->getpBuffer(), test)){
         bLegal = false;
         break;
      }
   }

   ////////////////////////////////////////////////////////////////////
   // Loescht Eintrag in CDA_WhiteList und fWhiteList
   if(!bLegal){
      CDA_BlackList.delItem(nItem);

      fBlackList.open(BLPATH, ios_base::in | ios_base::out | ios_base::trunc);
      if(fBlackList.is_open()) bBlackList = true;
   
      for(DWORD nItem = 0; nItem < CDA_BlackList.getnItems(); nItem++){
         C_Array* pData = CDA_BlackList.getpItempData(nItem);
         fBlackList.write(pData->getpBuffer(), pData->getcBuffer() - 1); // - ZERO
         fBlackList << endl;
      }

      fBlackList.close();
      bBlackList = false;
   }

   return(C_BLIST_READY);
}