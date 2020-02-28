public class CUserToken
{
    private object m_UserToken;
    private CMessageResolver messageResolver=new CMessageResolver();
    public void OnReceive(byte[] buffer, int offset, int transfer)
    {
        this.messageResolver.OnReceive(buffer,offset,transfer);
    }
}
