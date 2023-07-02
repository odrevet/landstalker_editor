//////////////////////////////////////////////////////////////////////
// This file was auto-generated by codelite's wxCrafter Plugin
// wxCrafter project file: wxcrafter.wxcp
// Do not modify this file by hand!
//////////////////////////////////////////////////////////////////////

#include "wxcrafter.h"


// Declare the bitmap loading function
extern void wxC9ED9InitBitmapResources();

static bool bBitmapLoaded = false;


MainFrameBaseClass::MainFrameBaseClass(wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style)
    : wxFrame(parent, id, title, pos, size, style)
{
    if ( !bBitmapLoaded ) {
        // We need to initialise the default bitmap handler
        wxXmlResource::Get()->AddHandler(new wxBitmapXmlHandler);
        wxC9ED9InitBitmapResources();
        bBitmapLoaded = true;
    }
    // Set icon(s) to the application/dialog
    wxIconBundle app_icons;
    {
        wxBitmap iconBmp = wxXmlResource::Get()->LoadBitmap(wxT("msword"));
        wxIcon icn;
        icn.CopyFromBitmap(iconBmp);
        app_icons.AddIcon( icn );
    }
    {
        wxBitmap iconBmp = wxXmlResource::Get()->LoadBitmap(wxT("msword"));
        wxIcon icn;
        icn.CopyFromBitmap(iconBmp);
        app_icons.AddIcon( icn );
    }
    {
        wxBitmap iconBmp = wxXmlResource::Get()->LoadBitmap(wxT("msword"));
        wxIcon icn;
        icn.CopyFromBitmap(iconBmp);
        app_icons.AddIcon( icn );
    }
    {
        wxBitmap iconBmp = wxXmlResource::Get()->LoadBitmap(wxT("msword"));
        wxIcon icn;
        icn.CopyFromBitmap(iconBmp);
        app_icons.AddIcon( icn );
    }
    {
        wxBitmap iconBmp = wxXmlResource::Get()->LoadBitmap(wxT("msword"));
        wxIcon icn;
        icn.CopyFromBitmap(iconBmp);
        app_icons.AddIcon( icn );
    }
    SetIcons( app_icons );

    m_config = new wxConfig(APPLICATION_NAME + "_conf");
    
    m_menubar = new wxMenuBar(0);
    this->SetMenuBar(m_menubar);
    
    m_mnu_file = new wxMenu();
    m_menubar->Append(m_mnu_file, _("&File"));
    
    m_mnu_open = new wxMenuItem(m_mnu_file, wxID_ANY, _("&Open\tCtrl-O"), _("Open"), wxITEM_NORMAL);
    m_mnu_file->Append(m_mnu_open);

    m_mnu_save_to_rom = new wxMenuItem(m_mnu_file, wxID_ANY, _("Save As &ROM...\tF2"), _("Save As Rom"), wxITEM_NORMAL);
    m_mnu_file->Append(m_mnu_save_to_rom);

    m_mnu_save_as_asm = new wxMenuItem(m_mnu_file, wxID_ANY, _("Save As &Assembly...\tF3"), _("Save As Assembly"), wxITEM_NORMAL);
    m_mnu_file->Append(m_mnu_save_as_asm);

    m_mnu_save = new wxMenuItem(m_mnu_file, wxID_ANY, _("&Save...\tCtrl-S"), _("Save Changes"), wxITEM_NORMAL);
    m_mnu_file->Append(m_mnu_save);

    m_mnu_build_asm = new wxMenuItem(m_mnu_file, wxID_ANY, _("&Build Assembly...\tF5"), _("Build Assembly"), wxITEM_NORMAL);
    m_mnu_file->Append(m_mnu_build_asm);

    m_mnu_run_emu = new wxMenuItem(m_mnu_file, wxID_ANY, _("Run ROM in &Emulator\tF6"), _("Run ROM in Emulator"), wxITEM_NORMAL);
    m_mnu_file->Append(m_mnu_run_emu);
    
    m_mnu_file->AppendSeparator();

    m_mnu_recent_files = new wxMenu();
    m_mnu_file->AppendSubMenu(m_mnu_recent_files, "Open Re&cent");

    m_mnu_file->AppendSeparator();

    m_mnu_preferences = new wxMenuItem(m_mnu_file, wxID_ANY, _("&Preferences...\tF12"), _("Preferences"), wxITEM_NORMAL);
    m_mnu_file->Append(m_mnu_preferences);
    
    m_mnu_exit = new wxMenuItem(m_mnu_file, wxID_EXIT, _("E&xit\tAlt-X"), _("Quit"), wxITEM_NORMAL);
    m_mnu_file->Append(m_mnu_exit);

    
    m_mnu_help = new wxMenu();
    m_menubar->Append(m_mnu_help, _("&Help"));
    
    m_mnu_about = new wxMenuItem(m_mnu_help, wxID_ABOUT, _("&About..."), wxT(""), wxITEM_NORMAL);
    m_mnu_help->Append(m_mnu_about);
    
    m_statusbar = new wxStatusBar(this, wxID_ANY, wxSTB_DEFAULT_STYLE);
    m_statusbar->SetFieldsCount(3);
    this->SetStatusBar(m_statusbar);
    
    m_window = new wxAuiManager;
    m_window->SetManagedWindow( this );
    m_window->SetFlags( wxAUI_MGR_LIVE_RESIZE|wxAUI_MGR_TRANSPARENT_HINT|wxAUI_MGR_TRANSPARENT_DRAG|wxAUI_MGR_ALLOW_ACTIVE_PANE|wxAUI_MGR_ALLOW_FLOATING);
    m_window->GetArtProvider()->SetMetric(wxAUI_DOCKART_GRADIENT_TYPE, wxAUI_GRADIENT_NONE);
    
    m_panel_browser = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDLG_UNIT(this, wxSize(-1,-1)), wxTAB_TRAVERSAL);
    
    wxAuiPaneInfo browserPane = wxAuiPaneInfo()
                                  .Caption(_("Browser"))
                                  .Direction(wxAUI_DOCK_LEFT)
                                  .Layer(0)
                                  .Row(0)
                                  .Position(0)
                                  .MinSize(100,100)
                                  .BestSize(300,100)
                                  .FloatingSize(300,300)
                                  .CaptionVisible(true)
                                  .MaximizeButton(false)
                                  .CloseButton(false)
                                  .MinimizeButton(false)
                                  .PinButton(false);
    browserPane.dock_proportion = 1;
    m_window->AddPane(m_panel_browser, browserPane);
    
    wxBoxSizer* m_boxsizer_browser = new wxBoxSizer(wxVERTICAL);
    m_panel_browser->SetSizer(m_boxsizer_browser);
    
    m_browser = new wxTreeCtrl(m_panel_browser, wxID_ANY, wxDefaultPosition, wxDLG_UNIT(m_panel_browser, wxSize(300,50)), wxTR_DEFAULT_STYLE|wxTR_HIDE_ROOT|wxFULL_REPAINT_ON_RESIZE);
    
    m_boxsizer_browser->Add(m_browser, 1, wxALL|wxEXPAND, WXC_FROM_DIP(5));
    m_browser->SetMinSize(wxSize(300,50));
    
    m_panel_properties = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDLG_UNIT(this, wxSize(-1,-1)), wxTAB_TRAVERSAL);
    
    wxAuiPaneInfo propsPane = wxAuiPaneInfo()
                                .Caption(_("Properties"))
                                .Direction(wxAUI_DOCK_LEFT)
                                .Layer(0)
                                .Row(0)
                                .Position(1)
                                .MinSize(100,100)
                                .BestSize(300,100)
                                .FloatingSize(300,600)
                                .CaptionVisible(true)
                                .MaximizeButton(false)
                                .CloseButton(false)
                                .MinimizeButton(false)
                                .PinButton(false);
    propsPane.dock_proportion = 2;
    m_window->AddPane(m_panel_properties, propsPane);
    
    wxBoxSizer* m_boxsizer_properties = new wxBoxSizer(wxVERTICAL);
    m_panel_properties->SetSizer(m_boxsizer_properties);
    
    m_properties = new wxPropertyGridManager(m_panel_properties, wxID_ANY, wxDefaultPosition, wxDLG_UNIT(m_panel_properties, wxSize(300,50)), wxPG_LIMITED_EDITING|wxPG_BOLD_MODIFIED);

    m_boxsizer_properties->Add(m_properties, 1, wxALL|wxEXPAND, WXC_FROM_DIP(5));
    m_properties->SetMinSize(wxSize(300,50));

    m_mainwin = new wxWindow(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxFULL_REPAINT_ON_RESIZE);
    m_mainwin->SetBackgroundColour(*wxBLACK);

    m_window->AddPane(m_mainwin, wxAuiPaneInfo().CenterPane().Layer(0).Row(0).Position(0));
    m_window->Update();
    
    SetName(wxT("MainFrameBaseClass"));
    SetSize(wxSize(1024,768));
    SetMinSize(wxSize(640,480));
    SetInitialSize(wxSize(1024,768));

    if (GetSizer()) {
         GetSizer()->Fit(this);
    }
    if(GetParent()) {
        CentreOnParent(wxBOTH);
    } else {
        CentreOnScreen(wxBOTH);
    }
