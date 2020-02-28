using System.Collections.Generic;
using System.Net.Sockets;

internal class BufferManager
{ 
    private int numBytes;
    private byte[] buffer;
    private Stack<int> freeIndexPool;
    private int currentIndex;
    private int bufferSize;

    public BufferManager(int totalBytes, int argBufferSize)
    {
        numBytes=totalBytes;
        currentIndex=0;
        this.bufferSize=argBufferSize;
        freeIndexPool=new Stack<int>();
    }

    public void InitBuffer()
    {
        buffer=new byte[numBytes];
    }

    public bool SetBuffer(SocketAsyncEventArgs args)
    {
        if (freeIndexPool.Count > 0)
        {
            args.SetBuffer(buffer,freeIndexPool.Pop(),bufferSize);
        }
        else
        {
            if (numBytes - bufferSize < currentIndex)
            {
                return false;
            }
            args.SetBuffer(buffer,currentIndex,bufferSize);
            currentIndex+=bufferSize;
        }

        return true;
    }

    public void FreeBuffer(SocketAsyncEventArgs args)
    {
        freeIndexPool.Push(args.Offset);
        args.SetBuffer(null,0,0);
    }
}
