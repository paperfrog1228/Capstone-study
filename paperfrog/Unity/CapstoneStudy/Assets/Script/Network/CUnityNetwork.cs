using System;
using System.Collections;
using System.Collections.Generic;
using System.Net.Sockets;
using System.Runtime.CompilerServices;
using UnityEngine;
using UnityEngine.UI;

public class CUnityNetwork : MonoBehaviour
{
	private CUserToken mainToken;
	public void OnConnectCompleted(Socket socket, CUserToken token)
	{ 
		token.SetSocket(socket);
		SocketAsyncEventArgs receiveEventArg = new SocketAsyncEventArgs();
		receiveEventArg.UserToken = token;
		receiveEventArg.SetBuffer(new byte[1024], 0, 1024);
		mainToken=token;
		SocketAsyncEventArgs sendEventArg = new SocketAsyncEventArgs();
		//sendEventArg.Completed += new EventHandler<SocketAsyncEventArgs>(send_completed);
		sendEventArg.UserToken = token;
		sendEventArg.SetBuffer(null, 0, 0);
		token.SetEventArgs(receiveEventArg,sendEventArg);
		//begin_receive(socket, receive_event_arg, send_event_arg);
	}
	public void Send(CUserToken token, CPacket msg)
	{
		if (token == null)
		{
			return;
		}
		try
		{
			token.Send(msg);
		}
		catch (Exception e)
		{
			Debug.Log(e.Message);
		}
	}

	private void Awake()
	{
		_cUnityNetwork=this;
	}

	private void Update()
	{
		if (NetworkManager.Instance().SendMessageQueue.Count > 0)
		{
			CPacket temp=NetworkManager.Instance().SendMessageQueue.Dequeue();
			Send(mainToken,temp);
		}
	}

	private static CUnityNetwork _cUnityNetwork;

	public static CUnityNetwork Instance()
	{
		return _cUnityNetwork;
	}
}
