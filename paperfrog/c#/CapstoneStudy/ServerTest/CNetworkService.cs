using System;
using System.Diagnostics;
using System.Globalization;
using System.Net;
using System.Net.Sockets;

public class CNetworkService
{
    public CListener listener=new CListener();
    SocketAsyncEventArgsPool receiveEventArgsPool=new SocketAsyncEventArgsPool(100);
    SocketAsyncEventArgsPool sendEventArgsPool=new SocketAsyncEventArgsPool(100);
    private CUserToken userToken;
    private SocketAsyncEventArgs args;
    private BufferManager bufferManager=new BufferManager(10,2);

    public delegate void SessionHandler(CUserToken token);

    public SessionHandler sessionCreateCallback { get; set; }

    public CNetworkService()
    {
    }

    public void Init()
    {   
        listener =new CListener();

        listener.CallbackOnNewClientHandler+=OnNewClient;
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
            bufferManager.SetBuffer(args);
            args.UserToken=userToken;
            sendEventArgsPool.Push(args);
        }
    }

    void Check(CUserToken token)
    {
        Console.WriteLine("이 "+token+"은 수신이 가능합니다");
    }

    void OnNewClient(Socket clientSocket, object token)
    {
        SocketAsyncEventArgs receiveArgs=receiveEventArgsPool.Pop();
        SocketAsyncEventArgs sendArgs=sendEventArgsPool.Pop();
        CUserToken userToken=new CUserToken();
        receiveArgs.UserToken=userToken;
        sendArgs.UserToken=userToken;
        sessionCreateCallback+=Check;
        
        if(sessionCreateCallback!=null)
        {
            sessionCreateCallback(this.userToken);
        }
        BeginReceive(clientSocket, receiveArgs,sendArgs);
    }

    void BeginReceive(Socket socket, SocketAsyncEventArgs receiveArgs,SocketAsyncEventArgs sendArgs)
    {
        CUserToken userToken=receiveArgs.UserToken as CUserToken;
        userToken.Socket=socket;
        userToken.SetEventArgs(receiveArgs,sendArgs);
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
        throw new ArgumentException("The last operation completed on the socket was not a receive.");
    }

    private void ProcessReceive(SocketAsyncEventArgs e)
    {
        Console.WriteLine("processRecive 발동");
        CUserToken userToken=e.UserToken as CUserToken;
        if (e.BytesTransferred > 0 && e.SocketError == SocketError.Success)
        {
            userToken.OnReceive(e.Buffer, e.Offset, e.BytesTransferred);
            bool pending = userToken.Socket.ReceiveAsync(e);
            if (!pending)
            {
                    ProcessReceive(e);
            }
            }
            else
            {
                try
                {
                }
                catch (Exception)
                {
                    Console.WriteLine("Already closed this socket.");
                }
            }
		}
        }

