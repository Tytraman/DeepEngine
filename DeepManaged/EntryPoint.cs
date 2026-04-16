using System;
using System.Runtime.InteropServices;

namespace DeepManaged
{
    public static class EntryPoint
    {
        [UnmanagedCallersOnly]
        public static int Initialize()
        {
            Console.Write("DeepManaged initialization...");

            Console.WriteLine(" OK");

            return 0;
        }

        [UnmanagedCallersOnly]
        public static void Shutdown()
        {
            Console.Write("DeepManaged shutdown...");

            Console.WriteLine(" OK");
        }
    }
}
