//////////////////////////////////////////////////////////////////////////////////
// [ Udp_Socket_Class_Header ]
//////////////////////////////////////////////////////////////////////////////////
//
// [UDP Socket]
//
// Last modi: 19.08.17 L.ey (µ~)
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

 #include <sigc++/sigc++.h>
 
//////////////////////////////////////////////////////////////////////////////////
// CONSTANT
//////////////////////////////////////////////////////////////////////////////////

 const int C_NET_UDP_SOCKET_READY = 0x00000001;
 const int C_NET_UDP_SOCKET_ERROR = 0x00000000;

//////////////////////////////////////////////////////////////////////////////////
// CLASS
//////////////////////////////////////////////////////////////////////////////////
 
 class C_Net_Udp_Socket {
     
    public: 

       C_Net_Udp_Socket(); 
      ~C_Net_Udp_Socket();  

       int create();
       int close();

       int connect(char* psIP, unsigned short Port);
       int send(unsigned char* pData, unsigned long cData);

       int listen(const char* psIP, unsigned short Port);

       // Blocking
       int recive(unsigned char* pData, unsigned int* pcData, sockaddr_in* pAddress_Source);

       // Non Blocking
       int start(int id, unsigned char* pBuffer, unsigned int cBuffer);
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
       //C_Array* pCAData;
       unsigned char* pBuffer;
       unsigned int   cBuffer;
 };

#endif // _C_NET_UDP_SOCKET_H_
