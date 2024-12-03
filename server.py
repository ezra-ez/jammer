# Library
import socket
from time import sleep

# Fungsi mendapatkan IP Address Laptop
def get_ip():
    s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    s.connect(("8.8.8.8", 80))
    _ip = s.getsockname()[0]
    s.close()
    return _ip

# Mengatur nilai IP dan Port Server
ip, port = get_ip(), 9999
serverSock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

# mengatur IP dan Port sebagai server
serverSock.bind((ip, port))
print("IP: ", ip, port)


while True:
    # Menunggu data kirim dari client
    print("Nunggu client...")
    data, addr = serverSock.recvfrom(1024)
    print("Message: ", data, addr)
