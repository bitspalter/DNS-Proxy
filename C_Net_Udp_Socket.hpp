//////////////////////////////////////////////////////////////////////////////////
// [ Udp_Socket_Class_Header ]
//////////////////////////////////////////////////////////////////////////////////
//
// [UDP Socket]
//
// Last modi: 17.01.17 L.ey (µ~)
//
#ifndef _C_NET_UDP_SOCKET_H_
 #define _C_NET_UDP_SOCKET_H_

 #include <iostream>

 using namespace std;
 
 #include <errno.h>
 
 #include <sys/socket.h>   // Socket Funktionen wie socket(), bind() und listen()
 #include <arpa/inet.h>    // Funktionen wie inet_addr()
 #include <unistd.h>
 #include <string.h>
 
 #include <gtk/gtk.h>
 #include "C_Thread.hpp"
 #include "C_Array.hpp" 
 
 #include <sigc++/sigc++.h>
 
//////////////////////////////////////////////////////////////////////////////////

   const int C_NET_UDP_SOCKET_READY = 0x00000001;
   const int C_NET_UDP_SOCKET_ERROR = 0x00000000;
 
   const int C_NET_UDP_SOCKET_SUB_PTR    = 0x00000100;
   const int C_NET_UDP_SOCKET_SUB_API    = 0x00000200;
   const int C_NET_UDP_SOCKET_SUB_EXIST  = 0x00000300;
   const int C_NET_UDP_SOCKET_SUB_INVALD = 0x00000400;
   const int C_NET_UDP_SOCKET_SUB_STATE  = 0x00000500;

//////////////////////////////////////////////////////////////////////////////////

   class C_Net_Udp_Socket {
     
      public: 

         C_Net_Udp_Socket(); 
        ~C_Net_Udp_Socket();  

         int create();
         int close();

         int connect(char* psIP, unsigned short Port);
         int send(unsigned char* pData, unsigned long cData);
	 
	 int listen(char* psIP, unsigned short Port);
	 
	 // Blocking
         int recive(unsigned char* pData, unsigned int* pcData, sockaddr_in* pAddress_Source);

	 // Non Blocking
         int start(int id, C_Array* pCArray);
         int stop();
	 
         //////////////////////////////////////////////////////////////////////

         int getAddressLocal(sockaddr_in* pAddress_Local);
         int getAddressRemote(sockaddr_in* pAddress_Remote);

         // Inline
         //
         int gethSocket(){return(hSocket);}
         
         // Signal
         typedef sigc::signal<void, int, int> type_signal_data;
	 type_signal_data signal_data();
	 
      protected:
         type_signal_data m_signal_data;
  
      private:

         int  hSocket;
         bool bSocket;

         bool bListen;
         bool bConnect;

         sockaddr_in Address_Local;
         sockaddr_in Address_Remote;
	 
         //////////////////////////////////////////////////////////
         // RECIVE LOOP ((( THREAD )))
         C_Thread <C_Net_Udp_Socket> CThread;
	 
	 void recive();
	 bool bRun;
	 int  id;
	 
         /////////////////
	 C_Array* pCAData;
   };

#endif // _C_NET_UDP_SOCKET_H_
