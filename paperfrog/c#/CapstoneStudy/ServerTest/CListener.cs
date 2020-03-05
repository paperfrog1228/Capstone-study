using System;
using System.Net;
using System.Net.Sockets;
using System.Threading;
//이 클래스는 클라이언트의 TCP 접속을 받기 위한 리스너이다.
public class CListener
{
    private SocketAsyncEventArgs acceptArgs;
    private Socket listenSocket;
    private AutoResetEvent flowControlEvent;

    public delegate void NewClientHandler(Socket clientSocket, object token);
    public NewClientHandler CallbackOnNewClientHandler;

    public CListener()
    {
        CallbackOnNewClientHandler=null;
    }
    public void Start(string host, int port, int backlog)
    {
        listenSocket=new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
        IPAddress address;
        if (host == "0.0.0.0")
            address=IPAddress.Any;
        else
            address=IPAddress.Parse(host);
        Console.WriteLine("hostIP : " + address);
        IPEndPoint endPoint=new IPEndPoint(address, port);
        try
        {
            listenSocket.Bind(endPoint);
            listenSocket.Listen(backlog);
            acceptArgs          = new SocketAsyncEventArgs();

            Thread listenThread=new Thread(DoListen);
            listenThread.Start();
        }
        catch (Exception e)
        {
            Console.WriteLine(e.Message);
        }
    }
    void DoListen()
    {
        flowControlEvent=new AutoResetEvent(false);
        
        while (true)
        { 
            
            acceptArgs          = new SocketAsyncEventArgs();
            acceptArgs.Completed+=OnAcceptCompleted;
                acceptArgs.AcceptSocket=null;
                bool pending=true;
                try
                {
                    pending=listenSocket.AcceptAsync(acceptArgs);
                }
                catch (Exception e)
                {
                    Console.WriteLine(e.Message);
                    //continue;
                }

                if(!pending)
                {
                    OnAcceptCompleted(null,acceptArgs);
                }

                this.flowControlEvent.WaitOne();
        }
    }
    
    void OnAcceptCompleted(object sender, SocketAsyncEventArgs e)
    {
        if (e.SocketError == SocketError.Success)
        {
            Console.WriteLine("연결 성공!");
            Socket clientSocket=e.AcceptSocket;
            this.flowControlEvent.Set();
        
            Console.WriteLine("a"+e.UserToken);
            if (CallbackOnNewClientHandler != null)
            {
               CallbackOnNewClientHandler(clientSocket, e.UserToken);
            }
        }
        else
            Console.WriteLine("연결 실패!");

        flowControlEvent.Set();
    }
}
