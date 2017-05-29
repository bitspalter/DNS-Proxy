//////////////////////////////////////////////////////////////////////////////////
// [ Net_Class_Header ]
//////////////////////////////////////////////////////////////////////////////////
//
// Net
//
// [::Last modi: 01.03.17 L.ey (µ~)::]
//
//
#ifndef _C_NET_H_
 #define _C_NET_H_
 
   #include <iostream>
   #include <fstream>
   #include <string>
   #include <sstream>

   #include <errno.h>
   
   using namespace std;
 
   #include <sigc++/sigc++.h>
   
   #include <glibmm/dispatcher.h>
   
   #include "Net_Protokoll_Header.hpp"
   
   #include "C_Net_Interface.hpp"
   #include "C_Net_Udp_Socket.hpp"
   
   typedef unsigned char  UCHAR;
   typedef unsigned short WORD;
   typedef unsigned int   DWORD;
 
//////////////////////////////////////////////////////////////////////////////////
// CONSTANT
//////////////////////////////////////////////////////////////////////////////////
 
   const int C_NET_ERROR = 0x00;
   const int C_NET_READY = 0x01;
   
   const int C_NET_ID_CLIENT = 0xFFAA;
   const int C_NET_ID_SERVER = 0xFFBB;
   
   const int C_NET_MODUS_DEFAULT   = 0x00;
   const int C_NET_MODUS_WHITELIST = 0x01;
   const int C_NET_MODUS_BLACKLIST = 0x02;
   
//////////////////////////////////////////////////////////////////////////////////
// STRUCT
//////////////////////////////////////////////////////////////////////////////////

   struct Dns_Request{
      WORD  ID;
      DWORD cData;
      DWORD Client_IP;
      WORD  Client_Port;
   };

   struct Dns_Result{
      DWORD  pIP;
      string psName;
      UCHAR  pResult[4]; 
      char*  pName;
      bool   bBlock;
   };
   
//////////////////////////////////////////////////////////////////////////////////
// CLASS
//////////////////////////////////////////////////////////////////////////////////
   
   class C_Net : public sigc::trackable {
     
      public:
	
         C_Net(){this->NetModus = C_NET_MODUS_DEFAULT;this->status = C_NET_ERROR;};
        ~C_Net(){};

	 int start(C_DArray* pCDA_WList, C_DArray* pCDA_BList);
	
	 void on_client_data(UCHAR* pData, DWORD cData);
	 void on_server_data(UCHAR* pData, DWORD cData);
	 
	 ///////////////////////////////////////////////////////////////
       
         C_Net_Interface  CNInterface;
	 
         C_Net_Udp_Socket CNUdpClient;
	 C_Net_Udp_Socket CNUdpServer;
	 
	 C_Array CAClient;
	 C_Array CAServer;
	 
	 /////////////////////////////////////////
	 
	 int  getNetModus(){return(NetModus);};
	 void setNetModus(int Modus){NetModus = Modus;};
	 
	 // Recive Signal
	 void on_udp_data(int id, int cData);
	 
	 Glib::Dispatcher sig_dns_data;
	 
         //test
	 Dns_Result dns_result;

      private: 
	
         void get_request(DWORD pIP, UCHAR* pDNS_Packet, DWORD cDNS_Packet);

	 //////////
	 
	 int NetModus;
	 
         ///////////////////////////////////////////////////////////////
	   
	 C_DArray cda_request; // Dynamic List
	 
	 /////////////////
	 
	 C_DArray* pCDA_WhiteList;
         C_DArray* pCDA_BlackList;

	 /////////////////
	 
         int status;
   };
 
#endif // _C_NET_H_
 