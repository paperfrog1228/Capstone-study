using System;
using System.Diagnostics;
using System.Globalization;
using System.Net.Sockets;

public class CNetworkService
{
    private CListener listener;
    SocketAsyncEventArgsPool receiveEventArgsPool=new SocketAsyncEventArgsPool(100);
    SocketAsyncEventArgsPool sendEventArgsPool=new SocketAsyncEventArgsPool(100);
    private CUserToken userToken;
    private SocketAsyncEventArgs args;
    private BufferManager bufferManager;
    public CNetworkService()
    {
        for (int i=0; i < 100; i++)
        {
            userToken=new CUserToken();
            args=new SocketAsyncEventArgs();
            args.Completed+=ReceiveComplete;
            args.UserToken=userToken;
            receiveEventArgsPool.Push(args);
            bufferManager.SetBuffer(args);
            userToken=new CUserToken();
            
            
            args=new SocketAsyncEventArgs();
//            args.Completed+=new EventHandler<SocketAsyncEventArgs>();
            bufferManager.SetBuffer(args);
            args.UserToken=userToken;
            sendEventArgsPool.Push(args);
        }
    }

    void onNewClient(Socket clientSocket, object token)
    {
        SocketAsyncEventArgs receiverArgs=receiveEventArgsPool.Pop();
        SocketAsyncEventArgs sendArgs=sendEventArgsPool.Pop();
        //if()session created callback 이런거 안만들어줬다고 시밤
        {
            CUserToken userToken=receiverArgs.UserToken as CUserToken;
        }
        BeginReceive(clientSocket, receiverArgs, sendArgs);
    }

    void BeginReceive(Socket socket, SocketAsyncEventArgs receiveArgs, SocketAsyncEventArgs sendArgs)
    {
        CUserToken userToken=receiveArgs.UserToken as CUserToken;

        bool pending=socket.ReceiveAsync(receiveArgs);
        if (!pending)
        {
            ProcessReceive(receiveArgs);
        }
    }

    void ReceiveComplete(object sender, SocketAsyncEventArgs e)
    {
        if (e.LastOperation == SocketAsyncOperation.Receive)
        {
            ProcessReceive(e);
            return;
        }
    }

    private void ProcessReceive(SocketAsyncEventArgs e)
    {
        CUserToken userToken=e.UserToken as CUserToken;
        if (e.BytesTransferred > 0 && e.SocketError == SocketError.Success)
        {
            
        }
    }
}

