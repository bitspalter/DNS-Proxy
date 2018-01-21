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
  
   if(bSocket) return(C_NET_UDP_SOCKET_ERROR);

   if(!(hSocket = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP))) return(C_NET_UDP_SOCKET_ERROR);

   bSocket = true;

   return(C_NET_UDP_SOCKET_READY);
}
//////////////////////////////////////////////////////////////////////////////////
// [close]  
//////////////////////////////////////////////////////////////////////////////////
int C_Net_Udp_Socket::close(){
  
   if(!bSocket) return(C_NET_UDP_SOCKET_ERROR);

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
  
   unsigned int address = 0;

   if(!psIP) return(C_NET_UDP_SOCKET_ERROR);

   if((address = inet_addr(psIP)) == INADDR_NONE) return(C_NET_UDP_SOCKET_ERROR);

   Address_Remote.sin_family = AF_INET;
   Address_Remote.sin_port   = htons(Port);

   inet_aton(psIP, &Address_Remote.sin_addr);

   bConnect = true;

   return(C_NET_UDP_SOCKET_READY);
}
//////////////////////////////////////////////////////////////////////////////////
// [listen]  
//////////////////////////////////////////////////////////////////////////////////
int C_Net_Udp_Socket::listen(const char* psIP, unsigned short Port){
    
   unsigned int address = 0;

   if(!bSocket || !psIP) return(C_NET_UDP_SOCKET_ERROR);

   if((address = inet_addr(psIP)) == INADDR_NONE) return(C_NET_UDP_SOCKET_ERROR);

   inet_aton(psIP, &Address_Local.sin_addr);

   Address_Local.sin_family = AF_INET;
   Address_Local.sin_port   = htons(Port);
 
   if(bind(hSocket, (sockaddr*) &Address_Local, sizeof(Address_Local)) == -1)
      return(C_NET_UDP_SOCKET_ERROR);
   
   bListen = true;

   return(C_NET_UDP_SOCKET_READY);
}
//////////////////////////////////////////////////////////////////////////////////
// [send]  
//////////////////////////////////////////////////////////////////////////////////
int C_Net_Udp_Socket::send(unsigned char* pData, unsigned long cData){
  
   if(!bSocket || !bConnect || !pData) return(C_NET_UDP_SOCKET_ERROR);

   if(!sendto(hSocket, pData, cData, 0, (sockaddr*) &Address_Remote, sizeof(Address_Remote))) 
      return(C_NET_UDP_SOCKET_ERROR);

   return(C_NET_UDP_SOCKET_READY);
}
//////////////////////////////////////////////////////////////////////////////////
// [recive]  
//////////////////////////////////////////////////////////////////////////////////
int C_Net_Udp_Socket::recive(unsigned char* pData, unsigned int* pcData, sockaddr_in* pAddress_Source){

   int cAddress_Source = sizeof(sockaddr_in);

   if(!bSocket || !bListen || !pData || !pcData) return(C_NET_UDP_SOCKET_ERROR);

   if(pAddress_Source)
      *pcData = recvfrom(hSocket, pData, *pcData, 0, (sockaddr*) pAddress_Source, (socklen_t*)&cAddress_Source);
   else
      *pcData = recvfrom(hSocket, pData, *pcData, 0, 0, 0);

   if(!*pcData) return(C_NET_UDP_SOCKET_ERROR);

   return(C_NET_UDP_SOCKET_READY);
}
//////////////////////////////////////////////////////////////////////////////////
// [getAddressLocal]  
//////////////////////////////////////////////////////////////////////////////////
int C_Net_Udp_Socket::getAddressLocal(sockaddr_in* pAddress_Local){
  
   if(!pAddress_Local) return(C_NET_UDP_SOCKET_ERROR);

   *pAddress_Local = Address_Local;

   return(C_NET_UDP_SOCKET_READY); 
}
//////////////////////////////////////////////////////////////////////////////////
// [getAddressRemote]  
//////////////////////////////////////////////////////////////////////////////////
int C_Net_Udp_Socket::getAddressRemote(sockaddr_in* pAddress_Remote){
  
   if(!pAddress_Remote) return(C_NET_UDP_SOCKET_ERROR);

   *pAddress_Remote = Address_Remote;

   return(C_NET_UDP_SOCKET_READY); 
}
//////////////////////////////////////////////////////////////////////////////////
// [start]  
//////////////////////////////////////////////////////////////////////////////////
int C_Net_Udp_Socket::start(int idEx, unsigned char* pBuf, unsigned int cBuf){
  
   if(bRun || !bSocket || !pBuf || !cBuf) return(C_NET_UDP_SOCKET_ERROR);
   
   ////////////
   //pCAData = pCArrayEx;
   pBuffer = pBuf;
   cBuffer = cBuf;
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
      cData = recvfrom(hSocket, pBuffer, cBuffer, 0, (struct sockaddr*)&Address_Remote, (socklen_t*) &sa_len);
      if(cData > 0) m_signal_data.emit(id, cData);
   }
}
//////////////////////////////////////////////////////////////////////////////////
// [signal_data]
//////////////////////////////////////////////////////////////////////////////////
C_Net_Udp_Socket::type_signal_data C_Net_Udp_Socket::signal_data(){
   return m_signal_data;
}