#if wxVERSION_NUMBER >= 2900
    if(!wxPersistenceManager::Get().Find(this)) {
        wxPersistenceManager::Get().RegisterAndRestore(this);
    } else {
        wxPersistenceManager::Get().Restore(this);
    }
#endif
    // Connect events
    this->Connect(wxEVT_CLOSE_WINDOW, wxCloseEventHandler(MainFrameBaseClass::OnClose), NULL, this);
    this->Connect(m_mnu_open->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrameBaseClass::OnOpen), NULL, this);
    this->Connect(m_mnu_save_as_asm->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrameBaseClass::OnSaveAsAsm), NULL, this);
    this->Connect(m_mnu_save_to_rom->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrameBaseClass::OnSaveToRom), NULL, this);
    this->Connect(m_mnu_save->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrameBaseClass::OnSave), NULL, this);
    this->Connect(m_mnu_build_asm->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrameBaseClass::OnBuildAsm), NULL, this);
    this->Connect(m_mnu_run_emu->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrameBaseClass::OnRunEmulator), NULL, this);
    this->Connect(wxID_FILE1, wxID_FILE9, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrameBaseClass::OnMRUFile), NULL, this);
    this->Connect(m_mnu_preferences->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrameBaseClass::OnPreferences), NULL, this);
    this->Connect(m_mnu_exit->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrameBaseClass::OnExit), NULL, this);
    this->Connect(m_mnu_about->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrameBaseClass::OnAbout), NULL, this);
    m_browser->Connect(wxEVT_COMMAND_TREE_ITEM_ACTIVATED, wxTreeEventHandler(MainFrameBaseClass::OnBrowserSelect), NULL, this);
    
    m_filehistory = new wxFileHistory(10);
    m_filehistory->UseMenu(m_mnu_recent_files);
    m_filehistory->AddFilesToMenu(m_mnu_recent_files);
    m_filehistory->Load(*m_config);
}

