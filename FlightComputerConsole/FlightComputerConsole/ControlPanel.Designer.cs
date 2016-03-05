namespace FlightComputerConsole
{
    partial class ControlPanel
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            this.labelOrientationQuaternion = new System.Windows.Forms.Label();
            this.labelQ0 = new System.Windows.Forms.Label();
            this.labelQ1 = new System.Windows.Forms.Label();
            this.labelQ2 = new System.Windows.Forms.Label();
            this.labelQ3 = new System.Windows.Forms.Label();
            this.labelYaw = new System.Windows.Forms.Label();
            this.labelPitch = new System.Windows.Forms.Label();
            this.labelRoll = new System.Windows.Forms.Label();
            this.labelOrientationEuler = new System.Windows.Forms.Label();
            this.groupFlightData = new System.Windows.Forms.GroupBox();
            this.outMotor6 = new System.Windows.Forms.Label();
            this.outMotor5 = new System.Windows.Forms.Label();
            this.outMotor4 = new System.Windows.Forms.Label();
            this.outMotor3 = new System.Windows.Forms.Label();
            this.outMotor2 = new System.Windows.Forms.Label();
            this.outMotor1 = new System.Windows.Forms.Label();
            this.outAltitude = new System.Windows.Forms.Label();
            this.outCurrentPressure = new System.Windows.Forms.Label();
            this.outStartingPressure = new System.Windows.Forms.Label();
            this.outCurrentTemp = new System.Windows.Forms.Label();
            this.outYaw = new System.Windows.Forms.Label();
            this.outPitch = new System.Windows.Forms.Label();
            this.outRoll = new System.Windows.Forms.Label();
            this.outQ3 = new System.Windows.Forms.Label();
            this.outQ2 = new System.Windows.Forms.Label();
            this.outQ1 = new System.Windows.Forms.Label();
            this.outQ0 = new System.Windows.Forms.Label();
            this.labelMotor6 = new System.Windows.Forms.Label();
            this.labelMotor5 = new System.Windows.Forms.Label();
            this.labelMotor4 = new System.Windows.Forms.Label();
            this.labelMotor3 = new System.Windows.Forms.Label();
            this.labelMotor2 = new System.Windows.Forms.Label();
            this.labelMotor1 = new System.Windows.Forms.Label();
            this.labelMotorStats = new System.Windows.Forms.Label();
            this.labelTemp = new System.Windows.Forms.Label();
            this.labelStartingPressure = new System.Windows.Forms.Label();
            this.labelCurrentPressure = new System.Windows.Forms.Label();
            this.labelAltitude = new System.Windows.Forms.Label();
            this.labelFlightData = new System.Windows.Forms.Label();
            this.groupCommands = new System.Windows.Forms.GroupBox();
            this.ResponseBox = new System.Windows.Forms.RichTextBox();
            this.CommandLine = new System.Windows.Forms.TextBox();
            this.CommsWorker = new System.ComponentModel.BackgroundWorker();
            this.serialPort = new System.IO.Ports.SerialPort(this.components);
            this.groupFlightData.SuspendLayout();
            this.groupCommands.SuspendLayout();
            this.SuspendLayout();
            // 
            // labelOrientationQuaternion
            // 
            this.labelOrientationQuaternion.AutoSize = true;
            this.labelOrientationQuaternion.Font = new System.Drawing.Font("Microsoft Sans Serif", 18F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.labelOrientationQuaternion.Location = new System.Drawing.Point(6, 16);
            this.labelOrientationQuaternion.Name = "labelOrientationQuaternion";
            this.labelOrientationQuaternion.Size = new System.Drawing.Size(272, 29);
            this.labelOrientationQuaternion.TabIndex = 0;
            this.labelOrientationQuaternion.Text = "Orientation (Quaternion)";
            // 
            // labelQ0
            // 
            this.labelQ0.AutoSize = true;
            this.labelQ0.Font = new System.Drawing.Font("Microsoft Sans Serif", 10F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.labelQ0.Location = new System.Drawing.Point(13, 50);
            this.labelQ0.Name = "labelQ0";
            this.labelQ0.Size = new System.Drawing.Size(31, 17);
            this.labelQ0.TabIndex = 1;
            this.labelQ0.Text = "Q0:";
            // 
            // labelQ1
            // 
            this.labelQ1.AutoSize = true;
            this.labelQ1.Font = new System.Drawing.Font("Microsoft Sans Serif", 10F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.labelQ1.Location = new System.Drawing.Point(13, 67);
            this.labelQ1.Name = "labelQ1";
            this.labelQ1.Size = new System.Drawing.Size(31, 17);
            this.labelQ1.TabIndex = 2;
            this.labelQ1.Text = "Q1:";
            // 
            // labelQ2
            // 
            this.labelQ2.AutoSize = true;
            this.labelQ2.Font = new System.Drawing.Font("Microsoft Sans Serif", 10F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.labelQ2.Location = new System.Drawing.Point(13, 84);
            this.labelQ2.Name = "labelQ2";
            this.labelQ2.Size = new System.Drawing.Size(31, 17);
            this.labelQ2.TabIndex = 3;
            this.labelQ2.Text = "Q2:";
            // 
            // labelQ3
            // 
            this.labelQ3.AutoSize = true;
            this.labelQ3.Font = new System.Drawing.Font("Microsoft Sans Serif", 10F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.labelQ3.Location = new System.Drawing.Point(13, 101);
            this.labelQ3.Name = "labelQ3";
            this.labelQ3.Size = new System.Drawing.Size(31, 17);
            this.labelQ3.TabIndex = 4;
            this.labelQ3.Text = "Q3:";
            // 
            // labelYaw
            // 
            this.labelYaw.AutoSize = true;
            this.labelYaw.Font = new System.Drawing.Font("Microsoft Sans Serif", 10F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.labelYaw.Location = new System.Drawing.Point(397, 84);
            this.labelYaw.Name = "labelYaw";
            this.labelYaw.Size = new System.Drawing.Size(38, 17);
            this.labelYaw.TabIndex = 8;
            this.labelYaw.Text = "Yaw:";
            // 
            // labelPitch
            // 
            this.labelPitch.AutoSize = true;
            this.labelPitch.Font = new System.Drawing.Font("Microsoft Sans Serif", 10F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.labelPitch.Location = new System.Drawing.Point(397, 67);
            this.labelPitch.Name = "labelPitch";
            this.labelPitch.Size = new System.Drawing.Size(43, 17);
            this.labelPitch.TabIndex = 7;
            this.labelPitch.Text = "Pitch:";
            // 
            // labelRoll
            // 
            this.labelRoll.AutoSize = true;
            this.labelRoll.Font = new System.Drawing.Font("Microsoft Sans Serif", 10F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.labelRoll.Location = new System.Drawing.Point(397, 50);
            this.labelRoll.Name = "labelRoll";
            this.labelRoll.Size = new System.Drawing.Size(36, 17);
            this.labelRoll.TabIndex = 6;
            this.labelRoll.Text = "Roll:";
            // 
            // labelOrientationEuler
            // 
            this.labelOrientationEuler.AutoSize = true;
            this.labelOrientationEuler.Font = new System.Drawing.Font("Microsoft Sans Serif", 18F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.labelOrientationEuler.Location = new System.Drawing.Point(395, 16);
            this.labelOrientationEuler.Name = "labelOrientationEuler";
            this.labelOrientationEuler.Size = new System.Drawing.Size(210, 29);
            this.labelOrientationEuler.TabIndex = 5;
            this.labelOrientationEuler.Text = "Orientation (Euler)";
            // 
            // groupFlightData
            // 
            this.groupFlightData.Controls.Add(this.outMotor6);
            this.groupFlightData.Controls.Add(this.outMotor5);
            this.groupFlightData.Controls.Add(this.outMotor4);
            this.groupFlightData.Controls.Add(this.outMotor3);
            this.groupFlightData.Controls.Add(this.outMotor2);
            this.groupFlightData.Controls.Add(this.outMotor1);
            this.groupFlightData.Controls.Add(this.outAltitude);
            this.groupFlightData.Controls.Add(this.outCurrentPressure);
            this.groupFlightData.Controls.Add(this.outStartingPressure);
            this.groupFlightData.Controls.Add(this.outCurrentTemp);
            this.groupFlightData.Controls.Add(this.outYaw);
            this.groupFlightData.Controls.Add(this.outPitch);
            this.groupFlightData.Controls.Add(this.outRoll);
            this.groupFlightData.Controls.Add(this.outQ3);
            this.groupFlightData.Controls.Add(this.outQ2);
            this.groupFlightData.Controls.Add(this.outQ1);
            this.groupFlightData.Controls.Add(this.outQ0);
            this.groupFlightData.Controls.Add(this.labelMotor6);
            this.groupFlightData.Controls.Add(this.labelMotor5);
            this.groupFlightData.Controls.Add(this.labelMotor4);
            this.groupFlightData.Controls.Add(this.labelMotor3);
            this.groupFlightData.Controls.Add(this.labelMotor2);
            this.groupFlightData.Controls.Add(this.labelMotor1);
            this.groupFlightData.Controls.Add(this.labelMotorStats);
            this.groupFlightData.Controls.Add(this.labelTemp);
            this.groupFlightData.Controls.Add(this.labelStartingPressure);
            this.groupFlightData.Controls.Add(this.labelCurrentPressure);
            this.groupFlightData.Controls.Add(this.labelAltitude);
            this.groupFlightData.Controls.Add(this.labelFlightData);
            this.groupFlightData.Controls.Add(this.labelOrientationQuaternion);
            this.groupFlightData.Controls.Add(this.labelYaw);
            this.groupFlightData.Controls.Add(this.labelQ0);
            this.groupFlightData.Controls.Add(this.labelPitch);
            this.groupFlightData.Controls.Add(this.labelQ1);
            this.groupFlightData.Controls.Add(this.labelRoll);
            this.groupFlightData.Controls.Add(this.labelQ2);
            this.groupFlightData.Controls.Add(this.labelOrientationEuler);
            this.groupFlightData.Controls.Add(this.labelQ3);
            this.groupFlightData.Location = new System.Drawing.Point(12, 12);
            this.groupFlightData.Name = "groupFlightData";
            this.groupFlightData.Size = new System.Drawing.Size(731, 281);
            this.groupFlightData.TabIndex = 9;
            this.groupFlightData.TabStop = false;
            this.groupFlightData.Text = "Orientation Data";
            // 
            // outMotor6
            // 
            this.outMotor6.AutoSize = true;
            this.outMotor6.Font = new System.Drawing.Font("Microsoft Sans Serif", 10F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.outMotor6.ForeColor = System.Drawing.Color.DarkRed;
            this.outMotor6.Location = new System.Drawing.Point(516, 250);
            this.outMotor6.Name = "outMotor6";
            this.outMotor6.Size = new System.Drawing.Size(51, 17);
            this.outMotor6.TabIndex = 37;
            this.outMotor6.Text = "Stalled";
            // 
            // outMotor5
            // 
            this.outMotor5.AutoSize = true;
            this.outMotor5.Font = new System.Drawing.Font("Microsoft Sans Serif", 10F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.outMotor5.ForeColor = System.Drawing.Color.DarkRed;
            this.outMotor5.Location = new System.Drawing.Point(516, 233);
            this.outMotor5.Name = "outMotor5";
            this.outMotor5.Size = new System.Drawing.Size(51, 17);
            this.outMotor5.TabIndex = 36;
            this.outMotor5.Text = "Stalled";
            // 
            // outMotor4
            // 
            this.outMotor4.AutoSize = true;
            this.outMotor4.Font = new System.Drawing.Font("Microsoft Sans Serif", 10F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.outMotor4.ForeColor = System.Drawing.Color.DarkRed;
            this.outMotor4.Location = new System.Drawing.Point(516, 216);
            this.outMotor4.Name = "outMotor4";
            this.outMotor4.Size = new System.Drawing.Size(51, 17);
            this.outMotor4.TabIndex = 35;
            this.outMotor4.Text = "Stalled";
            // 
            // outMotor3
            // 
            this.outMotor3.AutoSize = true;
            this.outMotor3.Font = new System.Drawing.Font("Microsoft Sans Serif", 10F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.outMotor3.ForeColor = System.Drawing.Color.DarkRed;
            this.outMotor3.Location = new System.Drawing.Point(516, 199);
            this.outMotor3.Name = "outMotor3";
            this.outMotor3.Size = new System.Drawing.Size(51, 17);
            this.outMotor3.TabIndex = 34;
            this.outMotor3.Text = "Stalled";
            // 
            // outMotor2
            // 
            this.outMotor2.AutoSize = true;
            this.outMotor2.Font = new System.Drawing.Font("Microsoft Sans Serif", 10F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.outMotor2.ForeColor = System.Drawing.Color.DarkRed;
            this.outMotor2.Location = new System.Drawing.Point(516, 182);
            this.outMotor2.Name = "outMotor2";
            this.outMotor2.Size = new System.Drawing.Size(51, 17);
            this.outMotor2.TabIndex = 33;
            this.outMotor2.Text = "Stalled";
            // 
            // outMotor1
            // 
            this.outMotor1.AutoSize = true;
            this.outMotor1.Font = new System.Drawing.Font("Microsoft Sans Serif", 10F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.outMotor1.ForeColor = System.Drawing.Color.DarkRed;
            this.outMotor1.Location = new System.Drawing.Point(516, 165);
            this.outMotor1.Name = "outMotor1";
            this.outMotor1.Size = new System.Drawing.Size(51, 17);
            this.outMotor1.TabIndex = 32;
            this.outMotor1.Text = "Stalled";
            // 
            // outAltitude
            // 
            this.outAltitude.AutoSize = true;
            this.outAltitude.Font = new System.Drawing.Font("Microsoft Sans Serif", 10F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.outAltitude.ForeColor = System.Drawing.Color.DarkRed;
            this.outAltitude.Location = new System.Drawing.Point(164, 165);
            this.outAltitude.Name = "outAltitude";
            this.outAltitude.Size = new System.Drawing.Size(91, 17);
            this.outAltitude.TabIndex = 31;
            this.outAltitude.Text = "Not Available";
            // 
            // outCurrentPressure
            // 
            this.outCurrentPressure.AutoSize = true;
            this.outCurrentPressure.Font = new System.Drawing.Font("Microsoft Sans Serif", 10F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.outCurrentPressure.ForeColor = System.Drawing.Color.DarkRed;
            this.outCurrentPressure.Location = new System.Drawing.Point(164, 182);
            this.outCurrentPressure.Name = "outCurrentPressure";
            this.outCurrentPressure.Size = new System.Drawing.Size(91, 17);
            this.outCurrentPressure.TabIndex = 30;
            this.outCurrentPressure.Text = "Not Available";
            // 
            // outStartingPressure
            // 
            this.outStartingPressure.AutoSize = true;
            this.outStartingPressure.Font = new System.Drawing.Font("Microsoft Sans Serif", 10F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.outStartingPressure.ForeColor = System.Drawing.Color.DarkRed;
            this.outStartingPressure.Location = new System.Drawing.Point(164, 199);
            this.outStartingPressure.Name = "outStartingPressure";
            this.outStartingPressure.Size = new System.Drawing.Size(91, 17);
            this.outStartingPressure.TabIndex = 29;
            this.outStartingPressure.Text = "Not Available";
            // 
            // outCurrentTemp
            // 
            this.outCurrentTemp.AutoSize = true;
            this.outCurrentTemp.Font = new System.Drawing.Font("Microsoft Sans Serif", 10F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.outCurrentTemp.ForeColor = System.Drawing.Color.DarkRed;
            this.outCurrentTemp.Location = new System.Drawing.Point(164, 216);
            this.outCurrentTemp.Name = "outCurrentTemp";
            this.outCurrentTemp.Size = new System.Drawing.Size(91, 17);
            this.outCurrentTemp.TabIndex = 28;
            this.outCurrentTemp.Text = "Not Available";
            // 
            // outYaw
            // 
            this.outYaw.AutoSize = true;
            this.outYaw.Font = new System.Drawing.Font("Microsoft Sans Serif", 10F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.outYaw.ForeColor = System.Drawing.Color.DarkRed;
            this.outYaw.Location = new System.Drawing.Point(439, 84);
            this.outYaw.Name = "outYaw";
            this.outYaw.Size = new System.Drawing.Size(91, 17);
            this.outYaw.TabIndex = 27;
            this.outYaw.Text = "Not Available";
            // 
            // outPitch
            // 
            this.outPitch.AutoSize = true;
            this.outPitch.Font = new System.Drawing.Font("Microsoft Sans Serif", 10F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.outPitch.ForeColor = System.Drawing.Color.DarkRed;
            this.outPitch.Location = new System.Drawing.Point(439, 67);
            this.outPitch.Name = "outPitch";
            this.outPitch.Size = new System.Drawing.Size(91, 17);
            this.outPitch.TabIndex = 26;
            this.outPitch.Text = "Not Available";
            // 
            // outRoll
            // 
            this.outRoll.AutoSize = true;
            this.outRoll.Font = new System.Drawing.Font("Microsoft Sans Serif", 10F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.outRoll.ForeColor = System.Drawing.Color.DarkRed;
            this.outRoll.Location = new System.Drawing.Point(439, 50);
            this.outRoll.Name = "outRoll";
            this.outRoll.Size = new System.Drawing.Size(91, 17);
            this.outRoll.TabIndex = 25;
            this.outRoll.Text = "Not Available";
            // 
            // outQ3
            // 
            this.outQ3.AutoSize = true;
            this.outQ3.Font = new System.Drawing.Font("Microsoft Sans Serif", 10F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.outQ3.ForeColor = System.Drawing.Color.DarkRed;
            this.outQ3.Location = new System.Drawing.Point(50, 101);
            this.outQ3.Name = "outQ3";
            this.outQ3.Size = new System.Drawing.Size(91, 17);
            this.outQ3.TabIndex = 24;
            this.outQ3.Text = "Not Available";
            // 
            // outQ2
            // 
            this.outQ2.AutoSize = true;
            this.outQ2.Font = new System.Drawing.Font("Microsoft Sans Serif", 10F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.outQ2.ForeColor = System.Drawing.Color.DarkRed;
            this.outQ2.Location = new System.Drawing.Point(50, 84);
            this.outQ2.Name = "outQ2";
            this.outQ2.Size = new System.Drawing.Size(91, 17);
            this.outQ2.TabIndex = 23;
            this.outQ2.Text = "Not Available";
            // 
            // outQ1
            // 
            this.outQ1.AutoSize = true;
            this.outQ1.Font = new System.Drawing.Font("Microsoft Sans Serif", 10F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.outQ1.ForeColor = System.Drawing.Color.DarkRed;
            this.outQ1.Location = new System.Drawing.Point(50, 67);
            this.outQ1.Name = "outQ1";
            this.outQ1.Size = new System.Drawing.Size(91, 17);
            this.outQ1.TabIndex = 22;
            this.outQ1.Text = "Not Available";
            // 
            // outQ0
            // 
            this.outQ0.AutoSize = true;
            this.outQ0.Font = new System.Drawing.Font("Microsoft Sans Serif", 10F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.outQ0.ForeColor = System.Drawing.Color.DarkRed;
            this.outQ0.Location = new System.Drawing.Point(50, 50);
            this.outQ0.Name = "outQ0";
            this.outQ0.Size = new System.Drawing.Size(88, 17);
            this.outQ0.TabIndex = 21;
            this.outQ0.Text = "Not Availabe";
            // 
            // labelMotor6
            // 
            this.labelMotor6.AutoSize = true;
            this.labelMotor6.Font = new System.Drawing.Font("Microsoft Sans Serif", 10F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.labelMotor6.Location = new System.Drawing.Point(397, 250);
            this.labelMotor6.Name = "labelMotor6";
            this.labelMotor6.Size = new System.Drawing.Size(113, 17);
            this.labelMotor6.TabIndex = 20;
            this.labelMotor6.Text = "Motor #6 Speed:";
            // 
            // labelMotor5
            // 
            this.labelMotor5.AutoSize = true;
            this.labelMotor5.Font = new System.Drawing.Font("Microsoft Sans Serif", 10F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.labelMotor5.Location = new System.Drawing.Point(397, 233);
            this.labelMotor5.Name = "labelMotor5";
            this.labelMotor5.Size = new System.Drawing.Size(113, 17);
            this.labelMotor5.TabIndex = 19;
            this.labelMotor5.Text = "Motor #5 Speed:";
            // 
            // labelMotor4
            // 
            this.labelMotor4.AutoSize = true;
            this.labelMotor4.Font = new System.Drawing.Font("Microsoft Sans Serif", 10F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.labelMotor4.Location = new System.Drawing.Point(397, 216);
            this.labelMotor4.Name = "labelMotor4";
            this.labelMotor4.Size = new System.Drawing.Size(113, 17);
            this.labelMotor4.TabIndex = 18;
            this.labelMotor4.Text = "Motor #4 Speed:";
            // 
            // labelMotor3
            // 
            this.labelMotor3.AutoSize = true;
            this.labelMotor3.Font = new System.Drawing.Font("Microsoft Sans Serif", 10F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.labelMotor3.Location = new System.Drawing.Point(397, 199);
            this.labelMotor3.Name = "labelMotor3";
            this.labelMotor3.Size = new System.Drawing.Size(113, 17);
            this.labelMotor3.TabIndex = 17;
            this.labelMotor3.Text = "Motor #3 Speed:";
            // 
            // labelMotor2
            // 
            this.labelMotor2.AutoSize = true;
            this.labelMotor2.Font = new System.Drawing.Font("Microsoft Sans Serif", 10F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.labelMotor2.Location = new System.Drawing.Point(397, 182);
            this.labelMotor2.Name = "labelMotor2";
            this.labelMotor2.Size = new System.Drawing.Size(113, 17);
            this.labelMotor2.TabIndex = 16;
            this.labelMotor2.Text = "Motor #2 Speed:";
            // 
            // labelMotor1
            // 
            this.labelMotor1.AutoSize = true;
            this.labelMotor1.Font = new System.Drawing.Font("Microsoft Sans Serif", 10F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.labelMotor1.Location = new System.Drawing.Point(397, 165);
            this.labelMotor1.Name = "labelMotor1";
            this.labelMotor1.Size = new System.Drawing.Size(113, 17);
            this.labelMotor1.TabIndex = 15;
            this.labelMotor1.Text = "Motor #1 Speed:";
            // 
            // labelMotorStats
            // 
            this.labelMotorStats.AutoSize = true;
            this.labelMotorStats.Font = new System.Drawing.Font("Microsoft Sans Serif", 18F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.labelMotorStats.Location = new System.Drawing.Point(395, 132);
            this.labelMotorStats.Name = "labelMotorStats";
            this.labelMotorStats.Size = new System.Drawing.Size(176, 29);
            this.labelMotorStats.TabIndex = 14;
            this.labelMotorStats.Text = "Motor Statistics";
            // 
            // labelTemp
            // 
            this.labelTemp.AutoSize = true;
            this.labelTemp.Font = new System.Drawing.Font("Microsoft Sans Serif", 10F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.labelTemp.Location = new System.Drawing.Point(13, 216);
            this.labelTemp.Name = "labelTemp";
            this.labelTemp.Size = new System.Drawing.Size(145, 17);
            this.labelTemp.TabIndex = 13;
            this.labelTemp.Text = "Current Temperature:";
            // 
            // labelStartingPressure
            // 
            this.labelStartingPressure.AutoSize = true;
            this.labelStartingPressure.Font = new System.Drawing.Font("Microsoft Sans Serif", 10F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.labelStartingPressure.Location = new System.Drawing.Point(13, 199);
            this.labelStartingPressure.Name = "labelStartingPressure";
            this.labelStartingPressure.Size = new System.Drawing.Size(122, 17);
            this.labelStartingPressure.TabIndex = 12;
            this.labelStartingPressure.Text = "Starting Pressure:";
            // 
            // labelCurrentPressure
            // 
            this.labelCurrentPressure.AutoSize = true;
            this.labelCurrentPressure.Font = new System.Drawing.Font("Microsoft Sans Serif", 10F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.labelCurrentPressure.Location = new System.Drawing.Point(13, 182);
            this.labelCurrentPressure.Name = "labelCurrentPressure";
            this.labelCurrentPressure.Size = new System.Drawing.Size(120, 17);
            this.labelCurrentPressure.TabIndex = 11;
            this.labelCurrentPressure.Text = "Current Pressure:";
            // 
            // labelAltitude
            // 
            this.labelAltitude.AutoSize = true;
            this.labelAltitude.Font = new System.Drawing.Font("Microsoft Sans Serif", 10F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.labelAltitude.Location = new System.Drawing.Point(13, 165);
            this.labelAltitude.Name = "labelAltitude";
            this.labelAltitude.Size = new System.Drawing.Size(59, 17);
            this.labelAltitude.TabIndex = 10;
            this.labelAltitude.Text = "Altitude:";
            // 
            // labelFlightData
            // 
            this.labelFlightData.AutoSize = true;
            this.labelFlightData.Font = new System.Drawing.Font("Microsoft Sans Serif", 18F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.labelFlightData.Location = new System.Drawing.Point(6, 132);
            this.labelFlightData.Name = "labelFlightData";
            this.labelFlightData.Size = new System.Drawing.Size(128, 29);
            this.labelFlightData.TabIndex = 9;
            this.labelFlightData.Text = "Flight Data";
            // 
            // groupCommands
            // 
            this.groupCommands.Controls.Add(this.ResponseBox);
            this.groupCommands.Controls.Add(this.CommandLine);
            this.groupCommands.Location = new System.Drawing.Point(12, 299);
            this.groupCommands.Name = "groupCommands";
            this.groupCommands.Size = new System.Drawing.Size(731, 395);
            this.groupCommands.TabIndex = 10;
            this.groupCommands.TabStop = false;
            this.groupCommands.Text = "Commands";
            // 
            // ResponseBox
            // 
            this.ResponseBox.DetectUrls = false;
            this.ResponseBox.Location = new System.Drawing.Point(6, 20);
            this.ResponseBox.Name = "ResponseBox";
            this.ResponseBox.ReadOnly = true;
            this.ResponseBox.ScrollBars = System.Windows.Forms.RichTextBoxScrollBars.ForcedVertical;
            this.ResponseBox.Size = new System.Drawing.Size(719, 341);
            this.ResponseBox.TabIndex = 1;
            this.ResponseBox.Text = "Messages from drone...";
            this.ResponseBox.TextChanged += new System.EventHandler(this.ResponseBox_TextChanged);
            // 
            // CommandLine
            // 
            this.CommandLine.ForeColor = System.Drawing.SystemColors.ControlDark;
            this.CommandLine.Location = new System.Drawing.Point(7, 367);
            this.CommandLine.Name = "CommandLine";
            this.CommandLine.Size = new System.Drawing.Size(718, 20);
            this.CommandLine.TabIndex = 0;
            this.CommandLine.Text = "Command...";
            this.CommandLine.GotFocus += new System.EventHandler(this.CommandLine_GotFocus);
            this.CommandLine.KeyDown += new System.Windows.Forms.KeyEventHandler(this.CommandLine_KeyDown);
            this.CommandLine.LostFocus += new System.EventHandler(this.CommandLine_LostFocus);
            // 
            // CommsWorker
            // 
            this.CommsWorker.DoWork += new System.ComponentModel.DoWorkEventHandler(this.CommsWorker_DoWork);
            this.CommsWorker.ProgressChanged += new System.ComponentModel.ProgressChangedEventHandler(this.CommsWorker_ProgressChanged);
            this.CommsWorker.RunWorkerCompleted += new System.ComponentModel.RunWorkerCompletedEventHandler(this.CommsWorker_RunWorkerCompleted);
            // 
            // serialPort
            // 
            this.serialPort.PortName = "COM4";
            // 
            // ControlPanel
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(755, 695);
            this.Controls.Add(this.groupCommands);
            this.Controls.Add(this.groupFlightData);
            this.KeyPreview = true;
            this.Name = "ControlPanel";
            this.Text = "CARE-Drone Control Panel";
            this.Load += new System.EventHandler(this.ControlPanel_Load);
            this.groupFlightData.ResumeLayout(false);
            this.groupFlightData.PerformLayout();
            this.groupCommands.ResumeLayout(false);
            this.groupCommands.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Label labelOrientationQuaternion;
        private System.Windows.Forms.Label labelQ0;
        private System.Windows.Forms.Label labelQ1;
        private System.Windows.Forms.Label labelQ2;
        private System.Windows.Forms.Label labelQ3;
        private System.Windows.Forms.Label labelYaw;
        private System.Windows.Forms.Label labelPitch;
        private System.Windows.Forms.Label labelRoll;
        private System.Windows.Forms.Label labelOrientationEuler;
        private System.Windows.Forms.GroupBox groupFlightData;
        private System.Windows.Forms.Label labelFlightData;
        private System.Windows.Forms.Label labelMotor6;
        private System.Windows.Forms.Label labelMotor5;
        private System.Windows.Forms.Label labelMotor4;
        private System.Windows.Forms.Label labelMotor3;
        private System.Windows.Forms.Label labelMotor2;
        private System.Windows.Forms.Label labelMotor1;
        private System.Windows.Forms.Label labelMotorStats;
        private System.Windows.Forms.Label labelTemp;
        private System.Windows.Forms.Label labelStartingPressure;
        private System.Windows.Forms.Label labelCurrentPressure;
        private System.Windows.Forms.Label labelAltitude;
        private System.Windows.Forms.Label outMotor6;
        private System.Windows.Forms.Label outMotor5;
        private System.Windows.Forms.Label outMotor4;
        private System.Windows.Forms.Label outMotor3;
        private System.Windows.Forms.Label outMotor2;
        private System.Windows.Forms.Label outMotor1;
        private System.Windows.Forms.Label outAltitude;
        private System.Windows.Forms.Label outCurrentPressure;
        private System.Windows.Forms.Label outStartingPressure;
        private System.Windows.Forms.Label outCurrentTemp;
        private System.Windows.Forms.Label outYaw;
        private System.Windows.Forms.Label outPitch;
        private System.Windows.Forms.Label outRoll;
        private System.Windows.Forms.Label outQ3;
        private System.Windows.Forms.Label outQ2;
        private System.Windows.Forms.Label outQ1;
        private System.Windows.Forms.Label outQ0;
        private System.Windows.Forms.GroupBox groupCommands;
        private System.Windows.Forms.RichTextBox ResponseBox;
        private System.Windows.Forms.TextBox CommandLine;
        private System.ComponentModel.BackgroundWorker CommsWorker;
        private System.IO.Ports.SerialPort serialPort;
    }
}

