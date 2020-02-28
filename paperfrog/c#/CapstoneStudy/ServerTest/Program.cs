    using System.Collections.Generic;

    internal class Program
    {
        public static void Main(string[] args)
        {
            List<CGameUser> userlist=new List<CGameUser>();
            CNetworkService networkService=new CNetworkService();
            networkService.Start("0.0.0.0",999,1);
            while (true)
            {
            }
            
        }
    }
