namespace SegwayMonitor
{
    partial class Form1
    {
        /// <summary>
        /// Variable nécessaire au concepteur.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Nettoyage des ressources utilisées.
        /// </summary>
        /// <param name="disposing">true si les ressources managées doivent être supprimées ; sinon, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Code généré par le Concepteur Windows Form

        /// <summary>
        /// Méthode requise pour la prise en charge du concepteur - ne modifiez pas
        /// le contenu de cette méthode avec l'éditeur de code.
        /// </summary>
        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.tbController = new System.Windows.Forms.TextBox();
            this.cbSerialPorts = new System.Windows.Forms.ComboBox();
            this.serialPort1 = new System.IO.Ports.SerialPort(this.components);
            this.groupBox2 = new System.Windows.Forms.GroupBox();
            this.btConnect = new System.Windows.Forms.Button();
            this.groupBox3 = new System.Windows.Forms.GroupBox();
            this.groupBox4 = new System.Windows.Forms.GroupBox();
            this.tbAngularPosDegree = new System.Windows.Forms.TextBox();
            this.label5 = new System.Windows.Forms.Label();
            this.label6 = new System.Windows.Forms.Label();
            this.tbAngularPosRad = new System.Windows.Forms.TextBox();
            this.groupBox15 = new System.Windows.Forms.GroupBox();
            this.label4 = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            this.tbPotValueDeg = new System.Windows.Forms.TextBox();
            this.tbPotValueRad = new System.Windows.Forms.TextBox();
            this.groupBox5 = new System.Windows.Forms.GroupBox();
            this.label10 = new System.Windows.Forms.Label();
            this.tbAngularSpeed = new System.Windows.Forms.TextBox();
            this.label9 = new System.Windows.Forms.Label();
            this.tbSpeed = new System.Windows.Forms.TextBox();
            this.tbTorque = new System.Windows.Forms.TextBox();
            this.label7 = new System.Windows.Forms.Label();
            this.label8 = new System.Windows.Forms.Label();
            this.tbLatency = new System.Windows.Forms.TextBox();
            this.pgraph = new System.Windows.Forms.Panel();
            this.groupBox8 = new System.Windows.Forms.GroupBox();
            this.btReset = new System.Windows.Forms.Button();
            this.btStart = new System.Windows.Forms.Button();
            this.btStop = new System.Windows.Forms.Button();
            this.groupBox11 = new System.Windows.Forms.GroupBox();
            this.groupBox12 = new System.Windows.Forms.GroupBox();
            this.label2 = new System.Windows.Forms.Label();
            this.label1 = new System.Windows.Forms.Label();
            this.trackBarAngle = new System.Windows.Forms.TrackBar();
            this.groupBox13 = new System.Windows.Forms.GroupBox();
            this.labelMaxAngle = new System.Windows.Forms.Label();
            this.labelMiddleAngle = new System.Windows.Forms.Label();
            this.labelMinAngle = new System.Windows.Forms.Label();
            this.groupBox14 = new System.Windows.Forms.GroupBox();
            this.rbUsePotentiometer = new System.Windows.Forms.RadioButton();
            this.rbUseTrackbar = new System.Windows.Forms.RadioButton();
            this.groupBox1.SuspendLayout();
            this.groupBox2.SuspendLayout();
            this.groupBox3.SuspendLayout();
            this.groupBox4.SuspendLayout();
            this.groupBox15.SuspendLayout();
            this.groupBox5.SuspendLayout();
            this.groupBox8.SuspendLayout();
            this.groupBox11.SuspendLayout();
            this.groupBox12.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.trackBarAngle)).BeginInit();
            this.groupBox13.SuspendLayout();
            this.groupBox14.SuspendLayout();
            this.SuspendLayout();
            // 
            // groupBox1
            // 
            this.groupBox1.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left)));
            this.groupBox1.Controls.Add(this.tbController);
            this.groupBox1.Location = new System.Drawing.Point(12, 12);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(199, 426);
            this.groupBox1.TabIndex = 0;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "Log";
            // 
            // tbController
            // 
            this.tbController.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.tbController.BackColor = System.Drawing.SystemColors.ControlLightLight;
            this.tbController.Location = new System.Drawing.Point(6, 19);
            this.tbController.Multiline = true;
            this.tbController.Name = "tbController";
            this.tbController.ReadOnly = true;
            this.tbController.Size = new System.Drawing.Size(187, 400);
            this.tbController.TabIndex = 0;
            // 
            // cbSerialPorts
            // 
            this.cbSerialPorts.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.cbSerialPorts.FormattingEnabled = true;
            this.cbSerialPorts.Location = new System.Drawing.Point(6, 19);
            this.cbSerialPorts.Name = "cbSerialPorts";
            this.cbSerialPorts.Size = new System.Drawing.Size(109, 21);
            this.cbSerialPorts.Sorted = true;
            this.cbSerialPorts.TabIndex = 1;
            this.cbSerialPorts.SelectedIndexChanged += new System.EventHandler(this.cbSerialPorts_SelectedIndexChanged);
            // 
            // serialPort1
            // 
            this.serialPort1.DtrEnable = true;
            this.serialPort1.DataReceived += new System.IO.Ports.SerialDataReceivedEventHandler(this.serialPort1_DataReceived);
            // 
            // groupBox2
            // 
            this.groupBox2.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.groupBox2.Controls.Add(this.cbSerialPorts);
            this.groupBox2.Location = new System.Drawing.Point(436, 398);
            this.groupBox2.Name = "groupBox2";
            this.groupBox2.Size = new System.Drawing.Size(121, 50);
            this.groupBox2.TabIndex = 2;
            this.groupBox2.TabStop = false;
            this.groupBox2.Text = "Controller COM Port";
            // 
            // btConnect
            // 
            this.btConnect.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.btConnect.Location = new System.Drawing.Point(565, 398);
            this.btConnect.Name = "btConnect";
            this.btConnect.Size = new System.Drawing.Size(124, 50);
            this.btConnect.TabIndex = 3;
            this.btConnect.Text = "Connect";
            this.btConnect.UseVisualStyleBackColor = true;
            this.btConnect.Click += new System.EventHandler(this.btConnect_Click);
            this.btConnect.Enter += new System.EventHandler(this.btConnect_Enter);
            // 
            // groupBox3
            // 
            this.groupBox3.Controls.Add(this.groupBox4);
            this.groupBox3.Controls.Add(this.groupBox15);
            this.groupBox3.Controls.Add(this.groupBox5);
            this.groupBox3.Location = new System.Drawing.Point(217, 12);
            this.groupBox3.Name = "groupBox3";
            this.groupBox3.Size = new System.Drawing.Size(213, 257);
            this.groupBox3.TabIndex = 1;
            this.groupBox3.TabStop = false;
            this.groupBox3.Text = "Values";
            // 
            // groupBox4
            // 
            this.groupBox4.Controls.Add(this.tbAngularPosDegree);
            this.groupBox4.Controls.Add(this.label5);
            this.groupBox4.Controls.Add(this.label6);
            this.groupBox4.Controls.Add(this.tbAngularPosRad);
            this.groupBox4.Location = new System.Drawing.Point(6, 83);
            this.groupBox4.Name = "groupBox4";
            this.groupBox4.Size = new System.Drawing.Size(201, 57);
            this.groupBox4.TabIndex = 2;
            this.groupBox4.TabStop = false;
            this.groupBox4.Text = "Segway Angle";
            // 
            // tbAngularPosDegree
            // 
            this.tbAngularPosDegree.BackColor = System.Drawing.SystemColors.Window;
            this.tbAngularPosDegree.Location = new System.Drawing.Point(110, 32);
            this.tbAngularPosDegree.Name = "tbAngularPosDegree";
            this.tbAngularPosDegree.ReadOnly = true;
            this.tbAngularPosDegree.Size = new System.Drawing.Size(78, 20);
            this.tbAngularPosDegree.TabIndex = 4;
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(106, 16);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(58, 13);
            this.label5.TabIndex = 3;
            this.label5.Text = "in Degrees";
            // 
            // label6
            // 
            this.label6.AutoSize = true;
            this.label6.Location = new System.Drawing.Point(6, 16);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(57, 13);
            this.label6.TabIndex = 2;
            this.label6.Text = "in Radians";
            // 
            // tbAngularPosRad
            // 
            this.tbAngularPosRad.BackColor = System.Drawing.SystemColors.Window;
            this.tbAngularPosRad.Location = new System.Drawing.Point(6, 32);
            this.tbAngularPosRad.Name = "tbAngularPosRad";
            this.tbAngularPosRad.ReadOnly = true;
            this.tbAngularPosRad.Size = new System.Drawing.Size(85, 20);
            this.tbAngularPosRad.TabIndex = 1;
            // 
            // groupBox15
            // 
            this.groupBox15.Controls.Add(this.label4);
            this.groupBox15.Controls.Add(this.label3);
            this.groupBox15.Controls.Add(this.tbPotValueDeg);
            this.groupBox15.Controls.Add(this.tbPotValueRad);
            this.groupBox15.Location = new System.Drawing.Point(6, 19);
            this.groupBox15.Name = "groupBox15";
            this.groupBox15.Size = new System.Drawing.Size(201, 58);
            this.groupBox15.TabIndex = 1;
            this.groupBox15.TabStop = false;
            this.groupBox15.Text = "User Angle";
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(107, 16);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(58, 13);
            this.label4.TabIndex = 3;
            this.label4.Text = "in Degrees";
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(6, 16);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(57, 13);
            this.label3.TabIndex = 2;
            this.label3.Text = "in Radians";
            // 
            // tbPotValueDeg
            // 
            this.tbPotValueDeg.BackColor = System.Drawing.SystemColors.Window;
            this.tbPotValueDeg.Location = new System.Drawing.Point(110, 32);
            this.tbPotValueDeg.Name = "tbPotValueDeg";
            this.tbPotValueDeg.ReadOnly = true;
            this.tbPotValueDeg.Size = new System.Drawing.Size(78, 20);
            this.tbPotValueDeg.TabIndex = 1;
            // 
            // tbPotValueRad
            // 
            this.tbPotValueRad.BackColor = System.Drawing.SystemColors.Window;
            this.tbPotValueRad.Location = new System.Drawing.Point(6, 32);
            this.tbPotValueRad.Name = "tbPotValueRad";
            this.tbPotValueRad.ReadOnly = true;
            this.tbPotValueRad.Size = new System.Drawing.Size(83, 20);
            this.tbPotValueRad.TabIndex = 0;
            // 
            // groupBox5
            // 
            this.groupBox5.Controls.Add(this.label10);
            this.groupBox5.Controls.Add(this.tbAngularSpeed);
            this.groupBox5.Controls.Add(this.label9);
            this.groupBox5.Controls.Add(this.tbSpeed);
            this.groupBox5.Controls.Add(this.tbTorque);
            this.groupBox5.Controls.Add(this.label7);
            this.groupBox5.Controls.Add(this.label8);
            this.groupBox5.Controls.Add(this.tbLatency);
            this.groupBox5.Location = new System.Drawing.Point(6, 141);
            this.groupBox5.Name = "groupBox5";
            this.groupBox5.Size = new System.Drawing.Size(201, 101);
            this.groupBox5.TabIndex = 5;
            this.groupBox5.TabStop = false;
            this.groupBox5.Text = "Others";
            // 
            // label10
            // 
            this.label10.AutoSize = true;
            this.label10.Location = new System.Drawing.Point(106, 58);
            this.label10.Name = "label10";
            this.label10.Size = new System.Drawing.Size(77, 13);
            this.label10.TabIndex = 15;
            this.label10.Text = "Angular Speed";
            // 
            // tbAngularSpeed
            // 
            this.tbAngularSpeed.BackColor = System.Drawing.SystemColors.Window;
            this.tbAngularSpeed.Location = new System.Drawing.Point(109, 75);
            this.tbAngularSpeed.Name = "tbAngularSpeed";
            this.tbAngularSpeed.ReadOnly = true;
            this.tbAngularSpeed.Size = new System.Drawing.Size(85, 20);
            this.tbAngularSpeed.TabIndex = 1;
            // 
            // label9
            // 
            this.label9.AutoSize = true;
            this.label9.Location = new System.Drawing.Point(107, 16);
            this.label9.Name = "label9";
            this.label9.Size = new System.Drawing.Size(41, 13);
            this.label9.TabIndex = 14;
            this.label9.Text = "Torque";
            // 
            // tbSpeed
            // 
            this.tbSpeed.BackColor = System.Drawing.SystemColors.Window;
            this.tbSpeed.Location = new System.Drawing.Point(7, 75);
            this.tbSpeed.Name = "tbSpeed";
            this.tbSpeed.ReadOnly = true;
            this.tbSpeed.Size = new System.Drawing.Size(84, 20);
            this.tbSpeed.TabIndex = 13;
            // 
            // tbTorque
            // 
            this.tbTorque.BackColor = System.Drawing.SystemColors.Window;
            this.tbTorque.Location = new System.Drawing.Point(110, 35);
            this.tbTorque.Name = "tbTorque";
            this.tbTorque.ReadOnly = true;
            this.tbTorque.Size = new System.Drawing.Size(84, 20);
            this.tbTorque.TabIndex = 1;
            // 
            // label7
            // 
            this.label7.AutoSize = true;
            this.label7.Location = new System.Drawing.Point(6, 59);
            this.label7.Name = "label7";
            this.label7.Size = new System.Drawing.Size(72, 13);
            this.label7.TabIndex = 3;
            this.label7.Text = "Speed (km/h)";
            // 
            // label8
            // 
            this.label8.AutoSize = true;
            this.label8.Location = new System.Drawing.Point(6, 16);
            this.label8.Name = "label8";
            this.label8.Size = new System.Drawing.Size(45, 13);
            this.label8.TabIndex = 2;
            this.label8.Text = "Latency";
            // 
            // tbLatency
            // 
            this.tbLatency.BackColor = System.Drawing.SystemColors.Window;
            this.tbLatency.Location = new System.Drawing.Point(7, 32);
            this.tbLatency.Name = "tbLatency";
            this.tbLatency.ReadOnly = true;
            this.tbLatency.Size = new System.Drawing.Size(84, 20);
            this.tbLatency.TabIndex = 1;
            // 
            // pgraph
            // 
            this.pgraph.Location = new System.Drawing.Point(7, 22);
            this.pgraph.Name = "pgraph";
            this.pgraph.Size = new System.Drawing.Size(200, 100);
            this.pgraph.TabIndex = 4;
            // 
            // groupBox8
            // 
            this.groupBox8.Controls.Add(this.pgraph);
            this.groupBox8.Location = new System.Drawing.Point(217, 275);
            this.groupBox8.Name = "groupBox8";
            this.groupBox8.Size = new System.Drawing.Size(213, 128);
            this.groupBox8.TabIndex = 0;
            this.groupBox8.TabStop = false;
            this.groupBox8.Text = "Visual Representation";
            // 
            // btReset
            // 
            this.btReset.Enabled = false;
            this.btReset.Location = new System.Drawing.Point(168, 19);
            this.btReset.Name = "btReset";
            this.btReset.Size = new System.Drawing.Size(75, 23);
            this.btReset.TabIndex = 5;
            this.btReset.Text = "Reset";
            this.btReset.UseVisualStyleBackColor = true;
            this.btReset.Click += new System.EventHandler(this.btReset_Click);
            // 
            // btStart
            // 
            this.btStart.Enabled = false;
            this.btStart.Location = new System.Drawing.Point(6, 19);
            this.btStart.Name = "btStart";
            this.btStart.Size = new System.Drawing.Size(75, 23);
            this.btStart.TabIndex = 6;
            this.btStart.Text = "Start";
            this.btStart.UseVisualStyleBackColor = true;
            this.btStart.Click += new System.EventHandler(this.btStart_Click);
            // 
            // btStop
            // 
            this.btStop.Enabled = false;
            this.btStop.Location = new System.Drawing.Point(87, 19);
            this.btStop.Name = "btStop";
            this.btStop.Size = new System.Drawing.Size(75, 23);
            this.btStop.TabIndex = 7;
            this.btStop.Text = "Stop";
            this.btStop.UseVisualStyleBackColor = true;
            this.btStop.Click += new System.EventHandler(this.btStop_Click);
            // 
            // groupBox11
            // 
            this.groupBox11.Controls.Add(this.btStart);
            this.groupBox11.Controls.Add(this.btReset);
            this.groupBox11.Controls.Add(this.btStop);
            this.groupBox11.Location = new System.Drawing.Point(436, 332);
            this.groupBox11.Name = "groupBox11";
            this.groupBox11.Size = new System.Drawing.Size(253, 48);
            this.groupBox11.TabIndex = 8;
            this.groupBox11.TabStop = false;
            this.groupBox11.Text = "Simulation Controls";
            // 
            // groupBox12
            // 
            this.groupBox12.Controls.Add(this.label2);
            this.groupBox12.Controls.Add(this.label1);
            this.groupBox12.Location = new System.Drawing.Point(217, 409);
            this.groupBox12.Name = "groupBox12";
            this.groupBox12.Size = new System.Drawing.Size(150, 37);
            this.groupBox12.TabIndex = 9;
            this.groupBox12.TabStop = false;
            this.groupBox12.Text = "Labels";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.ForeColor = System.Drawing.Color.Blue;
            this.label2.Location = new System.Drawing.Point(87, 16);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(59, 13);
            this.label2.TabIndex = 1;
            this.label2.Text = "User Angle";
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.ForeColor = System.Drawing.Color.Red;
            this.label1.Location = new System.Drawing.Point(6, 16);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(75, 13);
            this.label1.TabIndex = 0;
            this.label1.Text = "Segway Angle";
            // 
            // trackBarAngle
            // 
            this.trackBarAngle.Location = new System.Drawing.Point(6, 19);
            this.trackBarAngle.Maximum = 50;
            this.trackBarAngle.Minimum = -50;
            this.trackBarAngle.Name = "trackBarAngle";
            this.trackBarAngle.Size = new System.Drawing.Size(188, 45);
            this.trackBarAngle.TabIndex = 10;
            this.trackBarAngle.Scroll += new System.EventHandler(this.trackBarAngle_Scroll);
            // 
            // groupBox13
            // 
            this.groupBox13.Controls.Add(this.labelMaxAngle);
            this.groupBox13.Controls.Add(this.labelMiddleAngle);
            this.groupBox13.Controls.Add(this.labelMinAngle);
            this.groupBox13.Controls.Add(this.trackBarAngle);
            this.groupBox13.Location = new System.Drawing.Point(436, 95);
            this.groupBox13.Name = "groupBox13";
            this.groupBox13.Size = new System.Drawing.Size(201, 89);
            this.groupBox13.TabIndex = 11;
            this.groupBox13.TabStop = false;
            this.groupBox13.Text = "Angle";
            // 
            // labelMaxAngle
            // 
            this.labelMaxAngle.AutoSize = true;
            this.labelMaxAngle.Location = new System.Drawing.Point(176, 51);
            this.labelMaxAngle.Name = "labelMaxAngle";
            this.labelMaxAngle.Size = new System.Drawing.Size(53, 13);
            this.labelMaxAngle.TabIndex = 13;
            this.labelMaxAngle.Text = "maxAngle";
            // 
            // labelMiddleAngle
            // 
            this.labelMiddleAngle.AutoSize = true;
            this.labelMiddleAngle.Location = new System.Drawing.Point(94, 51);
            this.labelMiddleAngle.Name = "labelMiddleAngle";
            this.labelMiddleAngle.Size = new System.Drawing.Size(64, 13);
            this.labelMiddleAngle.TabIndex = 12;
            this.labelMiddleAngle.Text = "middleAngle";
            this.labelMiddleAngle.TextAlign = System.Drawing.ContentAlignment.TopCenter;
            // 
            // labelMinAngle
            // 
            this.labelMinAngle.AutoSize = true;
            this.labelMinAngle.Location = new System.Drawing.Point(3, 51);
            this.labelMinAngle.Name = "labelMinAngle";
            this.labelMinAngle.Size = new System.Drawing.Size(50, 13);
            this.labelMinAngle.TabIndex = 11;
            this.labelMinAngle.Text = "minAngle";
            // 
            // groupBox14
            // 
            this.groupBox14.Controls.Add(this.rbUsePotentiometer);
            this.groupBox14.Controls.Add(this.rbUseTrackbar);
            this.groupBox14.Location = new System.Drawing.Point(436, 13);
            this.groupBox14.Name = "groupBox14";
            this.groupBox14.Size = new System.Drawing.Size(201, 76);
            this.groupBox14.TabIndex = 12;
            this.groupBox14.TabStop = false;
            this.groupBox14.Text = "Angle Generator";
            // 
            // rbUsePotentiometer
            // 
            this.rbUsePotentiometer.AutoSize = true;
            this.rbUsePotentiometer.Location = new System.Drawing.Point(7, 41);
            this.rbUsePotentiometer.Name = "rbUsePotentiometer";
            this.rbUsePotentiometer.Size = new System.Drawing.Size(112, 17);
            this.rbUsePotentiometer.TabIndex = 1;
            this.rbUsePotentiometer.Text = "Use Potentiometer";
            this.rbUsePotentiometer.UseVisualStyleBackColor = true;
            // 
            // rbUseTrackbar
            // 
            this.rbUseTrackbar.AutoSize = true;
            this.rbUseTrackbar.Checked = true;
            this.rbUseTrackbar.Location = new System.Drawing.Point(7, 17);
            this.rbUseTrackbar.Name = "rbUseTrackbar";
            this.rbUseTrackbar.Size = new System.Drawing.Size(90, 17);
            this.rbUseTrackbar.TabIndex = 0;
            this.rbUseTrackbar.TabStop = true;
            this.rbUseTrackbar.Text = "Use Trackbar";
            this.rbUseTrackbar.UseVisualStyleBackColor = true;
            this.rbUseTrackbar.CheckedChanged += new System.EventHandler(this.rbUseTrackbar_CheckedChanged);
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(694, 460);
            this.Controls.Add(this.groupBox14);
            this.Controls.Add(this.groupBox13);
            this.Controls.Add(this.groupBox12);
            this.Controls.Add(this.groupBox11);
            this.Controls.Add(this.groupBox8);
            this.Controls.Add(this.groupBox3);
            this.Controls.Add(this.btConnect);
            this.Controls.Add(this.groupBox2);
            this.Controls.Add(this.groupBox1);
            this.Name = "Form1";
            this.Text = "Segway Supervisor";
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.Form1_FormClosing);
            this.Load += new System.EventHandler(this.Form1_Load);
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            this.groupBox2.ResumeLayout(false);
            this.groupBox3.ResumeLayout(false);
            this.groupBox4.ResumeLayout(false);
            this.groupBox4.PerformLayout();
            this.groupBox15.ResumeLayout(false);
            this.groupBox15.PerformLayout();
            this.groupBox5.ResumeLayout(false);
            this.groupBox5.PerformLayout();
            this.groupBox8.ResumeLayout(false);
            this.groupBox11.ResumeLayout(false);
            this.groupBox12.ResumeLayout(false);
            this.groupBox12.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.trackBarAngle)).EndInit();
            this.groupBox13.ResumeLayout(false);
            this.groupBox13.PerformLayout();
            this.groupBox14.ResumeLayout(false);
            this.groupBox14.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.TextBox tbController;
        private System.Windows.Forms.ComboBox cbSerialPorts;
        private System.IO.Ports.SerialPort serialPort1;
        private System.Windows.Forms.GroupBox groupBox2;
        private System.Windows.Forms.Button btConnect;
        private System.Windows.Forms.GroupBox groupBox3;
        private System.Windows.Forms.TextBox tbTorque;
        private System.Windows.Forms.TextBox tbPotValueDeg;
        private System.Windows.Forms.TextBox tbLatency;
        private System.Windows.Forms.Panel pgraph;
        private System.Windows.Forms.GroupBox groupBox8;
        private System.Windows.Forms.TextBox tbAngularSpeed;
        private System.Windows.Forms.TextBox tbAngularPosRad;
        private System.Windows.Forms.Button btReset;
        private System.Windows.Forms.Button btStart;
        private System.Windows.Forms.Button btStop;
        private System.Windows.Forms.GroupBox groupBox11;
        private System.Windows.Forms.GroupBox groupBox12;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.TrackBar trackBarAngle;
        private System.Windows.Forms.GroupBox groupBox13;
        private System.Windows.Forms.Label labelMaxAngle;
        private System.Windows.Forms.Label labelMiddleAngle;
        private System.Windows.Forms.Label labelMinAngle;
        private System.Windows.Forms.GroupBox groupBox14;
        private System.Windows.Forms.RadioButton rbUsePotentiometer;
        private System.Windows.Forms.RadioButton rbUseTrackbar;
        private System.Windows.Forms.GroupBox groupBox15;
        private System.Windows.Forms.GroupBox groupBox4;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.Label label6;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.TextBox tbPotValueRad;
        private System.Windows.Forms.TextBox tbAngularPosDegree;
        private System.Windows.Forms.GroupBox groupBox5;
        private System.Windows.Forms.Label label10;
        private System.Windows.Forms.Label label9;
        private System.Windows.Forms.TextBox tbSpeed;
        private System.Windows.Forms.Label label7;
        private System.Windows.Forms.Label label8;
    }
}

