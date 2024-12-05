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


def command_esp(port):
    # Mengatur nilai IP dan Port Server
    ip = get_ip()
    serverSock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

    # mengatur IP dan Port sebagai server
    serverSock.bind((ip, port))
    print("IP: ", ip, port)

    # Menunggu data kirim dari client
    print("Wait for client...")
    data, addr = serverSock.recvfrom(1024)
    print(data, addr)
    text = data.decode(encoding="utf_8")

    command_data = ""
    if text == "0":
        command_data = "1"
    elif text == "1":
        command_data = "0"
    else:
        pass
    
    if data:
        _send = command_data.encode()
        serverSock.sendto(_send, addr)

    serverSock.close()


command_esp(9999)
