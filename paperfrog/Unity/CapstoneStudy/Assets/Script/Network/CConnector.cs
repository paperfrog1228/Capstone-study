using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Net;
using System.Net.Sockets;
using UnityEngine;
/// <summary>
/// 서버로의 연결, 토큰 받아오기
/// </summary>
public class CConnector
{
	public delegate void ConnectedHandler(CUserToken token);
	public ConnectedHandler connected_callback { get; set; }
	private Socket _clientSocket;
	
	public void Connect(IPEndPoint remote_endpoint)
	{
		Debug.Log("서버 접속 중 ");
		_clientSocket = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
		_clientSocket.NoDelay = true;
		SocketAsyncEventArgs eventArg = new SocketAsyncEventArgs();
		eventArg.Completed += OnConnectCompleted; 
		eventArg.RemoteEndPoint = remote_endpoint;
		bool pending = _clientSocket.ConnectAsync(eventArg);
		if (!pending)
		{
			Debug.Log("pending");
			OnConnectCompleted(null, eventArg);
		}
	}
	private void OnConnectCompleted(object sender, SocketAsyncEventArgs e)
	{
		Debug.Log("서버에 연결 되었습니다!");
		if (e.SocketError == SocketError.Success)
		{ 
			CUserToken token = new CUserToken();
			
			CUnityNetwork.Instance().OnConnectCompleted(_clientSocket,token);

				if (connected_callback != null)
				{
					connected_callback(token);
				}
		}
			else
			{
				Debug.Log(e.SocketError);
			}
		} }

