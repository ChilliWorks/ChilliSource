using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using System.Diagnostics;
using System.Windows.Threading;
using System.IO;

namespace MoFlowMeshToolset.Classes
{
    /// <summary>
    /// struct for storing all the options for converting to mocollision
    /// </summary>
    public struct MoCollisionOptions
    {
        public bool mbMirrorInXZPlane;
        public bool mbSwapYAndZ;
    }
    public class CMoCollisionConverter
    {
        //constants
        const String kstrPathToMoModelTool = "Tools/ColladaToMoCollisionConverter/ColladaToMoCollisionConverter.jar";
        const String kstrPathOutput = "Output/CollisionMeshes/";

        //private data
        MainWindow mMainWindow;
        bool mbTaskActive;
        string[] mastrFilenames;
        MoCollisionOptions mOptions;
        string mstrCurrentStandardError;

        /// <summary>
        /// Constructor
        /// </summary>
        public CMoCollisionConverter(MainWindow inParent)
        {
            mMainWindow = inParent;
            mbTaskActive = false;
        }
        /// <summary>
        /// Adds a line of text to the output window. This can be called
        /// from any thread.
        /// </summary>
        /// <param name="instrText"></param>
        public void WriteLineToOutput(string instrText)
        {
            mMainWindow.GetMainThreadDispatcher().BeginInvoke(new MainWindow.AddTextDelegate(mMainWindow.WriteLineToOutput), instrText);
        }
        /// <summary>
        /// Event called when there is new data from the processes std out
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void ReadStandardOutputEvent(object sender, DataReceivedEventArgs e)
        {
            WriteLineToOutput(e.Data);
        }
        /// <summary>
        /// event received when there is new data from the process std error
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void ReadStandardErrorEvent(object sender, DataReceivedEventArgs e)
        {
            mstrCurrentStandardError += e.Data + "\n";
        }
        /// <summary>
        /// Takes each of the passed in files and runs them through the MoModelToMoCollisionConverter, with the user selected options.
        /// </summary>
        /// <param name="inastrFilenames"></param>
        public void ConvertToMoCollision(string[] inastrFilenames, MoCollisionOptions inOptions)
        {
            if (mbTaskActive == false)
            {
                mOptions = inOptions;

                //start progress bar
                mMainWindow.StartProgressBar((double)inastrFilenames.Length);

                //clear the current output window
                mMainWindow.ClearOutputWindow();

                //take a copy of the data to be used for conversion
                mastrFilenames = new string[inastrFilenames.Length];
                inastrFilenames.CopyTo(mastrFilenames, 0);

                //start the task and flag the background task as running.
                mbTaskActive = true;
                ThreadPool.QueueUserWorkItem(this.MoCollisionConversionTask);
            }
        }
        /// <summary>
        /// Task for loading a mocollision in the background.
        /// </summary>
        public void MoCollisionConversionTask(object o)
        {
            List<KeyValuePair<string, string>> ErrorLog = new List<KeyValuePair<string, string>>();

            //iterate through each file
            foreach (string strFilepath in mastrFilenames)
            {
                WriteLineToOutput("Converting '" + mastrFilenames[0] + "':");

                //get the output name
                string[] strFilepathBrokenUp = strFilepath.Split('/');
                string strWrongExtension = strFilepathBrokenUp[strFilepathBrokenUp.Length - 1];
                string strOutputName = strWrongExtension.Replace(".dae", "");
                strOutputName = strOutputName.Replace(".DAE", "");

                //Create the output directory
                Directory.CreateDirectory(kstrPathOutput);

                //create and start the process
                Process process = new Process();
                process.StartInfo.CreateNoWindow = true;
                process.StartInfo.FileName = "java.exe";
                process.StartInfo.Arguments = "-jar " + kstrPathToMoModelTool
                    + " --input \"" + strFilepath + "\""
                    + " --output \"" + kstrPathOutput + strOutputName + ".mocollision\""
                    + " --transforms " + GetTransformsString(mOptions)
                    + " --logginglevel verbose"
                    + " --errordisplay whenreceived";

                process.StartInfo.UseShellExecute = false;
                process.StartInfo.RedirectStandardOutput = true;
                process.StartInfo.RedirectStandardError = true;
                process.StartInfo.ErrorDialog = true;
                process.Start();

                //start asyc read all standard output and print it to the output window.
                mstrCurrentStandardError = "";
                process.OutputDataReceived += this.ReadStandardOutputEvent;
                process.ErrorDataReceived += this.ReadStandardErrorEvent;
                process.BeginErrorReadLine();
                process.BeginOutputReadLine();

                //wait for this process to finish
                process.WaitForExit();

                //remove the events
                process.OutputDataReceived -= this.ReadStandardOutputEvent;
                process.ErrorDataReceived -= this.ReadStandardErrorEvent;

                //store error output
                ErrorLog.Add(new KeyValuePair<string, string>(strOutputName, mstrCurrentStandardError));
                mstrCurrentStandardError = "";

                //incriment the progress bar
                mMainWindow.GetMainThreadDispatcher().BeginInvoke(new MainWindow.ProgressDelegate(mMainWindow.IncrimentProgressBar));
            }

            bool bErrorFound = false;
            //write the error output
            foreach (KeyValuePair<string, string> kvp in ErrorLog)
            {
                if (kvp.Value.Length > 2)
                {
                    WriteLineToOutput(kvp.Key + ":\n" + kvp.Value);
                    //WriteLineToOutput();
                    bErrorFound = true;
                }
            }

            if (bErrorFound == false)
            {
                WriteLineToOutput("All Files successfully converted!");
            }

            //finish the progress bar
            mMainWindow.GetMainThreadDispatcher().BeginInvoke(new MainWindow.ProgressDelegate(mMainWindow.EndProgressBar));

            //clean up and reset to allow the task to be started again.
            mastrFilenames = null;
            mbTaskActive = false;
        }

        /// <summary>
        /// Returns the "transforms" string for passing to the MoCollision converter.
        /// </summary>
        /// <param name="inOptions"></param>
        /// <returns></returns>
        private string GetTransformsString(MoCollisionOptions inOptions)
        {
            string output = "";

            if (inOptions.mbMirrorInXZPlane == true)
                output += "m";
            if (inOptions.mbSwapYAndZ == true)
                output += "y";
            
            if (output.Length == 0)
                output = "0";

            return output;
        }
    }
}
