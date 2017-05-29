//////////////////////////////////////////////////////////////////////////////////
// [ APP_Class_Source ]
//////////////////////////////////////////////////////////////////////////////////

#include "C_App.hpp"

//////////////////////////////////////////////////////////////////////////////////
// [ Konstructor ]  
//////////////////////////////////////////////////////////////////////////////////
C_App::C_App(int argc, char *argv[]){
  
   this->set_icon_from_file("1.png");
   this->set_title("DNS - Proxy");
   this->resize(750, 500);
   this->maximize();

   /////////////////////////////////////////////

   m_TreeView.setFilter(&CBlacklist, &CWhitelist);
   
   //Add the TreeView, inside a ScrolledWindow, with the button underneath:
   m_ScrolledWindow.add(m_TreeView);
   
   //Only show the scrollbars when they are necessary:
   m_ScrolledWindow.set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);

   /////////////////////////////////////////////
   
   CResult.init(&m_TreeView, m_TreeView.m_refTreeModel, &m_TreeView.m_Columns, (S_Result*)&CNet.dns_result);
  
   /////////////////////////////////////////////
   
   if(CNet.start(&CWhitelist.CDA_WhiteList, &CBlacklist.CDA_BlackList) != C_NET_READY){
      cout << "ERROR: CNet.start" << endl;
      return;
   }

   // signal
   CNet.CNUdpClient.signal_data().connect(sigc::mem_fun(CNet, &C_Net::on_udp_data));
   CNet.CNUdpServer.signal_data().connect(sigc::mem_fun(CNet, &C_Net::on_udp_data));
   
   CNet.sig_dns_data.connect(sigc::mem_fun(CResult, &C_Result::on_dns_data));

   //Fill the combo:
   C_DArray*        pDA_Interface = 0;
   S_Net_Interface* pSInterface   = 0;
   
   if((pDA_Interface = CNet.CNInterface.get_pDA_Interface())){
      for(int n = 0; n < pDA_Interface->getnItems(); n++){
         S_C_DArray* pCA_F = pDA_Interface->getpItem(n);
         if((pSInterface = (S_Net_Interface*)pCA_F->pData->getpBuffer()))
            m_IFCombo.append(pSInterface->_ps_Name);
      }
   }else{
      cout << "ERROR: CNInterface.get_pDA_Interface" << endl;
      return;
   }
   
   m_IFCombo.set_active(CNet.CNInterface.get_First_Active());
   m_IFCombo.set_size_request(100, 30); 
   
   //////////////////////////////////////////////////////////////

   sbutton.start.set_label("start");
   sbutton.start.set_size_request(100, 30);
   sbutton.start.signal_clicked().connect(sigc::mem_fun(*this, &C_App::on_button_start));
   
   sbutton.stop.set_label("stop");
   sbutton.stop.set_size_request(100, 30);
   sbutton.stop.set_sensitive(false);
   sbutton.stop.signal_clicked().connect(sigc::mem_fun(*this, &C_App::on_button_stop));
   
   ////////////////
   
   sbutton.whitelist.set_label("whitelist");
   sbutton.whitelist.set_size_request(100, 30);
   
   sbutton.blacklist.set_label("blacklist");
   sbutton.blacklist.set_size_request(100, 30);
   
   sbutton.nolist.set_label("default");
   sbutton.nolist.set_size_request(100, 30);
   
   sbutton.blacklist.join_group(sbutton.whitelist);
   sbutton.nolist.join_group(sbutton.whitelist);
  
   sbutton.nolist.set_active();
   
   ///////////////////////////////////////////////////////

   EditIpD.set_size(3, 30);
   EditIpD.setIP("0.0.0.0");

   ///////////////////////////////////////////////////////
   
   m_layout.put(m_IFCombo,        0,  0);
   m_layout.put(sbutton.start,  100,  0);
   m_layout.put(sbutton.stop,   200,  0);
   
   m_layout.put(EditIpD.aEdit[0], 300, 0);
   m_layout.put(EditIpD.aEdit[1], 342, 0);
   m_layout.put(EditIpD.aEdit[2], 384, 0);
   m_layout.put(EditIpD.aEdit[3], 426, 0);
   
   m_layout.put(sbutton.whitelist, 468, 0);
   m_layout.put(sbutton.blacklist, 568, 0);
   m_layout.put(sbutton.nolist,    668, 0);
   
   m_layout.put(m_ScrolledWindow, 0, 30);
   
   add(m_layout);
   
   ////////////////////////
   
   signal_configure_event().connect(sigc::mem_fun(*this, &C_App::on_configure_changed), false);
   
   ////////////////////////
   
   show_all_children();
   
   ////////////////////////
   
   if(argc > 1){
      if(argc > 2){
	 if(argc > 3){
	    EditIpD.setIP(argv[3]);
	 }
         string arg3 = argv[2];
         if(arg3 == "black") sbutton.blacklist.set_active();
         else
         if(arg3 == "white") sbutton.whitelist.set_active();
         else                sbutton.nolist.set_active();
      }
      string arg2 = argv[1];
      if(arg2 == "auto") on_button_start();
   }
}
//////////////////////////////////////////////////////////////////////////////////
// [ Destructor ]
//////////////////////////////////////////////////////////////////////////////////
C_App::~C_App(){
  
}
//////////////////////////////////////////////////////////////////////////////////
// [ on_button_start ]
//////////////////////////////////////////////////////////////////////////////////
void C_App::on_button_start(){
   C_DArray*        pDA_Interface = 0;
   S_Net_Interface* pSInterface   = 0;
   
   //////////////////////////////

   if((pDA_Interface = CNet.CNInterface.get_pDA_Interface())){
     
      int nInterface = m_IFCombo.get_active_row_number();
     
      S_C_DArray* pCA_F = pDA_Interface->getpItem(nInterface);
     
      if((pSInterface = (S_Net_Interface*)pCA_F->pData->getpBuffer())){

	 string strIP_DNS;
	 EditIpD.getIP(&strIP_DNS);

         if(CNet.CNUdpClient.create() != C_NET_UDP_SOCKET_READY){
            cout << "Error create Client Socket" << endl;
            return;
         }
	 
         if(CNet.CNUdpClient.connect((char*)strIP_DNS.c_str(), 53) != C_NET_UDP_SOCKET_READY){
            cout << "Error connect Client Socket" << endl;
            return;
	 }

	 if(CNet.CNUdpClient.start(C_NET_ID_CLIENT, &CNet.CAClient) != C_NET_UDP_SOCKET_READY){
            cout << "Error start Client Socket" << endl;
            return;
	 }  
 
         ///////////////////////////////////////////

         if(CNet.CNUdpServer.create() != C_NET_UDP_SOCKET_READY){
            cout << "Error create Server Socket" << endl;
            return;
         }
   
         if(CNet.CNUdpServer.listen(pSInterface->_ps_IP, 53) != C_NET_UDP_SOCKET_READY){
            cout << "Error listen Server Socket" << endl;
            return;
	 }

	 if(CNet.CNUdpServer.start(C_NET_ID_SERVER, &CNet.CAServer) != C_NET_UDP_SOCKET_READY){
            cout << "Error start Server Socket" << endl;
            return;
	 } 
	 
      }else{
         cout << "ERROR: pSInterface" << endl;
         return;
      }
   }

   /////////////////////////////////////////////////
   
   CWhitelist.read();
   CBlacklist.read();
   
   if(sbutton.whitelist.get_active()){
      CNet.setNetModus(C_NET_MODUS_WHITELIST);
   }else
   if(sbutton.blacklist.get_active()){  
      CNet.setNetModus(C_NET_MODUS_BLACKLIST);
   }else{
     CNet.setNetModus(C_NET_MODUS_DEFAULT);
   }

   /////////////////////////////////////////////////
   
   m_IFCombo.set_sensitive(false);
   
   sbutton.start.set_sensitive(false);
   sbutton.stop.set_sensitive(true);
   
   sbutton.whitelist.set_sensitive(false);
   sbutton.blacklist.set_sensitive(false);
   sbutton.nolist.set_sensitive(false);
   
   EditIpD.disable();
}
//////////////////////////////////////////////////////////////////////////////////
// [ on_button_stop ]
//////////////////////////////////////////////////////////////////////////////////
void C_App::on_button_stop(){
  
   if(CNet.CNUdpClient.close() != C_NET_UDP_SOCKET_READY){
      cout << "Error close Client Socket" << endl;
      return;
   } 
   
   if(CNet.CNUdpServer.close() != C_NET_UDP_SOCKET_READY){
      cout << "Error close Server Socket" << endl;
      return;
   } 
   
   /////////////////////////////////////////////////
   
   m_IFCombo.set_sensitive(true);
   
   sbutton.start.set_sensitive(true);
   sbutton.stop.set_sensitive(false);
   
   sbutton.whitelist.set_sensitive(true);
   sbutton.blacklist.set_sensitive(true);
   sbutton.nolist.set_sensitive(true);
   
   EditIpD.enable();
}
//////////////////////////////////////////////////////////////////////////////////
// [ on_configure_changed ]
//////////////////////////////////////////////////////////////////////////////////
bool C_App::on_configure_changed(GdkEventConfigure* configure_event){
   m_ScrolledWindow.set_size_request(configure_event->width, configure_event->height - 30);
   return(false);
}
