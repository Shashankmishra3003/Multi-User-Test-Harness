///////////////////////////////////////////////////////////////////////
// MainWindow.xaml.cs - Client UI OOD Project #4                    //
// ver 1.2                                                         //
// Source: Jim Fawcett, CSE687 - Object Oriented Design, Fall 2018//
// Author:Shashank Mishra, Syracuse University			       	 //
//		  smishr06@syr.edu                                      //
/////////////////////////////////////////////////////////////////
/*
 * Package Operations:
 * -------------------
 * This package defines one class MainWindow that provides tabs:
 * - Find Libs: Navigate through local directory to find files for testing
 *   - Shows local directories and files
 *   - Can navigate by double clicking directories
 *   - Can select and remove files by clicking on button.
 *   
 * - Request Tests: Select a file to create a test request
 * - Show Results:  Display the result of test request in a pop up window
 * 
 * Required Files:
 * ---------------
 * MainWindow.xaml, MainWindow.xaml.cs
 * MsgPassingCommunication
 * 
 * Maintenance History:
 * --------------------
 * ver 1.2 : 29 Nov 2018
 * - added Test request and test result functionality
 * ver 1.1 : 16 Nov 2018
 * - fixed bug in Files_SelectionChanged by checking e.AddedItems.Count
 *   and returning if 0.
 * ver 1.0 : 15 Nov 2018
 * - first release
 * 
 */

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using System.IO;
using System.Threading;
using MsgPassingCommunication;

namespace Project4_client
{
    ///////////////////////////////////////////////////////////////////
    // MainWindow class
    // - Provides navigation and selection to find libraries to test.
    // - Select the Dll's to test and create a test request.
    public partial class MainWindow : Window
    {
        public string path { get; set; }
        public string rpath { get; set; }
        public string repositoryPath { get; set; }
        public string[] cmd;

        public MainWindow()
        {
            InitializeComponent();
        }

        private CsEndPoint endPoint_, serverEndPoint;
        private Translater translater;
        private Thread receivedMessage;

        //----< open window showing contents of project directory >------
        private void Window_Loaded(object sender, RoutedEventArgs e)
        {
            endPoint_ = new CsEndPoint();
            endPoint_.machineAddress = "localhost";
            cmd = Environment.GetCommandLineArgs();
            endPoint_.port = Int32.Parse(cmd[1]);

            translater = new Translater();
            translater.listen(endPoint_);

            serverEndPoint = new CsEndPoint();
            serverEndPoint.machineAddress = "localhost";
            serverEndPoint.port = 8082;

            path = Directory.GetCurrentDirectory();
            path = getAncestorPath(3, path);
            path += "\\TestLibraries";

            rpath = Directory.GetCurrentDirectory();
            rpath = getAncestorPath(3, rpath);
            rpath += "\\TestLogFromChildProcess";

            repositoryPath = Directory.GetCurrentDirectory();
            repositoryPath = getAncestorPath(3, repositoryPath);
            repositoryPath += "\\TestLibraries";

            LoadNavTab(path);
            receiveMessage();

        //----<Calling the automated TestSuit for the client runing on port 8081>----
            if(Int32.Parse(cmd[1]) == 8081)
                automatedTestSuit();
        }

        //----< find parent paths >--------------------------------------

        string getAncestorPath(int n, string path)
        {
            for (int i = 0; i < n; ++i)
                path = Directory.GetParent(path).FullName;
            return path;
        }

        //----< Find Libs tab with subdirectories and files >-------

        void LoadNavTab(string path)
        {
            Dirs.Items.Clear();
            CurrPath.Text = path;
            string[] dirs = Directory.GetDirectories(path);
            Dirs.Items.Add("..");
            foreach (string dir in dirs)
            {
                DirectoryInfo di = new DirectoryInfo(dir);
                string name = System.IO.Path.GetDirectoryName(dir);
                Dirs.Items.Add(di.Name);
            }
            Files.Items.Clear();
            string[] files = Directory.GetFiles(path,"*.dll");
            foreach (string file in files)
            {
                string name = System.IO.Path.GetFileName(file);
                Files.Items.Add(name);
            }
        }
        //----< Sets the path to the result directory of the client>-----
        void LoadResTab(string path)
        {
           // string newPath = rpath + "\\TestLogFromChildProcess";
            ResPath.Text = rpath;

            ResFiles.Items.Clear();
            string[] resFiles = Directory.GetFiles(rpath);
            foreach (string file in resFiles)
            {
                string name = System.IO.Path.GetFileName(file);
                ResFiles.Items.Add(name);
            }

        }
        //----< move into double-clicked directory, display contents >---

        private void Dirs_MouseDoubleClick(object sender, MouseButtonEventArgs e)
        {
            string selectedDir = Dirs.SelectedItem.ToString();
            if (selectedDir == "..")
                path = getAncestorPath(1, path);
            else
                path = System.IO.Path.Combine(path, selectedDir);
            LoadNavTab(path);
        }

        //----< show selected file in code popup window >----------------

