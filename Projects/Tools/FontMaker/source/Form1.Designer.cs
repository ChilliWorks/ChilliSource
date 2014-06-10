namespace FontTool
{
    partial class Form1
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing) {
            if (disposing && (components != null)) {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent() {
            this.components = new System.ComponentModel.Container();
            this.fontDialog1 = new System.Windows.Forms.FontDialog();
            this.button1 = new System.Windows.Forms.Button();
            this.CharactersToIncludeBox = new System.Windows.Forms.TextBox();
            this.label2 = new System.Windows.Forms.Label();
            this.richTextBox1 = new System.Windows.Forms.RichTextBox();
            this.label1 = new System.Windows.Forms.Label();
            this.button2 = new System.Windows.Forms.Button();
            this.outputNameBox = new System.Windows.Forms.TextBox();
            this.label3 = new System.Windows.Forms.Label();
            this.button3 = new System.Windows.Forms.Button();
            this.fontColourDialog = new System.Windows.Forms.ColorDialog();
            this.enableAntialiasing = new System.Windows.Forms.CheckBox();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.FontColourSample = new System.Windows.Forms.PictureBox();
            this.groupBox2 = new System.Windows.Forms.GroupBox();
            this.PreviewBGColour = new System.Windows.Forms.PictureBox();
            this.PreviewBGButton = new System.Windows.Forms.Button();
            this.AutoPreviewBackColour = new System.Windows.Forms.CheckBox();
            this.fontNameBox = new System.Windows.Forms.TextBox();
            this.enableDropshadows = new System.Windows.Forms.CheckBox();
            this.shadowColourSample = new System.Windows.Forms.PictureBox();
            this.shadowColourButton = new System.Windows.Forms.Button();
            this.shadowXOffsetLabel = new System.Windows.Forms.Label();
            this.shadowYOffsetLabel = new System.Windows.Forms.Label();
            this.groupBox3 = new System.Windows.Forms.GroupBox();
            this.shadowYOffsetText = new System.Windows.Forms.TextBox();
            this.shadowXOffsetText = new System.Windows.Forms.TextBox();
            this.button4 = new System.Windows.Forms.Button();
            this.toolTip1 = new System.Windows.Forms.ToolTip(this.components);
            this.groupBox4 = new System.Windows.Forms.GroupBox();
            this.outlineThicknessText = new System.Windows.Forms.TextBox();
            this.enableOutline = new System.Windows.Forms.CheckBox();
            this.outlineColourButton = new System.Windows.Forms.Button();
            this.outlineThicknessLabel = new System.Windows.Forms.Label();
            this.outlineColourSample = new System.Windows.Forms.PictureBox();
			this.ExportScalableFont	= new System.Windows.Forms.CheckBox();
            this.groupBox5 = new System.Windows.Forms.GroupBox();
            this.button5 = new System.Windows.Forms.Button();
            this.groupBox1.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.FontColourSample)).BeginInit();
            this.groupBox2.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.PreviewBGColour)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.shadowColourSample)).BeginInit();
            this.groupBox3.SuspendLayout();
            this.groupBox4.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.outlineColourSample)).BeginInit();
            this.groupBox5.SuspendLayout();
            this.SuspendLayout();
            // 
            // button1
            // 
            this.button1.Location = new System.Drawing.Point(8, 12);
            this.button1.Name = "button1";
            this.button1.Size = new System.Drawing.Size(75, 23);
            this.button1.TabIndex = 0;
            this.button1.Text = "Pick Font";
            this.button1.UseVisualStyleBackColor = true;
            this.button1.Click += new System.EventHandler(this.OnSelectFontPressed);
            // 
            // CharactersToIncludeBox
            // 
            this.CharactersToIncludeBox.Location = new System.Drawing.Point(8, 106);
            this.CharactersToIncludeBox.Multiline = true;
            this.CharactersToIncludeBox.Name = "CharactersToIncludeBox";
            this.CharactersToIncludeBox.Size = new System.Drawing.Size(474, 55);
            this.CharactersToIncludeBox.TabIndex = 1;
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(9, 90);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(58, 13);
            this.label2.TabIndex = 3;
            this.label2.Text = "Characters";
            // 
            // richTextBox1
            // 
            this.richTextBox1.Location = new System.Drawing.Point(8, 183);
            this.richTextBox1.Name = "richTextBox1";
            this.richTextBox1.ReadOnly = true;
            this.richTextBox1.Size = new System.Drawing.Size(474, 125);
            this.richTextBox1.TabIndex = 4;
            this.richTextBox1.Text = "The quick brown fox jumps over the lazy dog 0 1 2 3 4 5 6 7 8 9";
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(9, 164);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(45, 13);
            this.label1.TabIndex = 5;
            this.label1.Text = "Preview";
            // 
            // button2
            // 
            this.button2.Location = new System.Drawing.Point(523, 20);
            this.button2.Name = "button2";
            this.button2.Size = new System.Drawing.Size(171, 23);
            this.button2.TabIndex = 6;
            this.button2.Text = "Save Individual Images";
            this.toolTip1.SetToolTip(this.button2, "Export each character as an individual png");
            this.button2.UseVisualStyleBackColor = true;
            this.button2.Click += new System.EventHandler(this.OnSavePressed);
            // 
            // outputNameBox
            // 
            this.outputNameBox.Location = new System.Drawing.Point(336, 20);
            this.outputNameBox.Name = "outputNameBox";
            this.outputNameBox.Size = new System.Drawing.Size(146, 20);
            this.outputNameBox.TabIndex = 7;
            this.outputNameBox.Text = "MyFont";
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(333, 5);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(70, 13);
            this.label3.TabIndex = 8;
            this.label3.Text = "Output Name";
            // 
            // button3
            // 
            this.button3.Location = new System.Drawing.Point(8, 41);
            this.button3.Name = "button3";
            this.button3.Size = new System.Drawing.Size(75, 23);
            this.button3.TabIndex = 9;
            this.button3.Text = "Pick Colour";
            this.button3.UseVisualStyleBackColor = true;
            this.button3.Click += new System.EventHandler(this.OnPickColour);
            // 
            // enableAntialiasing
            // 
            this.enableAntialiasing.AutoSize = true;
            this.enableAntialiasing.Checked = true;
            this.enableAntialiasing.CheckState = System.Windows.Forms.CheckState.Checked;
            this.enableAntialiasing.Location = new System.Drawing.Point(6, 19);
            this.enableAntialiasing.Name = "enableAntialiasing";
            this.enableAntialiasing.Size = new System.Drawing.Size(101, 17);
            this.enableAntialiasing.TabIndex = 10;
            this.enableAntialiasing.Text = "Use Antialiasing";
            this.toolTip1.SetToolTip(this.enableAntialiasing, "For small font sizes antialiasing may cause blurriness");
            this.enableAntialiasing.UseVisualStyleBackColor = true;
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.enableAntialiasing);
            this.groupBox1.Location = new System.Drawing.Point(488, 100);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(206, 50);
            this.groupBox1.TabIndex = 11;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "Options";
            // 
            // FontColourSample
            // 
            this.FontColourSample.BackColor = System.Drawing.Color.White;
            this.FontColourSample.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.FontColourSample.Location = new System.Drawing.Point(89, 41);
            this.FontColourSample.Name = "FontColourSample";
            this.FontColourSample.Size = new System.Drawing.Size(75, 32);
            this.FontColourSample.TabIndex = 12;
            this.FontColourSample.TabStop = false;
            this.FontColourSample.Click += new System.EventHandler(this.TextColorSample_Click);
            // 
            // groupBox2
            // 
            this.groupBox2.Controls.Add(this.PreviewBGColour);
            this.groupBox2.Controls.Add(this.PreviewBGButton);
            this.groupBox2.Controls.Add(this.AutoPreviewBackColour);
            this.groupBox2.Location = new System.Drawing.Point(8, 322);
            this.groupBox2.Name = "groupBox2";
            this.groupBox2.Size = new System.Drawing.Size(231, 106);
            this.groupBox2.TabIndex = 13;
            this.groupBox2.TabStop = false;
            this.groupBox2.Text = "Tool Options";
            // 
            // PreviewBGColour
            // 
            this.PreviewBGColour.BackColor = System.Drawing.Color.White;
            this.PreviewBGColour.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.PreviewBGColour.Enabled = false;
            this.PreviewBGColour.Location = new System.Drawing.Point(6, 68);
            this.PreviewBGColour.Name = "PreviewBGColour";
            this.PreviewBGColour.Size = new System.Drawing.Size(125, 32);
            this.PreviewBGColour.TabIndex = 15;
            this.PreviewBGColour.TabStop = false;
            this.PreviewBGColour.Click += new System.EventHandler(this.PresentPreviewBGColour);
            // 
            // PreviewBGButton
            // 
            this.PreviewBGButton.Enabled = false;
            this.PreviewBGButton.Location = new System.Drawing.Point(6, 39);
            this.PreviewBGButton.Name = "PreviewBGButton";
            this.PreviewBGButton.Size = new System.Drawing.Size(125, 23);
            this.PreviewBGButton.TabIndex = 14;
            this.PreviewBGButton.Text = "Preview Back Colour";
            this.PreviewBGButton.UseVisualStyleBackColor = true;
            this.PreviewBGButton.Click += new System.EventHandler(this.PresentPreviewBGColour);
            // 
            // AutoPreviewBackColour
            // 
            this.AutoPreviewBackColour.AutoSize = true;
            this.AutoPreviewBackColour.Checked = true;
            this.AutoPreviewBackColour.CheckState = System.Windows.Forms.CheckState.Checked;
            this.AutoPreviewBackColour.Location = new System.Drawing.Point(6, 22);
            this.AutoPreviewBackColour.Name = "AutoPreviewBackColour";
            this.AutoPreviewBackColour.Size = new System.Drawing.Size(172, 17);
            this.AutoPreviewBackColour.TabIndex = 0;
            this.AutoPreviewBackColour.Text = "Automatic preview back colour";
            this.AutoPreviewBackColour.UseVisualStyleBackColor = true;
            this.AutoPreviewBackColour.CheckedChanged += new System.EventHandler(this.AutoPreviewColour_CheckedChanged);
            // 
            // fontNameBox
            // 
            this.fontNameBox.Location = new System.Drawing.Point(89, 12);
            this.fontNameBox.Name = "fontNameBox";
            this.fontNameBox.ReadOnly = true;
            this.fontNameBox.Size = new System.Drawing.Size(169, 20);
            this.fontNameBox.TabIndex = 14;
            // 
            // enableDropshadows
            // 
            this.enableDropshadows.AutoSize = true;
            this.enableDropshadows.Location = new System.Drawing.Point(6, 19);
            this.enableDropshadows.Name = "enableDropshadows";
            this.enableDropshadows.Size = new System.Drawing.Size(120, 17);
            this.enableDropshadows.TabIndex = 11;
            this.enableDropshadows.Text = "Create Dropshadow";
            this.enableDropshadows.UseVisualStyleBackColor = true;
            this.enableDropshadows.CheckedChanged += new System.EventHandler(this.EnableDropshadows_CheckedChanged);
            // 
            // shadowColourSample
            // 
            this.shadowColourSample.BackColor = System.Drawing.Color.DimGray;
            this.shadowColourSample.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.shadowColourSample.Enabled = false;
            this.shadowColourSample.Location = new System.Drawing.Point(100, 42);
            this.shadowColourSample.Name = "shadowColourSample";
            this.shadowColourSample.Size = new System.Drawing.Size(88, 32);
            this.shadowColourSample.TabIndex = 17;
            this.shadowColourSample.TabStop = false;
            this.shadowColourSample.Click += new System.EventHandler(this.PresentShadowColour);
            // 
            // shadowColourButton
            // 
            this.shadowColourButton.Enabled = false;
            this.shadowColourButton.Location = new System.Drawing.Point(6, 42);
            this.shadowColourButton.Name = "shadowColourButton";
            this.shadowColourButton.Size = new System.Drawing.Size(88, 23);
            this.shadowColourButton.TabIndex = 16;
            this.shadowColourButton.Text = "Shadow Colour";
            this.shadowColourButton.UseVisualStyleBackColor = true;
            this.shadowColourButton.Click += new System.EventHandler(this.PresentShadowColour);
            // 
            // shadowXOffsetLabel
            // 
            this.shadowXOffsetLabel.AutoSize = true;
            this.shadowXOffsetLabel.Enabled = false;
            this.shadowXOffsetLabel.Location = new System.Drawing.Point(7, 80);
            this.shadowXOffsetLabel.Name = "shadowXOffsetLabel";
            this.shadowXOffsetLabel.Size = new System.Drawing.Size(45, 13);
            this.shadowXOffsetLabel.TabIndex = 19;
            this.shadowXOffsetLabel.Text = "X Offset";
            // 
            // shadowYOffsetLabel
            // 
            this.shadowYOffsetLabel.AutoSize = true;
            this.shadowYOffsetLabel.Enabled = false;
            this.shadowYOffsetLabel.Location = new System.Drawing.Point(108, 80);
            this.shadowYOffsetLabel.Name = "shadowYOffsetLabel";
            this.shadowYOffsetLabel.Size = new System.Drawing.Size(45, 13);
            this.shadowYOffsetLabel.TabIndex = 21;
            this.shadowYOffsetLabel.Text = "Y Offset";
            // 
            // groupBox3
            // 
            this.groupBox3.Controls.Add(this.shadowYOffsetText);
            this.groupBox3.Controls.Add(this.shadowXOffsetText);
            this.groupBox3.Controls.Add(this.shadowYOffsetLabel);
            this.groupBox3.Controls.Add(this.enableDropshadows);
            this.groupBox3.Controls.Add(this.shadowColourButton);
            this.groupBox3.Controls.Add(this.shadowXOffsetLabel);
            this.groupBox3.Controls.Add(this.shadowColourSample);
            this.groupBox3.Location = new System.Drawing.Point(488, 156);
            this.groupBox3.Name = "groupBox3";
            this.groupBox3.Size = new System.Drawing.Size(206, 154);
            this.groupBox3.TabIndex = 15;
            this.groupBox3.TabStop = false;
            this.groupBox3.Text = "Shadow";
            // 
            // shadowYOffsetText
            // 
            this.shadowYOffsetText.Enabled = false;
            this.shadowYOffsetText.Location = new System.Drawing.Point(111, 96);
            this.shadowYOffsetText.Name = "shadowYOffsetText";
            this.shadowYOffsetText.Size = new System.Drawing.Size(84, 20);
            this.shadowYOffsetText.TabIndex = 23;
            this.shadowYOffsetText.Text = "1";
            this.shadowYOffsetText.TextChanged += new System.EventHandler(this.ShadowYOffsetText_TextChanged);
            // 
            // shadowXOffsetText
            // 
            this.shadowXOffsetText.Enabled = false;
            this.shadowXOffsetText.Location = new System.Drawing.Point(10, 97);
            this.shadowXOffsetText.Name = "shadowXOffsetText";
            this.shadowXOffsetText.Size = new System.Drawing.Size(84, 20);
            this.shadowXOffsetText.TabIndex = 22;
            this.shadowXOffsetText.Text = "1";
            this.shadowXOffsetText.TextChanged += new System.EventHandler(this.ShadowXOffsetText_TextChanged);
            // 
            // button4
            // 
            this.button4.Location = new System.Drawing.Point(523, 78);
            this.button4.Name = "button4";
            this.button4.Size = new System.Drawing.Size(171, 23);
            this.button4.TabIndex = 16;
            this.button4.Text = "Export Current to moFlow";
            this.toolTip1.SetToolTip(this.button4, "Uses the SpriteTool to export into moFlow format");
            this.button4.UseVisualStyleBackColor = true;
            this.button4.Click += new System.EventHandler(this.OnExportPressed);
            // 
            // groupBox4
            // 
            this.groupBox4.Controls.Add(this.outlineThicknessText);
            this.groupBox4.Controls.Add(this.enableOutline);
            this.groupBox4.Controls.Add(this.outlineColourButton);
            this.groupBox4.Controls.Add(this.outlineThicknessLabel);
            this.groupBox4.Controls.Add(this.outlineColourSample);
            this.groupBox4.Location = new System.Drawing.Point(488, 316);
            this.groupBox4.Name = "groupBox4";
            this.groupBox4.Size = new System.Drawing.Size(206, 81);
            this.groupBox4.TabIndex = 17;
            this.groupBox4.TabStop = false;
            this.groupBox4.Text = "Outline";
            // 
            // outlineThicknessText
            // 
            this.outlineThicknessText.Enabled = false;
            this.outlineThicknessText.Location = new System.Drawing.Point(10, 97);
            this.outlineThicknessText.Name = "outlineThicknessText";
            this.outlineThicknessText.Size = new System.Drawing.Size(84, 20);
            this.outlineThicknessText.TabIndex = 22;
            this.outlineThicknessText.Text = "1";
            this.outlineThicknessText.Visible = false;
            // 
            // enableOutline
            // 
            this.enableOutline.AutoSize = true;
            this.enableOutline.Location = new System.Drawing.Point(6, 19);
            this.enableOutline.Name = "enableOutline";
            this.enableOutline.Size = new System.Drawing.Size(93, 17);
            this.enableOutline.TabIndex = 11;
            this.enableOutline.Text = "Create Outline";
            this.enableOutline.UseVisualStyleBackColor = true;
            this.enableOutline.CheckedChanged += new System.EventHandler(this.EnableOutline_CheckedChanged);
            // 
            // outlineColourButton
            // 
            this.outlineColourButton.Enabled = false;
            this.outlineColourButton.Location = new System.Drawing.Point(6, 42);
            this.outlineColourButton.Name = "outlineColourButton";
            this.outlineColourButton.Size = new System.Drawing.Size(88, 23);
            this.outlineColourButton.TabIndex = 16;
            this.outlineColourButton.Text = "Outline Colour";
            this.outlineColourButton.UseVisualStyleBackColor = true;
            this.outlineColourButton.Click += new System.EventHandler(this.PresentOutlineColour);
            // 
            // outlineThicknessLabel
            // 
            this.outlineThicknessLabel.AutoSize = true;
            this.outlineThicknessLabel.Enabled = false;
            this.outlineThicknessLabel.Location = new System.Drawing.Point(7, 80);
            this.outlineThicknessLabel.Name = "outlineThicknessLabel";
            this.outlineThicknessLabel.Size = new System.Drawing.Size(56, 13);
            this.outlineThicknessLabel.TabIndex = 19;
            this.outlineThicknessLabel.Text = "Thickness";
            this.outlineThicknessLabel.Visible = false;
            // 
            // outlineColourSample
            // 
            this.outlineColourSample.BackColor = System.Drawing.Color.DimGray;
            this.outlineColourSample.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.outlineColourSample.Enabled = false;
            this.outlineColourSample.Location = new System.Drawing.Point(100, 42);
            this.outlineColourSample.Name = "outlineColourSample";
            this.outlineColourSample.Size = new System.Drawing.Size(88, 32);
            this.outlineColourSample.TabIndex = 17;
            this.outlineColourSample.TabStop = false;
            this.outlineColourSample.Click += new System.EventHandler(this.PresentOutlineColour);
			// 
			// ExportScalableFont
			// 
			this.ExportScalableFont.AutoSize = true;
			this.ExportScalableFont.Checked = false;
			this.ExportScalableFont.CheckState = System.Windows.Forms.CheckState.Unchecked;
			this.ExportScalableFont.Location = new System.Drawing.Point(6, 19);
			this.ExportScalableFont.Name = "exportScalableFont";
			this.ExportScalableFont.Size = new System.Drawing.Size(101, 17);
			this.ExportScalableFont.TabIndex = 10;
			this.ExportScalableFont.Text = "Export Scalable Font";
			this.toolTip1.SetToolTip(this.ExportScalableFont, "Converts font to a format that can be scaled");
			this.ExportScalableFont.UseVisualStyleBackColor = true;
			this.ExportScalableFont.CheckedChanged += new System.EventHandler(this.OnChangeExportScalable);
            // 
            // groupBox5
            // 
			this.groupBox5.Controls.Add(this.ExportScalableFont);
            this.groupBox5.Location = new System.Drawing.Point(251, 322);
            this.groupBox5.Name = "groupBox5";
            this.groupBox5.Size = new System.Drawing.Size(231, 106);
            this.groupBox5.TabIndex = 18;
            this.groupBox5.TabStop = false;
            this.groupBox5.Text = "Export Options";
            // 
            // button5
            // 
            this.button5.Location = new System.Drawing.Point(523, 49);
            this.button5.Name = "button5";
            this.button5.Size = new System.Drawing.Size(171, 23);
            this.button5.TabIndex = 19;
            this.button5.Text = "Export from Saved Images";
            this.toolTip1.SetToolTip(this.button5, "Uses the SpriteTool to export into moFlow format");
            this.button5.UseVisualStyleBackColor = true;
            this.button5.Click += new System.EventHandler(this.OnExportPreviousPressed);
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(700, 435);
            this.Controls.Add(this.button5);
            this.Controls.Add(this.groupBox5);
            this.Controls.Add(this.groupBox4);
            this.Controls.Add(this.button4);
            this.Controls.Add(this.groupBox3);
            this.Controls.Add(this.fontNameBox);
            this.Controls.Add(this.groupBox2);
            this.Controls.Add(this.FontColourSample);
            this.Controls.Add(this.groupBox1);
            this.Controls.Add(this.button3);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.outputNameBox);
            this.Controls.Add(this.button2);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.richTextBox1);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.CharactersToIncludeBox);
            this.Controls.Add(this.button1);
            this.Name = "Form1";
            this.Text = "Font Maker";
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.FontColourSample)).EndInit();
            this.groupBox2.ResumeLayout(false);
            this.groupBox2.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.PreviewBGColour)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.shadowColourSample)).EndInit();
            this.groupBox3.ResumeLayout(false);
            this.groupBox3.PerformLayout();
            this.groupBox4.ResumeLayout(false);
            this.groupBox4.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.outlineColourSample)).EndInit();
            this.groupBox5.ResumeLayout(false);
            this.groupBox5.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.FontDialog fontDialog1;
        private System.Windows.Forms.Button button1;
        private System.Windows.Forms.TextBox CharactersToIncludeBox;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.RichTextBox richTextBox1;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Button button2;
        private System.Windows.Forms.TextBox outputNameBox;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Button button3;
        private System.Windows.Forms.ColorDialog fontColourDialog;
        private System.Windows.Forms.CheckBox enableAntialiasing;
        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.PictureBox FontColourSample;
        private System.Windows.Forms.GroupBox groupBox2;
        private System.Windows.Forms.PictureBox PreviewBGColour;
        private System.Windows.Forms.Button PreviewBGButton;
        private System.Windows.Forms.CheckBox AutoPreviewBackColour;
        private System.Windows.Forms.TextBox fontNameBox;
        private System.Windows.Forms.CheckBox enableDropshadows;
        private System.Windows.Forms.PictureBox shadowColourSample;
        private System.Windows.Forms.Button shadowColourButton;
        private System.Windows.Forms.Label shadowXOffsetLabel;
        private System.Windows.Forms.Label shadowYOffsetLabel;
        private System.Windows.Forms.GroupBox groupBox3;
        private System.Windows.Forms.TextBox shadowYOffsetText;
        private System.Windows.Forms.TextBox shadowXOffsetText;
        private System.Windows.Forms.ToolTip toolTip1;
        private System.Windows.Forms.Button button4;
        private System.Windows.Forms.GroupBox groupBox4;
        private System.Windows.Forms.CheckBox enableOutline;
        private System.Windows.Forms.Button outlineColourButton;
        private System.Windows.Forms.PictureBox outlineColourSample;
        private System.Windows.Forms.TextBox outlineThicknessText;
        private System.Windows.Forms.Label outlineThicknessLabel;
		private System.Windows.Forms.CheckBox ExportScalableFont;
        private System.Windows.Forms.GroupBox groupBox5;
        private System.Windows.Forms.Button button5;
    }
}

