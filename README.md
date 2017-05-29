# DNS-Proxy

1. compile:
g++ *.cpp -std=c++11 -o DNS-Proxy `pkg-config gtkmm-3.0 --cflags --libs`

2. set right to open udp port 53 as non root:
setcap 'cap_net_bind_service=+ep' ./DNS-Proxy

Browser ---> DNS-Proxy(whitelist | Blacklist) ---> DNS-Server

![alt text](https://github.com/bitspalter/DNS-Proxy/DNS-Proxy.png)
