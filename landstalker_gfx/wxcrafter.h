//////////////////////////////////////////////////////////////////////
// This file was auto-generated by codelite's wxCrafter Plugin
// wxCrafter project file: wxcrafter.wxcp
// Do not modify this file by hand!
//////////////////////////////////////////////////////////////////////

#ifndef _LANDSTALKER_GFX_WXCRAFTER_BASE_CLASSES_H
#define _LANDSTALKER_GFX_WXCRAFTER_BASE_CLASSES_H

// clang-format off
#include <wx/settings.h>
#include <wx/xrc/xmlres.h>
#include <wx/xrc/xh_bmp.h>
#include <wx/frame.h>
#include <wx/iconbndl.h>
#include <wx/artprov.h>
#include <wx/sizer.h>
#include <wx/menu.h>
#include <wx/statusbr.h>
#include <wx/aui/framemanager.h>
#include <wx/aui/dockart.h>
#include <wx/panel.h>
#include <wx/treectrl.h>
#include <wx/propgrid/manager.h>
#include <wx/radiobut.h>
#include <wx/checkbox.h>
#include <wx/slider.h>
#include <wx/scrolwin.h>
#include <wx/toolbar.h>
#include <wx/imaglist.h>
#include <wx/bitmap.h>
#include <map>
#include <wx/icon.h>
#if wxVERSION_NUMBER >= 2900
#include <wx/persist.h>
#include <wx/persist/toplevel.h>
#include <wx/persist/bookctrl.h>
#include <wx/persist/treebook.h>
#endif

#ifdef WXC_FROM_DIP
#undef WXC_FROM_DIP
#endif
#if wxVERSION_NUMBER >= 3100
#define WXC_FROM_DIP(x) wxWindow::FromDIP(x, NULL)
#else
#define WXC_FROM_DIP(x) x
#endif

// clang-format on

