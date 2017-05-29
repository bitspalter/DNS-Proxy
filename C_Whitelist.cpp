//////////////////////////////////////////////////////////////////////////////////
// [ Whitelist_Class_Source ]
//////////////////////////////////////////////////////////////////////////////////

#include "C_Whitelist.hpp"

//////////////////////////////////////////////////////////////////////////////////
// [ read ] Sichern !!!
//////////////////////////////////////////////////////////////////////////////////
int C_Whitelist::read(){

   CDA_WhiteList.clear();

   fWhiteList.open(WLPATH, ios_base::in | ios_base::out | ios_base::app);
   if(fWhiteList.is_open()) bWhiteList = true;
   
   char zeile[201];
   memset(zeile, 0, sizeof(zeile));

   while(fWhiteList.getline(zeile, 200)){
      if(strlen(zeile)){
         C_Array* pData = CDA_WhiteList.addItem(1, strlen(zeile) + 1);
         memcpy(pData->getpBuffer(), zeile, strlen(zeile) + 1);
      }
   }
   fWhiteList.close();
   bWhiteList = false;

   return(C_WLIST_READY);
}
//////////////////////////////////////////////////////////////////////////////////
// [ add ]
//////////////////////////////////////////////////////////////////////////////////
int C_Whitelist::add(const char* pSite){

   if(!pSite) return(C_WLIST_ERROR);
  
   long cfff = strlen(pSite);
   
   if(!cfff) return(C_WLIST_ERROR);

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
   // Test ob schon vorhanden
   bool bLegal = true;

   for(DWORD nItem = 0; nItem < CDA_WhiteList.getnItems(); nItem++){
      C_Array* pData = CDA_WhiteList.getpItempData(nItem);
      if(!strcmp(pData->getpBuffer(), test)){
         bLegal = false;
         break;
      }
   }

   ////////////////////////////////////////////////////////////////////
   // Speichert neuen Eintrag in CDA_WhiteList und fWhiteList
   if(bLegal){
      C_Array* pData = CDA_WhiteList.addItem(1, strlen(test) + 1); // + ZERO
      memcpy(pData->getpBuffer(), test, strlen(test) + 1);         // + ZERO

      fWhiteList.open(WLPATH, ios_base::in | ios_base::out | ios_base::trunc);
      if(fWhiteList.is_open()) bWhiteList = true;
   
      for(DWORD nItem = 0; nItem < CDA_WhiteList.getnItems(); nItem++){
         C_Array* pData = CDA_WhiteList.getpItempData(nItem);
         fWhiteList.write(pData->getpBuffer(), pData->getcBuffer() - 1); // - ZERO
         fWhiteList << endl;
      }

      fWhiteList.close();
      bWhiteList = false;
   }

   return(C_WLIST_READY);
}
//////////////////////////////////////////////////////////////////////////////////
// [ del ]
//////////////////////////////////////////////////////////////////////////////////
int C_Whitelist::del(const char* pSite){

   if(!pSite) return(C_WLIST_ERROR);
  
   long cfff = strlen(pSite);
   
   if(!cfff) return(C_WLIST_ERROR);

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
   for(nItem = 0; nItem < CDA_WhiteList.getnItems(); nItem++){
      C_Array* pData = CDA_WhiteList.getpItempData(nItem);
      if(!strcmp(pData->getpBuffer(), test)){
         bLegal = false;
         break;
      }
   }

   ////////////////////////////////////////////////////////////////////
   // Loescht Eintrag in CDA_WhiteList und fWhiteList
   if(!bLegal){
      CDA_WhiteList.delItem(nItem);

      fWhiteList.open(WLPATH, ios_base::in | ios_base::out | ios_base::trunc);
      if(fWhiteList.is_open()) bWhiteList = true;
   
      for(DWORD nItem = 0; nItem < CDA_WhiteList.getnItems(); nItem++){
         C_Array* pData = CDA_WhiteList.getpItempData(nItem);
         fWhiteList.write(pData->getpBuffer(), pData->getcBuffer() - 1); // - ZERO
         fWhiteList << endl;
      }

      fWhiteList.close();
      bWhiteList = false;
   }

   return(C_WLIST_READY);
}