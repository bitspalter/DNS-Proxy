# DNS-Proxy (alpha)

1. compile:
g++ *.cpp -std=c++11 -o DNS-Proxy -no-pie -Wall \`pkg-config gtkmm-3.0 --cflags --libs\`

2. set right to open udp port 53 as non root:
setcap 'cap_net_bind_service=+ep' ./DNS-Proxy

Options:
 1. -h help
 2. -d dns server
 3. -a autostart
 4. -l list (white|black)
 5. -i interface
 6. -b blacklist (path)
 7. -w whitelist (path)

Example: ./dns-proxy -i eth0 -d 192.168.0.1 -l black -b /home/foo/blacklist.txt -a
    
Browser <---> DNS-Proxy(whitelist | Blacklist) <---> DNS-Server

![alt tag](https://github.com/bitspalter/DNS-Proxy/blob/master/DNS-Proxy.png "DNS-Proxy")