class MainFrameBaseClass : public wxFrame
{
protected:
    wxMenuBar* m_menubar;
    wxMenu* m_mnu_file;
    wxMenuItem* m_mnu_open;
    wxMenuItem* m_mnu_export_png;
    wxMenuItem* m_mnu_export_txt;
    wxMenuItem* m_mnu_sep1;
    wxMenuItem* m_mnu_exit;
    wxMenu* m_mnu_help;
    wxMenuItem* m_mnu_about;
    wxStatusBar* m_statusbar;
    wxAuiManager* m_window;
    wxPanel* m_panel_browser;
    wxTreeCtrl* m_browser;
    wxPanel* m_panel_properties;
    wxPropertyGridManager* m_properties;
    wxPanel* m_panel_layers;
    wxRadioButton* m_optBgSelect;
    wxCheckBox* m_checkBgVisible;
    wxSlider* m_sliderBgOpacity;
    wxRadioButton* m_optFg1Select;
    wxCheckBox* m_checkFg1Visible;
    wxSlider* m_sliderFg1Opacity;
    wxRadioButton* m_optFg2Select;
    wxCheckBox* m_checkFg2Visible;
    wxSlider* m_sliderFg2Opacity;
    wxRadioButton* m_optHeightmapSelect;
    wxCheckBox* m_checkHeightmapVisible;
    wxSlider* m_sliderHeightmapOpacity;
    wxRadioButton* m_optSpritesSelect;
    wxCheckBox* m_checkSpritesVisible;
    wxSlider* m_sliderSpritesOpacity;
    wxScrolledWindow* m_scrollwindow;
    wxPanel* m_panel_tiles;
    wxToolBar* m_toolbar;

protected:
    virtual void OnMousewheel(wxMouseEvent& event)
    {
        event.Skip();
    }
    virtual void OnKeyDown(wxKeyEvent& event)
    {
        event.Skip();
    }
    virtual void OnKeyUp(wxKeyEvent& event)
    {
        event.Skip();
    }
    virtual void OnOpen(wxCommandEvent& event)
    {
        event.Skip();
    }
    virtual void OnExportPng(wxCommandEvent& event)
    {
        event.Skip();
    }
    virtual void OnExportTxt(wxCommandEvent& event)
    {
        event.Skip();
    }
    virtual void OnExit(wxCommandEvent& event)
    {
        event.Skip();
    }
    virtual void OnAbout(wxCommandEvent& event)
    {
        event.Skip();
    }
    virtual void OnBrowserSelect(wxTreeEvent& event)
    {
        event.Skip();
    }
    virtual void OnLayerSelect(wxCommandEvent& event)
    {
        event.Skip();
    }
    virtual void OnLayerVisibilityChange(wxCommandEvent& event)
    {
        event.Skip();
    }
    virtual void OnLayerOpacityChange(wxScrollEvent& event)
    {
        event.Skip();
    }
    virtual void OnScrollWindowPaint(wxPaintEvent& event)
    {
        event.Skip();
    }
    virtual void OnScrollWindowMousewheel(wxMouseEvent& event)
    {
        event.Skip();
    }
    virtual void OnScrollWindowLeftDown(wxMouseEvent& event)
    {
        event.Skip();
    }
    virtual void OnScrollWindowLeftUp(wxMouseEvent& event)
    {
        event.Skip();
    }
    virtual void OnScrollWindowRightDown(wxMouseEvent& event)
    {
        event.Skip();
    }
    virtual void OnScrollWindowRightUp(wxMouseEvent& event)
    {
        event.Skip();
    }
    virtual void OnScrollWindowKeyDown(wxKeyEvent& event)
    {
        event.Skip();
    }
    virtual void OnScrollWindowKeyUp(wxKeyEvent& event)
    {
        event.Skip();
    }
    virtual void OnButton1(wxCommandEvent& event)
    {
        event.Skip();
    }
    virtual void OnButton2(wxCommandEvent& event)
    {
        event.Skip();
    }
    virtual void OnButton3(wxCommandEvent& event)
    {
        event.Skip();
    }
    virtual void OnButton4(wxCommandEvent& event)
    {
        event.Skip();
    }
    virtual void OnButton5(wxCommandEvent& event)
    {
        event.Skip();
    }
    virtual void OnButton6(wxCommandEvent& event)
    {
        event.Skip();
    }
    virtual void OnButton7(wxCommandEvent& event)
    {
        event.Skip();
    }
    virtual void OnButton8(wxCommandEvent& event)
    {
        event.Skip();
    }
    virtual void OnButton9(wxCommandEvent& event)
    {
        event.Skip();
    }
    virtual void OnButton10(wxCommandEvent& event)
    {
        event.Skip();
    }

public:
    wxMenuBar* GetMenubar()
    {
        return m_menubar;
    }
    wxStatusBar* GetStatusbar()
    {
        return m_statusbar;
    }
    wxTreeCtrl* GetBrowser()
    {
        return m_browser;
    }
    wxPanel* GetPanel_browser()
    {
        return m_panel_browser;
    }
    wxPropertyGridManager* GetProperties()
    {
        return m_properties;
    }
    wxPanel* GetPanel_properties()
    {
        return m_panel_properties;
    }
    wxRadioButton* GetOptBgSelect()
    {
        return m_optBgSelect;
    }
    wxCheckBox* GetCheckBgVisible()
    {
        return m_checkBgVisible;
    }
    wxSlider* GetSliderBgOpacity()
    {
        return m_sliderBgOpacity;
    }
    wxRadioButton* GetOptFg1Select()
    {
        return m_optFg1Select;
    }
    wxCheckBox* GetCheckFg1Visible()
    {
        return m_checkFg1Visible;
    }
    wxSlider* GetSliderFg1Opacity()
    {
        return m_sliderFg1Opacity;
    }
    wxRadioButton* GetOptFg2Select()
    {
        return m_optFg2Select;
    }
    wxCheckBox* GetCheckFg2Visible()
    {
        return m_checkFg2Visible;
    }
    wxSlider* GetSliderFg2Opacity()
    {
        return m_sliderFg2Opacity;
    }
    wxRadioButton* GetOptHeightmapSelect()
    {
        return m_optHeightmapSelect;
    }
    wxCheckBox* GetCheckHeightmapVisible()
    {
        return m_checkHeightmapVisible;
    }
    wxSlider* GetSliderHeightmapOpacity()
    {
        return m_sliderHeightmapOpacity;
    }
    wxRadioButton* GetOptSpritesSelect()
    {
        return m_optSpritesSelect;
    }
    wxCheckBox* GetCheckSpritesVisible()
    {
        return m_checkSpritesVisible;
    }
    wxSlider* GetSliderSpritesOpacity()
    {
        return m_sliderSpritesOpacity;
    }
    wxPanel* GetPanel_layers()
    {
        return m_panel_layers;
    }
    wxScrolledWindow* GetScrollwindow()
    {
        return m_scrollwindow;
    }
    wxPanel* GetPanel_tiles()
    {
        return m_panel_tiles;
    }
    wxAuiManager* GetWindow()
    {
        return m_window;
    }
    wxToolBar* GetToolbar()
    {
        return m_toolbar;
    }
    MainFrameBaseClass(wxWindow* parent,
        wxWindowID id = wxID_ANY,
        const wxString& title = _("Landstalker Graphics Viewer"),
        const wxPoint& pos = wxDefaultPosition,
        const wxSize& size = wxSize(800, 600),
        long style = wxCAPTION | wxRESIZE_BORDER | wxMAXIMIZE_BOX | wxMINIMIZE_BOX | wxSYSTEM_MENU | wxCLOSE_BOX);
    virtual ~MainFrameBaseClass();
};

class ImgLst : public wxImageList
{
protected:
    // Maintain a map of all bitmaps representd by their name
    std::map<wxString, wxBitmap> m_bitmaps;
    // The requested image resolution (can be one of @2x, @1.5x, @1.25x or an empty string (the default)
    wxString m_resolution;
    int m_imagesWidth;
    int m_imagesHeight;

protected:
public:
    ImgLst();
    const wxBitmap& Bitmap(const wxString& name) const
    {
        if(!m_bitmaps.count(name + m_resolution))
            return wxNullBitmap;
        return m_bitmaps.find(name + m_resolution)->second;
    }

    void SetBitmapResolution(const wxString& res = wxEmptyString)
    {
        m_resolution = res;
    }

    virtual ~ImgLst();
};

#endif
