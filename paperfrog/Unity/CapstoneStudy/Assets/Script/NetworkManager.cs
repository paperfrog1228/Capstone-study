using System;
using System.Collections;
using System.Collections.Generic;
using System.Net;
using System.Net.Sockets;
using UnityEngine;
using Sirenix;
using Sirenix.OdinInspector;
using UnityEngine.Experimental.PlayerLoop;

public class NetworkManager : MonoBehaviour
{
    [BoxGroup("Setting")][SerializeField]private string _ipAddress;
    [BoxGroup("Setting")][SerializeField]private short _port;
    [ShowInInspector]public  Queue<CPacket> SendMessageQueue=new Queue<CPacket>();
    private Socket _socket;
    private byte[] _positionBuffer;
    private CUserToken _mainToken;
    public void Connect()
    {
        _socket=new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
        var ep=new IPEndPoint(IPAddress.Parse(_ipAddress),_port);
        CConnector connector=new CConnector();
        connector.Connect(ep);
    }

    private GameObject Test;
    public void SendPos(Vector3 pos)
    {
        CPacket packet= CPacket.create(1);
        packet.Push(pos);
        SendMessageQueue.Enqueue(packet);
    }

    public void ReceivePos(Vector3 Pos)
    {
        Test.transform.position=Pos;
    }


    #region mono 

    private void Awake()
    {
        _networkManager=this;
    }
    
    

 private static NetworkManager _networkManager;

 public static NetworkManager Instance()
 {
     return _networkManager;
 }

 #endregion
}
