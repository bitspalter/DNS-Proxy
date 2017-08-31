//////////////////////////////////////////////////////////////////////////////////
// [ Edit_Ip_Class_Source ]
//////////////////////////////////////////////////////////////////////////////////

#include "C_Edit_IP.hpp"

//////////////////////////////////////////////////////////////////////////////////
// [ Konstructor ]  
//////////////////////////////////////////////////////////////////////////////////
C_Edit_IP::C_Edit_IP(){
   for(int n = 0; n < 4; n++){
      aEdit[n].set_max_length(3);
   }   
}
//////////////////////////////////////////////////////////////////////////////////
// [set_size]
//////////////////////////////////////////////////////////////////////////////////
void C_Edit_IP::setSize(int x, int y){
   for(int n = 0; n < 4; n++){
      aEdit[n].set_width_chars(x);
      aEdit[n].set_size_request(-1, y);
   }
}
//////////////////////////////////////////////////////////////////////////////////
// [clear] 
//////////////////////////////////////////////////////////////////////////////////
void C_Edit_IP::clear(){
   for(int n = 0; n < 4; n++) aEdit[n].set_text("");
}
//////////////////////////////////////////////////////////////////////////////////
// [setIP] (char*)
//////////////////////////////////////////////////////////////////////////////////
int C_Edit_IP::setIP(char* psV1, char* psV2, char* psV3, char* psV4){
    
   if(!psV1 || !psV2 || !psV3 || !psV4) return(C_EDIT_IP_ERROR);
   
   aEdit[0].set_text(psV1);
   aEdit[1].set_text(psV2);
   aEdit[2].set_text(psV3);
   aEdit[3].set_text(psV4);
   
   return(C_EDIT_IP_READY);
}
//////////////////////////////////////////////////////////////////////////////////
// [setIP] (UCHAR)
//////////////////////////////////////////////////////////////////////////////////
int C_Edit_IP::setIP(UCHAR ucV1, UCHAR ucV2, UCHAR ucV3, UCHAR ucV4){
   
   ostringstream Str_1;
   Str_1 << uppercase << setw(2) << setfill('0') << (int) ucV1;
   aEdit[0].set_text(Str_1.str());

   ostringstream Str_2;
   Str_2 << uppercase << setw(2) << setfill('0') << (int) ucV2; 
   aEdit[1].set_text(Str_2.str());

   ostringstream Str_3;
   Str_3 << uppercase << setw(2) << setfill('0') << (int) ucV3;
   aEdit[2].set_text(Str_3.str());

   ostringstream Str_4;
   Str_4 << uppercase << setw(2) << setfill('0') << (int) ucV4;
   aEdit[3].set_text(Str_4.str());

   return(C_EDIT_IP_READY);
}
//////////////////////////////////////////////////////////////////////////////////
// [setIP] (string)
//////////////////////////////////////////////////////////////////////////////////
int C_Edit_IP::setIP(const char* psData){

   if(!psData) return(C_EDIT_IP_ERROR);

   string strIP = psData;

   size_t found1 = 0;
   size_t found2 = 0;

   int nt = 0;
   string t[4];

   while(1){
      found1 = strIP.find(".", found2);
      if(found1 != string::npos){
         t[nt++] = strIP.substr(found2, found1 - found2);
      }else{
         t[nt] = strIP.substr(found2, strIP.size() - found2);
         break;
      }

      found2 = found1 + 1;
   }

   setIP((char*)t[0].c_str(), (char*)t[1].c_str(), (char*)t[2].c_str(), (char*)t[3].c_str());

   return(C_EDIT_IP_READY); 
}
//////////////////////////////////////////////////////////////////////////////////
// [getIP] (char*)
//////////////////////////////////////////////////////////////////////////////////
int C_Edit_IP::getIP(string* psData){

   Glib::ustring SEdit;

   if(!psData) return(C_EDIT_IP_ERROR);

   for(int i = 0; i < 4; i++){
      SEdit.clear();
      SEdit = aEdit[i].get_text();
      *psData += SEdit;
      if(i < 3) *psData += ".";
   }
      
   return(C_EDIT_IP_READY);
}
//////////////////////////////////////////////////////////////////////////////////
// [getIP] (UCHAR*)
//////////////////////////////////////////////////////////////////////////////////
int C_Edit_IP::getIP(UCHAR* pData){
  
   Glib::ustring SEdit;
   unsigned int dwData = 0;

   if(!pData) return(C_EDIT_IP_ERROR);

   for(int n = 0; n < 4; n++){
      SEdit.clear();
      SEdit = aEdit[n].get_text();

      dwData = stoul(SEdit, nullptr, 0);
      if(dwData > 255) return(C_EDIT_IP_ERROR);

      pData[n] = (UCHAR)dwData;
   }

   return(C_EDIT_IP_READY);
}
