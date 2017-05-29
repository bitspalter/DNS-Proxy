//////////////////////////////////////////////////////////////////////////////////
// [ Udp_Socket_Class_Source ]
//////////////////////////////////////////////////////////////////////////////////

#include "C_Net_Udp_Socket.hpp"

//////////////////////////////////////////////////////////////////////////////////
// [Constructor]  
//////////////////////////////////////////////////////////////////////////////////
C_Net_Udp_Socket::C_Net_Udp_Socket(){
   hSocket  = 0;
   bSocket  = false;
   bListen  = false;
   bConnect = false;
   bRun     = false;
}
//////////////////////////////////////////////////////////////////////////////////
// [Destructor]  
//////////////////////////////////////////////////////////////////////////////////
C_Net_Udp_Socket::~C_Net_Udp_Socket(){

}
//////////////////////////////////////////////////////////////////////////////////
// [create]  
//////////////////////////////////////////////////////////////////////////////////
int C_Net_Udp_Socket::create(){
  
   if(bSocket) return(C_NET_UDP_SOCKET_ERROR | C_NET_UDP_SOCKET_SUB_EXIST);

   hSocket = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP); 

   if(!hSocket) return(C_NET_UDP_SOCKET_ERROR | C_NET_UDP_SOCKET_SUB_API);

   bSocket = true;

   return(C_NET_UDP_SOCKET_READY);
}
//////////////////////////////////////////////////////////////////////////////////
// [close]  
//////////////////////////////////////////////////////////////////////////////////
int C_Net_Udp_Socket::close(){
  
   if(!bSocket) return(C_NET_UDP_SOCKET_ERROR | C_NET_UDP_SOCKET_SUB_EXIST);

   stop();
   
   ::close(hSocket);

   hSocket  = 0;
   bSocket  = false;
   bListen  = false;
   bConnect = false;
   
   memset(&Address_Remote, 0, sizeof(Address_Remote));
   memset(&Address_Local,  0, sizeof(Address_Local));

   return(C_NET_UDP_SOCKET_READY);
}
//////////////////////////////////////////////////////////////////////////////////
// [connect]  
//////////////////////////////////////////////////////////////////////////////////
int C_Net_Udp_Socket::connect(char* psIP, unsigned short Port){
  
   int address = 0;

   if(!psIP) return(C_NET_UDP_SOCKET_ERROR | C_NET_UDP_SOCKET_SUB_PTR);

   address = inet_addr(psIP);

   if(address == INADDR_NONE) return(C_NET_UDP_SOCKET_ERROR | C_NET_UDP_SOCKET_SUB_INVALD);

   Address_Remote.sin_family = AF_INET;
   Address_Remote.sin_port   = htons(Port);

   inet_aton(psIP, &Address_Remote.sin_addr);

   bConnect = true;

   return(C_NET_UDP_SOCKET_READY);
}
//////////////////////////////////////////////////////////////////////////////////
// [listen]  
//////////////////////////////////////////////////////////////////////////////////
int C_Net_Udp_Socket::listen(char* psIP, unsigned short Port){
   int result  = 0;
   int address = 0;

   if(!bSocket) return(C_NET_UDP_SOCKET_ERROR | C_NET_UDP_SOCKET_SUB_EXIST);

   if(!psIP) return(C_NET_UDP_SOCKET_ERROR | C_NET_UDP_SOCKET_SUB_PTR);

   address = inet_addr(psIP);

   if(address == INADDR_NONE) return(C_NET_UDP_SOCKET_ERROR | C_NET_UDP_SOCKET_SUB_INVALD);

   inet_aton(psIP, &Address_Local.sin_addr);

   Address_Local.sin_family = AF_INET;
   Address_Local.sin_port   = htons(Port);
 
   result = bind(hSocket, (sockaddr*) &Address_Local, sizeof(Address_Local));

   if(result == -1){
      cout << errno << endl;
      return(C_NET_UDP_SOCKET_ERROR | C_NET_UDP_SOCKET_SUB_API);
   }
   
   bListen = true;

   return(C_NET_UDP_SOCKET_READY);
}
//////////////////////////////////////////////////////////////////////////////////
// [send]  
//////////////////////////////////////////////////////////////////////////////////
int C_Net_Udp_Socket::send(unsigned char* pData, unsigned long cData){
  
   int result = 0;

   if(!bSocket) return(C_NET_UDP_SOCKET_ERROR | C_NET_UDP_SOCKET_SUB_EXIST);

   if(!bConnect) return(C_NET_UDP_SOCKET_ERROR | C_NET_UDP_SOCKET_SUB_STATE);

   if(!pData) return(C_NET_UDP_SOCKET_ERROR | C_NET_UDP_SOCKET_SUB_PTR); 

   result = sendto(hSocket, pData, cData, 0, (sockaddr*) &Address_Remote, sizeof(Address_Remote));

   if(!result) return(C_NET_UDP_SOCKET_ERROR | C_NET_UDP_SOCKET_SUB_API);

   return(C_NET_UDP_SOCKET_READY);
}
//////////////////////////////////////////////////////////////////////////////////
// [recive]  
//////////////////////////////////////////////////////////////////////////////////
int C_Net_Udp_Socket::recive(unsigned char* pData, unsigned int* pcData, sockaddr_in* pAddress_Source){
  
   int result = 0;
   int cAddress_Source = sizeof(sockaddr_in);

   if(!bSocket) return(C_NET_UDP_SOCKET_ERROR | C_NET_UDP_SOCKET_SUB_EXIST);

   if(!bListen) return(C_NET_UDP_SOCKET_ERROR | C_NET_UDP_SOCKET_SUB_STATE);

   if(!pData || !pcData) return(C_NET_UDP_SOCKET_ERROR | C_NET_UDP_SOCKET_SUB_PTR); 

   if(pAddress_Source)
      result = recvfrom(hSocket, pData, *pcData, 0, (sockaddr*) pAddress_Source, (socklen_t*)&cAddress_Source);
   else
      result = recvfrom(hSocket, pData, *pcData, 0, 0, 0);

   if(!result) return(C_NET_UDP_SOCKET_ERROR | C_NET_UDP_SOCKET_SUB_API);

   *pcData = result;

   return(C_NET_UDP_SOCKET_READY);
}
//////////////////////////////////////////////////////////////////////////////////
// [getAddressLocal]  
//////////////////////////////////////////////////////////////////////////////////
int C_Net_Udp_Socket::getAddressLocal(sockaddr_in* pAddress_Local){
  
   if(!pAddress_Local) return(C_NET_UDP_SOCKET_ERROR | C_NET_UDP_SOCKET_SUB_PTR);

   *pAddress_Local = Address_Local;

   return(C_NET_UDP_SOCKET_READY); 
}
//////////////////////////////////////////////////////////////////////////////////
// [getAddressRemote]  
//////////////////////////////////////////////////////////////////////////////////
int C_Net_Udp_Socket::getAddressRemote(sockaddr_in* pAddress_Remote){
  
   if(!pAddress_Remote) return(C_NET_UDP_SOCKET_ERROR | C_NET_UDP_SOCKET_SUB_PTR);

   *pAddress_Remote = Address_Remote;

   return(C_NET_UDP_SOCKET_READY); 
}
//////////////////////////////////////////////////////////////////////////////////
// [start]  
//////////////////////////////////////////////////////////////////////////////////
int C_Net_Udp_Socket::start(int idEx, C_Array* pCArrayEx){
  
   if(bRun || !bSocket || !pCArrayEx) return(C_NET_UDP_SOCKET_ERROR);
   
   ////////////
   pCAData = pCArrayEx;
   id      = idEx;
   ////////////
   
   if(CThread.create(this, &C_Net_Udp_Socket::recive) == C_THREAD_READY){
      bRun = true;
   }else return(C_NET_UDP_SOCKET_ERROR);

   return(C_NET_UDP_SOCKET_READY);
}
//////////////////////////////////////////////////////////////////////////////////
// [stop]  
//////////////////////////////////////////////////////////////////////////////////
int C_Net_Udp_Socket::stop(){
  
   if(!bRun || !bSocket) return(C_NET_UDP_SOCKET_ERROR);
   
   CThread.terminate();
   
   bRun = false;
   return(C_NET_UDP_SOCKET_READY);
}
//////////////////////////////////////////////////////////////////////////////////
// [recive]  (( THREAD ))
//////////////////////////////////////////////////////////////////////////////////
void C_Net_Udp_Socket::recive(){

   int sa_len = sizeof(Address_Remote);
   int cData = 0;

   while(bRun){
      cData = recvfrom(hSocket, pCAData->getpBuffer(), pCAData->getcBuffer(), 0, (struct sockaddr*)&Address_Remote, (socklen_t*) &sa_len);
      if(cData > 0) m_signal_data.emit(id, cData);
   }
}
//////////////////////////////////////////////////////////////////////////////////
// [signal_data]
//////////////////////////////////////////////////////////////////////////////////
C_Net_Udp_Socket::type_signal_data C_Net_Udp_Socket::signal_data(){
   return m_signal_data;
}