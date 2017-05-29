//////////////////////////////////////////////////////////////////////////////////
// [ Net_Interface_Class_Source ]
//////////////////////////////////////////////////////////////////////////////////

#include "C_Net_Interface.hpp"
 
//////////////////////////////////////////////////////////////////////////////////
// [ create ]
//////////////////////////////////////////////////////////////////////////////////
int C_Net_Interface::create(){
   C_Array* pCA_Interface = 0;
   struct if_nameindex* pInterface = if_nameindex();

   DA_Interface.clear();

   if(pInterface){
      struct if_nameindex* t1 = pInterface;

      while(pInterface->if_index){
         pCA_Interface = DA_Interface.addItem(1, sizeof(S_Net_Interface));

         if(!pCA_Interface) return(C_NET_INTERFACE_ERROR);

         fill(pInterface->if_name, (S_Net_Interface*)pCA_Interface->getpBuffer());

         ++pInterface;
      }

      if_freenameindex(t1);
      
   }else return(C_NET_INTERFACE_ERROR);

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

   strncpy(pS_Interface->_ps_Name, psDevice, sizeof(if_data.ifr_name));

   strncpy(if_data.ifr_name, psDevice, sizeof(if_data.ifr_name));

   // Get Flags
   if(ioctl(Socket, SIOCGIFFLAGS, &if_data) < 0){
      close(Socket);
      return(C_NET_INTERFACE_ERROR);
   }
   pS_Interface->_dw_Flags = if_data.ifr_flags;

   // Get MAC
   if(ioctl(Socket, SIOCGIFHWADDR, &if_data) < 0){
      close(Socket);
      return(C_NET_INTERFACE_ERROR);
   }
   memcpy(pS_Interface->_uc_MAC, if_data.ifr_hwaddr.sa_data, 6);

   sprintf(pS_Interface->_ps_MAC,
           "%02X:%02X:%02X:%02X:%02X:%02X", 
           pS_Interface->_uc_MAC[0], pS_Interface->_uc_MAC[1], pS_Interface->_uc_MAC[2], 
           pS_Interface->_uc_MAC[3], pS_Interface->_uc_MAC[4], pS_Interface->_uc_MAC[5]);

   ///////////////////////////////////////////////////////////////////////////////

   if(IFF_LOOPBACK & pS_Interface->_dw_Flags) pS_Interface->_uc_LoopBack = 1;

   ///////////////////////////////////////////////////////////////////////////////

   if((IFF_UP & pS_Interface->_dw_Flags) && (IFF_RUNNING & pS_Interface->_dw_Flags)){
      pS_Interface->_uc_Active = 1;

      // Get IP
      if(ioctl(Socket, SIOCGIFADDR , &if_data) < 0){
         close(Socket);
         return(C_NET_INTERFACE_ERROR);
      }
      psTemp =  inet_ntoa( ((struct sockaddr_in*) (&if_data.ifr_addr))->sin_addr );
      memcpy(pS_Interface->_ps_IP, psTemp, strlen(psTemp) + 1);
      pS_Interface->_dw_IP = inet_addr( psTemp ); 

      // Get Netmask
      if(ioctl(Socket, SIOCGIFNETMASK , &if_data) < 0){
         close(Socket);
         return(C_NET_INTERFACE_ERROR);
      }
      psTemp =  inet_ntoa( ((struct sockaddr_in*) (&if_data.ifr_addr))->sin_addr );
      memcpy(pS_Interface->_ps_Netmask, psTemp, strlen(psTemp) + 1);
      pS_Interface->_dw_Netmask = inet_addr( psTemp ); 

      // Get Broadcast
      if(ioctl(Socket, SIOCGIFBRDADDR , &if_data) < 0){
         close(Socket);
         return(C_NET_INTERFACE_ERROR);
      }
      psTemp =  inet_ntoa( ((struct sockaddr_in*) (&if_data.ifr_addr))->sin_addr );
      memcpy(pS_Interface->_ps_Broadcast, psTemp, strlen(psTemp) + 1);
      pS_Interface->_dw_Broadcast = inet_addr( psTemp ); 

      // Get Network
      pS_Interface->_dw_Network = pS_Interface->_dw_IP & pS_Interface->_dw_Netmask;

      in_Network.s_addr = pS_Interface->_dw_Network;

      psTemp = inet_ntoa( in_Network );
      memcpy(pS_Interface->_ps_Network, psTemp, strlen(psTemp) + 1);
      
      // Get Index
      memset(&if_data, 0, sizeof(if_data));
      strncpy(if_data.ifr_name, pS_Interface->_ps_Name, IFNAMSIZ - 1);
      if(ioctl(Socket, SIOCGIFINDEX, &if_data) < 0){
         close(Socket);
         return(C_NET_INTERFACE_ERROR);
      }
      pS_Interface->_dw_index = if_data.ifr_ifindex;
   }

   ///////////////////////////////////////////////////////////////////////////////

   close(Socket);

   return(C_NET_INTERFACE_READY);
}
//////////////////////////////////////////////////////////////////////////////////
// [get_First_Active]
//////////////////////////////////////////////////////////////////////////////////
int C_Net_Interface::get_First_Active(){
   S_Net_Interface* pSInterface = 0;

   for(int nInterface = 0; nInterface < DA_Interface.getnItems(); nInterface++){
      S_C_DArray* pCA_F = DA_Interface.getpItem(nInterface);
      pSInterface = (S_Net_Interface*)pCA_F->pData->getpBuffer();

      if(!pSInterface){
         cout << "error pSInterface" << endl;
         continue;
      }

      if(pSInterface->_uc_Active && pSInterface->_uc_LoopBack != 1){
         return(nInterface);
      }
   }

   return(C_NET_INTERFACE_ERROR);
}
