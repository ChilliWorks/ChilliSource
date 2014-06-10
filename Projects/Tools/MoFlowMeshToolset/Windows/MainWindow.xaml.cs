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
using MoFlowMeshToolset.Classes;

namespace MoFlowMeshToolset
{
    /// <summary>
    /// Enum stating the output type from the converter
    /// </summary>
    enum ConvertType
    {
        NONE,
        MOMODEL,
        MOANIM,
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
        CMoModelConverter mMoModelConverter;
        CMoAnimConverter mMoAnimConverter;
        CMoCollisionConverter mMoCollisionConverter;
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
            mMoModelConverter = new CMoModelConverter(this);
            mMoAnimConverter = new CMoAnimConverter(this);
            mMoCollisionConverter = new CMoCollisionConverter(this);
            InitializeComponent();
            InitialiseEvents();
        }
        /// <summary>
        /// Adds event callbacks.
        /// </summary>
        private void InitialiseEvents()
        {
            CTBMoModel.Checked += OnMoModelChecked;
            CTBMoAnim.Checked += OnMoAnimChecked;
            CTBMoCollision.Checked += OnMoCollisionChecked;

            CTBMoModel.Unchecked += OnMoModelUnchecked;
            CTBMoAnim.Unchecked += OnMoAnimUnchecked;
            CTBMoCollision.Unchecked += OnMoCollisionUnchecked;
        }
        /// <summary>
        /// Events called when the "MoModel" box is checked
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void OnMoModelChecked(object sender, RoutedEventArgs e)
        {
            SetConvertType(ConvertType.MOMODEL);
        }
        /// <summary>
        /// Events called when the "MoModel" box is unchecked
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void OnMoModelUnchecked(object sender, RoutedEventArgs e)
        {
            if (meConvertType == ConvertType.MOMODEL)
                SetConvertType(ConvertType.NONE);
        }
        /// <summary>
        /// Event called when the "MoAnim" box is checked
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void OnMoAnimChecked(object sender, RoutedEventArgs e)
        {
            SetConvertType(ConvertType.MOANIM);
        }
        /// <summary>
        /// Events called when the "MoAnim" box is unchecked
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void OnMoAnimUnchecked(object sender, RoutedEventArgs e)
        {
            if (meConvertType == ConvertType.MOANIM)
                SetConvertType(ConvertType.NONE);
        }
        /// <summary>
        /// Event called when the "MoCollision" box is checked
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void OnMoCollisionChecked(object sender, RoutedEventArgs e)
        {
            SetConvertType(ConvertType.MOCOLLISION);
        }
        /// <summary>
        /// Events called when the "MoCollision" box is unchecked
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void OnMoCollisionUnchecked(object sender, RoutedEventArgs e)
        {
            if (meConvertType == ConvertType.MOCOLLISION)
                SetConvertType(ConvertType.NONE);
        }
        /// <summary>
        /// Returns the option set for a MoStatic conversion
        /// </summary>
        /// <returns></returns>
        private MoModelOptions GetMoModelOptions()
        {
            MoModelOptions options = new MoModelOptions();
            options.mbHasTexture = false;
            options.mbHasMaterial = (bool)MoModelHasMaterial.IsChecked;
            options.mbHasAnimationData = (bool)MoModelHasAnimation.IsChecked;
            options.mbVertexHasPosition = true;
            options.mbVertexHasNormals = (bool)MoModelHasNormals.IsChecked;
            options.mbVertexHasTextureCoordinates = (bool)MoModelHasUVs.IsChecked;
            options.mbVertexHasVertexColours = (bool)MoModelHasVertexColours.IsChecked;
            options.mbVertexHasWeights = (bool)MoModelHasAnimation.IsChecked;
            options.mbVertexHasJointIndices = (bool)MoModelHasAnimation.IsChecked;
            options.mfScale = 1.0f;
            options.mbFlipVerticalTexCoords = (bool)MoModelFlipVerticalTexCoords.IsChecked;
            options.mbSwapYAndZ = (bool)MoModelSwapYAndZ.IsChecked;
            options.mbMirrorInXZPlane = (bool)MoModelMirrorInXZPlane.IsChecked;
            options.mbCombineMeshes = (bool)MoModelCombineMeshes.IsChecked;
            return options;
        }
        /// <summary>
        /// Returns the option set for a MoAnim conversion
        /// </summary>
        /// <returns></returns>
        private MoAnimOptions GetMoAnimOptions()
        {
            MoAnimOptions options = new MoAnimOptions();
            options.mbMirrorInXZPlane = (bool)MoAnimMirrorInXZPlane.IsChecked;
            options.mbSwapYAndZ = (bool)MoAnimSwapYAndZ.IsChecked;
            return options;
        }
        /// <summary>
        /// Returns the option set for a MoCollision conversion
        /// </summary>
        /// <returns></returns>
        private MoCollisionOptions GetMoCollisionOptions()
        {
            MoCollisionOptions options = new MoCollisionOptions();
            options.mbMirrorInXZPlane = (bool)MoCollisionMirrorInXZPlane.IsChecked;
            options.mbSwapYAndZ = (bool)MoCollisionSwapYAndZ.IsChecked;
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
                    case ConvertType.MOMODEL:
                        mMoModelConverter.ConvertToMoModel(astrPreparedFilenames, GetMoModelOptions());
                        break;
                    case ConvertType.MOANIM:
                        mMoAnimConverter.ConvertToMoAnim(astrPreparedFilenames, GetMoAnimOptions());
                        break;
                    case ConvertType.MOCOLLISION:
                        mMoCollisionConverter.ConvertToMoCollision(astrPreparedFilenames, GetMoCollisionOptions());
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
            if (ineConvertType != ConvertType.MOMODEL)
                CTBMoModel.IsChecked = false;
            if (ineConvertType != ConvertType.MOANIM)
                CTBMoAnim.IsChecked = false;
            if (ineConvertType != ConvertType.MOCOLLISION)
                CTBMoCollision.IsChecked = false;

            //collapse all
            MoModelOptionsBar.Visibility = Visibility.Collapsed;
            MoAnimOptionsBar.Visibility = Visibility.Collapsed;
            MoCollisionOptionsBar.Visibility = Visibility.Collapsed;
            SetDragWindowType(DragWindowType.EMPTY);

            //open the correct options window
            switch (ineConvertType)
            {
                case ConvertType.MOMODEL:
                    MoModelOptionsBar.Visibility = Visibility.Visible;
                    SetDragWindowType(DragWindowType.DAE);
                    break;
                case ConvertType.MOANIM:
                    MoAnimOptionsBar.Visibility = Visibility.Visible;
                    SetDragWindowType(DragWindowType.DAE);
                    break;
                case ConvertType.MOCOLLISION:
                    MoCollisionOptionsBar.Visibility = Visibility.Visible;
                    SetDragWindowType(DragWindowType.DAE);
                    break;
                case ConvertType.NONE:
                    SetDragWindowType(DragWindowType.EMPTY);
                    break;
                default:
                    goto case ConvertType.NONE;
            }
        }
    }
}