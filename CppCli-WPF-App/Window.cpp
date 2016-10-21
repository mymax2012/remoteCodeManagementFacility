///////////////////////////////////////////////////////////////////////////
// Window.cpp - C++\CLI implementation of WPF Application                //
//                                                               //
// Zhenya Li,														     //
///////////////////////////////////////////////////////////////////////////
/*
*  To run as a Windows Application:
*  - Set Project Properties > Linker > System > Subsystem to Windows
*  - Comment out int main(...) at bottom
*  - Uncomment int _stdcall WinMain() at bottom
*  To run as a Console Application:
*  - Set Project Properties > Linker > System > Subsytem to Console
*  - Uncomment int main(...) at bottom
*  - Comment out int _stdcall WinMain() at bottom
*/



/*
settings:

preset servers:
server1:8000
server2:8500


*/
#include "Window.h"


using namespace CppCliWindows;

WPFCppCliDemo::WPFCppCliDemo(System::String^ portstr) :port_str(portstr)
{
  // set up channel

  ObjectFactory* pObjFact = new ObjectFactory;
  pSendr_ = pObjFact->createSendr();
 
  delete pObjFact;

  size_t portnum = std::stoul(toStdString(portstr));
  startListen(portnum);

  // set event handlers
  setUpButtonEvents();
  // set Window properties

  this->Title = "WPF C++/CLI Demo";
  this->Width = 800;
  this->Height = 600;

  // attach dock panel to Window

  this->Content = hDockPanel;
  hDockPanel->Children->Add(hStatusBar);
  hDockPanel->SetDock(hStatusBar, Dock::Bottom);
  hDockPanel->Children->Add(hGrid);

  // setup Window controls and views

  setUpTabControl();
  setUpStatusBar();
  setUpTextSearchView();
  setUpConnectionView();
  setUpUploadView();
  setUpDownloadView();

  //*time_before= std::chrono::system_clock::now();
}

WPFCppCliDemo::~WPFCppCliDemo()
{
  delete pSendr_;
  delete pRecvr_;
}
//start listen 
void WPFCppCliDemo::startListen(const size_t & port){
	ObjectFactory* pObjFact = new ObjectFactory;
	pRecvr_ = pObjFact->createRecvr(port);
	delete pObjFact;

	// client's receive thread
	pRecvr_->start();
	recvThread = gcnew Thread(gcnew ThreadStart(this, &WPFCppCliDemo::getMessage));
	recvThread->Start();
}
//status bar
void WPFCppCliDemo::setUpStatusBar()
{
  hStatusBar->Items->Add(hStatusBarItem);
  hStatus->Text = "very important messages will appear here";
  //status->FontWeight = FontWeights::Bold;
  hStatusBarItem->Content = hStatus;
  hStatusBar->Padding = Thickness(10, 2, 10, 2);
}
//tab control
void WPFCppCliDemo::setUpTabControl()
{
  hGrid->Children->Add(hTabControl);

  //hSendMessageTab->Header = "Send Message";
  //hFileListTab->Header = "File List";
  hConnectTab->Header = "Connect";
  hUploadFileTab->Header = "Upload File";
  hDownloadFileTab->Header = "Download File";
  //hFileSearchTab->Header = "File Search";
  hTextSearchTab->Header = "Text Search";

  //hTabControl->Items->Add(hSendMessageTab);
  //hTabControl->Items->Add(hFileSearchTab);
  hTabControl->Items->Add(hConnectTab);
  hTabControl->Items->Add(hTextSearchTab);
  hTabControl->Items->Add(hUploadFileTab);
  hTabControl->Items->Add(hDownloadFileTab);
  //hTabControl->Items->Add(hFileListTab);
  
}
//set up all the button event handlers
void WPFCppCliDemo::setUpButtonEvents(){
	this->Loaded +=
		gcnew System::Windows::RoutedEventHandler(this, &WPFCppCliDemo::OnLoaded);
	this->Closing +=
		gcnew CancelEventHandler(this, &WPFCppCliDemo::Unloading);
	hConnectServer1Button->Click += gcnew RoutedEventHandler(this, &WPFCppCliDemo::connectToS1);
	hConnectServer2Button->Click += gcnew RoutedEventHandler(this, &WPFCppCliDemo::connectToS2);
	hFileSearchButton->Click += gcnew RoutedEventHandler(this, &WPFCppCliDemo::setFileSearchButton);
	hUploadBrowseButton->Click += gcnew RoutedEventHandler(this, &WPFCppCliDemo::uploadBrowse);
	hUploadButton->Click += gcnew RoutedEventHandler(this, &WPFCppCliDemo::upload);
	hDownloadBrowseButton->Click += gcnew RoutedEventHandler(this, &WPFCppCliDemo::downloadBrowse);
	hDownloadButton->Click += gcnew RoutedEventHandler(this, &WPFCppCliDemo::download);
	hExploreButton->Click += gcnew RoutedEventHandler(this, &WPFCppCliDemo::setExploreButton);
	hSearchClearButton->Click += gcnew RoutedEventHandler(this, &WPFCppCliDemo::searchClear);
	hUploadClearButton->Click += gcnew RoutedEventHandler(this, &WPFCppCliDemo::uploadClear);
	hDownloadClearButton->Click += gcnew RoutedEventHandler(this, &WPFCppCliDemo::downloadClear);


}

//----------------------
//----Connect view
//----------------------
void WPFCppCliDemo::setUpConnectionView()
{
	Console::Write("\n  setting up Connection view");
	hConnectGrid->Margin = Thickness(20);
	hConnectTab->Content = hConnectGrid;

	RowDefinition^ hRow1def = gcnew RowDefinition();
	hRow1def->Height = GridLength(275);
	hConnectGrid->RowDefinitions->Add(hRow1def);
	setStartClientPanel();
	hStackPanelConnect1->Orientation = Orientation::Horizontal;
	hStackPanelConnect1->HorizontalAlignment = System::Windows::HorizontalAlignment::Center;
	hStackPanelConnect1->VerticalAlignment = System::Windows::VerticalAlignment::Bottom;
	hConnectGrid->SetRow(hStackPanelConnect1, 0);
	hConnectGrid->Children->Add(hStackPanelConnect1);

	RowDefinition^ hRow2def = gcnew RowDefinition();
	hRow2def->Height = GridLength(75);
	hConnectGrid->RowDefinitions->Add(hRow2def);
	setConnectButtons();
	hStackPanelConnect2->Orientation = Orientation::Horizontal;
	hStackPanelConnect2->HorizontalAlignment = System::Windows::HorizontalAlignment::Center;
	hConnectGrid->SetRow(hStackPanelConnect2, 1);
	hConnectGrid->Children->Add(hStackPanelConnect2);
}

//setting up client input port 
void WPFCppCliDemo::setStartClientPanel(){
	setClientPortTx();
	setClientPortBlock();
	//setClientPortBox();
	//setClientPortStartButton();
}
//setting the text box showing port number of client

//setting client port text
void WPFCppCliDemo::setClientPortTx(){
	
	hClientPortTx->Padding = Thickness(15);
	hClientPortTx->Text = hClientPortString;
	hClientPortTx->FontFamily = gcnew Windows::Media::FontFamily("Tahoma");
	hClientPortTx->FontWeight = FontWeights::Bold;
	hClientPortTx->FontSize = 18;

	Border^ hBorder = gcnew Border();
	hBorder->BorderThickness = Thickness(0);
	hBorder->Child = hClientPortTx;
	hBorder->VerticalAlignment = System::Windows::VerticalAlignment::Center;

	TextBlock^ hSpacer = gcnew TextBlock();
	hSpacer->Width = 5;

	hStackPanelConnect1->Children->Add(hBorder);
	hStackPanelConnect1->Children->Add(hSpacer);
	hStackPanelConnect1->Orientation = Orientation::Horizontal;
}
//setting client port block showing port number
void WPFCppCliDemo::setClientPortBlock(){
	hPortBlock->Text = port_str;
	hPortBlock->FontFamily = gcnew Windows::Media::FontFamily("Tahoma");
	hPortBlock->Padding = Thickness(15);
	hPortBlock->FontSize = 22;
	hPortBlock->Background = Brushes::MediumSeaGreen;
	hPortBlock->Foreground = Brushes::Ivory;
	hPortBlock->TextAlignment = TextAlignment::Center;
	hPortBlock->VerticalAlignment = System::Windows::VerticalAlignment::Center;

	Border^ hBorder = gcnew Border();
	hBorder->Width = 100;
	hBorder->Height = 40;
	hBorder->BorderThickness = Thickness(1);
	hBorder->BorderBrush = Brushes::Black;
	hBorder->Child = hPortBlock;
	
	TextBlock^ hSpacer = gcnew TextBlock();
	hSpacer->Width = 15;
	hBorder->HorizontalAlignment = System::Windows::HorizontalAlignment::Left;
	hBorder->VerticalAlignment = System::Windows::VerticalAlignment::Center;
	hStackPanelConnect1->Children->Add(hBorder);
	hStackPanelConnect1->Children->Add(hSpacer);
}

