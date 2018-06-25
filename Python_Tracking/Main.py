import socket
import cam

H, V = 320, 240 # Set Cam half max size
captured = False

#Init socket
IP = '0.0.0.0'
PORT = 8080
print("Server is start!")
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.bind((IP, PORT))
s.listen(1)

while 1:
    print("Wait for connecting")
    conn, addr = s.accept() # catch TCP connection
    print('Connection address:', addr)

    while 1:
        data = conn.recv(1024).decode()
        if (data == '1'):
            H, V, captured = cam.capture(H, V)
            Send = '{},{},{}'.format(H, V, captured)
            print("Send data:", Send)
            conn.send(str(Send).encode())  # Send Coordnate data
        elif (data == '0'):
            print("Session closed")
            conn.close()
            break