MainFrameBaseClass::~MainFrameBaseClass()
{
    this->Disconnect(wxEVT_CLOSE_WINDOW, wxCloseEventHandler(MainFrameBaseClass::OnClose), NULL, this);
    this->Disconnect(m_mnu_open->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrameBaseClass::OnOpen), NULL, this);
    this->Disconnect(m_mnu_save_as_asm->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrameBaseClass::OnSaveAsAsm), NULL, this);
    this->Disconnect(m_mnu_save_to_rom->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrameBaseClass::OnSaveToRom), NULL, this);
    this->Disconnect(m_mnu_save->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrameBaseClass::OnSave), NULL, this);
    this->Disconnect(m_mnu_build_asm->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrameBaseClass::OnSaveAsAsm), NULL, this);
    this->Disconnect(m_mnu_run_emu->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrameBaseClass::OnSaveToRom), NULL, this);
    this->Disconnect(wxID_FILE1, wxID_FILE9, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrameBaseClass::OnMRUFile), NULL, this);
    this->Disconnect(m_mnu_preferences->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrameBaseClass::OnSaveToRom), NULL, this);
    this->Disconnect(m_mnu_exit->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrameBaseClass::OnExit), NULL, this);
    this->Disconnect(m_mnu_about->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrameBaseClass::OnAbout), NULL, this);
    m_browser->Disconnect(wxEVT_COMMAND_TREE_ITEM_ACTIVATED, wxTreeEventHandler(MainFrameBaseClass::OnBrowserSelect), NULL, this);

    m_filehistory->Save(*m_config);
    m_config->Flush();
    delete m_filehistory;
    delete m_config;
    m_window->UnInit();
    delete m_window;
}
