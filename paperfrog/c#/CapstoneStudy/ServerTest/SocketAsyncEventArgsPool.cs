    using System;
    using System.Collections;
    using System.Collections.Generic;
    using System.Net.Sockets;

    public class SocketAsyncEventArgsPool
    {
        private Stack<SocketAsyncEventArgs> pool;

        public SocketAsyncEventArgsPool(int maxCapacity)
        {
            pool=new Stack<SocketAsyncEventArgs>();
        }

        public void Push(SocketAsyncEventArgs item)
        {
            if (item == null)
            {
                throw new ArgumentNullException("SocketAsynceEventArgs is null!");
            }

            lock (pool)
            {
                pool.Push(item);
            }
        }

        public SocketAsyncEventArgs Pop()
        {
            lock (pool)
            {
               return pool.Pop();
            }
        }

        public int Count
        {
            get { return pool.Count; }
        }
    }