//setting client port text box
//void WPFCppCliDemo::setClientPortBox(){
//	hInputPortBox->TextWrapping = TextWrapping::NoWrap;
//
//	Border^ hBorder = gcnew Border();
//	hBorder->Width = 100;
//	hBorder->Height = 40;
//	hBorder->BorderThickness = Thickness(1);
//	hBorder->BorderBrush = Brushes::Black;
//	hBorder->Child = hInputPortBox;
//
//	TextBlock^ hSpacer = gcnew TextBlock();
//	hSpacer->Width = 15;
//	//hBorder->HorizontalAlignment = System::Windows::HorizontalAlignment::Left;
//	//hBorder->VerticalAlignment = System::Windows::VerticalAlignment::Top;
//	hStackPanelConnect1->Children->Add(hBorder);
//	hStackPanelConnect1->Children->Add(hSpacer);
//}
//setting start button
//void WPFCppCliDemo::setClientPortStartButton(){
//	hClientPortStartButton->Content = "Start";
//	hClientPortStartButton->Foreground = Brushes::Ivory;
//	hClientPortStartButton->Background = Brushes::DarkGreen;
//
//	Border^ hBorder = gcnew Border();
//	hBorder->Width = 100;
//	hBorder->Height = 35;
//	hBorder->BorderThickness = Thickness(1);
//	hBorder->BorderBrush = Brushes::Black;
//	hBorder->Child = hClientPortStartButton;
//
//	hStackPanelConnect1->Children->Add(hBorder);
//}

//setting up conncet tab connect buttons
void WPFCppCliDemo::setConnectButtons(){
	
	hConnectServer1Button->Content = "Connect server:8000";
	Border^ hBorder1 = gcnew Border();
	hBorder1->Width = 120;
	hBorder1->Height = 30;
	hBorder1->BorderThickness = Thickness(1);
	hBorder1->BorderBrush = Brushes::Black;
	hBorder1->Child = hConnectServer1Button;

	hConnectServer2Button->Content = "Connect server:8500";
	Border^ hBorder2 = gcnew Border();
	hBorder2->Width = 120;
	hBorder2->Height = 30;
	hBorder2->BorderThickness = Thickness(1);
	hBorder2->BorderBrush = Brushes::Black;
	hBorder2->Child = hConnectServer2Button;

	hStackPanelConnect2->Children->Add(hBorder1);
	TextBlock^ hSpacer = gcnew TextBlock();
	hSpacer->Width = 10;
	hStackPanelConnect2->Children->Add(hSpacer);
	hStackPanelConnect2->Children->Add(hBorder2);
	
}


//setting up TextSearch view
//----------------------
//----Text search view
//----------------------

void WPFCppCliDemo::setUpTextSearchView(){
	Console::Write("\n  setting up FileSearch view");
	hTextSearchGrid->Margin = Thickness(20);
	hTextSearchTab->Content = hTextSearchGrid;
	setTextSearchSearchBlock();
}
//set text search blocks
void WPFCppCliDemo::setTextSearchSearchBlock(){
	//set up text blocks----1st row
	RowDefinition^ hRow1Def = gcnew RowDefinition();
	hRow1Def->Height = GridLength(35);
	hTextSearchGrid->RowDefinitions->Add(hRow1Def);
	inputAndOutputText();
	hTextSearchGrid->SetRow(hTextSearchPanel1, 0);
	hTextSearchGrid->Children->Add(hTextSearchPanel1);
	
	//set up text boxes------2nd row
	RowDefinition^ hRow2Def = gcnew RowDefinition();
	hRow2Def->Height = GridLength(240);
	hTextSearchGrid->RowDefinitions->Add(hRow2Def);
	inputAndOutputBox();

	//set up file directory input-----5th row
	RowDefinition^ hRow5Def = gcnew RowDefinition();
	hRow5Def->Height = GridLength(60);
	hTextSearchGrid->RowDefinitions->Add(hRow5Def);
	setSearchCheckbox();

	////set up port input box ----3rd row
	//RowDefinition^ hRow3Def = gcnew RowDefinition();
	//hRow3Def->Height = GridLength(80);
	//hTextSearchGrid->RowDefinitions->Add(hRow3Def);
	//setPortBox();

	//set up file patterns input-----4th row
	RowDefinition^ hRow4Def = gcnew RowDefinition();
	hRow4Def->Height = GridLength(60);
	hTextSearchGrid->RowDefinitions->Add(hRow4Def);
	setFilePatternInput();

	//set up file patterns input-----6th row
	RowDefinition^ hRow6Def = gcnew RowDefinition();
	hRow6Def->Height = GridLength(60);
	hTextSearchGrid->RowDefinitions->Add(hRow6Def);
	setPathInput();
	
}

//set up input and output text boxes
void WPFCppCliDemo::inputAndOutputBox(){
	setInputTextBox();
	//search/explore button in middle of two blocks
	setSearchAndExploreButtons();
	setOutputBlock();
}
//input text box
void WPFCppCliDemo::setInputTextBox(){
	hTextSearchBox->AcceptsReturn = true;
	hTextSearchBox->TextWrapping = TextWrapping::Wrap;
	hTextSearchScroll->VerticalScrollBarVisibility = ScrollBarVisibility::Auto;
	hTextSearchScroll->Content = hTextSearchBox;

	Border^ hBorder1 = gcnew Border();
	hBorder1->Width = 250;
	hBorder1->Height = 200;
	hBorder1->BorderThickness = Thickness(1);
	hBorder1->BorderBrush = Brushes::Black;
	hBorder1->Child = hTextSearchScroll;
	hBorder1->HorizontalAlignment = System::Windows::HorizontalAlignment::Left;
	hBorder1->VerticalAlignment = System::Windows::VerticalAlignment::Top;

	hTextSearchGrid->SetRow(hBorder1, 1);
	hTextSearchGrid->Children->Add(hBorder1);
}
//output text block
void WPFCppCliDemo::setOutputBlock(){
	hSearchOutputBlock->TextWrapping = TextWrapping::NoWrap;
	hSearchOutputBlock->Padding = Thickness(15);
	hSearchOutputBlock->Text = "";
	hSearchOutputBlock->FontFamily = gcnew Windows::Media::FontFamily("Tahoma");
	hSearchOutputBlock->FontWeight = FontWeights::Light;
	hSearchOutputBlock->FontSize = 12;

	hSearchOutputScroll->VerticalScrollBarVisibility = ScrollBarVisibility::Auto;
	hSearchOutputScroll->HorizontalScrollBarVisibility = ScrollBarVisibility::Auto;
	hSearchOutputScroll->Content = hSearchOutputBlock;

	Border^ hBorder2 = gcnew Border();
	hBorder2->Width = 250;
	hBorder2->Height = 200;
	hBorder2->BorderThickness = Thickness(1);
	hBorder2->BorderBrush = Brushes::Black;
	hBorder2->Child = hSearchOutputScroll;
	hTextSearchGrid->SetRow(hBorder2, 1);
	hTextSearchGrid->Children->Add(hBorder2);

	hBorder2->VerticalAlignment = System::Windows::VerticalAlignment::Top;
	hBorder2->HorizontalAlignment = System::Windows::HorizontalAlignment::Right;
}
//set serach button and explore button
void WPFCppCliDemo::setSearchAndExploreButtons(){
	hFileSearchButton->Content = "Search =>";
	hFileSearchButton->Background = Brushes::Coral;

	Border^ hBorder1 = gcnew Border();
	hBorder1->Width = 80;
	hBorder1->Height = 70;
	hBorder1->BorderThickness = Thickness(2);
	hBorder1->BorderBrush = Brushes::Black;
	hBorder1->Child = hFileSearchButton;
	hBorder1->VerticalAlignment = System::Windows::VerticalAlignment::Center;

	hExploreButton->Content = "Explore\nDirectory";
	hExploreButton->Background = Brushes::Coral;

	Border^ hBorder2 = gcnew Border();
	hBorder2->Width = 80;
	hBorder2->Height = 40;
	hBorder2->BorderThickness = Thickness(2);
	hBorder2->BorderBrush = Brushes::Black;
	hBorder2->Child = hExploreButton;
	hBorder2->VerticalAlignment = System::Windows::VerticalAlignment::Top;
	
	TextBlock^ hSpacer1 = gcnew TextBlock();
	hSpacer1->Width = 40;
	TextBlock^ hSpacer2 = gcnew TextBlock();
	hSpacer2->Width = 40;

	hSearchButtonPanel->Children->Add(hBorder1);
	hSearchButtonPanel->Children->Add(hSpacer1);
	hSearchButtonPanel->Children->Add(hBorder2);
	hSearchButtonPanel->Children->Add(hSpacer2);
	setSearchClearButton();

	hSearchButtonPanel->Orientation = Orientation::Vertical;
	hSearchButtonPanel->VerticalAlignment = System::Windows::VerticalAlignment::Center;

	hTextSearchGrid->SetRow(hSearchButtonPanel, 1);
	hTextSearchGrid->Children->Add(hSearchButtonPanel);
}
//setting clear button
void WPFCppCliDemo::setSearchClearButton(){
	hSearchClearButton->Content = "Clear";
	Border^ hBorder = gcnew Border();
	hBorder->Width = 80;
	hBorder->Height = 30;
	hBorder->BorderThickness = Thickness(1);
	hBorder->BorderBrush = Brushes::Black;
	hBorder->Child = hSearchClearButton;

	hSearchButtonPanel->Children->Add(hBorder);

}
//set up input and output text blocks
void WPFCppCliDemo::inputAndOutputText(){
	Border^ hBorder1 = gcnew Border();
	hBorder1->BorderThickness = Thickness(0);
	//hBorder1->BorderBrush = Brushes::Black;
	hBorder1->Child = hFileSearchTextTx;
	hFileSearchTextTx->Padding = Thickness(15);
	hFileSearchTextTx->Text = hFileSearchTextString;
	hFileSearchTextTx->FontFamily = gcnew Windows::Media::FontFamily("Tahoma");
	hFileSearchTextTx->FontWeight = FontWeights::Bold;
	hFileSearchTextTx->FontSize = 12;

	Border^ hBorder2 = gcnew Border();
	hBorder2->BorderThickness = Thickness(0);
	//hBorder1->BorderBrush = Brushes::Black;
	hBorder2->Child = hFileSearchOutputTx;
	hFileSearchOutputTx->Padding = Thickness(15);
	hFileSearchOutputTx->Text = hFileSearchOutputString;
	hFileSearchOutputTx->FontFamily = gcnew Windows::Media::FontFamily("Tahoma");
	hFileSearchOutputTx->FontWeight = FontWeights::Bold;
	hFileSearchOutputTx->FontSize = 12;

	TextBlock^ hSpacer = gcnew TextBlock();
	hSpacer->Width = 370;

	hTextSearchPanel1->Children->Add(hBorder1);
	hTextSearchPanel1->Children->Add(hSpacer);
	hTextSearchPanel1->Children->Add(hBorder2);
	hTextSearchPanel1->Orientation = Orientation::Horizontal;
}
//set port text and input box

