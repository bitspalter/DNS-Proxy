//////////////////////////////////////////////////////////////////////////////////
// [ TreeView_Class_Source ]
//////////////////////////////////////////////////////////////////////////////////

#include "C_TreeView.hpp"

//////////////////////////////////////////////////////////////////////////////////
// [ Konstructor ]
//////////////////////////////////////////////////////////////////////////////////
C_TreeView::C_TreeView(){
  
   //Create the Tree model:
   m_refTreeModel = Gtk::ListStore::create(m_Columns);
   set_model(m_refTreeModel);
   
   //Add the TreeView's view columns:
   //This number will be shown with the default numeric formatting.
   append_column("Nr",      m_Columns.m_col_nr);
   append_column("Time",    m_Columns.m_col_time);
   append_column("User",    m_Columns.m_col_user);
   append_column("Request", m_Columns.m_col_request);
   //append_column("Result",  m_Columns.m_col_result);
   
   ////////////////////////////////////////
   //TEST
   //Gtk::TreeViewColumn* view_column = Gtk::manage(new Gtk::TreeViewColumn("Result"));
   //Gtk::CellRendererText* cell = Gtk::manage(new Gtk::CellRendererText());
   //Die Spalte Colour soll farbig dargestellt werden
   //Zuerst legen wir einen entsprechenden CellRenderer an
   //Gtk::CellRendererText *cell = new Gtk::CellRendererText;
   //Wir fuegen die Spalte hinzu
   int column_count = append_column("Result", m_cell);
 
   //Dann holen wir uns die gerade hinzugefuegte Spalte
   Gtk::TreeViewColumn *pColumn = get_column(column_count - 1); 
	
   //Gtk::TreeViewColumn* pColumn = get_column(4);
   //pColumn->pack_start(m_cell, false);
   pColumn->set_cell_data_func(m_cell, sigc::mem_fun(*this, &C_TreeView::on_cell_data_name));
   //append_column("Result", *view_column);
   
   ////////////////////////////////////////
   
   //Make all the columns reorderable:
   //This is not necessary, but it's nice to show the feature.
   //You can use TreeView::set_column_drag_function() to more
   //finely control column drag and drop.
   for(guint i = 0; i < 5; i++){
      auto column = get_column(i);
      column->set_reorderable();
      column->set_resizable(true);
      
      column->set_expand(true);
   }
   
   ////////////////////////
   //Fill popup menu:

   auto item = Gtk::manage(new Gtk::MenuItem("_whitelist", true));
   m_Popup.append(*item);
   
   auto item2 = Gtk::manage(new Gtk::MenuItem("_add", true));
   item2->signal_activate().connect(sigc::mem_fun(*this, &C_TreeView::on_popup_WhiteAdd));
   m_Popup_White.append(*item2);
   
   item2 = Gtk::manage(new Gtk::MenuItem("_del", true));
   item2->signal_activate().connect(sigc::mem_fun(*this, &C_TreeView::on_popup_WhiteDel));
   m_Popup_White.append(*item2);
   
   item2 = Gtk::manage(new Gtk::MenuItem("_edi", true));
   item2->signal_activate().connect(sigc::mem_fun(*this, &C_TreeView::on_popup_WhiteEdi));
   m_Popup_White.append(*item2);
   
   item->set_submenu(m_Popup_White);
   
   ////////////////////////////
   
   item = Gtk::manage(new Gtk::MenuItem("_blacklist", true));
   m_Popup.append(*item);
   
   item2 = Gtk::manage(new Gtk::MenuItem("_add", true));
   item2->signal_activate().connect(sigc::mem_fun(*this, &C_TreeView::on_popup_BlackAdd));
   m_Popup_Black.append(*item2);
   
   item2 = Gtk::manage(new Gtk::MenuItem("_del", true));
   item2->signal_activate().connect(sigc::mem_fun(*this, &C_TreeView::on_popup_BlackDel));
   m_Popup_Black.append(*item2);
   
   item2 = Gtk::manage(new Gtk::MenuItem("_edi", true));
   item2->signal_activate().connect(sigc::mem_fun(*this, &C_TreeView::on_popup_BlackEdi));
   m_Popup_Black.append(*item2);
   
   item->set_submenu(m_Popup_Black);
   
   ////////////////////////////
   
   item = Gtk::manage(new Gtk::MenuItem("_result", true));
   m_Popup.append(*item);
   
   item2 = Gtk::manage(new Gtk::MenuItem("_clear", true));
   item2->signal_activate().connect(sigc::mem_fun(*this, &C_TreeView::on_popup_ResultClear));
   m_Popup_Result.append(*item2);
   
   item2 = Gtk::manage(new Gtk::MenuItem("_save", true));
   item2->signal_activate().connect(sigc::mem_fun(*this, &C_TreeView::on_popup_ResultSave));
   m_Popup_Result.append(*item2);
   
   item->set_submenu(m_Popup_Result);
   
   ////////////////////////////
   
   m_Popup.accelerate(*this);
   m_Popup.show_all(); //Show all menu items when the menu pops up
}
//////////////////////////////////////////////////////////////////////////////////
// [ Destructor ]
//////////////////////////////////////////////////////////////////////////////////
C_TreeView::~C_TreeView(){
  
}
//////////////////////////////////////////////////////////////////////////////////
// [ setFilter ]
//////////////////////////////////////////////////////////////////////////////////
int C_TreeView::setFilter(C_Blacklist* pBlist, C_Whitelist* pWlist){
   
   if(!pBlist || !pWlist) return(C_TREEVIEW_ERROR);
  
   pBlacklist = pBlist;
   pWhitelist = pWlist;
   
   return(C_TREEVIEW_READY); 
}
//////////////////////////////////////////////////////////////////////////////////
// [ on_button_press_event ]
//////////////////////////////////////////////////////////////////////////////////
bool C_TreeView::on_button_press_event(GdkEventButton* button_event){
   bool return_value = false;

   //Call base class, to allow normal handling,
   //such as allowing the row to be selected by the right-click:
   return_value = TreeView::on_button_press_event(button_event);

   //Then do our custom stuff:
   if((button_event->type == GDK_BUTTON_PRESS) && (button_event->button == 3)){
      //m_Popup.popup_at_pointer((GdkEvent*)button_event);

      // Menu::popup_at_pointer() is new in gtkmm 3.22.
      // If you have an older revision, try this:
      m_Popup.popup(button_event->button, button_event->time);
   }

   return return_value;
}
//////////////////////////////////////////////////////////////////////////////////
// [ on_popup_WhiteAdd ]
//////////////////////////////////////////////////////////////////////////////////
void C_TreeView::on_popup_WhiteAdd(){

   auto refSelection = get_selection();
   
   if(refSelection){
      Gtk::TreeModel::iterator iter = refSelection->get_selected();
      if(iter){
         Glib::ustring request = (*iter)[m_Columns.m_col_request];
	 pWhitelist->add(request.c_str());
      }
   }
}
//////////////////////////////////////////////////////////////////////////////////
// [ on_popup_WhiteDel ]
//////////////////////////////////////////////////////////////////////////////////
void C_TreeView::on_popup_WhiteDel(){

   auto refSelection = get_selection();
   
   if(refSelection){
      Gtk::TreeModel::iterator iter = refSelection->get_selected();
      if(iter){
         Glib::ustring request = (*iter)[m_Columns.m_col_request];
	 pWhitelist->del(request.c_str());
      }
   }
}
//////////////////////////////////////////////////////////////////////////////////
// [ on_popup_WhiteEdi ]
//////////////////////////////////////////////////////////////////////////////////
void C_TreeView::on_popup_WhiteEdi(){

   m_refTreeModel->clear();
   
   for(int nWhite = 0; nWhite < pWhitelist->CDA_WhiteList.getnItems(); nWhite++){

      C_Array* pData = pWhitelist->CDA_WhiteList.getpItempData(nWhite);
      
      Gtk::TreeModel::iterator iter = m_refTreeModel->append();
      Gtk::TreeModel::Row row       = *iter;
   
      row.set_value(0, nWhite + 1);
      row.set_value(3, (Glib::ustring)pData->getpBuffer());
   }
}
//////////////////////////////////////////////////////////////////////////////////
// [ on_popup_BlackAdd ]
//////////////////////////////////////////////////////////////////////////////////
void C_TreeView::on_popup_BlackAdd(){

   auto refSelection = get_selection();
   
   if(refSelection){
      Gtk::TreeModel::iterator iter = refSelection->get_selected();
      if(iter){
         Glib::ustring request = (*iter)[m_Columns.m_col_request];
	 pBlacklist->add(request.c_str());
      }
   }
}
//////////////////////////////////////////////////////////////////////////////////
// [ on_popup_BlackDel ]
//////////////////////////////////////////////////////////////////////////////////
void C_TreeView::on_popup_BlackDel(){

   auto refSelection = get_selection();
   
   if(refSelection){
      Gtk::TreeModel::iterator iter = refSelection->get_selected();
      if(iter){
         Glib::ustring request = (*iter)[m_Columns.m_col_request];
	 pBlacklist->del(request.c_str());
      }
   }
}
//////////////////////////////////////////////////////////////////////////////////
// [ on_popup_BlackEdi ]
//////////////////////////////////////////////////////////////////////////////////
void C_TreeView::on_popup_BlackEdi(){

   m_refTreeModel->clear();
   
   for(int nBlack = 0; nBlack < pBlacklist->CDA_BlackList.getnItems(); nBlack++){

      C_Array* pData = pBlacklist->CDA_BlackList.getpItempData(nBlack);
      
      Gtk::TreeModel::iterator iter = m_refTreeModel->append();
      Gtk::TreeModel::Row row       = *iter;
   
      row.set_value(0, nBlack + 1);
      row.set_value(3, (Glib::ustring)pData->getpBuffer());
   }
}
//////////////////////////////////////////////////////////////////////////////////
// [ on_popup_ResultClear ]
//////////////////////////////////////////////////////////////////////////////////
void C_TreeView::on_popup_ResultClear(){
   m_refTreeModel->clear();
}
//////////////////////////////////////////////////////////////////////////////////
// [ on_popup_ResultSave ]
//////////////////////////////////////////////////////////////////////////////////
void C_TreeView::on_popup_ResultSave(){

   fstream fOutput;

   fOutput.open("23.txt", ios_base::out | ios_base::binary); 

   if(!fOutput.is_open()) return;

   typedef Gtk::TreeModel::Children type_children; //minimise code length.
   type_children children = m_refTreeModel->children();
   
   int nRow = 0;
   
   for(type_children::iterator iter = children.begin(); iter != children.end(); ++iter){

      fOutput << nRow++ << " " 
              << (*iter)[m_Columns.m_col_time]    << " " 
              << (*iter)[m_Columns.m_col_user]    << " " 
              << (*iter)[m_Columns.m_col_request] << " " 
              << (*iter)[m_Columns.m_col_result]  << endl;
   }

   fOutput.close();
}
//////////////////////////////////////////////////////////////////////////////////
// [ on_cell_data_name ]
//////////////////////////////////////////////////////////////////////////////////
void C_TreeView::on_cell_data_name(Gtk::CellRenderer* renderer, const Gtk::TreeModel::iterator& iter){
   //Get the value from the model and show it appropriately in the view:
   Gtk::TreeModel::Row row = *iter;
   Glib::ustring result = row[m_Columns.m_col_result];

   //if(id == SOME_MAGIC_NUMBER)
   //result = "<b>" + result + "</b>";

   Glib::ustring sKey = "cell-background";
   Glib::ustring sVal = "magenta";
   //pNameRend->set_property(sKey, sVal);
   
   Gdk::RGBA putzi = Gdk::RGBA("#2baf33");
   Gdk::RGBA patzi = Gdk::RGBA("#af2b33");
   
   Gtk::CellRendererText* text_renderer = dynamic_cast<Gtk::CellRendererText*>(renderer);
   if(text_renderer){
      //cout << "on_cell_data_name:" << (result == "BLOCK" ? patzi : putzi) << endl;
      text_renderer->property_foreground_rgba() = (result == "BLOCK" ? patzi : putzi);
      text_renderer->property_text()            = result;
      
      //text_renderer->set_property(sKey, sVal);
      
      //text_renderer->property_markup() = result;
   }
}