        private void localFiles_MouseDoubleClick(object sender, MouseButtonEventArgs e)
        {
            string fileNames = ResFiles.SelectedValue as string;
           // string newPath = rpath + "\\TestLogFromChildProcess";

            try
            {
                string path = System.IO.Path.Combine(rpath, fileNames);
                string contents = File.ReadAllText(path);
                ClientGui.ResultPopup popup = new ClientGui.ResultPopup();
                popup.codeView.Text = contents;
                popup.Show();
            }
            catch (Exception ex)
            {
                string msg = ex.Message;
            }
        }

        //----< Selects the Dll's from the files List box
        private void selectDll(Object sender, RoutedEventArgs e)
        {
            if (!Files.Items.IsEmpty)
            {
                    string filePath = CurrPath.Text;
                    string currentItem = Files.SelectedValue as string;
                    string[] currentFiles = Directory.GetFiles(repositoryPath);
                    if (!currentFiles.Contains(System.IO.Path.Combine(filePath, currentItem)))
                    {
                        File.Copy(System.IO.Path.Combine(filePath,currentItem),System.IO.Path.Combine(repositoryPath, currentItem));
                    } 
                    selectedDlls.Items.Add(currentItem);
                
            }

        }

        //----< Removes unwanted Dll's from the Test Request ListBox
        private void removeDll(Object sender, RoutedEventArgs e)
        {
            if (!selectedDlls.Items.IsEmpty)
            {
                string currentItem = selectedDlls.SelectedValue.ToString();
                int currentIndex = selectedDlls.SelectedIndex;
                selectedDlls.Items.RemoveAt(selectedDlls.Items.IndexOf(selectedDlls.SelectedItem));
            }

        }

        //----< Sends the Dll's as a Test request to the test Harness>------
        private void sendTr(Object sender, RoutedEventArgs e)
        {
            if (!selectedDlls.Items.IsEmpty)
            {
                int count = 3;
                string stestRequest;
                string[] testRequest = new string[3 + selectedDlls.Items.Count];
                testRequest[0] = "TestRequest";
                testRequest[1] = "Shashank";
                testRequest[2] = DateTime.Now.ToString();

                foreach (string item in selectedDlls.Items)
                {
                    testRequest[count] = item;
                    count++;
                    sendTestDlls(item);

                }
                stestRequest = string.Join(", ", testRequest);
                Console.WriteLine("\nTest Tequest: "+ stestRequest);
                CsMessage message = new CsMessage();
                message.add("to", CsEndPoint.toString(serverEndPoint));
                message.add("from", CsEndPoint.toString(endPoint_));
                message.add("command", "testReq");
                message.attributes.Add("Request", stestRequest);
                message.attributes.Add("FileName", "TestRequest" + DateTime.Now.Hour + DateTime.Now.Minute +DateTime.Now.Millisecond);
                translater.postMessage(message);
                selectedDlls.Items.Clear();
            }
        }

        //----< Saves the Test Dll's from the client directory to the Test Harness Directory>----
        private void sendTestDlls(string selectedDll)
        {
            CsMessage message = new CsMessage();
            message.add("to", CsEndPoint.toString(serverEndPoint));
            message.add("from", CsEndPoint.toString(endPoint_));
            message.add("command", "sendingFile");
            message.add("name", "sendFile");
            message.add("value", "sendingFile");
            message.add("sendingFile", selectedDll);
            translater.postMessage(message);
            
        }
        //----<Receives Test result for the Test Requests>----
        private void receiveMessage()
        {
            int i = 0;
            ThreadStart recvThread = () =>
            {
                while (true)
                {
                    CsMessage message = translater.getMessage();
                    Console.WriteLine("\n Message: " + message.ToString());
                    if (message.attributes.ContainsKey("command"))
                    {
                        if (message.value("command") == "sendingFile")
                        {
                            Console.WriteLine(i + "\nFiles Received from Child Process: " + message.value("FileName"));
                            Dispatcher.BeginInvoke(System.Windows.Threading.DispatcherPriority.Normal, new Action(() => LoadResTab(rpath)));
                        }
                        if (message.value("command") == "test")
                            Console.WriteLine("\nFile Received");

                    }
                }
            };
            receivedMessage = new Thread(recvThread);
            receivedMessage.IsBackground = true;
            receivedMessage.Start();
        }

        private void showResultWindow()
        {
           // string newPath = rpath + "\\TestLogFromChildProcess";

            try
            {
                string[] fileName = Directory.GetFiles(rpath);
                string path = System.IO.Path.Combine(rpath, fileName[0]);
                string contents = File.ReadAllText(path);
                ClientGui.ResultPopup popup = new ClientGui.ResultPopup();
                popup.codeView.Text = contents;
                popup.Show();
            }
            catch (Exception ex)
            {
                string msg = ex.Message;
            }

        }
        //----<Senda one test request and shows result log in a popup window>----
        private void automatedTestSuit()
        {
            Files.SelectedItem = Files.Items.GetItemAt(0);
            buttonSelect.RaiseEvent(new RoutedEventArgs(Button.ClickEvent));
            buttonSendFile.RaiseEvent(new RoutedEventArgs(Button.ClickEvent));
            Thread.Sleep(2000);
            showResultWindow();
        }

    }
}