//void WPFCppCliDemo::setPortBox(){
//	TextBlock^ hSpacer1 = gcnew TextBlock();
//	hSpacer1->Width = 20;
//	TextBlock^ hSpacer2 = gcnew TextBlock();
//	hSpacer2->Width = 15;
//
//	setPortText();
//	hTextSearchPanel3->Children->Add(hSpacer1);
//	setPortInputBox();
//	hTextSearchPanel3->Children->Add(hSpacer2);
//	setPortConnectButton();
//
//	hTextSearchGrid->SetRow(hTextSearchPanel3, 4);
//	hTextSearchGrid->Children->Add(hTextSearchPanel3);
//	hTextSearchPanel3->Orientation = Orientation::Horizontal;
//	hTextSearchPanel3->VerticalAlignment = System::Windows::VerticalAlignment::Top;
//	hTextSearchPanel3->HorizontalAlignment = System::Windows::HorizontalAlignment::Center;
//}
////set port input text
//void WPFCppCliDemo::setPortText(){	
//	hFileSearchPortTx->Text = hFileSearchPortString;
//	hFileSearchPortTx->FontFamily = gcnew Windows::Media::FontFamily("Tahoma");
//	hFileSearchPortTx->FontWeight = FontWeights::Bold;
//	hFileSearchPortTx->FontSize = 12;
//
//	Border^ hBorder = gcnew Border();
//	hBorder->BorderThickness = Thickness(0);
//	hBorder->Child = hFileSearchPortTx;
//	hBorder->VerticalAlignment = System::Windows::VerticalAlignment::Center;
//	hTextSearchPanel3->Children->Add(hBorder);
//}
////set port input textbox
//void WPFCppCliDemo::setPortInputBox(){
//	Border^ hBorder = gcnew Border();
//	hBorder->Width = 100;
//	hBorder->Height = 30;
//	hBorder->BorderThickness = Thickness(1);
//	hBorder->BorderBrush = Brushes::Black;
//	hBorder->Child = hTextSearchPortBox;
//
//	hTextSearchPanel3->Children->Add(hBorder);
//}
////set connect button
//void WPFCppCliDemo::setPortConnectButton(){
//	hSearchTabConnectButton->Content = "connect";
//
//	Border^ hBorder = gcnew Border();
//	hBorder->Width = 50;
//	hBorder->Height = 30;
//	hBorder->BorderThickness = Thickness(1);
//	hBorder->BorderBrush = Brushes::Black;
//	hBorder->Child = hSearchTabConnectButton;
//	hBorder->VerticalAlignment = System::Windows::VerticalAlignment::Center;
//
//	hTextSearchPanel3->Children->Add(hBorder);
//}

//set file pattern input text and box
void WPFCppCliDemo::setFilePatternInput(){
	TextBlock^ hSpacer = gcnew TextBlock();
	hSpacer->Width = 20;

	setFilePatternText();
	hTextSearchPanel2->Children->Add(hSpacer);
	setFilePatternBox();

	hTextSearchGrid->SetRow(hTextSearchPanel2, 3);
	hTextSearchGrid->Children->Add(hTextSearchPanel2);
	hTextSearchPanel2->Orientation = Orientation::Horizontal;
	hTextSearchPanel2->VerticalAlignment = System::Windows::VerticalAlignment::Top;
	hTextSearchPanel2->HorizontalAlignment = System::Windows::HorizontalAlignment::Center;
}

