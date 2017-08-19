//////////////////////////////////////////////////////////////////////////////////
// [ Net_Class_Source ]
//////////////////////////////////////////////////////////////////////////////////

#include "C_Net.hpp"
//////////////////////////////////////////////////////////////////////////////////
// [ start ]
//////////////////////////////////////////////////////////////////////////////////
int C_Net::start(C_DArray<char>* pCDA_WList, C_DArray<char>* pCDA_BList){
  
   if(!pCDA_WList || !pCDA_BList) return(C_NET_ERROR);
   
   this->pCDA_WhiteList = pCDA_WList;
   this->pCDA_BlackList = pCDA_BList;
   
   if(CNInterface.create() != C_NET_INTERFACE_READY){
      cout << "ERROR: CNInterface.create" << endl;
      return(C_NET_ERROR);
   }

   return(C_NET_READY);
}
//////////////////////////////////////////////////////////////////////////////////
// [ on_udp_data ]
//////////////////////////////////////////////////////////////////////////////////
void C_Net::on_udp_data(int id, int cData){
   if(id == C_NET_ID_CLIENT) on_client_data(&CAClient[0], (DWORD)cData);
   else
   if(id == C_NET_ID_SERVER) on_server_data(&CAServer[0], (DWORD)cData);
}
//////////////////////////////////////////////////////////////////////////////////
// [ on_client_data ]
//////////////////////////////////////////////////////////////////////////////////
void C_Net::on_client_data(UCHAR* pBuffer, DWORD cBuffer){

   if(cBuffer < sizeof(DNS_HEADER)) return;

   // GET DNS FLAG
   WORD _DNS_Flag = ntohs(((DNS_HEADER*)pBuffer)->DNS_Flags);

   if(_DNS_Flag & DNS_FLAG_TYP_RESPONSE){
      // CHECK ALL REQUESTS
      for(DWORD n = 0; n < cda_request.getnItems(); n++){
         // GET REQUEST
         C_Array<Dns_Request>* pRequest = cda_request.getpItempData(n);
         if(!pRequest) return;
         // GET DATA
         Dns_Request* pDns = pRequest->getpBuffer();
         if(!pDns) return;
         // CHECK DNS ID
         if(pDns->ID == ((DNS_HEADER*)pBuffer)->DNS_ID){
            // SET DESTINATON
            in_addr in;
            in.s_addr = pDns->Client_IP;
            CNUdpServer.connect(inet_ntoa(in), pDns->Client_Port);
            // SEND DATA
            CNUdpServer.send(pBuffer, cBuffer);
            // SHOW REQUEST
            get_request(pDns->Client_IP, pBuffer, cBuffer);
            // DELETE REQUEST
            cda_request.delItem(n);
            // READY
            return;
         }
      }
   }
}
//////////////////////////////////////////////////////////////////////////////////
// [ on_server_data ]
//////////////////////////////////////////////////////////////////////////////////
void C_Net::on_server_data(UCHAR* pBuffer, DWORD cBuffer){

   if(cBuffer < sizeof(DNS_HEADER)) return;

   WORD _DNS_Flag = ntohs(((DNS_HEADER*)pBuffer)->DNS_Flags);

   if(_DNS_Flag & DNS_FLAG_TYP_RESPONSE){ // Response
      //message( "DNS Response" );
   }else{ // Request wirklich ?!?
 
      sockaddr_in sock;
      CNUdpServer.getAddressRemote(&sock);
      
      ////////////////////////////////////////////////////////////////////////
      // Test gegen Black | White List
      if(this->NetModus == C_NET_MODUS_WHITELIST || this->NetModus == C_NET_MODUS_BLACKLIST){
         DWORD off = sizeof(DNS_HEADER);
         DWORD offDomain = 0;
         UCHAR cDNS_Name = pBuffer[off];

         UCHAR pDomain[1024];

         ///////////////////////////////////////////////////////////////////////
         // Mach Aus pBuffer[] = "[3][G][M][X][2][D][E]"
         // pDomain[] = "[G][M][X][.][D][E][0]"

         while(pBuffer[off]){
            memcpy(&pDomain[offDomain], &pBuffer[off + 1], pBuffer[off]);
            offDomain += pBuffer[off];
            pDomain[offDomain] = 46;
            offDomain++;
            off += pBuffer[off] + 1;

            if(offDomain > 1024){
               offDomain = 1024;
               break;
            }
         }

         pDomain[offDomain - 1] = 0;

         ///////////////////////////////////////////////////////////////////
         // Mach aus aaa.bbbb.cc.de
         // pTLD->"cc.de"

         long cfff = strlen((char*)pDomain);

         long pPoint[2];
         long cPoint = 0;

         memset(pPoint, 0, sizeof(pPoint));

         for(long n = cfff; n > 0; n--){
            switch(pDomain[n]){
               case 46:{
                  pPoint[cPoint++] = n; break;
               }
            }
            if(cPoint == 2) break;
         }

         char* pTLD = (char*)&pDomain;
         if(cPoint > 1)
            pTLD = (char*)&pDomain[pPoint[1]] + 1;

         ////////////////////////////////////////////////////////////////////
         // Test gegen White | Black List

         bool bLegal = false;

         if(this->NetModus == C_NET_MODUS_WHITELIST){
            for(DWORD nWL = 0; nWL < pCDA_WhiteList->getnItems(); nWL++){
               if(C_Array<char>* pData = pCDA_WhiteList->getpItempData(nWL)){
                  if(char* pTB = pData->getpBuffer()){
                     if(!strcmp(pTB, pTLD)){
                        bLegal = true;
                        break;
                     }
                  }
               }
            }
         }else
         if(this->NetModus == C_NET_MODUS_BLACKLIST){
            bLegal = true;
            for(DWORD nBL = 0; nBL < pCDA_BlackList->getnItems(); nBL++){
               if(C_Array<char>* pData = pCDA_BlackList->getpItempData(nBL)){
                  if(char* pTB = pData->getpBuffer()){
                     if(!strcmp(pTB, pTLD)){
                        bLegal = false;
                        break;
                     }
                  }
               }
            }
         }

         ////////////////////////////////////////////////////////77

         if(bLegal == false){

            ///////////////////////////////////////////////////////////////////////////
            // Packet hat Black | White List nicht bestanden
            // Sende [Not Exist Error]-->Client
            ((DNS_HEADER*)pBuffer)->DNS_Flags = 0x8381;

            //////////////////////////////////////////////////
            Dns_Request dnsRequest;
            // REQUEST ID
            dnsRequest.ID = ((DNS_HEADER*)pBuffer)->DNS_ID;
            // REQUEST SIZE
            dnsRequest.cData = cBuffer;
	    
            //////////////////////////////////////////////////    
            // SET MAC:IP:PORT FOR UDP SOCKET
            CNUdpServer.connect(inet_ntoa(sock.sin_addr), ntohs(sock.sin_port));
            // SEND DATA UDP SOCKET
            CNUdpServer.send(pBuffer, cBuffer);

            //////////////////////////////////////////////////////////////////////////
            // Ausgabe 
            UCHAR NETBCIP[4] = {0xFF,0xFF,0xFF,0xFF};

            dns_result.pIP        = (DWORD)sock.sin_addr.s_addr;
            dns_result.psName     = (char*)pBuffer + sizeof(DNS_HEADER);
            dns_result.pResult[0] = NETBCIP[0];
            dns_result.pResult[1] = NETBCIP[1];
            dns_result.pResult[2] = NETBCIP[2];
            dns_result.pResult[3] = NETBCIP[3];
            dns_result.pName      = 0;
            dns_result.bBlock     = true;

            sig_dns_data();
	    
            return; // FERTIG
         }
      }

      ///////////////////////////////////////////////////////////////////////////
      // Packet hat Black | White List bestanden
      // Speicher Request, Sende Request an Original DNS Server
      C_Array<Dns_Request>* pRequest = cda_request.addItem(1);
      if(!pRequest) return;
      //////////////////////////////////////////////////
      Dns_Request dnsRequest;
      // REQUEST ID
      dnsRequest.ID = ((DNS_HEADER*)pBuffer)->DNS_ID;
      // REQUEST SIZE
      dnsRequest.cData = cBuffer;
      // REQUEST IP
      dnsRequest.Client_IP = sock.sin_addr.s_addr;
      // REQUEST Port
      dnsRequest.Client_Port = htons(sock.sin_port);
      // SAVE REQUEST
      memcpy(pRequest->getpBuffer(), &dnsRequest, sizeof(Dns_Request));
      //////////////////////////////////////////////////    
      // SEND DATA UDP SOCKET
      CNUdpClient.send(pBuffer, cBuffer);
   }
}
//////////////////////////////////////////////////////////////////////////////////
// [ get_request ]
//////////////////////////////////////////////////////////////////////////////////
void C_Net::get_request(DWORD pIP, UCHAR* pDNS_Packet, DWORD cDNS_Packet){
  
   UCHAR* pData = pDNS_Packet;
 
   char   psDomain[1024];
   size_t csDomain = 0;
   size_t csName   = 0;

   if(cDNS_Packet < sizeof(DNS_HEADER)) return;

   WORD cQuest  = ntohs(((DNS_HEADER*)pDNS_Packet)->DNS_cQuestion);
   WORD cAnswer = ntohs(((DNS_HEADER*)pDNS_Packet)->DNS_cAnswer);
   
   pData += sizeof(DNS_HEADER);
   
   //////////////////////////////////////////////////////////////
   // DNS_cQuestion
   if(cQuest){
      for(DWORD nQuest = 0; nQuest < cQuest; nQuest++){
         csName = strlen((char*)pData);

         for(DWORD n = 0; n < csName; n++){
            char cPart = pData[n];

            memcpy(&psDomain[csDomain], &pData[n + 1], cPart);
            csDomain += cPart;
            psDomain[csDomain++] = 46;
            n += cPart;
         }

         if(csDomain) psDomain[--csDomain] = 0x00;
	 
         pData += csDomain;
         pData += 6;
      }
   }
   //////////////////////////////////////////////////////////////
   // DNS_cAnswer
   if(cAnswer){
      for(DWORD nAnswer = 0; nAnswer < cAnswer; nAnswer++){
         DNS_ANSWER* pAnswer = (DNS_ANSWER*)pData;

         pData += sizeof(DNS_ANSWER);

         // DATA = ADDRESS LIKE 190.37.233.45
         if(pAnswer->DNS_Type == DNS_QUEST_TYP_ADDRESS){
	   
            dns_result.pIP        = pIP;
            dns_result.psName     = (char*)pDNS_Packet + sizeof(DNS_HEADER);
            dns_result.pResult[0] = pData[0];
            dns_result.pResult[1] = pData[1];
            dns_result.pResult[2] = pData[2];
            dns_result.pResult[3] = pData[3];
            dns_result.pName      = 0;
            dns_result.bBlock     = false;

            sig_dns_data();
	    
            pData += 4;
         }else
         // DATA = ADDRESS LIKE www3.L.[0xC0][POINTER ON STRING]
         if(pAnswer->DNS_Type == DNS_QUEST_TYP_CNAME){

            memset(psDomain, 0, sizeof(psDomain));

            csDomain = 0;
            csName = 0;

            csName = ntohs(pAnswer->DNS_cData);

            for(DWORD n = 0; n < csName; n++){
               UCHAR cPart = pData[n]; 

               if(!cPart) break;   // STRING END
               else 
               if(cPart == 0xC0){  // pData[n + 1] == OFFSET
                  UCHAR* pTemp = pDNS_Packet;
                  // pTemp -> STRING
                  pTemp += pData[n + 1];
                  size_t cTemp = strlen((char*)pTemp);

                  for(DWORD nTemp = 0; nTemp < cTemp; nTemp++){
                     UCHAR __cPart = pTemp[nTemp];

                     memcpy(&psDomain[csDomain], &pTemp[nTemp + 1], __cPart);
                     csDomain += __cPart;

                     // Appand "."
                     psDomain[csDomain++] = 46; 

                     nTemp += __cPart;
                  }

                  // [0xC0][0xOFFSET]
                  n += 2;
               }else{ // pData[n] == SIZEOF(pData[n + 1])

                  memcpy(&psDomain[csDomain], &pData[n + 1], cPart);
                  csDomain += cPart;

                  // Appand "."
                  psDomain[csDomain++] = 46;

                  n += cPart;
               }  
            }

            // Delete "."
            if(csDomain) psDomain[ --csDomain ] = 0x00;

            pData += csName;
         }
      }
   }

   return;
}
