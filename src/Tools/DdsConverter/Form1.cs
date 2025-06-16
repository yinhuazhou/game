using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using Microsoft.DirectX.Direct3D;

namespace DdsConverter
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
            textBox2.Text = "状态：准备完毕";
        }

        private void listBox1_DragDrop(object sender, DragEventArgs e)
        {
            if (e.Data.GetDataPresent(DataFormats.FileDrop))
            {
                string[] s = (string[])e.Data.GetData(DataFormats.FileDrop);
                foreach (string t in s)
                {
                    listBox1.Items.Add(t);
                }
            }
        }

        private void listBox1_DragOver(object sender, DragEventArgs e)
        {
            if (e.Data.GetDataPresent(DataFormats.FileDrop))
            {
                e.Effect = DragDropEffects.Copy;
            }
        }

        private void textBox1_DragDrop(object sender, DragEventArgs e)
        {
            if (e.Data.GetDataPresent(DataFormats.FileDrop))
            {
                string[] s = (string[])e.Data.GetData(DataFormats.FileDrop);
                textBox1.Text = s[0];
            }

        }

        private void textBox1_DragOver(object sender, DragEventArgs e)
        {
            if (e.Data.GetDataPresent(DataFormats.FileDrop))
            {
                e.Effect = DragDropEffects.Copy;
            }
        }

        unsafe private void button1_Click(object sender, EventArgs e)
        {
            //DirectX初始化
            PresentParameters presentParams = new PresentParameters();
            presentParams.Windowed = true;
            presentParams.SwapEffect = SwapEffect.Discard;
            Device device = new Device(0, DeviceType.Hardware, this, CreateFlags.SoftwareVertexProcessing, presentParams);
            //透明
            Bitmap alphaImage = null;
            System.Drawing.Imaging.BitmapData alphaBitmapData = null;
            int ah = 0;
            int aw = 0;
            if (textBox1.Text != "")
            {
                alphaImage = new Bitmap(textBox1.Text);
                ah = alphaImage.Height;
                aw = alphaImage.Width;
                Rectangle rect = new Rectangle(0, 0, aw, ah);
                alphaBitmapData = new System.Drawing.Imaging.BitmapData();
                alphaImage.LockBits(rect, System.Drawing.Imaging.ImageLockMode.ReadOnly, System.Drawing.Imaging.PixelFormat.Format32bppArgb, alphaBitmapData);
            }
            //逐张处理图片
            foreach (string s in listBox1.Items)
            {
                textBox2.Text = "状态：处理中……";
                //载入图片
                Bitmap image = new Bitmap(s);
                int h = image.Height;
                int w = image.Width;
                Rectangle rect = new Rectangle(0, 0, w, h);
                System.Drawing.Imaging.BitmapData bitmapData = new System.Drawing.Imaging.BitmapData();
                image.LockBits(rect, System.Drawing.Imaging.ImageLockMode.ReadWrite, System.Drawing.Imaging.PixelFormat.Format32bppArgb, bitmapData);
                uint* src;
                uint* dst;
                if ((alphaImage != null) && (ah == h) && (aw == w))
                {
                    //透明通道转换
                    src = (uint*)alphaBitmapData.Scan0;
                    dst = (uint*)bitmapData.Scan0;
                    for (uint y = 0; y < h; ++y)
                    {
                        for (uint x = 0; x < w; ++x)
                        {
                            dst[x] = (dst[x] & 0xffffff) | ((src[x] & 0xff) << 24);
                        }
                        src += alphaBitmapData.Stride / 4;
                        dst += bitmapData.Stride / 4;
                    }
                }
                //纹理化
                Texture t = new Texture(device, w, h, 1, Usage.None, Format.A8R8G8B8, Pool.SystemMemory);
                Microsoft.DirectX.GraphicsStream gs = t.LockRectangle(0, LockFlags.None);
                dst = (uint*)bitmapData.Scan0;
                for (uint y = 0; y < h; ++y)
                {
                    for (uint x = 0; x < w; ++x)
                    {
                        gs.Write(dst[x]);

                    }
                    dst += bitmapData.Stride / 4;
                }
                t.UnlockRectangle(0);
                image.UnlockBits(bitmapData);
                string dstFilename = System.IO.Path.GetDirectoryName(s);
                dstFilename += "¥¥";
                dstFilename += System.IO.Path.GetFileNameWithoutExtension(s);
                dstFilename += ".dds";
                TextureLoader.Save(dstFilename, ImageFileFormat.Dds, t);
                //析构
                image.Dispose();
                image = null;
                if (alphaImage != null)
                {
                    alphaImage.Dispose();
                    alphaImage = null;
                }
                bitmapData = null;
                alphaBitmapData = null;
                t.Dispose();
                t = null;
                gs.Close();
                gs = null;
            }
            textBox2.Text = "状态：准备完毕";
        }

        private void textBox2_TextChanged(object sender, EventArgs e)
        {
            Application.DoEvents();
        }

        private void button2_Click(object sender, EventArgs e)
        {
            listBox1.Items.Clear();
            textBox1.Text = "";
        }
    }
}