//set the "file patterns" text
void WPFCppCliDemo::setFilePatternText(){
	hFileSearchPatternTx->Text = hFileSearchPatternString;
	hFileSearchPatternTx->FontFamily = gcnew Windows::Media::FontFamily("Tahoma");
	hFileSearchPatternTx->FontWeight = FontWeights::Bold;
	hFileSearchPatternTx->FontSize = 12;

	Border^ hBorder = gcnew Border();
	hBorder->BorderThickness = Thickness(0);
	hBorder->Child = hFileSearchPatternTx;
	hBorder->VerticalAlignment = System::Windows::VerticalAlignment::Center;
	hTextSearchPanel2->Children->Add(hBorder);
}
//set the file pattern input textbox
void WPFCppCliDemo::setFilePatternBox(){
	Border^ hBorder = gcnew Border();
	hBorder->Width = 100;
	hBorder->Height = 30;
	hBorder->BorderThickness = Thickness(1);
	hBorder->BorderBrush = Brushes::Black;
	hBorder->Child = hTextSearchPatternBox;

	hTextSearchPanel2->Children->Add(hBorder);
}
////set file search directory input
//void WPFCppCliDemo::setFileSearchDirInput(){
//
//	TextBlock^ hSpacer = gcnew TextBlock();
//	hSpacer->Width = 20;
//
//	setFileSearchDirText();
//	hTextSearchPanel4->Children->Add(hSpacer);
//	setFileSearchDirBox();
//
//	hTextSearchGrid->SetRow(hTextSearchPanel4, 2);
//	hTextSearchGrid->Children->Add(hTextSearchPanel4);
//	hTextSearchPanel4->Orientation = Orientation::Horizontal;
//	hTextSearchPanel4->VerticalAlignment = System::Windows::VerticalAlignment::Top;
//	hTextSearchPanel4->HorizontalAlignment = System::Windows::HorizontalAlignment::Center;
//	
//}
//setting search file panel checkboxes
void WPFCppCliDemo::setSearchCheckbox(){
	TextBlock^ hSpacer = gcnew TextBlock();
	hSpacer->Width = 20;
	
	setSearchCheckbox1();
	hTextSearchPanel4->Children->Add(hSpacer);
	setSearchCheckbox2();
	
	hTextSearchGrid->SetRow(hTextSearchPanel4, 2);
	hTextSearchGrid->Children->Add(hTextSearchPanel4);
	hTextSearchPanel4->Orientation = Orientation::Horizontal;
	hTextSearchPanel4->VerticalAlignment = System::Windows::VerticalAlignment::Top;
	hTextSearchPanel4->HorizontalAlignment = System::Windows::HorizontalAlignment::Center;

}
//setting search file panel checkbox 1
void WPFCppCliDemo::setSearchCheckbox1(){
	fileSearchServer1TX->Text = "server:8000";
	fileSearchServer1TX->FontFamily = gcnew Windows::Media::FontFamily("Tahoma");
	fileSearchServer1TX->FontWeight = FontWeights::Bold;
	fileSearchServer1TX->FontSize = 12;
	Border^ hBorder1 = gcnew Border();
	hBorder1->BorderThickness = Thickness(0);
	hBorder1->Child = fileSearchServer1TX;
	hBorder1->VerticalAlignment = System::Windows::VerticalAlignment::Center;
	hTextSearchPanel4->Children->Add(hBorder1);

	hTextSearchPanel4->Children->Add(fileSearchServer1Box);

}
//setting search file panel checkboxs 2
void WPFCppCliDemo::setSearchCheckbox2(){
	fileSearchServer2TX->Text = "server:8500";
	fileSearchServer2TX->FontFamily = gcnew Windows::Media::FontFamily("Tahoma");
	fileSearchServer2TX->FontWeight = FontWeights::Bold;
	fileSearchServer2TX->FontSize = 12;
	Border^ hBorder1 = gcnew Border();
	hBorder1->BorderThickness = Thickness(0);
	hBorder1->Child = fileSearchServer2TX;
	hBorder1->VerticalAlignment = System::Windows::VerticalAlignment::Center;
	hTextSearchPanel4->Children->Add(hBorder1);

	hTextSearchPanel4->Children->Add(fileSearchServer2Box);

}
//void setting search path panel
void WPFCppCliDemo::setPathInput(){
	TextBlock^ hSpacer = gcnew TextBlock();
	hSpacer->Width = 20;

	setPathText();
	hSearchPathPanel->Children->Add(hSpacer);
	setPathBox();

	hTextSearchGrid->SetRow(hSearchPathPanel, 5);
	hTextSearchGrid->Children->Add(hSearchPathPanel);
	hSearchPathPanel->Orientation = Orientation::Horizontal;
	hSearchPathPanel->VerticalAlignment = System::Windows::VerticalAlignment::Top;
	hSearchPathPanel->HorizontalAlignment = System::Windows::HorizontalAlignment::Center;
}
void WPFCppCliDemo::setPathText(){
	hSearchPathBlock->Text = hFileSearchPathString;
	hSearchPathBlock->FontFamily = gcnew Windows::Media::FontFamily("Tahoma");
	hSearchPathBlock->FontWeight = FontWeights::Bold;
	hSearchPathBlock->FontSize = 12;

	Border^ hBorder = gcnew Border();
	hBorder->BorderThickness = Thickness(0);
	hBorder->Child = hSearchPathBlock;
	hBorder->VerticalAlignment = System::Windows::VerticalAlignment::Center;
	
	hSearchPathPanel->Children->Add(hBorder);
	
}
void WPFCppCliDemo::setPathBox(){
	Border^ hBorder = gcnew Border();
	hBorder->Width = 200;
	hBorder->Height = 30;
	hBorder->BorderThickness = Thickness(1);
	hBorder->BorderBrush = Brushes::Black;
	hBorder->Child = hSearchPathBox;

	hSearchPathPanel->Children->Add(hBorder);

}
//void WPFCppCliDemo::setFileSearchDirText(){
//	hFileSearchDirTx->Text = hFileSearchDirString;
//	hFileSearchDirTx->FontFamily = gcnew Windows::Media::FontFamily("Tahoma");
//	hFileSearchDirTx->FontWeight = FontWeights::Bold;
//	hFileSearchDirTx->FontSize = 12;
//
//	Border^ hBorder = gcnew Border();
//	hBorder->BorderThickness = Thickness(0);
//	hBorder->Child = hFileSearchDirTx;
//	hBorder->VerticalAlignment = System::Windows::VerticalAlignment::Center;
//	hTextSearchPanel4->Children->Add(hBorder);
//
//}
//void WPFCppCliDemo::setFileSearchDirBox(){
//	Border^ hBorder = gcnew Border();
//	hBorder->Width = 300;
//	hBorder->Height = 30;
//	hBorder->BorderThickness = Thickness(1);
//	hBorder->BorderBrush = Brushes::Black;
//	hBorder->Child = hTextSearchDirBox;
//
//	hTextSearchPanel4->Children->Add(hBorder);
//
//}



//----------------------
//----upload view
//----------------------
//setting up upload view
void WPFCppCliDemo::setUpUploadView(){
	Console::Write("\n  setting up upload view");
	hUploadGrid->Margin = Thickness(20);
	hUploadFileTab->Content = hUploadGrid;

	RowDefinition^ hRow1Def = gcnew RowDefinition();
	hRow1Def->Height = GridLength(35);
	hUploadGrid->RowDefinitions->Add(hRow1Def);
	setUploadText();
	hUploadTextPanel->Orientation = Orientation::Horizontal;
	hUploadGrid->SetRow(hUploadTextPanel, 0);
	hUploadGrid->Children->Add(hUploadTextPanel);

	RowDefinition^ hRow2Def = gcnew RowDefinition();
	hRow2Def->Height = GridLength(320);
	hUploadGrid->RowDefinitions->Add(hRow2Def);
	setListBoxOfCheckbox();
	setListBoxOfFiles();
	setResultBlock();
	hStackPanelUpload->Orientation = Orientation::Horizontal;
	hUploadGrid->SetRow(hStackPanelUpload, 1);
	hUploadGrid->Children->Add(hStackPanelUpload);

	RowDefinition^ hRow3Def = gcnew RowDefinition();
	hRow3Def->Height = GridLength(35);
	hUploadGrid->RowDefinitions->Add(hRow3Def);
	setUploadButtons();
	hUploadButtonPanel->Orientation = Orientation::Horizontal;
	//hUploadButtonPanel->HorizontalAlignment = System::Windows::HorizontalAlignment::Center;
	hUploadGrid->SetRow(hUploadButtonPanel, 2);
	hUploadGrid->Children->Add(hUploadButtonPanel);
}
//setting up upload texts
void WPFCppCliDemo::setUploadText(){
	hConnectedServersTx->Text = hConnectedServersString;

	Border^ hBorder1 = gcnew Border();
	hBorder1->Width = 100;
	hBorder1->Height = 30;
	hBorder1->BorderThickness = Thickness(0);
	hBorder1->Child = hConnectedServersTx;

	hSelectUploadFileTx->Text = hSelectUploadFileString;
	Border^ hBorder2 = gcnew Border();
	hBorder2->Width = 100;
	hBorder2->Height = 30;
	hBorder2->BorderThickness = Thickness(0);
	hBorder2->Child = hSelectUploadFileTx;

	hUploadResultTx->Text = hUploadResultString;
	Border^ hBorder3 = gcnew Border();
	hBorder3->Width = 100;
	hBorder3->Height = 30;
	hBorder3->BorderThickness = Thickness(0);
	hBorder3->Child = hUploadResultTx;

	TextBlock^ hSpacer1 = gcnew TextBlock();
	hSpacer1->Width = 50;
	TextBlock^ hSpacer2 = gcnew TextBlock();
	hSpacer2->Width = 250;

	hUploadTextPanel->Children->Add(hBorder1);
	hUploadTextPanel->Children->Add(hSpacer1);
	hUploadTextPanel->Children->Add(hBorder2);
	hUploadTextPanel->Children->Add(hSpacer2);
	hUploadTextPanel->Children->Add(hBorder3);
}

