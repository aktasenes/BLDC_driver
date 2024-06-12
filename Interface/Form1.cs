using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Diagnostics;
using System.Drawing;
using System.Globalization;
using System.IO.Ports;
using System.Linq;
using System.Media;
using System.Reflection.Emit;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Windows.Forms.DataVisualization.Charting;
using static System.Windows.Forms.VisualStyles.VisualStyleElement;


namespace ESC1
{

    public partial class Form1 : Form
    {
        string raw_volt;
        string raw_akim;
        ulong max = 15;
        ulong min = 0;
        float volt;
        double akim;
        double akim_max;
        double akim_max2;
        double akim_max3;
        double akim_max4;
        double akim_max5;
        float akim1;
        float volt1;
        float akim2;
        float volt2;
        string veri;
        string[] ayrik_veri;
        string pwm;
        string rpm;
        double rpm1;
        int rpm2;
        bool dur = false;
        bool saat_yon = true;
        float volt_kes=10;
        float akim_kes=500;
        float volt_kes_text;
        float akim_kes_text;
        bool rpm_basla = false;
        Stopwatch stopwatch = new Stopwatch();
        public Form1()
        {
            InitializeComponent();
            
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            stopwatch.Start();
            Control.CheckForIllegalCrossThreadCalls = false;
            foreach (var seri in SerialPort.GetPortNames())
            {
                comboBox1.Items.Add(seri);
            }
            if (comboBox1.SelectedIndex == -1)
            {
                comboBox1.Text = "Seri Port Yok";
            }
            else
            {
                comboBox1.SelectedIndex = 0;
            }
            button2.Enabled = false;
            button4.Enabled = false;
            button3.Enabled = false;
            button6.Enabled = false;
            button7.Enabled = false;
            label1.Text = "";
            label2.Text = "";
            button6.Text = "CCW dön";
            textBox3.Text = "10";
            textBox4.Text = "500";
        }

