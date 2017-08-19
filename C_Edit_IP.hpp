//////////////////////////////////////////////////////////////////////////////////
// [ Edit_Ip_Class_Header ]
//////////////////////////////////////////////////////////////////////////////////
//
//
// [::Last modi: 16.08.17 L.ey (µ~)::]  
//
//
#ifndef _C_EDIT_IP_H_
 #define _C_EDIT_IP_H_

  #include <iostream>
  #include <sstream>
  #include <string> 
  #include <iomanip>

  using namespace std;

  #include <gtkmm/entry.h>

  typedef unsigned char UCHAR;

//////////////////////////////////////////////////////////////////////////////////
 
 const int C_EDIT_IP_READY = 0x01;
 const int C_EDIT_IP_ERROR = 0x00;

//////////////////////////////////////////////////////////////////////////////////

 class C_Edit_IP {
     
    public:

       C_Edit_IP(){};  
      ~C_Edit_IP(){};  

       void setSize(int x, int y);
       void clear();

       //////////////////////////////////

       int setIP(char* psV1, char* psV2, char* psV3, char* psV4);
       int setIP(UCHAR ucV1, UCHAR ucV2, UCHAR ucV3, UCHAR ucV4);
       int setIP(const char* psData);

       //////////////////////////////////

       int getIP(string* psData); 
       int getIP(UCHAR* pData);

       /////////////////////////////////
       // INLINE
       void hide(){aEdit[0].hide(); aEdit[1].hide(); aEdit[2].hide(); aEdit[3].hide();}
       void show(){aEdit[0].show(); aEdit[1].show(); aEdit[2].show(); aEdit[3].show();}

       void enable(){aEdit[0].set_sensitive(true);aEdit[1].set_sensitive(true);aEdit[2].set_sensitive(true);aEdit[3].set_sensitive(true);};
       void disable(){aEdit[0].set_sensitive(false);aEdit[1].set_sensitive(false);aEdit[2].set_sensitive(false);aEdit[3].set_sensitive(false);};

       Gtk::Entry aEdit[4];
 };

#endif // _C_EDIT_IP_H_