//setting up listbox with server checkboxes
void WPFCppCliDemo::setListBoxOfCheckbox(){
	uploadServer1TX->Text = "server:8000";
	uploadServer1TX->FontFamily = gcnew Windows::Media::FontFamily("Tahoma");
	uploadServer1TX->FontWeight = FontWeights::Bold;
	uploadServer1TX->FontSize = 12;
	Border^ hBorder1 = gcnew Border();
	hBorder1->BorderThickness = Thickness(0);
	hBorder1->Child = uploadServer1TX;
	hBorder1->VerticalAlignment = System::Windows::VerticalAlignment::Center;
	checkbox1panel->Children->Add(hBorder1);
	checkbox1panel->Children->Add(uploadServer1Box);

	uploadServer2TX->Text = "server:8500";
	uploadServer2TX->FontFamily = gcnew Windows::Media::FontFamily("Tahoma");
	uploadServer2TX->FontWeight = FontWeights::Bold;
	uploadServer2TX->FontSize = 12;
	Border^ hBorder2 = gcnew Border();
	hBorder2->BorderThickness = Thickness(0);
	hBorder2->Child = uploadServer2TX;
	hBorder2->VerticalAlignment = System::Windows::VerticalAlignment::Center;
	checkbox2panel->Children->Add(hBorder2);
	checkbox2panel->Children->Add(uploadServer2Box);

	hCurrentServerCheckboxes->Items->Add(checkbox1panel);
	hCurrentServerCheckboxes->Items->Add(checkbox2panel);
	//hCurrentServerCheckboxes->orientation

	Border^ hBorder3 = gcnew Border();
	hBorder3->Width = 100;
	hBorder3->Height = 300;
	hBorder3->BorderThickness = Thickness(1);
	hBorder3->BorderBrush = Brushes::Black;
	hBorder3->Child = hCurrentServerCheckboxes;

	TextBlock^ hSpacer = gcnew TextBlock();
	hSpacer->Width = 50;

	hStackPanelUpload->Children->Add(hBorder3);
	hStackPanelUpload->Children->Add(hSpacer);
}


//setting up listbox with file paths
void WPFCppCliDemo::setListBoxOfFiles(){

	hFileLookupListScroll->VerticalScrollBarVisibility = ScrollBarVisibility::Auto;
	hFileLookupListScroll->HorizontalScrollBarVisibility = ScrollBarVisibility::Auto;
	hFileLookupListScroll->Content = hFileLookupListBox;

	Border^ hBorder = gcnew Border();
	hBorder->Width = 300;
	hBorder->Height = 300;
	hBorder->BorderThickness = Thickness(1);
	hBorder->BorderBrush = Brushes::Black;
	hBorder->Child = hFileLookupListScroll;

	TextBlock^ hSpacer = gcnew TextBlock();
	hSpacer->Width = 50;
	hStackPanelUpload->Children->Add(hBorder);
	hStackPanelUpload->Children->Add(hSpacer);
}
//setting up result block
void WPFCppCliDemo::setResultBlock(){
	hUploadResultBlock->Text = "";
	hUploadResultBlock->TextWrapping = TextWrapping::Wrap;

	hUploadResultScroll->VerticalScrollBarVisibility = ScrollBarVisibility::Auto;
	hUploadResultScroll->HorizontalScrollBarVisibility = ScrollBarVisibility::Auto;
	hUploadResultScroll->Content = hUploadResultBlock;

	Border^ hBorder = gcnew Border();
	hBorder->Width = 200;
	hBorder->Height = 300;
	hBorder->BorderThickness = Thickness(1);
	hBorder->BorderBrush = Brushes::Black;
	hBorder->Child = hUploadResultScroll;

	hStackPanelUpload->Children->Add(hBorder);
}
//setting upload tab buttons
void WPFCppCliDemo::setUploadButtons(){
	setUploadBrowseButton();
	setUploadUploadButton();
	setUploadClearButton();
}
//setting up the browse button
void WPFCppCliDemo::setUploadBrowseButton(){
	hUploadBrowseButton->Content = "Browse file";
	Border^ hBorder = gcnew Border();
	hBorder->Width = 100;
	hBorder->Height = 30;
	hBorder->BorderThickness = Thickness(1);
	hBorder->BorderBrush = Brushes::Black;
	hBorder->Child = hUploadBrowseButton;

	TextBlock^ hSpacer1 = gcnew TextBlock();
	hSpacer1->Width = 180;
	TextBlock^ hSpacer2 = gcnew TextBlock();
	hSpacer2->Width = 30;

	hUploadButtonPanel->Children->Add(hSpacer1);
	hUploadButtonPanel->Children->Add(hBorder);
	hUploadButtonPanel->Children->Add(hSpacer2);
}
//setting upload buttons in upload tab
void WPFCppCliDemo::setUploadUploadButton(){
	hUploadButton->Content = "Upload file";
	Border^ hBorder = gcnew Border();
	hBorder->Width = 100;
	hBorder->Height = 30;
	hBorder->BorderThickness = Thickness(1);
	hBorder->BorderBrush = Brushes::Black;
	hBorder->Child = hUploadButton;

	TextBlock^ hSpacer1 = gcnew TextBlock();
	hSpacer1->Width = 180;

	hUploadButtonPanel->Children->Add(hBorder);
	hUploadButtonPanel->Children->Add(hSpacer1);
}
//setting clear button
void WPFCppCliDemo::setUploadClearButton(){
	hUploadClearButton->Content = "Clear";
	Border^ hBorder = gcnew Border();
	hBorder->Width = 100;
	hBorder->Height = 30;
	hBorder->BorderThickness = Thickness(1);
	hBorder->BorderBrush = Brushes::Black;
	hBorder->Child = hUploadClearButton;

	hUploadButtonPanel->Children->Add(hBorder);

}

