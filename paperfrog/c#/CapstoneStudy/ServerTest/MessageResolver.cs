using System;
using System.Runtime.CompilerServices;
/// <summary>
/// tcp 통신에서 메세지 버퍼 읽어오는 클래스인데 나중에 기회되면 다시 내 손으로 짜볼것.
/// </summary>
public class CMessageResolver
{
    public delegate void CompletedMessageCallback(byte[] buffer);

    private int messageSize;
    byte[] messageBuffer=new byte[1024];
    int curPos;
    int posToRead;
    int remainByte;

    public CMessageResolver()
    {
        messageSize=0;
        curPos=0;
        posToRead=0;
        remainByte=0;
    }

    public void OnReceive(byte[] buffer, int offset, int transffered)
    {
        remainByte=transffered;
        int srcPos=offset;
            bool completed=false;
        while (remainByte > 0)
        {
            if (curPos < Defines.HEADERSIZE)
            {
                posToRead=Defines.HEADERSIZE;
                completed=ReadUntil(buffer, ref srcPos, offset, transffered);
            }

            if (!completed)
            {
                return;
            }

            messageSize=GetBodySize();
            posToRead=messageSize + Defines.HEADERSIZE;
        }
        //헤더 다 읽었으니 메세지 읽는 부분
        completed=ReadUntil(buffer, ref srcPos, offset, transffered);
        if (completed)
        {
            ClearBuffer();
        }
    }

    private int GetBodySize()
    {
        Type type=Defines.HEADERSIZE.GetType();
        if (type.Equals(typeof(Int16)))
            return BitConverter.ToInt16(messageBuffer,0);
        return BitConverter.ToInt32(messageBuffer, 0);
    }

    private void ClearBuffer()
    {
        Array.Clear(messageBuffer,0,messageBuffer.Length);
        curPos=0;
        messageSize=0;
    }

    private bool ReadUntil(byte[] buffer,ref int  srcPos,int offset,int transffered)
    {
        if (curPos >= offset + transffered)
            return false;
        int copySize=posToRead - curPos;
        if (remainByte < copySize)
        {
            copySize=remainByte;
        }
        Array.Copy(buffer,srcPos,messageBuffer,curPos,copySize);
        srcPos+=copySize;
        curPos+=copySize;
        remainByte-=copySize;
        if (curPos < posToRead)
            return false;
        else return true;

    }
}
