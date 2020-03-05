using System;
using System.Collections;
using System.Collections.Generic;
using System.Net;
using System.Net.Sockets;
using System.Text;
using SocketIO;
using UnityEngine;


public class TcpSocketManager : MonoBehaviour
{
    public TcpClient tcp;
    public string clientName;
    private Socket socket;
    private byte[] sendBuff;
    void Start()
    {
        socket=new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
        var ep=new IPEndPoint(IPAddress.Parse("127.0.0.1"),999);
        socket.Connect(ep);
        sendBuff=new byte[8192];
    }

    private AsyncCallback cb;
    public void Send()
    {
        sendBuff=Encoding.Unicode.GetBytes(clientName);
        
        Debug.Log("send 누름"+sendBuff);
        socket.BeginSend(sendBuff,0,sendBuff.Length,SocketFlags.None,cb,sendBuff);
    }

    void Update()
    {
        
 }
}
