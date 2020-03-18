using System;
using System.Collections.Generic;
using System.Linq;
using System.Net.Sockets;
using Sirenix.OdinInspector;
using UnityEngine;
public class CUserToken
{
    public SocketAsyncEventArgs ReceiveEventArgs;
    public SocketAsyncEventArgs SendEventArgs;
    
    public Socket Socket { set; get; }
    
    private CMessageResolver messageResolver=new CMessageResolver();
    public enum State
    {
        Idle,
        Connected,
        // 종료가 예약됨.
        // sending_list에 대기중인 상태에서 disconnect를 호출한 경우,
        // 남아있는 패킷을 모두 보낸 뒤 끊도록 하기 위한 상태값.
        ReserveClosing,
        // 소켓이 완전히 종료됨.
        Closed,
    }

    [ShowInInspector]private List<ArraySegment<byte>> _sendingList=new List<ArraySegment<byte>>();
    private object _csSendingQueue=new object();
        
    
    public State CurState=State.Idle;
    public void OnReceive(byte[] buffer, int offset, int transfer)
    {
        messageResolver.OnReceive(buffer,offset,transfer);
    }

    private void CheckSend(ArraySegment<byte> data)
    {
        lock (_csSendingQueue)
        {
            _sendingList.Add(data);
           /* if (_sendingList.Count > 1)
            { 
                // 큐에 무언가가 들어 있다면 아직 이전 전송이 완료되지 않은 상태이므로 큐에 추가만 하고 리턴한다.
                // 현재 수행중인 SendAsync가 완료된 이후에 큐를 검사하여 데이터가 있으면 SendAsync를 호출하여 전송해줄 것이다.
                    return;
            }*/
        }
        StartSend();
    }

    private void StartSend()
    { 
        try
        {
            SendEventArgs.BufferList=_sendingList;
            Debug.Log("ttt"+_sendingList.Count);
        
            if (SendEventArgs.BufferList == null)
            {
                Debug.Log("aaa");
            }

            if (Socket == null)
            {
                Debug.Log("ccc");
            }

            bool pending = Socket.SendAsync(SendEventArgs);
            if (!pending)
            { 
                Debug.Log("bbb");
                ProcessSend(SendEventArgs);
            }
        }
        catch (Exception e)
        {
            Debug.Log("왜 에러남 왜");
            Debug.Log(e.Message);
            throw new Exception(e.Message, e);
        }
        _sendingList.Clear();
    }
    private void ProcessSend(SocketAsyncEventArgs e)
    { 
        if (e.BytesTransferred <= 0 || e.SocketError != SocketError.Success)
        { 
            Debug.Log("메세지 보냄 실패!");
            return;
        }
        var size = _sendingList.Sum(obj => obj.Count);
        // 전송이 완료되기 전에 추가 전송 요청을 했다면 sending_list에 무언가 더 들어있을 것이다.
        if (e.BytesTransferred != size)
        { 
            if (e.BytesTransferred < _sendingList[0].Count)
            { 
                string error = string.Format("Need to send more! transferred {0},  packet size {1}", e.BytesTransferred, size);
                Debug.Log(error);

                //close();
                        return;
            }

            // 보낸 만큼 빼고 나머지 대기중인 데이터들을 한방에 보내버린다.
            int sentIndex = 0;
            int sum = 0;
            for (int i = 0; i < _sendingList.Count; ++i)
            {
                sum += _sendingList[i].Count;
                if (sum <= e.BytesTransferred)
                {
                    sentIndex = i;
                    continue;
                }
                break;
            }
            _sendingList.RemoveRange(0, sentIndex + 1);
            StartSend();
            return;
        }
        _sendingList.Clear();
        if (CurState == State.ReserveClosing)
        {
            Socket.Shutdown(SocketShutdown.Send);
        }
    }

    #region PUBLIC 
    public void SetEventArgs(SocketAsyncEventArgs receiveEventArgs, SocketAsyncEventArgs sendEventArgs)
    {
        ReceiveEventArgs=receiveEventArgs;
        SendEventArgs=sendEventArgs;

    }

    public void SetSocket(Socket socket)
    {
        Socket=socket;
    }

    public void OnConnect()
    {
        CurState=State.Connected;
    }
    public void Send(CPacket msg)
    {
        msg.record_size();
        CheckSend(new ArraySegment<byte>(msg.buffer, 0, msg.position));
    }
    #endregion
}
