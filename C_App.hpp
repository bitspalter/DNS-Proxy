//////////////////////////////////////////////////////////////////////////////////
// [ APP_Class_Header ]
//////////////////////////////////////////////////////////////////////////////////
//
// DNS-PROXY ver:0.6
//
// [::Last modi: 19.08.17 L.ey (µ~)::]
//
//
#ifndef _C_APP_H_
 #define _C_APP_H_

 #include <iostream>
 using namespace std;
 
 #include <gtkmm/window.h>
 #include <gtkmm/button.h>
 #include <gtkmm/radiobutton.h>
 #include <gtkmm/comboboxtext.h>
 #include <gtkmm/scrolledwindow.h>
 #include <gtkmm/layout.h>
 
 #include "C_TreeView.hpp"
 #include "C_Edit_IP.hpp"
 
 #include "C_Net.hpp"
 #include "C_Result.hpp"
 
 #include "C_Filterlist.hpp"
 
//////////////////////////////////////////////////////////////////////////////////
// CONSTANT
//////////////////////////////////////////////////////////////////////////////////
 
 const int C_APP_ERROR = 0x00;
 const int C_APP_READY = 0x01;
   
 const char WLPATH[] = "whitelist.txt";
 const char BLPATH[] = "blacklist.txt";
 
//////////////////////////////////////////////////////////////////////////////////
// STRUCT
//////////////////////////////////////////////////////////////////////////////////

 struct SButton {
    Gtk::Button start;
    Gtk::Button stop;
    Gtk::RadioButton whitelist;
    Gtk::RadioButton blacklist;
    Gtk::RadioButton nolist;
 };
   
//////////////////////////////////////////////////////////////////////////////////
// CLASS
//////////////////////////////////////////////////////////////////////////////////
 
 class C_App : public Gtk::Window {
  
    public:
        
       C_App(int argc, char *argv[]);
       virtual ~C_App();

    protected:
     
       C_TreeView CTreeView;
      
       //Child widgets:
       Gtk::ComboBoxText m_IFCombo;
       Gtk::Layout       m_layout;

       SButton sbutton;
      
       C_Edit_IP EditIpD;
      
       ////////////////////////

       Gtk::ScrolledWindow m_ScrolledWindow;

       ////////////////////////
      
       //Signal handlers:
       void on_button_start();
       void on_button_stop();
       bool on_configure_changed(GdkEventConfigure* configure_event);
      
       ////////////////////////
      
       C_Net CNet;
      
       C_Result CResult;
      
       ////////////////////////
      
       C_Filterlist CBlacklist;
       C_Filterlist CWhitelist;
       
       ////////////////////////
       
       void help();
 };

#endif // _C_APP_H_
