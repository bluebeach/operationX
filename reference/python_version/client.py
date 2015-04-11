import socket

sock = socket.socket( socket.AF_INET, socket.SOCK_STREAM )

sock.connect( ('',9999 ) )
while 1:
    input = raw_input("send : ")
    if input is 'quit':
        break
    sock.send(input)
    print sock.recv(100)
sock.close()
