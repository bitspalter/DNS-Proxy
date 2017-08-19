//////////////////////////////////////////////////////////////////////////////////
// [ Result_Class_Header ]
//////////////////////////////////////////////////////////////////////////////////
//
// RESULT
//
// [::Last modi: 15.03.17 L.ey (µ~)::]
//
//
#ifndef _C_RESULT_H_
 #define _C_RESULT_H_
 
 #include <iostream>
 
 using namespace std;
 
 #include <string.h>
 #include <arpa/inet.h>    // Funktionen wie inet_addr()
 
 #include "C_TreeView.hpp"
 
 #include <sigc++/sigc++.h>
 
 #include <mutex>
 
 typedef unsigned char  UCHAR;
 typedef unsigned short WORD;
 typedef unsigned int   DWORD;
   
//////////////////////////////////////////////////////////////////////////////////
// CONSTANT
//////////////////////////////////////////////////////////////////////////////////
 
 const int C_RESULT_ERROR = 0x00;
 const int C_RESULT_READY = 0x01;
   
//////////////////////////////////////////////////////////////////////////////////
// STRUCT
//////////////////////////////////////////////////////////////////////////////////

 struct S_Result{
    DWORD  pIP;
    string psName;
    UCHAR  pResult[4]; 
    char*  pName;
    bool   bBlock;
 };
   
//////////////////////////////////////////////////////////////////////////////////
// CLASS
//////////////////////////////////////////////////////////////////////////////////
 
 class C_Result : public sigc::trackable {
  
    public:
        
       C_Result(){};
      ~C_Result(){};
     
       int init(C_TreeView* pTV, Glib::RefPtr<Gtk::ListStore> pLS, Gtk::TreeModel::ColumnRecord* pCR, S_Result* pDnsData){
          pTreeView     = pTV; 
          pListStore    = pLS; 
          pColumnRecord = pCR; 
          pDns_Result   = pDnsData;
       };
     
       void on_dns_data();
      
    private:
  
       C_TreeView*                   pTreeView;
       Glib::RefPtr<Gtk::ListStore>  pListStore;
       Gtk::TreeModel::ColumnRecord* pColumnRecord;
      
       S_Result* pDns_Result;

       mutable mutex m_Mutex;
 };

#endif // _C_RESULT_H_