//----------------------
//----download view
//----------------------
//setting up download view
void WPFCppCliDemo::setUpDownloadView(){
	Console::Write("\n  setting up download view");
	hDownloadGrid->Margin = Thickness(20);
	hDownloadFileTab->Content = hDownloadGrid;

	RowDefinition^ hRow1Def = gcnew RowDefinition();
	hRow1Def->Height = GridLength(35);
	hDownloadGrid->RowDefinitions->Add(hRow1Def);
	setDownloadText();
	hDownloadTextPanel->Orientation = Orientation::Horizontal;
	hDownloadGrid->SetRow(hDownloadTextPanel, 0);
	hDownloadGrid->Children->Add(hDownloadTextPanel);

	RowDefinition^ hRow2Def = gcnew RowDefinition();
	hRow2Def->Height = GridLength(320);
	hDownloadGrid->RowDefinitions->Add(hRow2Def);
	setListBoxOfRadiobox();
	setDownloadFilesList();
	setDownloadResultBlock();
	hStackPanelDownload->Orientation = Orientation::Horizontal;
	hDownloadGrid->SetRow(hStackPanelDownload, 1);
	hDownloadGrid->Children->Add(hStackPanelDownload);

	RowDefinition^ hRow3Def = gcnew RowDefinition();
	hRow3Def->Height = GridLength(35);
	hDownloadGrid->RowDefinitions->Add(hRow3Def);
	setDownloadButtons();
	hDownloadButtonPanel->Orientation = Orientation::Horizontal;
	hDownloadGrid->SetRow(hDownloadButtonPanel, 2);
	hDownloadGrid->Children->Add(hDownloadButtonPanel);

}
//setting up download view texts
void WPFCppCliDemo::setDownloadText(){
	hDownFromServersTx->Text = hDownFromServersString;

	Border^ hBorder1 = gcnew Border();
	hBorder1->Width = 100;
	hBorder1->Height = 30;
	hBorder1->BorderThickness = Thickness(0);
	hBorder1->Child = hDownFromServersTx;

	hSelectDownloadFileTx->Text = hSelectDownloadFileString;
	Border^ hBorder2 = gcnew Border();
	hBorder2->Width = 100;
	hBorder2->Height = 30;
	hBorder2->BorderThickness = Thickness(0);
	hBorder2->Child = hSelectDownloadFileTx;

	hDownloadResultTx->Text = hDownloadResultString;
	Border^ hBorder3 = gcnew Border();
	hBorder3->Width = 100;
	hBorder3->Height = 30;
	hBorder3->BorderThickness = Thickness(0);
	hBorder3->Child = hDownloadResultTx;

	TextBlock^ hSpacer1 = gcnew TextBlock();
	hSpacer1->Width = 50;
	TextBlock^ hSpacer2 = gcnew TextBlock();
	hSpacer2->Width = 250;

	hDownloadTextPanel->Children->Add(hBorder1);
	hDownloadTextPanel->Children->Add(hSpacer1);
	hDownloadTextPanel->Children->Add(hBorder2);
	hDownloadTextPanel->Children->Add(hSpacer2);
	hDownloadTextPanel->Children->Add(hBorder3);
}
//setting list box of radio box with current servers
void WPFCppCliDemo::setListBoxOfRadiobox(){
	downloadServer1Radio->GroupName = "server";
	downloadServer2Radio->GroupName = "server";

	downloadServer1TX->Text = "server:8000";
	downloadServer1TX->FontFamily = gcnew Windows::Media::FontFamily("Tahoma");
	downloadServer1TX->FontWeight = FontWeights::Bold;
	downloadServer1TX->FontSize = 12;
	Border^ hBorder1 = gcnew Border();
	hBorder1->BorderThickness = Thickness(0);
	hBorder1->Child = downloadServer1TX;
	hBorder1->VerticalAlignment = System::Windows::VerticalAlignment::Center;
	radio1panel->Children->Add(hBorder1);
	radio1panel->Children->Add(downloadServer1Radio);

	downloadServer2TX->Text = "server:8500";
	downloadServer2TX->FontFamily = gcnew Windows::Media::FontFamily("Tahoma");
	downloadServer2TX->FontWeight = FontWeights::Bold;
	downloadServer2TX->FontSize = 12;
	Border^ hBorder2 = gcnew Border();
	hBorder2->BorderThickness = Thickness(0);
	hBorder2->Child = downloadServer2TX;
	hBorder2->VerticalAlignment = System::Windows::VerticalAlignment::Center;
	radio2panel->Children->Add(hBorder2);
	radio2panel->Children->Add(downloadServer2Radio);

	hCurrentServerRadios->Items->Add(radio1panel);
	hCurrentServerRadios->Items->Add(radio2panel);
	Border^ hBorder = gcnew Border();
	hBorder->Width = 100;
	hBorder->Height = 300;
	hBorder->BorderThickness = Thickness(1);
	hBorder->BorderBrush = Brushes::Black;
	hBorder->Child = hCurrentServerRadios;

	TextBlock^ hSpacer = gcnew TextBlock();
	hSpacer->Width = 50;

	hStackPanelDownload->Children->Add(hBorder);
	hStackPanelDownload->Children->Add(hSpacer);
}
//setting download file list block
void WPFCppCliDemo::setDownloadFilesList(){
	hServerFileListScroll->VerticalScrollBarVisibility = ScrollBarVisibility::Auto;
	hServerFileListScroll->HorizontalScrollBarVisibility = ScrollBarVisibility::Auto;
	hServerFileListScroll->Content = hServerFileListBox;

	Border^ hBorder = gcnew Border();
	hBorder->Width = 300;
	hBorder->Height = 300;
	hBorder->BorderThickness = Thickness(1);
	hBorder->BorderBrush = Brushes::Black;
	hBorder->Child = hServerFileListScroll;

	TextBlock^ hSpacer = gcnew TextBlock();
	hSpacer->Width = 50;
	hStackPanelDownload->Children->Add(hBorder);
	hStackPanelDownload->Children->Add(hSpacer);
}
//setting download result block
void WPFCppCliDemo::setDownloadResultBlock(){

	hDownloadResultBlock->Text = "";
	hDownloadResultBlock->TextWrapping = TextWrapping::Wrap;

	hDownloadResultScroll->VerticalScrollBarVisibility = ScrollBarVisibility::Auto;
	hDownloadResultScroll->HorizontalScrollBarVisibility = ScrollBarVisibility::Auto;
	hDownloadResultScroll->Content = hDownloadResultBlock;

	Border^ hBorder = gcnew Border();
	hBorder->Width = 200;
	hBorder->Height = 300;
	hBorder->BorderThickness = Thickness(1);
	hBorder->BorderBrush = Brushes::Black;
	hBorder->Child = hDownloadResultScroll;

	hStackPanelDownload->Children->Add(hBorder);
}
//setting download view buttons
void WPFCppCliDemo::setDownloadButtons(){
	setDownloadBrowseButton();
	setDownloadUploadButton();
	setDownloadClearButton();
}
//setting up download view browse button
void WPFCppCliDemo::setDownloadBrowseButton(){
	hDownloadBrowseButton->Content = "Browse file";
	Border^ hBorder = gcnew Border();
	hBorder->Width = 100;
	hBorder->Height = 30;
	hBorder->BorderThickness = Thickness(1);
	hBorder->BorderBrush = Brushes::Black;
	hBorder->Child = hDownloadBrowseButton;

	TextBlock^ hSpacer1 = gcnew TextBlock();
	hSpacer1->Width = 180;
	TextBlock^ hSpacer2 = gcnew TextBlock();
	hSpacer2->Width = 30;

	hDownloadButtonPanel->Children->Add(hSpacer1);
	hDownloadButtonPanel->Children->Add(hBorder);
	hDownloadButtonPanel->Children->Add(hSpacer2);
}
//setting up download view download button
void WPFCppCliDemo::setDownloadUploadButton(){
	hDownloadButton->Content = "Download file";
	Border^ hBorder = gcnew Border();
	hBorder->Width = 100;
	hBorder->Height = 30;
	hBorder->BorderThickness = Thickness(1);
	hBorder->BorderBrush = Brushes::Black;
	hBorder->Child = hDownloadButton;

	TextBlock^ hSpacer1 = gcnew TextBlock();
	hSpacer1->Width = 180;

	hDownloadButtonPanel->Children->Add(hBorder);
	hDownloadButtonPanel->Children->Add(hSpacer1);
}
//setting clear button
void WPFCppCliDemo::setDownloadClearButton(){
	hDownloadClearButton->Content = "Clear";
	Border^ hBorder = gcnew Border();
	hBorder->Width = 100;
	hBorder->Height = 30;
	hBorder->BorderThickness = Thickness(1);
	hBorder->BorderBrush = Brushes::Black;
	hBorder->Child = hDownloadClearButton;

	hDownloadButtonPanel->Children->Add(hBorder);

}
std::string WPFCppCliDemo::toStdString(String^ pStr)
{
  std::string dst;
  for (int i = 0; i < pStr->Length; ++i)
    dst += (char)pStr[i];
  return dst;
}

String^ WPFCppCliDemo::toSystemString(std::string& str)
{
  StringBuilder^ pStr = gcnew StringBuilder();
  for (size_t i = 0; i < str.size(); ++i)
    pStr->Append((Char)str[i]);
  return pStr->ToString();
}

//process message received from server
void WPFCppCliDemo::getMessage()
{
  // recvThread runs this function
	//auto time_now = std::chrono::high_resolution_clock::now();
	
  while (true)
  {
	

    std::cout << "\n  receive thread calling getMessage()";
    Message msg = pRecvr_->deQueue();

	auto after_time = std::chrono::system_clock::now();
	auto t = std::chrono::duration_cast<std::chrono::milliseconds>(after_time - *time_before);

	msg.addAttrib("COMMUNICATE_TIME", toStdString(t.count().ToString()));
    String^ sMsg = toSystemString(msg.toString());
    array<String^>^ args = gcnew array<String^>(1);
    args[0] = sMsg;

	if (msg.getHeader().getCommand() == "EXPLORE_RESULT"){
		Action<String^>^ act = gcnew Action<String^>(this, &WPFCppCliDemo::showExplreResult);
		Dispatcher->Invoke(act, args);  
	}
	if (msg.getHeader().getCommand() == "SEARCH_RESULT"){
		Action<String^>^ act = gcnew Action<String^>(this, &WPFCppCliDemo::showSearchResult);
		Dispatcher->Invoke(act, args);  
	}
	if (msg.getHeader().getCommand() == "UPLOAD_RECEIPT"){
		Action<String^>^ act = gcnew Action<String^>(this, &WPFCppCliDemo::showUploadResult);
		Dispatcher->Invoke(act, args);
	}
	if (msg.getHeader().getCommand() == "RECEIVED_FILE"){
		Console::Write(L"window received upload command");
		Action<String^>^ act = gcnew Action<String^>(this, &WPFCppCliDemo::showDownloadResult);
		Dispatcher->Invoke(act, args);
	}
  }
}
//show explore result
void WPFCppCliDemo::showExplreResult(String^ msg){
	std::string header,server_port,dir,edge;
	Message m(toStdString(msg));
	std::string res_time,com_time;
	for (auto at : m.getHeader().getAttributes()){
		if (at.first == "PORT")server_port = at.second;
		if (at.first == "DIRECTORY")dir = at.second;
		if (at.first == "RESPONSE_TIME")res_time = at.second;
		if (at.first == "COMMUNICATE_TIME")com_time = at.second;
	}
	edge = "\n========================\n";
	header = "The explore result of Directory: " + dir + " in server: " + server_port + " is: \n"+m.getBody();
	header = edge+header+"\nThe response time for expolre is: "+res_time+" milliseconds\n";
	header += ("The communication time for expolring between client and server is:"+com_time+" milliseconds\n");
	hSearchOutputBlock->Text += toSystemString(header);
}

