import socket

listen_socket = socket.socket( socket.AF_INET, socket.SOCK_STREAM )
listen_socket.bind( ('', 9999 ) )
listen_socket.listen( 5 )

while 1:
    accept_socket, (host, port) = listen_socket.accept()
    str = accept_socket.recv(100)
    print("host : " + host + " send : " + str)
    accept_socket.send( str )
accept_socket.close()


