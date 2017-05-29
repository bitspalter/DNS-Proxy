//////////////////////////////////////////////////////////////////////////////////
// [ APP_Class_Header ]
//////////////////////////////////////////////////////////////////////////////////
//
// APP
//
// [::Last modi: 24.03.17 L.ey (µ~)::]
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
 
 #include "C_Blacklist.hpp"
 #include "C_Whitelist.hpp"
 
//////////////////////////////////////////////////////////////////////////////////
// CONSTANT
//////////////////////////////////////////////////////////////////////////////////
 
 const int C_APP_ERROR = 0x00;
 const int C_APP_READY = 0x01;
   
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
 class C_App : public Gtk::Window{
  
    public:
       C_App(int argc, char *argv[]);
       virtual ~C_App();

    protected:
     
       C_TreeView m_TreeView;
      
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
      
       C_Blacklist CBlacklist;
       C_Whitelist CWhitelist;
 };

#endif // _C_APP_H_