//show search result
void WPFCppCliDemo::showSearchResult(String^ msg){
	std::string header, server_port, dir, edge;
	Message m(toStdString(msg));
	std::string res_time, com_time;
	for (auto at : m.getHeader().getAttributes()){
		if (at.first == "PORT")server_port = at.second;
		if (at.first == "DIRECTORY")dir = at.second;
		if (at.first == "RESPONSE_TIME")res_time = at.second;
		if (at.first == "COMMUNICATE_TIME")com_time = at.second;
	}
	edge = "\n========================\n";
	header = "The searching result of files in " + dir + " in server " + server_port + " is: \n" + m.getBody();
	header = edge + header + "\nThe response time for searching is: " + res_time + " milliseconds\n";
	header += ("The communication time for searching on server is:" + com_time + " milliseconds\n");
	hSearchOutputBlock->Text += toSystemString(header);
}
//show upload result
void WPFCppCliDemo::showUploadResult(String^ msg){
	std::string header, server_port, dir, edge,fn;
	Message m(toStdString(msg));
	std::string res_time, com_time;
	for (auto at : m.getHeader().getAttributes()){
		if (at.first == "FILE_NAME")fn = at.second;
		if (at.first == "PORT")server_port = at.second;
		if (at.first == "DIRECTORY")dir = at.second;
		if (at.first == "RESPONSE_TIME")res_time = at.second;
		if (at.first == "COMMUNICATE_TIME")com_time = at.second;
	}
	edge = "\n========================\n";
	header = "The file: " + fn + " in " + dir + " sucessfull uploaded to server: " + server_port+"\n";
	header = edge + header + "\nThe response time for upload is: " + res_time + " milliseconds\n";
	header += ("The communication time for uploading to server is:" + com_time + " milliseconds\n");
	hUploadResultBlock->Text += toSystemString(header);
}
//show download result
void WPFCppCliDemo::showDownloadResult(String^ msg){
	std::string header, server_port, dir, edge, fn;
	Message m(toStdString(msg));
	std::string res_time, com_time;
	for (auto at : m.getHeader().getAttributes()){
		if (at.first == "FILE_NAME")fn = at.second;
		if (at.first == "PORT")server_port = at.second;
		if (at.first == "DIRECTORY")dir = at.second;
		if (at.first == "RESPONSE_TIME")res_time = at.second;
		if (at.first == "COMMUNICATE_TIME")com_time = at.second;
	}
	edge = "\n========================\n";
	header = "The file: " + fn + " in " + dir + " sucessfull download from server: " + server_port + "\n";
	header = edge + header + "\nThe response time for download is: " + res_time + " milliseconds\n";
	header += ("The communication time for downloading from server is:" + com_time + " milliseconds\n");
	hDownloadResultBlock->Text += toSystemString(header);
}

//----when received a message, analyse and send it
bool WPFCppCliDemo::processSendingTask(const Message &msg){
	Message m(msg);
	std::string command = m.getHeader().getCommand();
	std::string filename, dir;

	if (command == "UPLOAD_FILE"){
		for (auto at : m.getHeader().getAttributes()){
			if (at.first == "FILE_NAME")
				filename = at.second;
			if (at.first == "DIRECTORY")
				dir = at.second;
		}
		sendFile(filename, dir);
	}
	else{
		addToSendingQ(msg);
		sendMessageOut();
	}
	Console::Write(L"Sending message to server :"+toSystemString(command));
	//Console::Write(L"msg=:"+toSystemString(command));
	return true;
}
//----send one message from the sending queue

bool WPFCppCliDemo::sendMessageOut(){

	return pSendr_->sendMessage();
}
//-----add a message task to sender's queue

void WPFCppCliDemo::addToSendingQ(const Message &msg){
	Message msg_with_port(msg);
	msg_with_port.addAttrib("PORT", toStdString(port_str));
	pSendr_->enQueue(msg_with_port);
}
//-----send a file to client port

bool WPFCppCliDemo::sendFile(const std::string& file_name, const std::string& dir){
	Message msg;
	msg.addCommand("UPLOAD_FILE");
	msg.addAttrib("FILE_NAME", file_name);
	msg.addAttrib("DIRECTORY", dir);
	addToSendingQ(msg);
	if (!sendMessageOut())return false;
	else return true;
}
//----connet to a Client if want to send message

bool WPFCppCliDemo::connectToServer(String^ ip, size_t port){
	Console::Write("[connect]connect to server :" + toSystemString(std::to_string(port)) + "\n");
	hStatus->Text = "connecting to server " + toSystemString(std::to_string(port));
	return pSendr_->connetToServer(toStdString(ip), port);
}

/******************************
	button events
*******************************/

