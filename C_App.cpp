//////////////////////////////////////////////////////////////////////////////////
// [ APP_Class_Source ]
//////////////////////////////////////////////////////////////////////////////////

#include "C_App.hpp"

//////////////////////////////////////////////////////////////////////////////////
// [ Konstructor ]  
//////////////////////////////////////////////////////////////////////////////////
C_App::C_App(int argc, char *argv[]){
  
   //this->set_icon_from_file("1.png");
   this->set_title("DNS - Proxy");
   this->resize(750, 500);
   this->maximize();

   /////////////////////////////////////////////
   
   CBlacklist.init(BLPATH);
   CWhitelist.init(WLPATH);
   
   /////////////////////////////////////////////
   
   CTreeView.setFilter(&CBlacklist, &CWhitelist);
   
   //Add the TreeView, inside a ScrolledWindow, with the button underneath:
   m_ScrolledWindow.add(CTreeView);
   
   //Only show the scrollbars when they are necessary:
   m_ScrolledWindow.set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);

   /////////////////////////////////////////////
   
   CResult.init(&CTreeView, CTreeView.m_refTreeModel, &CTreeView.m_Columns, (S_Result*)&CNet.dns_result);
  
   /////////////////////////////////////////////
   
   if(CNet.start(&CWhitelist.CDA_FilterList, &CBlacklist.CDA_FilterList) != C_NET_READY){
      cout << "ERROR: CNet.start" << endl;
      return;
   }

   // signal
   CNet.CNUdpClient.signal_data().connect(sigc::mem_fun(CNet, &C_Net::on_udp_data));
   CNet.CNUdpServer.signal_data().connect(sigc::mem_fun(CNet, &C_Net::on_udp_data));
   
   CNet.sig_dns_data.connect(sigc::mem_fun(CResult, &C_Result::on_dns_data));

   /////////////////////////////////////////////////////////////
   //Fill the combo:
   const vector<S_Net_Interface>* pDA_Interface = CNet.CNInterface.get_pDA_Interface();

   for(auto iface : *pDA_Interface) m_IFCombo.append(iface.ps_Name);
   
   //////////////////////////////////////////////////////////////
   
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

   EditIpD.setSize(3, 30);
   EditIpD.setIP("0.0.0.0");

   ///////////////////////////////////////////////////////
   
   m_layout.put(m_IFCombo,           0, 0);
   m_layout.put(sbutton.start,     100, 0);
   m_layout.put(sbutton.stop,      200, 0);
   
   m_layout.put(EditIpD.aEdit[0],  300, 0);
   m_layout.put(EditIpD.aEdit[1],  342, 0);
   m_layout.put(EditIpD.aEdit[2],  384, 0);
   m_layout.put(EditIpD.aEdit[3],  426, 0);
   
   m_layout.put(sbutton.whitelist, 468, 0);
   m_layout.put(sbutton.blacklist, 568, 0);
   m_layout.put(sbutton.nolist,    668, 0);
   
   m_layout.put(m_ScrolledWindow,   0, 30);
   
   add(m_layout);
   
   ////////////////////////
   
   signal_configure_event().connect(sigc::mem_fun(*this, &C_App::on_configure_changed), false);
   
   ////////////////////////
   
   show_all_children();
   
   ////////////////////////
   char tmp;
   bool bAutostart = false;
   
   while((tmp = getopt(argc, argv, "hd:l:i:b:w:a")) != -1){
      switch(tmp){
         case 'h': help(); break;
         case 'd': EditIpD.setIP(optarg); break;
         case 'a': bAutostart = true; break;
         case 'b': CBlacklist.init(optarg); break;
         case 'w': CWhitelist.init(optarg); break;
         case 'l':{
            if(strcmp(optarg, "black") == 0) sbutton.blacklist.set_active(); 
            else
            if(strcmp(optarg, "white") == 0) sbutton.whitelist.set_active();
            break;
         }
         case 'i':{
            int i = 0;
            for(auto iface : *pDA_Interface){
               if(strcmp(optarg, iface.ps_Name) == 0){
                   m_IFCombo.set_active(i);
                   break;
               }
               i++;
            }
            break;
         }
      }
   }

   if(bAutostart) on_button_start();
}
//////////////////////////////////////////////////////////////////////////////////
// [ help ]
//////////////////////////////////////////////////////////////////////////////////
void C_App::help(){
    cout << "Welcome" << endl;
    cout << "Options:" << endl;
    cout << "-h help" << endl;
    cout << "-d dns server" << endl;
    cout << "-a autostart" << endl;
    cout << "-l list (white|black)" << endl;
    cout << "-i interface" << endl;
    cout << "-b blacklist (path)" << endl;
    cout << "-w whitelist (path)" << endl;
    cout << "Example: ./dns-proxy -i eth0 -d 192.168.0.1 -l black -b /home/foo/blacklist.txt -a" << endl;
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
    
   const S_Net_Interface* pSInterface = 0;

   pSInterface = CNet.CNInterface.get_pInterface(m_IFCombo.get_active_row_number());

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

   if(CNet.CNUdpClient.start(C_NET_ID_CLIENT, &CNet.CAClient[0], C_NET_BUFFER) != C_NET_UDP_SOCKET_READY){
      cout << "Error start Client Socket" << endl;
      return;
   }  
 
   ///////////////////////////////////////////

   if(CNet.CNUdpServer.create() != C_NET_UDP_SOCKET_READY){
      cout << "Error create Server Socket" << endl;
      return;
   }
   
   if(CNet.CNUdpServer.listen(pSInterface->ps_IP, 53) != C_NET_UDP_SOCKET_READY){
      cout << "Error listen Server Socket" << endl;
      return;
   }

   if(CNet.CNUdpServer.start(C_NET_ID_SERVER, &CNet.CAServer[0], C_NET_BUFFER) != C_NET_UDP_SOCKET_READY){
      cout << "Error start Server Socket" << endl;
      return;
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
