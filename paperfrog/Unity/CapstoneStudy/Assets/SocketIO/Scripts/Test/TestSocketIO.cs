#region License
/*
 * TestSocketIO.cs
 *
 * The MIT License
 *
 * Copyright (c) 2014 Fabio Panettieri
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */
#endregion

using System;
using System.Collections;
using UnityEngine;
using SocketIO;
using Random = UnityEngine.Random;

public class TestSocketIO : MonoBehaviour
{
	private SocketIOComponent socket;

	private void Start()
	{
		GameObject go= GameObject.Find("SocketIO");
		socket=go.GetComponent<SocketIOComponent>();
		socket.On("matching",Matching);
		socket.On("NoticeConnected",NoticeConnected);
		socket.On("FoundUser",FoundUser);
	}

	public void Connect()
	{
		socket.Connect();
	}
	
	UserInfo userInfo=new UserInfo();

	public void SendUserInfo()
	{	
		userInfo.userId=LobbyManager.Instance().GetUserId();
		userInfo.elo=Random.Range(0, 100);
			//Unity <-> node.js 통신하려면 데이터 타입이 무조건 JSONObejct여야 하는 듯. 코드 개같다. 정말로.
		socket.Emit("UpdateUserInfo",	new JSONObject(JsonUtility.ToJson(userInfo)));
	}
	
	private void NoticeConnected(SocketIOEvent e)
	{
		Debug.Log("node js server connected success!"+e.name);
		SendUserInfo();
	}

	private void FoundUser(SocketIOEvent e)
	{
		LobbyManager.Instance().OpenMatchingPanel(e.data);
	}


	private void Matching(SocketIOEvent e)
	{
		Debug.Log("[SocketIO] Open received: " + e.name + " " + e.data);
	}
}