//setting up connect to server 1 button
void WPFCppCliDemo::connectToS1(Object^ sender, RoutedEventArgs^ args){
	Console::Write("\n  connecting to server:8000");
	hStatus->Text = "server:8000 connecting";
	pSendr_->shutDownSend();
	if (pSendr_->connetToServer("localhost", 8000)){
		hStatus->Text = "sucessfull connect to server 8000!";
		Console::Write("\n sucessfull connect to server 8000!");
	}
	else {
		hStatus->Text = "connecting to server 8000 failed!";
		Console::Write("\n connecting to server 8000 failed!");
	}
}
//setting up connect to server 2 button
void WPFCppCliDemo::connectToS2(Object^ sender, RoutedEventArgs^ args){
	Console::Write("\n  connecting to server:8500");
	hStatus->Text = "server:8500 connecting";
	pSendr_->shutDownSend();
	if (pSendr_->connetToServer("localhost", 8500)){
		hStatus->Text = "sucessfull connect to server 8500!";
		Console::Write("\n sucessfull connect to server 8500!");
	}
	else {
		hStatus->Text = "connecting to server 8500 failed!";
		Console::Write("\n connecting to server 8500 failed!");
	}
}
//setting search button
void WPFCppCliDemo::setFileSearchButton(Object^ sender, RoutedEventArgs^ args){
	Console::Write("\nsearching files");

	if ((!fileSearchServer1Box->IsChecked) && (!fileSearchServer2Box->IsChecked))
	{
		hSearchOutputBlock->Text += "\nError: Please choose at least one server.";
		return;
	}
	if (hSearchPathBox->Text == ""){
		hSearchOutputBlock->Text += "\nError: Please input a path to search.";
		return;
	}
	if (fileSearchServer1Box->IsChecked){
		searchInS1(sender, args);
	}
	if (fileSearchServer2Box->IsChecked){
		searchInS2(sender, args);
	}
}
//search file in server 1
void WPFCppCliDemo::searchInS1(Object^ sender, RoutedEventArgs^ args){
	*time_before = std::chrono::system_clock::now();
	connectToS1(sender, args);
	Message m;
	m.addCommand("SEARCH_FILE");
	std::vector<std::string> patterns = getPattersFromInput(toStdString(hTextSearchPatternBox->Text));
	if (patterns.size() == 0){
		hSearchOutputBlock->Text += "\nError:Please input at least one pattern to search";
		return;
	}
	for (auto pat : patterns){
		m.addAttrib("FILE_PATTERN",pat);
	}
	std::string searchtext = toStdString(hTextSearchBox->Text);
	m.addAttrib("SEARCH_TEXT", searchtext);
	m.addAttrib("DIRECTORY", toStdString(hSearchPathBox->Text));
	processSendingTask(m);
}
//search file in server 2
void WPFCppCliDemo::searchInS2(Object^ sender, RoutedEventArgs^ args){
	*time_before = std::chrono::system_clock::now();

	connectToS2(sender, args);
	Message m;
	m.addCommand("SEARCH_FILE");
	std::vector<std::string> patterns = getPattersFromInput(toStdString(hTextSearchPatternBox->Text));
	if (patterns.size() == 0){
		hSearchOutputBlock->Text += "\nError:Please input at least one pattern to search";
		return;
	}
	for (auto pat : patterns){
		m.addAttrib("FILE_PATTERN", pat);
	}
	std::string searchtext = toStdString(hTextSearchBox->Text);
	m.addAttrib("SEARCH_TEXT", searchtext);
	m.addAttrib("DIRECTORY", toStdString(hSearchPathBox->Text));

	processSendingTask(m);
}
//explore button event
void WPFCppCliDemo::setExploreButton(Object^ sender, RoutedEventArgs^ args){
	Console::Write("\n exploring server");

	if ((!fileSearchServer1Box->IsChecked) && (!fileSearchServer2Box->IsChecked))
	{
		hSearchOutputBlock->Text += "\nError: Please choose at least one server to explore.";
		return;
	}
	if (hSearchPathBox->Text == ""){
		hSearchOutputBlock->Text += "\nError: Please input a path to explore";
		return;
	}
	if (fileSearchServer1Box->IsChecked){
		exploreS1(sender, args);
	}
	//*time_before = std::chrono::system_clock::now();

	if (fileSearchServer2Box->IsChecked){
		exploreS2(sender, args);
	}
}
//explore server 1
void WPFCppCliDemo::exploreS1(Object^ sender, RoutedEventArgs^ args){
	*time_before = std::chrono::system_clock::now();

	connectToS1(sender, args);

	Message m;
	m.addCommand("EXPLORE_DIR");
	m.addAttrib("DIRECTORY", toStdString(hSearchPathBox->Text));
	processSendingTask(m);
}
//explore server 2
void WPFCppCliDemo::exploreS2(Object^ sender, RoutedEventArgs^ args){
	*time_before = std::chrono::system_clock::now();

	connectToS2(sender, args);
	
	Message m;
	m.addCommand("EXPLORE_DIR");
	m.addAttrib("DIRECTORY", toStdString(hSearchPathBox->Text));
	processSendingTask(m);
}
//parse file pattern input into vector of patterns
std::vector<std::string> WPFCppCliDemo::getPattersFromInput(const std::string& patterns){
	std::vector<std::string> v;
	std::string p = patterns;
	size_t pos = 0;
	while ((pos = p.find(" ")) != std::string::npos) {
		std::string pat = p.substr(0, pos);
		v.push_back(pat);
		p = p.substr(pos + 1);
	}
	v.push_back(p);
	return v;
}
//upload browse button setting
void WPFCppCliDemo::uploadBrowse(Object^ sender, RoutedEventArgs^ args){
	hFileLookupListBox->Items->Clear();

	System::Windows::Forms::DialogResult result;
	String^ dir=System::IO::Directory::GetCurrentDirectory();
	hUploadBrowserDialog->SelectedPath = dir;

	result = hUploadBrowserDialog->ShowDialog();
	  if (result == System::Windows::Forms::DialogResult::OK)
	  {
		String^ path = hUploadBrowserDialog->SelectedPath;    
	    array<String^>^ files = System::IO::Directory::GetFiles(path, L"*.*");
	    for (int i = 0; i < files->Length; ++i)
			hFileLookupListBox->Items->Add(files[i]);
	    array<String^>^ dirs = System::IO::Directory::GetDirectories(path);
	    
	  }

}
//upload button
void WPFCppCliDemo::upload(Object^ sender, RoutedEventArgs^ args){
	if (hFileLookupListBox->SelectedIndex == -1){
		hUploadResultBlock->Text += "\nError: Please select a file to upload.";
		return;
	}
	std::string filename = toStdString(hFileLookupListBox->SelectedItem->ToString());
	if ((!uploadServer1Box->IsChecked) && (!uploadServer2Box->IsChecked))
	{
		hUploadResultBlock->Text += "\nError: Please choose at least one server to upload.";
		return;
	}
	if (uploadServer1Box->IsChecked){
		*time_before = std::chrono::system_clock::now();

		connectToS1(sender, args);
		Message m;
		m.addCommand("UPLOAD_FILE");
		m.addAttrib("FILE_NAME", filename);
		m.addAttrib("DIRECTORY", "server_8000");
		processSendingTask(m);
		Console::Write("uploading to server:8000");
		hStatus->Text = "uploading to server:8000";
	}

	if (uploadServer2Box->IsChecked){
		*time_before = std::chrono::system_clock::now();

		connectToS2(sender, args);
		Message m;
		m.addCommand("UPLOAD_FILE");
		m.addAttrib("FILE_NAME", filename);
		m.addAttrib("DIRECTORY", "server_8500");
		processSendingTask(m);
		Console::Write("uploading to server:8500");
		hStatus->Text = "uploading to server:8500";
	}
}
//download browse button
void WPFCppCliDemo::downloadBrowse(Object^ sender, RoutedEventArgs^ args){
	hServerFileListBox->Items->Clear();

	System::Windows::Forms::DialogResult result;
	String^ dir = System::IO::Directory::GetCurrentDirectory();
	hDownBrowserDialog->SelectedPath = dir;

	result = hDownBrowserDialog->ShowDialog();
	if (result == System::Windows::Forms::DialogResult::OK)
	{
		String^ path = hDownBrowserDialog->SelectedPath;
		array<String^>^ files = System::IO::Directory::GetFiles(path, L"*.*");
		for (int i = 0; i < files->Length; ++i)
			hServerFileListBox->Items->Add(files[i]);
	}
	/*if ((!downloadServer1Radio->IsChecked) && (!downloadServer2Radio->IsChecked)){
		hDownloadResultBlock->Text += "\nError: Please choose one server to browse files.";
		return;
	}*/
	//System::Windows::Forms::DialogResult result;
	//String^ dir = System::IO::Directory::GetCurrentDirectory();
	//hUploadBrowserDialog->SelectedPath = dir;
	//if (downloadServer1Radio->IsChecked){
	//	String^ serverdir = "../server_8000";
	//	//hUploadBrowserDialog->SelectedPath = dir;
	//	array<String^>^ files = System::IO::Directory::GetFiles(serverdir, L"*.*");
	//	for (int i = 0; i < files->Length; ++i)
	//		hServerFileListBox->Items->Add(files[i]);
	//	array<String^>^ dirs = System::IO::Directory::GetDirectories(serverdir);
	//	for (int i = 0; i < dirs->Length; ++i)
	//		hServerFileListBox->Items->Add(L"<> " + dirs[i]);
	//}
	//else{
	//	String^ serverdir = "../server_8500";
	//	//hUploadBrowserDialog->SelectedPath = dir;
	//	array<String^>^ files = System::IO::Directory::GetFiles(serverdir, L"*.*");
	//	for (int i = 0; i < files->Length; ++i)
	//		hServerFileListBox->Items->Add(files[i]);
	//	array<String^>^ dirs = System::IO::Directory::GetDirectories(serverdir);
	//	for (int i = 0; i < dirs->Length; ++i)
	//		hServerFileListBox->Items->Add(L"<> " + dirs[i]);
	//}

}
//download button
void WPFCppCliDemo::download(Object^ sender, RoutedEventArgs^ args){
	if (hServerFileListBox->SelectedIndex == -1){
		hDownloadResultBlock->Text += "\nError: Please select a file to download.";
		return;
	}
	std::string filename = toStdString(hServerFileListBox->SelectedItem->ToString());
	if ((!downloadServer1Radio->IsChecked) && (!downloadServer2Radio->IsChecked))
	{
		hDownloadResultBlock->Text += "\nError: Please choose one server to download.";
		return;
	}
	if (downloadServer1Radio->IsChecked){
		*time_before = std::chrono::system_clock::now();

		connectToS1(sender, args);
		Message m;
		m.addCommand("DOWNLOAD_FILE");
		m.addAttrib("FILE_NAME", filename);
		m.addAttrib("DIRECTORY", "./");
		processSendingTask(m);
		Console::Write("downloading from server:8000");
		hStatus->Text = "downloading from server:8000";
	}
	if (downloadServer2Radio->IsChecked){
		*time_before = std::chrono::system_clock::now();

		connectToS2(sender, args);
		Message m;
		m.addCommand("DOWNLOAD_FILE");
		m.addAttrib("FILE_NAME", filename);
		m.addAttrib("DIRECTORY", "./");
		processSendingTask(m);
		Console::Write("downloading from server:8500");
		hStatus->Text = "downloading from server:8500";
	}
}

void WPFCppCliDemo::OnLoaded(Object^ sender, RoutedEventArgs^ args)
{
  Console::Write("\n  Window loaded");
}
void WPFCppCliDemo::Unloading(Object^ sender, System::ComponentModel::CancelEventArgs^ args)
{
  Console::Write("\n  Window closing");
}
void WPFCppCliDemo::searchClear(Object^ sender, RoutedEventArgs^ args){
	hSearchOutputBlock->Text = "";
}
void WPFCppCliDemo::uploadClear(Object^ sender, RoutedEventArgs^ args){
	hUploadResultBlock->Text = "";
}
void WPFCppCliDemo::downloadClear(Object^ sender, RoutedEventArgs^ args){
	hDownloadResultBlock->Text = "";
}
//----------------------------
//----setting up client
//----------------------------

//----shut down sending
bool WPFCppCliDemo::shutDownSend(){
	return pSendr_->shutDownSend();
}

[STAThread]
//int _stdcall WinMain()
int main(array<System::String^>^ args)
{
	System::String^ portstr = "9000";
  Console::WriteLine(L"\n Starting WPFCppCliDemo");
  Application^ app = gcnew Application();
  //app->Run(gcnew WPFCppCliDemo(args[1]));
  app->Run(gcnew WPFCppCliDemo(portstr));



  Console::WriteLine(L"\n\n");
}