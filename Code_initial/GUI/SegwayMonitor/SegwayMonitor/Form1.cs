using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.IO.Ports;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace SegwayMonitor
{
    public partial class Form1 : Form
    {
        Graphics g = null;
        byte[] rx_buffer= new byte[256];
        int rx_ptr = 0;

        public Form1()
        {
            InitializeComponent();
        }


        private void send_trackbar_angle()
        {
            byte[] angle = System.BitConverter.GetBytes((float)(((float)trackBarAngle.Value) * 0.0174533));
            //serialPort1.Write("<t" + angle.ToString());
            Console.WriteLine("<t" + String.Join("", angle) + "\n");
            if (serialPort1.IsOpen)
            {
                //serialPort1.Write("<t" + String.Join("", angle) + "\n");
                serialPort1.Write("<t");
                serialPort1.Write(angle, 0, 4);
                serialPort1.Write("\n");
            }
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            g = pgraph.CreateGraphics();


            labelMinAngle.Text = Convert.ToString(trackBarAngle.Minimum);
            labelMiddleAngle.Text = Convert.ToString((trackBarAngle.Maximum + trackBarAngle.Minimum)/2);
            labelMaxAngle.Text = Convert.ToString(trackBarAngle.Maximum);

            

            string[] ports = SerialPort.GetPortNames();

            cbSerialPorts.Items.AddRange(ports);
         
            if (ports.Length != 0)
            {
                cbSerialPorts.SelectedIndex = 0;
            }
            this.ActiveControl = btConnect;
        }

        private void cbSerialPorts_SelectedIndexChanged(object sender, EventArgs e)
        {

        }

        private void btConnect_Click(object sender, EventArgs e)
        {
            serialPort1.BaudRate = 921600;
            if(cbSerialPorts.SelectedItem != null)
            {
                serialPort1.PortName = cbSerialPorts.SelectedItem.ToString();

                try
                {
                    serialPort1.Open();
                    serialPort1.DiscardInBuffer();
                    btConnect.Enabled = false;
                    btReset.Enabled = true;
                    btStart.Enabled = true;
                    btStop.Enabled = true;
                    //serialPort1.Write("s");
                    serialPort1.Write("<cs___\n");
                    send_trackbar_angle();

                }
                catch (System.UnauthorizedAccessException)
                {
                    MessageBox.Show("Can't open COM port! (already in use?)");
                    //throw;
                }
            }
            else
            {
                MessageBox.Show("Couldn't find any availible COM port", "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
            


        }

        private void serialPort1_DataReceived(object sender, SerialDataReceivedEventArgs e)
        {

            int nb_byte_to_read = serialPort1.BytesToRead;
            int byte_read = 0;

            while(byte_read < nb_byte_to_read)
            {
                try{
                rx_buffer[rx_ptr++] = (byte)serialPort1.ReadByte();
                ++byte_read;

                }catch(IndexOutOfRangeException)
                {

                }

                if(rx_ptr > 6)
                {
                    if(rx_buffer[0] == '<' && rx_buffer[6] == '\n')
                    {
                        byte[] message = rx_buffer.Take(6).ToArray();
                        handleMessage(message);
                        rx_ptr = 0;
                    }
                }
            }
            
          
        }

        private void handleMessage(byte[] tab_b)
        {

            Action action = null;
            string tag = System.Text.Encoding.ASCII.GetString(tab_b, 0, 2);
            Console.WriteLine("tag : " + tag);
            if (tag == "<f")
            {

                //string str = serialPort1.read;

                Console.WriteLine(string.Join(string.Empty, Array.ConvertAll(tab_b, b => b.ToString("X2"))));
                float myFloat = System.BitConverter.ToSingle(tab_b, 2); // angle in rad
                action = () => tbPotValueRad.Text = (myFloat.ToString()) + " rad";
                tbController.Invoke(action);

                action = () => tbPotValueDeg.Text = (myFloat* 57.2958f).ToString() + " °";
                tbController.Invoke(action);

                //g.DrawEllipse(Pens.AliceBlue, new Rectangle(new Point(50, 50), new Size(50, 20)));
                g.FillRectangle(new SolidBrush(Color.FromArgb(0, Color.Black)), pgraph.DisplayRectangle);

                Point[] points = new Point[4];

                points[0] = new Point(0, 0);
                points[1] = new Point(0, pgraph.Height);
                points[2] = new Point(pgraph.Width, pgraph.Height);
                points[3] = new Point(pgraph.Width, 0);

                Brush brush = new SolidBrush(Color.White);

                g.FillPolygon(brush, points);

                Brush b2 = new SolidBrush(Color.Black);

                g.DrawEllipse(Pens.Blue, 0, 0, 200, 200);

                g.DrawLine(new Pen(Color.Blue,2.56f), new Point(100, 100), new Point(100 - (int)(100 * Math.Cos(myFloat+Math.PI/2)), 100-(int)(100 * Math.Sin(myFloat + Math.PI / 2))));

      
            }
            else if (tag == "<l")
            {
                int lat = System.BitConverter.ToInt32(tab_b, 2);
                action = () => tbLatency.Text = (lat.ToString() + " µs");
                tbController.Invoke(action);
            }
            else if(tag == "<t"){
                float myFloat = System.BitConverter.ToSingle(tab_b, 2); // angle in rad
                action = () => tbTorque.Text = (myFloat.ToString()) ;
                tbController.Invoke(action);
            }
            else if (tag == "<p")
            {
                float myFloat = System.BitConverter.ToSingle(tab_b, 2); // angle in rad
                action = () => tbAngularPosRad.Text = (myFloat.ToString());
                tbController.Invoke(action);

                action = () => tbAngularPosDegree.Text = (myFloat * 57.2958f).ToString() + " °";
                tbController.Invoke(action);

                g.DrawLine(new Pen(Color.Red, 2.56f), new Point(100, 100), new Point(100 - (int)(100 * Math.Cos(myFloat + Math.PI / 2)), 100 - (int)(100 * Math.Sin(myFloat + Math.PI / 2))));

            }
            else if (tag == "<s")
            {
                float myFloat = System.BitConverter.ToSingle(tab_b, 2); 
                action = () => tbAngularSpeed.Text = (myFloat.ToString());
                tbController.Invoke(action);

                action = () => tbSpeed.Text = ((myFloat * 0.127*3.6).ToString());
                tbController.Invoke(action);
            }

            else
            {
                //Console.WriteLine("Received : " + System.Text.Encoding.ASCII.GetString(tab_b, 0, nbBytes));
            }
        }

        private void Form1_FormClosing(object sender, FormClosingEventArgs e)
        {
            if (serialPort1.IsOpen)
            {
                //serialPort1.Write("e");
                serialPort1.Write("<ce___\n");

                System.Threading.Thread.Sleep(100);
            }
            Environment.Exit(0);
        }

        private void btConnect_Enter(object sender, EventArgs e)
        {
            //Console.WriteLine("ENTER");
        }

        private void btReset_Click(object sender, EventArgs e)
        {
            serialPort1.Write("<cr___\n");
        }

        private void btStart_Click(object sender, EventArgs e)
        {
            serialPort1.Write("<cs___\n");
        }

        private void btStop_Click(object sender, EventArgs e)
        {
            serialPort1.Write("<ce___\n");
        }

        private void trackBarAngle_Scroll(object sender, EventArgs e)
        {
            Console.WriteLine(trackBarAngle.Value);
            if (rbUseTrackbar.Checked)
            {
                send_trackbar_angle();
            }

        }

        private void rbUseTrackbar_CheckedChanged(object sender, EventArgs e)
        {
            if (rbUsePotentiometer.Checked)
            {
                if (serialPort1.IsOpen)
                {
                    serialPort1.Write("<p____\n");
                }
            }
            else
            {
                send_trackbar_angle();
            }
        }


    }
}
