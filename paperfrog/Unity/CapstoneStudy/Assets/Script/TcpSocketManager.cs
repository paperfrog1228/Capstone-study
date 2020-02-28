using System.Collections;
using System.Collections.Generic;
using System.Net;
using System.Net.Sockets;
using UnityEngine;


public class TcpSocketManager : MonoBehaviour
{
    public TcpClient tcp;
    public string clientName;
    private Socket socket;
    private byte[] receiverBuff;
    void Start()
    {
        socket=new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
        var ep=new IPEndPoint(IPAddress.Parse("127.0.0.1"),999);
        socket.Connect(ep);
        receiverBuff=new byte[8192];
    }

    // Update is called once per frame
    void Update()
    {
        
 }
}
