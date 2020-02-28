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
        this.CallbackOnNewClientHandler=null;
    }

    void Test(object sender, EventArgs e)
    {
        Console.WriteLine("connect: "+sender+e);
    }

    public void Start(string host, int port, int backlog)
    {
        this.listenSocket=new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
        IPAddress address;
        if (host == "0.0.0.0")
            address=IPAddress.Any;
        else
            address=IPAddress.Parse(host);
        Console.WriteLine("hostIP : " + address);
        IPEndPoint endPoint=new IPEndPoint(address, port);
        try
        {
            this.listenSocket.Bind(endPoint);
            this.listenSocket.Listen(backlog);
            this.acceptArgs          = new SocketAsyncEventArgs();
            this.acceptArgs.Completed+=new EventHandler<SocketAsyncEventArgs>(Test);
            this.acceptArgs.Completed+=new EventHandler<SocketAsyncEventArgs>(OnAcceptCompleted);


            Thread listenThread=new Thread(DoListen);
            listenThread.Start();
        }
        catch (Exception e)
        {
            Console.WriteLine(e.Message);
        }
    }

    void OnAcceptCompleted(object sender, SocketAsyncEventArgs e)
    {
        if (e.SocketError == SocketError.Success)
        {
            Socket clientSocket=e.AcceptSocket;
            this.flowControlEvent.Set();

            if (this.CallbackOnNewClientHandler != null)
            {
                this.CallbackOnNewClientHandler(clientSocket, e.UserToken);
            }
        }
        else
            Console.WriteLine("연결 실패!");

        flowControlEvent.Set();
    }

    void DoListen()
    {
            this.flowControlEvent=new AutoResetEvent(false);
            while (true)
            {
                this.acceptArgs.AcceptSocket=null;
                bool pending=true;
                try
                {
                    pending=listenSocket.AcceptAsync(this.acceptArgs);
                }
                catch (Exception e)
                {
                    Console.WriteLine(e.Message);
                    continue;
                }

                if(!pending)
                {
                    //
                }

                this.flowControlEvent.WaitOne();
            }
    }
    
}
