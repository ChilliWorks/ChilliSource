/**
 * MainWindow.xaml.cs
 * Chilli Source
 * Created by Ian Copland on 01/02/2013.
 * 
 * The MIT License (MIT)
 * 
 * Copyright (c) 2013 Tag Games Limited
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using System.Windows.Threading;
using System.Threading;
using System.Diagnostics;
using CSModelExporter.Classes;

namespace CSModelExporter
{
    /// <summary>
    /// Enum stating the output type from the converter
    /// </summary>
    enum ConvertType
    {
        NONE,
        CSMODEL,
        CSANIM,
        MOCOLLISION
    };
    /// <summary>
    /// Enum describing the type of drag window
    /// </summary>
    enum DragWindowType
    {
        EMPTY,
        DAE,
    };
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        //private data
        ConvertType meConvertType;
        CSModelConverter mCSModelConverter;
        CSAnimConverter mCSAnimConverter;
        Dispatcher mMainThreadDispatcher;

        //delegates
        public delegate void AddTextDelegate(string s);
        public delegate void ProgressDelegate();

        /// <summary>
        /// Constructor
        /// </summary>
        public MainWindow()
        {
            mMainThreadDispatcher = Dispatcher.CurrentDispatcher;
            meConvertType = ConvertType.NONE;
            mCSModelConverter = new CSModelConverter(this);
            mCSAnimConverter = new CSAnimConverter(this);
            InitializeComponent();
            InitialiseEvents();
        }
        /// <summary>
        /// Adds event callbacks.
        /// </summary>
        private void InitialiseEvents()
        {
            CTBCSModel.Checked += OnCSModelChecked;
            CTBCSAnim.Checked += OnCSAnimChecked;

            CTBCSModel.Unchecked += OnCSModelUnchecked;
            CTBCSAnim.Unchecked += OnCSAnimUnchecked;
        }
        /// <summary>
        /// Events called when the "CSModel" box is checked
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void OnCSModelChecked(object sender, RoutedEventArgs e)
        {
            SetConvertType(ConvertType.CSMODEL);
        }
        /// <summary>
        /// Events called when the "CSModel" box is unchecked
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void OnCSModelUnchecked(object sender, RoutedEventArgs e)
        {
            if (meConvertType == ConvertType.CSMODEL)
                SetConvertType(ConvertType.NONE);
        }
        /// <summary>
        /// Event called when the "CSAnim" box is checked
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void OnCSAnimChecked(object sender, RoutedEventArgs e)
        {
            SetConvertType(ConvertType.CSANIM);
        }
        /// <summary>
        /// Events called when the "CSAnim" box is unchecked
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void OnCSAnimUnchecked(object sender, RoutedEventArgs e)
        {
            if (meConvertType == ConvertType.CSANIM)
                SetConvertType(ConvertType.NONE);
        }
        /// <summary>
        /// Returns the option set for a MoStatic conversion
        /// </summary>
        /// <returns></returns>
        private CSModelOptions GetCSModelOptions()
        {
            CSModelOptions options = new CSModelOptions();
            options.mbHasAnimationData = (bool)CSModelHasAnimation.IsChecked;
            options.mbVertexHasPosition = true;
            options.mbVertexHasNormals = (bool)CSModelHasNormals.IsChecked;
            options.mbVertexHasTextureCoordinates = (bool)CSModelHasUVs.IsChecked;
            options.mbVertexHasVertexColours = (bool)CSModelHasVertexColours.IsChecked;
            options.mbVertexHasWeights = (bool)CSModelHasAnimation.IsChecked;
            options.mbVertexHasJointIndices = (bool)CSModelHasAnimation.IsChecked;
            options.mfScale = 1.0f;
            options.mbFlipVerticalTexCoords = (bool)CSModelFlipVerticalTexCoords.IsChecked;
            options.mbSwapYAndZ = (bool)CSModelSwapYAndZ.IsChecked;
            options.mbCombineMeshes = (bool)CSModelCombineMeshes.IsChecked;
            return options;
        }
        /// <summary>
        /// Returns the option set for a CSAnim conversion
        /// </summary>
        /// <returns></returns>
        private CSAnimOptions GetCSAnimOptions()
        {
            CSAnimOptions options = new CSAnimOptions();
            options.mbSwapYAndZ = (bool)CSAnimSwapYAndZ.IsChecked;
            return options;
        }
        /// <summary>
        /// Event called when Files are dropped onto the "Drop files here" text.
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        public void OnFilesDropped(object sender, DragEventArgs e)
        {
            if (e.Data.GetDataPresent(DataFormats.FileDrop))
            {
                //build list of files
                string[] astrFilenames = (string[])e.Data.GetData(DataFormats.FileDrop);

                //prepare the file names: insure "/" instead of "\\".
                string[] astrPreparedFilenames = new string[astrFilenames.Length];
                for (int i = 0; i < astrFilenames.Length; i++)
                {
                    astrPreparedFilenames[i] = astrFilenames[i].Replace("\\", "/");
                }

                //output!
                switch (meConvertType)
                {
                    case ConvertType.NONE:
                        WriteLineToOutput("No conversion type selected.");
                        break;
                    case ConvertType.CSMODEL:
                        mCSModelConverter.ConvertToCSModel(astrPreparedFilenames, GetCSModelOptions());
                        break;
                    case ConvertType.CSANIM:
                        mCSAnimConverter.ConvertToCSAnim(astrPreparedFilenames, GetCSAnimOptions());
                        break;
                    default:
                        goto case ConvertType.NONE;
                }
            }
        }
        /// <summary>
        /// 
        /// </summary>
        /// <returns>The main thread dispatcher</returns>
        public Dispatcher GetMainThreadDispatcher()
        {
            return mMainThreadDispatcher;
        }
        /// <summary>
        /// Prepares the progress bar and starts to display it.
        /// </summary>
        /// <param name="_segments"></param>
        public void StartProgressBar(double _segments)
        {
            ProgressIndicator.Visibility = System.Windows.Visibility.Visible;

            this.ProgressBar.Maximum = _segments;
            this.ProgressBar.Minimum = 0;
            this.ProgressBar.Value = 0;
        }
        /// <summary>
        /// Incriments the progress bar.
        /// </summary>
        public void IncrimentProgressBar()
        {
            this.ProgressBar.Value += 1.0f;
        }
        /// <summary>
        /// Ends the progress bar, and hides it.
        /// </summary>
        public void EndProgressBar()
        {
            ProgressIndicator.Visibility = System.Windows.Visibility.Collapsed;
        }
        /// <summary>
        /// Adds a line of text to the output window. This must be called from
        /// the main thread.
        /// </summary>
        /// <param name="instrText"></param>
        public void WriteLineToOutput(string instrText)
        {
            OutputTextWindow.Text += instrText + "\n";

            OutputScrollWindow.UpdateLayout();
            OutputScrollWindow.ScrollToVerticalOffset(double.MaxValue);
        }
        /// <summary>
        /// Clears the output window of all text
        /// </summary>
        public void ClearOutputWindow()
        {
            OutputTextWindow.Text = " ";

            OutputScrollWindow.UpdateLayout();
            OutputScrollWindow.ScrollToVerticalOffset(double.MaxValue);
        }
        /// <summary>
        /// Sets the drag window type and displays only the requested
        /// drag window.
        /// </summary>
        /// <param name="ineDragWindowType"></param>
        private void SetDragWindowType(DragWindowType ineDragWindowType)
        {
            //empty
            if (ineDragWindowType == DragWindowType.EMPTY)
                EmptyDragWindow.Visibility = Visibility.Visible;
            else
                EmptyDragWindow.Visibility = Visibility.Collapsed;

            //DAE
            if (ineDragWindowType == DragWindowType.DAE)
                DragDAEWindow.Visibility = Visibility.Visible;
            else
                DragDAEWindow.Visibility = Visibility.Collapsed;
        }
        /// <summary>
        /// Sets the current convert type. This will update the UI to
        /// display the items for this convert type.
        /// </summary>
        /// <param name="ineConvertType"></param>
        private void SetConvertType(ConvertType ineConvertType)
        {
            meConvertType = ineConvertType;

            //uncheck others
            if (ineConvertType != ConvertType.CSMODEL)
                CTBCSModel.IsChecked = false;
            if (ineConvertType != ConvertType.CSANIM)
                CTBCSAnim.IsChecked = false;

            //collapse all
            CSModelOptionsBar.Visibility = Visibility.Collapsed;
            CSAnimOptionsBar.Visibility = Visibility.Collapsed;
            SetDragWindowType(DragWindowType.EMPTY);

            //open the correct options window
            switch (ineConvertType)
            {
                case ConvertType.CSMODEL:
                    CSModelOptionsBar.Visibility = Visibility.Visible;
                    SetDragWindowType(DragWindowType.DAE);
                    break;
                case ConvertType.CSANIM:
                    CSAnimOptionsBar.Visibility = Visibility.Visible;
                    SetDragWindowType(DragWindowType.DAE);
                    break;
                case ConvertType.NONE:
                    SetDragWindowType(DragWindowType.EMPTY);
                    break;
                default:
                    goto case ConvertType.NONE;
            }
        }

        private void CTBCSModel_Checked(object sender, RoutedEventArgs e)
        {

        }
    }
}