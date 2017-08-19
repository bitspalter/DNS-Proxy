//////////////////////////////////////////////////////////////////////////////////
// [ TreeView_Class_Header ]
//////////////////////////////////////////////////////////////////////////////////
//
//
// [::Last modi: 19.03.17 L.ey (µ~)::]
//
//
#ifndef _C_TREEVIEW_H_
 #define _C_TREEVIEW_H_

 #include <iostream>
 
 using namespace std;
 
 #include <gtkmm/treeview.h>
 #include <gtkmm/liststore.h>
 #include <gtkmm/cellrenderertext.h>
 
 #include "C_Filterlist.hpp"

//////////////////////////////////////////////////////////////////////////////////
// CONSTANT
//////////////////////////////////////////////////////////////////////////////////
 
 const int C_TREEVIEW_ERROR = 0x00;
 const int C_TREEVIEW_READY = 0x01;
   
//////////////////////////////////////////////////////////////////////////////////
// CLASS
//////////////////////////////////////////////////////////////////////////////////
   
 class C_TreeView : public Gtk::TreeView {
   
    public:
       
       C_TreeView();
       virtual ~C_TreeView();
      
       int setFilter(C_Filterlist* pBlist, C_Filterlist* pWlist);

       ////////////////////////
       //Tree model columns:
       class ModelColumns : public Gtk::TreeModel::ColumnRecord {
           
          public:

            ModelColumns(){add(m_col_nr); add(m_col_time); add(m_col_user); add(m_col_request); add(m_col_result);}

            Gtk::TreeModelColumn<unsigned int>  m_col_nr;
            Gtk::TreeModelColumn<Glib::ustring> m_col_time;
            Gtk::TreeModelColumn<Glib::ustring> m_col_user;
            Gtk::TreeModelColumn<Glib::ustring> m_col_request;
            Gtk::TreeModelColumn<Glib::ustring> m_col_result;
       };

       ModelColumns m_Columns;
      
       Gtk::CellRendererText m_cell;
      
       /////////////////////////
      
       Glib::RefPtr<Gtk::ListStore> m_refTreeModel;
      
       /////////////////////////

       Gtk::Menu m_Popup;
       Gtk::Menu m_Popup_White;
       Gtk::Menu m_Popup_Black;
       Gtk::Menu m_Popup_Result;
      
       // Override Signal handler:
       // Alternatively, use signal_button_press_event().connect_notify()
       bool on_button_press_event(GdkEventButton* button_event) override;
      
       //Signal handler for popup menu items:
       void on_popup_WhiteAdd();
       void on_popup_WhiteDel();
       void on_popup_WhiteEdi();
   
       void on_popup_BlackAdd();
       void on_popup_BlackDel();
       void on_popup_BlackEdi();

       void on_popup_ResultClear();
       void on_popup_ResultSave();
      
       //Test
       C_Filterlist* pBlacklist;
       C_Filterlist* pWhitelist;
      
       void on_cell_data_name(Gtk::CellRenderer* renderer, const Gtk::TreeModel::iterator& iter);
 };
 
#endif // _C_TREEVIEW_H_
