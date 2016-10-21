#ifndef WINDOW_H
#define WINDOW_H
///////////////////////////////////////////////////////////////////////////
// Window.h - C++\CLI implementation of WPF Application                  //
//																		 //
// ver 2.0       
//
// surface pro 3, windows 8.1, visual studio 2013
// Zhenya Li,															//
///////////////////////////////////////////////////////////////////////////
/*
*  Package Operations:
*  -------------------
*  This pack specify a socket based file processing system GUI. 
   This panel works as a client which can send request messages to servers and get response from servers.
	
	GUI tabs:
	1>Connect:
		|-connect to server 8000
		|-connect to server 8500
*	2>Text Search
		|-search
			requires select at least one of the server checkbox to search from
			requires input of file patterns
			optional input text you want to search which files contain
		|-explore directory
			requires select at least one of the server checkbox to expolre

	3>upload file
		|-browse
			browse path and show files in the path in browse block
		|-upload
			requires select at least one of the server checkbox to upload
			requires select one file from the browse block to upload

	4>download file
		|-browse
			browse path and show files in the path in browse block
		|-download
			requires select one of the server radiobox to download
			requires select one file from the browse block to download



*  Required Files:
*  ---------------
*  Window.h, Window.cpp, myChannel.h, myChannel.cpp,
*  Cpp11-BlockingQueue.h, Cpp11-BlockingQueue.cpp
	Message.h, Message.cpp
*
*  Build Command:
*  --------------
*  devenv CppCli-WPF-App.sln
*  - this builds C++\CLI client application and native mock channel DLL
*

*/
/*
* Create C++/CLI Console Application
* Right-click project > Properties > Common Language Runtime Support > /clr
* Right-click project > Add > References
*   add references to :
*     System
*     System.Windows.Presentation
*     WindowsBase
*     PresentatioCore
*     PresentationFramework
*/
#include "../myChannel/myChannel.h"

using namespace System;
using namespace System::Text;
using namespace System::Windows;
using namespace System::Windows::Input;
using namespace System::Windows::Markup;
using namespace System::Windows::Media;                   // TextBlock formatting
using namespace System::Windows::Controls;                // TabControl
using namespace System::Windows::Controls::Primitives;    // StatusBar
using namespace System::Threading;
using namespace System::Threading::Tasks;
using namespace System::Windows::Threading;
using namespace System::ComponentModel;


#include <iostream>
#include <chrono>