        private void button1_Click(object sender, EventArgs e)
        {
            serialPort1.PortName = comboBox1.Text;
            serialPort1.BaudRate = 115200;
            serialPort1.Parity = Parity.None;
            serialPort1.StopBits = StopBits.One;
            serialPort1.DataBits = 8;
            try
            {
                serialPort1.Open();
            }
            catch(Exception ex)
            {
                MessageBox.Show($"HATA! \n Sebep:{ex.Message}","(,)))",MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
            if(serialPort1.IsOpen)
            {
                button1.Enabled = false;
                button2.Enabled = true;
                button4.Enabled = true;
                button3.Enabled = true;
                button5.Enabled = true;
                button6.Enabled = true;
                button7.Enabled = true;
                timer1.Start();
            }
        }

        private void button2_Click(object sender, EventArgs e)
        {
            serialPort1.Close();
            button1.Enabled = true;
            button2.Enabled=false;
            button4.Enabled = false;
            button3.Enabled = false;
            button5.Enabled = false;
            button6.Enabled = false;
            timer1.Stop();
        }


        private void serialPort1_DataReceived(object sender, SerialDataReceivedEventArgs e)
        {

            veri= serialPort1.ReadLine();
            ayrik_veri = veri.Split('*');
            raw_volt = ayrik_veri[0];
            raw_akim = ayrik_veri[1];
            pwm = ayrik_veri[2];
            rpm = ayrik_veri[3];
            
            rpm1 = (float)Convert.ToDecimal(rpm, CultureInfo.GetCultureInfo("en-US"));
            if (rpm1 == 1 && rpm_basla == false)
            {
                stopwatch.Start();
                rpm_basla = true;
            }
            else if (rpm1 == 1 && rpm_basla == true)
            {
                stopwatch.Stop();
                label9.Text = stopwatch.Elapsed.ToString();
                stopwatch.Restart();
                rpm_basla = false;
            }
                
                //label9.Text = stopwatch.Elapsed.ToString();

            Thread.Sleep(100);

            string data = serialPort1.ReadLine();       // bufferdan verileri oku

            if (textBox1.InvokeRequired)
            {
                textBox1.Invoke(new MethodInvoker(delegate { textBox1.Text = volt2 + "\r\n"; }));
                
            }
            if (textBox1.InvokeRequired)
            {
                textBox2.Invoke(new MethodInvoker(delegate { textBox2.Text = akim2*1000 + "\r\n"; }));

            }

            label6.Text = pwm;
            label1.Text = textBox1.Text.ToString();
            label2.Text = textBox2.Text.ToString();
            akim = (float)Convert.ToDecimal(raw_akim, CultureInfo.GetCultureInfo("en-US"));
            volt = (float)Convert.ToDecimal(raw_volt, CultureInfo.GetCultureInfo("en-US"));
            if (volt < 0.00000001)
            {
                volt = 0;
            }
            if (akim < 0.000000000000000000000000000001)
            {
                akim = 1;
            }
            volt1 = (float)Convert.ToDecimal(((volt * 3.3) / 4095) * 48 / 2.41);
            volt2 = (float)System.Math.Round(volt1, 3, MidpointRounding.ToEven);
            akim1 = (float)Convert.ToDecimal(((akim * 3.3) / 2048));//bu neden 10 bit?
            akim2 = (float)System.Math.Round(akim1, 4, MidpointRounding.ToEven);
            aGauge1.Value = Convert.ToInt16(volt2);
            aGauge2.Value = Convert.ToInt16(akim2 * 1000);
        }

        private void timer1_Tick(object sender, EventArgs e)
        {
            if(raw_volt != null)
                {
                
                //int tepe = Int32.Parse(volt);
                chart1.ChartAreas[0].AxisX.Minimum = min;
                chart1.ChartAreas[0].AxisX.Maximum = max;
                chart1.ChartAreas[0].AxisY.Minimum = volt2 * .5;
                chart1.ChartAreas[0].AxisY.Maximum = volt2 * 1.5;
                chart1.ChartAreas[0].AxisX.ScaleView.Zoom(min, max);
                this.chart1.Series[0].Points.AddXY((max - 1), volt2);
                chart1.ChartAreas[0].AxisY2.Minimum = akim2 * .5;
                chart1.ChartAreas[0].AxisY2.Maximum = akim2 * 1.5;
                this.chart1.Series[1].Points.AddXY((max - 1), akim2);
                max++;
                min++;
                if (volt2 < volt_kes)
                {
                    serialPort1.Write("6\r\n");
                    button5.Enabled = false;
                    button5.Text = "Düşük Gerilim";
                    SystemSounds.Beep.Play();
                }
                else if (akim2*1000 > akim_kes)
                {
                    serialPort1.Write("6\r\n");
                    button5.Enabled = false;
                    button5.Text = "Yüksek Akım";
                    SystemSounds.Beep.Play();
                }
                else
                {
                    button5.Enabled = true;
                    button5.Text="DUR!!!";
                }

            }
            }

        private void chart1_Click(object sender, EventArgs e)
        {

        }

        private void comboBox1_SelectedIndexChanged(object sender, EventArgs e)
        {

        }

        private void label3_Click(object sender, EventArgs e)
        {

        }

        private void label1_Click(object sender, EventArgs e)
        {

        }

        private void button3_Click(object sender, EventArgs e)
        {
            
            int pwm_suan = Int32.Parse(label6.Text);
            if(pwm_suan<290)
            {
                pwm_suan++;
                serialPort1.Write("1\r\n");
            }
            else
            {
                pwm_suan = 290;
            }
            //byte[] b = BitConverter.GetBytes(pwm_suan);
            //serialPort1.Write(b, 0, 4);

            //serialPort1.Write("1\r\n");
            Thread.Sleep(30);
        }

        private void button4_Click(object sender, EventArgs e)
        {
            
            int pwm_suan = Int32.Parse(label6.Text);
            if (pwm_suan > 0)
            {
                pwm_suan--;
                serialPort1.Write("3\r\n");
                //Thread.Sleep(30);
            }
            else
            {
                pwm_suan = 0;
            }
            Thread.Sleep(30);

            //serialPort1.Write("0\r\n");
            //byte[] b = BitConverter.GetBytes(pwm_suan);
            //serialPort1.Write(b, 0, 4);
        }

        private void button5_Click(object sender, EventArgs e)
        {
            if (dur == false)
            {
                serialPort1.Write("0\r\n");
                dur = true;
                Thread.Sleep(30);
            }
            else if(dur==true)
            {
                serialPort1.Write("2\r\n");
                dur = false;
                Thread.Sleep(30);
            }
        }

        private void aGauge1_ValueInRangeChanged(object sender, AGaugeApp.AGauge.ValueInRangeChangedEventArgs e)
        {

        }

        private void label2_Click(object sender, EventArgs e)
        {

        }

        private void button6_Click(object sender, EventArgs e)
        {
            if (saat_yon == true)
            {
                serialPort1.Write("4\r\n");
                saat_yon = false;
                Thread.Sleep(30);
                button6.Text = "CW dön";
            }
            else if (saat_yon == false)
            {
                serialPort1.Write("5\r\n");
                saat_yon = true;
                Thread.Sleep(30);
                button6.Text = "CCW dön";
            }
        }

        private void button7_Click(object sender, EventArgs e)
        {

            if (textBox4.Text == "")
            {
                MessageBox.Show($"Akım değeri girmelisin! \n", "Hata", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
            if (textBox3.Text == "")
            {
                MessageBox.Show($"Gerilim değeri girmelisin! \n", "Hata", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
            akim_kes_text = (float)Convert.ToDecimal(textBox4.Text);
            volt_kes_text = (float)Convert.ToDecimal(textBox3.Text);
            if (volt_kes_text != volt_kes)
            {
                volt_kes = volt_kes_text;
            }
            if (akim_kes_text != akim_kes)
            {
                akim_kes = akim_kes_text;
            }
        }

        private void textBox3_KeyPress(object sender, KeyPressEventArgs e)
        {
            e.Handled = !char.IsDigit(e.KeyChar) && !char.IsControl(e.KeyChar) && e.KeyChar != ',';
        }

        private void textBox4_KeyPress(object sender, KeyPressEventArgs e)
        {
            e.Handled = !char.IsDigit(e.KeyChar) && !char.IsControl(e.KeyChar) && e.KeyChar != ',';
        }

        private void timer2_Tick(object sender, EventArgs e)
        {

        }
    }
}
