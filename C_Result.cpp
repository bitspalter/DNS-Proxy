//////////////////////////////////////////////////////////////////////////////////
// [ Result_Class_Source ]
//////////////////////////////////////////////////////////////////////////////////

#include "C_Result.hpp"

//////////////////////////////////////////////////////////////////////////////////
// [ on_dns_data ]
//////////////////////////////////////////////////////////////////////////////////
void C_Result::on_dns_data(){
  
   lock_guard<mutex> lock(m_Mutex);

   //////////////////////////////////

   char   psDomain[1024];
   size_t csDomain = 0;
   size_t csName = 0;

   csName = pDns_Result->psName.size();

   for(DWORD n = 0; n < csName; n++){
      char cPart = pDns_Result->psName[n];

      memcpy(&psDomain[csDomain], &pDns_Result->psName[n + 1], cPart);
      csDomain += cPart;
      psDomain[csDomain++] = 46;

      n += cPart;
   }

   if(csDomain) psDomain[--csDomain] = 0x00;
   
   //////////////////////////////////

   cout << pListStore->children().size() << "  ";
   
   /////////////////
   
   char pItem1[256];

   time_t t = time(NULL);
   struct tm tm = *localtime(&t);
   sprintf(pItem1, "%d.%d.%d %d:%d:%d", tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900, tm.tm_hour, tm.tm_min, tm.tm_sec);

   cout << pItem1 << "  ";
   
   /////////////////
   
   char pItem2[256];

   in_addr in;
   in.s_addr = pDns_Result->pIP;    
   sprintf(pItem2, "%s", inet_ntoa(in));

   cout << pItem2 << "  ";
   
   /////////////////
   
   char pItem3[256];

   sprintf(pItem3, "%s", psDomain);

   cout << pItem3 << "  ";
   
   /////////////////
   
   char pItem4[256];

   if(pDns_Result->bBlock == true){
      if(pDns_Result->pResult) sprintf(pItem4, "%s", "BLOCK");
   }else{
      if(pDns_Result->pResult){
         sprintf(pItem4, "%d.%d.%d.%d", 
                (int)pDns_Result->pResult[0], (int)pDns_Result->pResult[1], 
                (int)pDns_Result->pResult[2], (int)pDns_Result->pResult[3]);
      }
   }

   cout << pItem4 << endl;
   
   /////////////////
   
   char pItem5[256];

   if(pDns_Result->pName){ 
      sprintf(pItem5, "%s", pDns_Result->pName);
      cout << "pDns_Result->pName != 0" << endl;
   }

   ////////////////////////////////////////////////////////////////
   
   //Fill the TreeView's model
   
   Gtk::TreeModel::iterator iter = pListStore->append();
   Gtk::TreeModel::Row row       = *iter;

   row.set_value(0, pListStore->children().size());
   row.set_value(1, (Glib::ustring)pItem1);
   row.set_value(2, (Glib::ustring)pItem2);
   row.set_value(3, (Glib::ustring)pItem3);
   row.set_value(4, (Glib::ustring)pItem4);

   // autoscroll
   Gtk::TreeModel::Path path = pListStore->get_path(iter);
   pTreeView->scroll_to_row(path);
}