namespace CppCliWindows
{
  ref class WPFCppCliDemo : Window
  {
	  //String^ time_now;//this is for chrono time counting
	  std::chrono::time_point<std::chrono::system_clock> *time_before = new std::chrono::time_point<std::chrono::system_clock>;
	  //************************************************
    // myChannel references
	//************************************************

    ISendr* pSendr_;
    IRecvr* pRecvr_;

	//************************************************
    // Controls for Window
	//************************************************
   
	DockPanel^ hDockPanel = gcnew DockPanel();      // support docking statusbar at bottom
    Grid^ hGrid = gcnew Grid();                    
    TabControl^ hTabControl = gcnew TabControl();
    //TabItem^ hSendMessageTab = gcnew TabItem();
    //TabItem^ hFileListTab = gcnew TabItem();
    TabItem^ hConnectTab = gcnew TabItem();
	TabItem^ hUploadFileTab = gcnew TabItem();
	TabItem^ hDownloadFileTab = gcnew TabItem();
	//TabItem^ hFileSearchTab = gcnew TabItem();
	TabItem^ hTextSearchTab = gcnew TabItem();

    StatusBar^ hStatusBar = gcnew StatusBar();
    StatusBarItem^ hStatusBarItem = gcnew StatusBarItem();
    TextBlock^ hStatus = gcnew TextBlock();

	
    
	//***********************************************
	// Controls for connect view
	//************************************************
	Grid^ hConnectGrid = gcnew Grid();

	TextBlock^ hClientPortTx = gcnew TextBlock();
	String^ hClientPortString = "Client port \nnumber:";
	//Button^ hClientPortStartButton = gcnew Button();

	Button^ hConnectServer1Button = gcnew Button();
	Button^ hConnectServer2Button = gcnew Button();

	//TextBox^ hInputPortBox = gcnew TextBox();
	TextBlock^ hPortBlock = gcnew TextBlock();
	StackPanel^ hStackPanelConnect1 = gcnew StackPanel();
	StackPanel^ hStackPanelConnect2 = gcnew StackPanel();
	
	//************************************************
	//Controls for UploadFile View
	//************************************************
	Grid^ hUploadGrid = gcnew Grid();
	Forms::FolderBrowserDialog^ hUploadBrowserDialog = gcnew Forms::FolderBrowserDialog();
	
	ListBox^ hFileLookupListBox = gcnew ListBox();
	Button^ hUploadBrowseButton = gcnew Button();
	Button^ hUploadButton = gcnew Button();
	Button^ hUploadClearButton = gcnew Button();
	void setUploadClearButton();

	StackPanel^ hUploadTextPanel = gcnew StackPanel();
	StackPanel^ hStackPanelUpload = gcnew StackPanel();
	StackPanel^ hUploadButtonPanel = gcnew StackPanel();
	
	TextBlock^ hConnectedServersTx = gcnew TextBlock();
	String^ hConnectedServersString = "Connected servers:";

	TextBlock^ hSelectUploadFileTx = gcnew TextBlock();
	String^ hSelectUploadFileString = "Select a file:";

	TextBlock^ hUploadResultBlock = gcnew TextBlock();
	TextBlock^ hUploadResultTx = gcnew TextBlock();
	String^ hUploadResultString = "Upload result:";

	ScrollViewer^ hFileLookupListScroll = gcnew ScrollViewer();
	ScrollViewer^ hUploadResultScroll = gcnew ScrollViewer();

	//for the upload view checkboxes
	ListBox^ hCurrentServerCheckboxes = gcnew ListBox();
	TextBlock^ uploadServer1TX = gcnew TextBlock();
	CheckBox^ uploadServer1Box = gcnew CheckBox();
	TextBlock^ uploadServer2TX = gcnew TextBlock();
	CheckBox^ uploadServer2Box = gcnew CheckBox();
	StackPanel^ checkbox1panel = gcnew StackPanel();
	StackPanel^ checkbox2panel = gcnew StackPanel();


	//************************************************
	//Controls for download view
	//************************************************
	Grid^ hDownloadGrid = gcnew Grid();
	Forms::FolderBrowserDialog^ hDownBrowserDialog = gcnew Forms::FolderBrowserDialog();
	
	ListBox^ hServerFileListBox = gcnew ListBox();
	Button^ hDownloadBrowseButton = gcnew Button();
	Button^ hDownloadButton = gcnew Button();
	Button^ hDownloadClearButton = gcnew Button();
	void setDownloadClearButton();

	StackPanel^ hDownloadTextPanel = gcnew StackPanel();
	StackPanel^ hStackPanelDownload = gcnew StackPanel();
	StackPanel^ hDownloadButtonPanel = gcnew StackPanel();

	TextBlock^ hDownFromServersTx = gcnew TextBlock();
	String^ hDownFromServersString = "Connected servers:";

	TextBlock^ hSelectDownloadFileTx = gcnew TextBlock();
	String^ hSelectDownloadFileString = "Select a file:";

	TextBlock^ hDownloadResultBlock = gcnew TextBlock();
	TextBlock^ hDownloadResultTx = gcnew TextBlock();
	String^ hDownloadResultString = "Download result:";

	ScrollViewer^ hServerFileListScroll = gcnew ScrollViewer();
	ScrollViewer^ hDownloadResultScroll = gcnew ScrollViewer();

	//for the upload view checkboxes
	ListBox^ hCurrentServerRadios = gcnew ListBox();
	TextBlock^ downloadServer1TX = gcnew TextBlock();
	RadioButton^ downloadServer1Radio = gcnew RadioButton();
	TextBlock^ downloadServer2TX = gcnew TextBlock();
	RadioButton^ downloadServer2Radio = gcnew RadioButton();
	StackPanel^ radio1panel = gcnew StackPanel();
	StackPanel^ radio2panel = gcnew StackPanel();

	//************************************************
	//Controls for TextSearch view
	//************************************************
	Grid^ hTextSearchGrid = gcnew Grid();
	StackPanel^ hTextSearchPanel1 = gcnew StackPanel();
	StackPanel^ hTextSearchPanel2 = gcnew StackPanel();
	StackPanel^ hTextSearchPanel3 = gcnew StackPanel();
	StackPanel^ hTextSearchPanel4 = gcnew StackPanel();
	StackPanel^ hSearchButtonPanel = gcnew StackPanel();
	StackPanel^ hSearchPathPanel = gcnew StackPanel();

	//TextBox^ hTextSearchPortBox = gcnew TextBox();
	TextBox^ hTextSearchBox = gcnew TextBox();
	TextBox^ hTextSearchPatternBox = gcnew TextBox();
	TextBox^ hSearchPathBox = gcnew TextBox();

	//for the server choosing checkboxes
	TextBlock^ fileSearchServer1TX = gcnew TextBlock();
	CheckBox^ fileSearchServer1Box = gcnew CheckBox();
	TextBlock^ fileSearchServer2TX = gcnew TextBlock();
	CheckBox^ fileSearchServer2Box = gcnew CheckBox();
	void setSearchCheckbox();
	void setSearchCheckbox1();
	void setSearchCheckbox2();
	//TextBox^ hTextSearchDirBox = gcnew TextBox();

	Button^ hFileSearchButton = gcnew Button();
	Button^ hExploreButton = gcnew Button();
	//Button^ hSearchTabConnectButton = gcnew Button();
	Button^ hSearchClearButton = gcnew Button();
	void setSearchClearButton();

	ScrollViewer^ hTextSearchScroll = gcnew ScrollViewer();
	ScrollViewer^ hSearchOutputScroll = gcnew ScrollViewer();

	//TextBlock^ hFileSearchPortTx = gcnew TextBlock();
	TextBlock^ hFileSearchOutputTx = gcnew TextBlock();
	TextBlock^ hFileSearchTextTx = gcnew TextBlock();
	TextBlock^ hFileSearchPatternTx = gcnew TextBlock();
	TextBlock^ hSearchOutputBlock = gcnew TextBlock();
	TextBlock^ hSearchPathBlock = gcnew TextBlock();

	//String^ hFileSearchPortString = "Port:";
	String^ hFileSearchOutputString = "Files found:";
	String^ hFileSearchTextString = "Text to search:";
	String^ hFileSearchPatternString = "File patterns:";
	String^ hFileSearchPathString = "Path:";
	//String^ hFileSearchDirString = "File directory:";
	//TextBlock^ hFileSearchDirTx = gcnew TextBlock();

    // receive thread

    Thread^ recvThread;

	//************************************************
	//client part
	//************************************************
	String^ port_str = "";				//port number of this client


  
public:
	WPFCppCliDemo(System::String^);
    ~WPFCppCliDemo();

	/*******************
		setting up GUI
	********************/
    void setUpStatusBar();
    void setUpTabControl();
	void setUpButtonEvents();//setting up button events
    //void setUpSendMessageView();
	void setUpTextSearchView();
    //void setUpFileListView();
    void setUpConnectionView(); //connect panel
	void setUpUploadView();
	void setUpDownloadView();


	//setting buttons

    void getMessage();
    //void browseForFolder(Object^ sender, RoutedEventArgs^ args);
    void OnLoaded(Object^ sender, RoutedEventArgs^ args);
    void Unloading(Object^ sender, System::ComponentModel::CancelEventArgs^ args);
 
	void connectToS1(Object^ sender, RoutedEventArgs^ args);		//connect to server1 event
	void connectToS2(Object^ sender, RoutedEventArgs^ args);		//connect to server2 event
	void setFileSearchButton(Object^ sender, RoutedEventArgs^ args);//search file button event
	void searchInS1(Object^ sender, RoutedEventArgs^ args);			//search file in server 1
	void searchInS2(Object^ sender, RoutedEventArgs^ args);			//search file in server 2
	void setExploreButton(Object^ sender, RoutedEventArgs^ args);	//explore a directory in servers
	void exploreS1(Object^ sender, RoutedEventArgs^ args);			//explore server 1
	void exploreS2(Object^ sender, RoutedEventArgs^ args);			//explore server 2

	void uploadBrowse(Object^ sender, RoutedEventArgs^ args);		//upload view browse button
	void upload(Object^ sender, RoutedEventArgs^ args);				//upload button
	void downloadBrowse(Object^ sender, RoutedEventArgs^ args);		//download browse button
	void download(Object^ sender, RoutedEventArgs^ args);			//download button

	void searchClear(Object^ sender, RoutedEventArgs^ args);
	void uploadClear(Object^ sender, RoutedEventArgs^ args);
	void downloadClear(Object^ sender, RoutedEventArgs^ args);
	/***************************
	setting up client functions
	****************************/
	void startListen(const size_t & port); //start receiver listening
	bool shutDownSend();			//shut down sending message	
	bool connectToServer(String^ ip, size_t port);			//connect to server if want to send message to it
	bool processSendingTask(const Message &msg);  //when got a message, analyse it and send
	//void changeServer(const std::string& ip, size_t port);

	bool sendMessageOut();			//send a message from the sending queue
	void addToSendingQ(const Message &msg);   //add a task of message to sender's queue 
	bool sendFile(const std::string& file_name, const std::string& dir);//send a file to client port

	std::vector<std::string> getPattersFromInput(const std::string&);	//parse file pattern input into vector of patterns
	
	//show results
	void showExplreResult(String^ msg);		//show explore results
	void showSearchResult(String^ msg);		//show searching results
	void showUploadResult(String^ msg);		//show upload result
	void showDownloadResult(String^ msg);	//

private:
    std::string toStdString(String^ pStr);
    String^ toSystemString(std::string& str);
   
	
	//for connect tab
	void setStartClientPanel();
		void setClientPortTx();
		void setClientPortBlock();
		/*void setClientPortBox();
		void setClientPortStartButton();*/
	void setConnectButtons();
	

	//for text search tab
	
	void setTextSearchSearchBlock();
		void inputAndOutputText();
		void inputAndOutputBox();
		void setInputTextBox();
		void setOutputBlock();

		void setSearchAndExploreButtons();


		void setFilePatternInput();
			void setFilePatternText();
			void setFilePatternBox();
		void setPathInput();
			void setPathText();
			void setPathBox();


	//for upload tab view
	void setUploadText();		//setting up texts
	void setListBoxOfCheckbox();//setting up listbox with server checkboxes
	void setListBoxOfFiles();	//setting up listbox with file paths
	void setResultBlock();		//setting up result text block
	void setUploadButtons();	//setting upload tab buttons
		void setUploadBrowseButton();
		void setUploadUploadButton();

	//for download tab view
	void setDownloadText();			//setting up texts
	void setListBoxOfRadiobox();	//setting up listbox with server radiobox
	void setDownloadFilesList();	//setting up listbox with file paths
	void setDownloadResultBlock();	//setting up result text block
	void setDownloadButtons();		//setting upload tab buttons
		void setDownloadBrowseButton();
		void setDownloadUploadButton();
  };
}


#endif
