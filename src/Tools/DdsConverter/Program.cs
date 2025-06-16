using System;
using System.Collections.Generic;
using System.Windows.Forms;

namespace DdsConverter
{
    static class Program
    {
        /// <summary>
        /// 程序入口
        /// </summary>
        [STAThread]
        static void Main()
        {
            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);
            Application.Run(new Form1());
        }
    }
}