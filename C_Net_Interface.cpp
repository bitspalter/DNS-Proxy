//////////////////////////////////////////////////////////////////////////////////
// [ Net_Interface_Class_Source ]
//////////////////////////////////////////////////////////////////////////////////

#include "C_Net_Interface.hpp"
 
//////////////////////////////////////////////////////////////////////////////////
// [ create ]
//////////////////////////////////////////////////////////////////////////////////
int C_Net_Interface::create(){
   // count interfaces
   int nInterface = 0;
   struct if_nameindex* pInterface = if_nameindex();
   if(!pInterface) return(C_NET_INTERFACE_ERROR);

   while(pInterface->if_index){
      pInterface++;
      nInterface++;
   }
   
   ///////////////////////////////////////////
   
   vInterface.resize(nInterface);
   
   ///////////////////////////////////////////
   
   nInterface = 0;
   pInterface = if_nameindex();
   
   struct if_nameindex* t1 = pInterface;
   
   while(pInterface->if_index){
      fill(pInterface->if_name, &vInterface[nInterface++]);
      pInterface++;
   }
   
   if_freenameindex(t1);
   
   return(C_NET_INTERFACE_READY);
}
//////////////////////////////////////////////////////////////////////////////////
// [ fill ]
//////////////////////////////////////////////////////////////////////////////////
int C_Net_Interface::fill(char* psDevice, S_Net_Interface* pS_Interface){
   int Socket = 0;
   char* psTemp = 0;

   struct ifreq if_data;
   in_addr in_Network;

   if(!psDevice || !pS_Interface) return(C_NET_INTERFACE_ERROR);

   ///////////////////////////////////////////////////////////////////////////////

   Socket = socket(AF_INET ,SOCK_DGRAM, 0);

   if(Socket == -1) return(C_NET_INTERFACE_ERROR);

   ///////////////////////////////////////////////////////////////////////////////

   memset(&if_data, 0, sizeof(if_data));

   strncpy(pS_Interface->ps_Name, psDevice, sizeof(if_data.ifr_name));

   strncpy(if_data.ifr_name, psDevice, sizeof(if_data.ifr_name));

   // Get Flags
   if(ioctl(Socket, SIOCGIFFLAGS, &if_data) < 0){
      close(Socket);
      return(C_NET_INTERFACE_ERROR);
   }
   pS_Interface->dw_Flags = if_data.ifr_flags;

   // Get MAC
   if(ioctl(Socket, SIOCGIFHWADDR, &if_data) < 0){
      close(Socket);
      return(C_NET_INTERFACE_ERROR);
   }
   memcpy(pS_Interface->uc_MAC, if_data.ifr_hwaddr.sa_data, 6);

   sprintf(pS_Interface->ps_MAC,
           "%02X:%02X:%02X:%02X:%02X:%02X", 
           pS_Interface->uc_MAC[0], pS_Interface->uc_MAC[1], pS_Interface->uc_MAC[2], 
           pS_Interface->uc_MAC[3], pS_Interface->uc_MAC[4], pS_Interface->uc_MAC[5]);

   ///////////////////////////////////////////////////////////////////////////////

   if(IFF_LOOPBACK & pS_Interface->dw_Flags) pS_Interface->uc_LoopBack = 1;

   ///////////////////////////////////////////////////////////////////////////////

   if((IFF_UP & pS_Interface->dw_Flags) && (IFF_RUNNING & pS_Interface->dw_Flags)){
      pS_Interface->uc_Active = 1;

      // Get IP
      if(ioctl(Socket, SIOCGIFADDR , &if_data) < 0){
         close(Socket);
         return(C_NET_INTERFACE_ERROR);
      }
      psTemp =  inet_ntoa( ((struct sockaddr_in*) (&if_data.ifr_addr))->sin_addr );
      memcpy(pS_Interface->ps_IP, psTemp, strlen(psTemp) + 1);
      pS_Interface->dw_IP = inet_addr( psTemp ); 

      // Get Netmask
      if(ioctl(Socket, SIOCGIFNETMASK , &if_data) < 0){
         close(Socket);
         return(C_NET_INTERFACE_ERROR);
      }
      psTemp =  inet_ntoa( ((struct sockaddr_in*) (&if_data.ifr_addr))->sin_addr );
      memcpy(pS_Interface->ps_Netmask, psTemp, strlen(psTemp) + 1);
      pS_Interface->dw_Netmask = inet_addr( psTemp ); 

      // Get Broadcast
      if(ioctl(Socket, SIOCGIFBRDADDR , &if_data) < 0){
         close(Socket);
         return(C_NET_INTERFACE_ERROR);
      }
      psTemp =  inet_ntoa( ((struct sockaddr_in*) (&if_data.ifr_addr))->sin_addr );
      memcpy(pS_Interface->ps_Broadcast, psTemp, strlen(psTemp) + 1);
      pS_Interface->dw_Broadcast = inet_addr( psTemp ); 

      // Get Network
      pS_Interface->dw_Network = pS_Interface->dw_IP & pS_Interface->dw_Netmask;

      in_Network.s_addr = pS_Interface->dw_Network;

      psTemp = inet_ntoa( in_Network );
      memcpy(pS_Interface->ps_Network, psTemp, strlen(psTemp) + 1);
      
      // Get Index
      memset(&if_data, 0, sizeof(if_data));
      strncpy(if_data.ifr_name, pS_Interface->ps_Name, IFNAMSIZ - 1);
      if(ioctl(Socket, SIOCGIFINDEX, &if_data) < 0){
         close(Socket);
         return(C_NET_INTERFACE_ERROR);
      }
      pS_Interface->dw_index = if_data.ifr_ifindex;
   }

   ///////////////////////////////////////////////////////////////////////////////

   close(Socket);

   return(C_NET_INTERFACE_READY);
}
//////////////////////////////////////////////////////////////////////////////////
// [get_First_Active]
//////////////////////////////////////////////////////////////////////////////////
int C_Net_Interface::get_First_Active(){
   for(vector<S_Net_Interface>::size_type i = 0; i != vInterface.size(); i++)
      if(vInterface[i].uc_Active && vInterface[i].uc_LoopBack != 1)
         return(i);  
   return(C_NET_INTERFACE_ERROR);
}
//////////////////////////////////////////////////////////////////////////////////
// [get_pInterface]
//////////////////////////////////////////////////////////////////////////////////
const S_Net_Interface* C_Net_Interface::get_pInterface(int nInterface){
   if(vInterface.size() < nInterface) return(nullptr);
   return(&vInterface[nInterface]); 
}
