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
        sendBuff=new byte[8192];
    }

    private AsyncCallback cb;
    public void Send()
    {
        sendBuff=Encoding.Unicode.GetBytes(clientName);
        
        Debug.Log("send 누름"+sendBuff);
    }

}
