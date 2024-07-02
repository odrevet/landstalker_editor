#ifndef _ENTITY_VIEWER_FRAME_H_
#define _ENTITY_VIEWER_FRAME_H_

#include <EditorFrame.h>
#include <EntityViewerCtrl.h>
#include <GameData.h>

#include <string>
#include <memory>

class EntityViewerFrame : public EditorFrame
{
public:
	EntityViewerFrame(wxWindow* parent, ImageList* imglst);
	virtual ~EntityViewerFrame();

	bool Open(int entity_id);
	virtual void SetGameData(std::shared_ptr<GameData> gd);
	virtual void ClearGameData();
private:
	virtual void InitProperties(wxPropertyGridManager& props) const;
	void RefreshLists() const;
	virtual void UpdateProperties(wxPropertyGridManager& props) const;
	void RefreshProperties(wxPropertyGridManager& props) const;
	virtual void OnPropertyChange(wxPropertyGridEvent& evt);

	mutable wxPGChoices m_hi_palettes;
	mutable wxPGChoices m_lo_palettes;
	mutable wxPGChoices m_sprites;
	mutable bool m_reset_props = false;

	mutable wxAuiManager m_mgr;
	EntityViewerCtrl* m_entity_ctrl = nullptr;
	std::shared_ptr<Palette> m_palette;
	int m_entity_id = -1;
};

#endif // _ENTITY_VIEWER_FRAME_H_
