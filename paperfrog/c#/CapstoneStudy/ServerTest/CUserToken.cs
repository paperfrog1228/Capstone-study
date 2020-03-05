using System;
using System.Net.Sockets;
using System.Xml.Schema;

public class CUserToken
{
    private object m_UserToken;
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

    public State CurState=State.Idle;
    public void OnReceive(byte[] buffer, int offset, int transfer)
    {
        this.messageResolver.OnReceive(buffer,offset,transfer);
    }

    public void OnConnect()
    {
        CurState=State.Connected;
    }

    public void SetEventArgs(SocketAsyncEventArgs receiveEventArgs, SocketAsyncEventArgs sendEventArgs)
    {
        byte[] buffer=new byte[1024];
        ReceiveEventArgs=receiveEventArgs;
        receiveEventArgs.SetBuffer(buffer,0,buffer.Length);
        SendEventArgs=sendEventArgs;
    }
}
