    using System.Collections.Generic;

    internal class Program
    {
        public static void Main(string[] args)
        {
            List<CGameUser> userlist=new List<CGameUser>();
            CNetworkService networkService=new CNetworkService();
            networkService.Init();
            networkService.listener.Start("0.0.0.0",999,1);
        }
    }
