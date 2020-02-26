namespace ServerTest
{
    internal class Program
    {
        public static void Main(string[] args)
        {
            CListener cListener=new CListener();
            cListener.Start("127.0.0.1",999,1);
            while (true)
            {
            }
        }
    }
}