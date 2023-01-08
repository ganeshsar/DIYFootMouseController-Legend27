using System;
using System.IO.Ports;
using System.Runtime.ConstrainedExecution;
using System.Runtime.InteropServices;

namespace SerialCommunicator// This is a workaround to support ardurino Mega 
{
    internal class Program
    {
        // ***You must build MouseLib first
        public const string MouseLibDLL = @"..\..\..\..\x64\Debug\MouseLib.dll";
        [DllImport(MouseLibDLL, CallingConvention = CallingConvention.Cdecl)]
        public static extern int MouseDown(int button);

        [DllImport(MouseLibDLL, CallingConvention = CallingConvention.Cdecl)]
        public static extern int MouseUp(int button);

        static void Main(string[] args)
        {
            Console.WriteLine("Serial Communicator!");
            
            SerialPort serialPort = new SerialPort();
            serialPort.PortName = "COM3";
            serialPort.BaudRate = 9600;
            serialPort.Open();
            serialPort.DataReceived += SerialPort_DataReceived;
            while (true)
            {
                // Console.WriteLine(serialPort.ReadExisting());
                // MouseDown(0);
                Thread.Sleep(2000);
            }
        }

        private static void SerialPort_DataReceived(object sender, SerialDataReceivedEventArgs e)
        {
            // this method is not exactly reliable, but I'm running out of time
            SerialPort serialPort = (SerialPort)sender;
            string s = serialPort.ReadExisting();
            // Console.WriteLine(s);
            char[] buffer = new char[2];
            int i = 0;
            foreach(char c in s)
            {
                if (c == ';')
                {
                    ProcessBuffer(buffer);
                }
                if (c == 'l' || c == 'r' || c == '1' || c == '0')
                {
                    buffer[i] = c;
                    i = (i + 1) % 2;
                }
            }
            
            
        }
        private static void ProcessBuffer(char[] buffer)
        {
            
            // Assume the correct buffer size
            char a = buffer[0];
            char b = buffer[1];

            
            if (a == 'l')
            {
                if (b == '0')
                {
                    Console.Write("ldown");
                    MouseDown(1);
                }
                else if (b == '1')
                {
                    Console.Write("lup");
                    MouseUp(1);
                }
            }
            


            if (a == 'r')
            {
                if (b == '0')
                {
                    Console.Write("rdown");
                    Console.Write(MouseDown(0));
                }
                else if (b == '1')
                {
                    Console.Write("rup");
                    MouseUp(0);
                }
            }
        }
    }
}