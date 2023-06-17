#include "RoomViewerCtrl.h"

#include <wx/dcbuffer.h>
#include <wx/graphics.h>
#include <unordered_map>

#include "EditorFrame.h"
#include "EntityPropertiesWindow.h"

wxBEGIN_EVENT_TABLE(RoomViewerCtrl, wxScrolledCanvas)
EVT_ERASE_BACKGROUND(RoomViewerCtrl::OnEraseBackground)
EVT_PAINT(RoomViewerCtrl::OnPaint)
EVT_SIZE(RoomViewerCtrl::OnSize)
EVT_MOTION(RoomViewerCtrl::OnMouseMove)
EVT_LEAVE_WINDOW(RoomViewerCtrl::OnMouseLeave)
EVT_LEFT_UP(RoomViewerCtrl::OnLeftClick)
EVT_LEFT_DCLICK(RoomViewerCtrl::OnLeftDblClick)
EVT_KEY_DOWN(RoomViewerCtrl::OnKeyDown)\
wxEND_EVENT_TABLE()

RoomViewerCtrl::RoomViewerCtrl(wxWindow* parent)
	: wxScrolledCanvas(parent, wxID_ANY),
	  m_roomnum(0),
      m_bmp(new wxBitmap),
      m_zoom(1.0),
      m_scroll_rate(SCROLL_RATE),
      m_width(1),
      m_height(1),
      m_buffer_width(1),
      m_buffer_height(1),
      m_redraw(false),
      m_repaint(false),
      m_selected(-1)
{
	SetBackgroundStyle(wxBG_STYLE_PAINT);
    SetBackgroundColour(*wxBLACK);
    m_layer_opacity = { {Layer::BACKGROUND1, 0xFF}, {Layer::BACKGROUND2, 0xFF}, {Layer::BG_SPRITES, 0xFF },
                        {Layer::FOREGROUND, 0xFF}, {Layer::FG_SPRITES, 0xFF}, {Layer::HEIGHTMAP, 0x80} };
    m_layer_bufs = { {Layer::BACKGROUND1, std::make_shared<ImageBuffer>()}, {Layer::BACKGROUND2, std::make_shared<ImageBuffer>()},
                     {Layer::BG_SPRITES,  std::make_shared<ImageBuffer>()}, {Layer::FOREGROUND,  std::make_shared<ImageBuffer>()},
                     {Layer::FG_SPRITES,  std::make_shared<ImageBuffer>()}};
}

RoomViewerCtrl::~RoomViewerCtrl()
{
    delete m_bmp;
}

void RoomViewerCtrl::SetGameData(std::shared_ptr<GameData> gd)
{
	m_g = gd;
	Refresh(true);
}

void RoomViewerCtrl::ClearGameData()
{
	m_g = nullptr;
	Refresh(true);
}

void RoomViewerCtrl::SetRoomNum(uint16_t roomnum, Mode mode)
{
	m_roomnum = roomnum;
    m_mode = mode;
    m_selected = -1;
    if (m_g != nullptr)
    {
        m_entities = m_g->GetSpriteData()->GetRoomEntities(roomnum);
    }
    UpdateRoomDescText(m_roomnum);
    RefreshGraphics();
}

void RoomViewerCtrl::SelectEntity(int selection)
{
    if (selection > 0 && selection != m_selected && selection < m_entities.size())
    {
        m_selected = selection;
        FireEvent(EVT_STATUSBAR_UPDATE);
        RedrawAllSprites();
        ForceRedraw();
    }
}

void RoomViewerCtrl::ClearSelection()
{
    if (m_selected != -1)
    {
        m_selected = -1;
        FireEvent(EVT_STATUSBAR_UPDATE);
        RedrawAllSprites();
        ForceRedraw();
    }
}

int RoomViewerCtrl::GetTotalEntities() const
{
    return m_entities.size();
}

bool RoomViewerCtrl::IsEntitySelected() const
{
    return (m_selected > 0 && m_selected <= m_entities.size());
}

int RoomViewerCtrl::GetSelectedEntityIndex() const
{
    return m_selected;
}

const Entity& RoomViewerCtrl::GetSelectedEntity() const
{
    assert(m_selected > 0 && m_selected <= m_entities.size());
    return m_entities[m_selected - 1];
}

const std::vector<Entity>& RoomViewerCtrl::GetEntities() const
{
    return m_entities;
}

void RoomViewerCtrl::SetZoom(double zoom)
{
    m_zoom = zoom;
    if (m_g == nullptr)
    {
        return;
    }
    auto map = m_g->GetRoomData()->GetMapForRoom(m_roomnum)->GetData();
    UpdateBuffer();
    auto q = PrepareSprites(m_roomnum);
    switch (m_mode)
    {
    case Mode::HEIGHTMAP:
        m_layers[Layer::HEIGHTMAP] = DrawHeightmapGrid(map);
        break;
    case Mode::WARPS:
        m_layers[Layer::WARPS] = DrawRoomWarps(m_roomnum);
        if (m_layer_opacity[Layer::HEIGHTMAP] > 0)
        {
            m_layers[Layer::HEIGHTMAP] = DrawHeightmapVisualisation(map, m_layer_opacity[Layer::HEIGHTMAP]);
        }
        DrawSpriteHitboxes(q);
        break;
    case Mode::NORMAL:
        if (m_layer_opacity[Layer::HEIGHTMAP] > 0)
        {
            m_layers[Layer::HEIGHTMAP] = DrawHeightmapVisualisation(map, m_layer_opacity[Layer::HEIGHTMAP]);
        }
        DrawSpriteHitboxes(q);
        break;
    }
    UpdateScroll();
    ForceRedraw();
}

void RoomViewerCtrl::SetLayerOpacity(Layer layer, uint8_t opacity)
{
    assert(m_layer_opacity.find(layer) != m_layer_opacity.cend());
    m_layer_opacity[layer] = opacity;
}

uint8_t RoomViewerCtrl::GetLayerOpacity(Layer layer) const
{
    assert(m_layer_opacity.find(layer) != m_layer_opacity.cend());
    return m_layer_opacity.find(layer)->second;
}

void RoomViewerCtrl::RedrawRoom()
{
    DrawRoom(m_roomnum);
    ForceRedraw();
}

void RoomViewerCtrl::DrawRoom(uint16_t roomnum)
{
    if (m_g == nullptr)
    {
        return;
    }
    auto map = m_g->GetRoomData()->GetMapForRoom(roomnum)->GetData();
    auto blocksets = m_g->GetRoomData()->GetBlocksetsForRoom(roomnum);
    auto tileset = m_g->GetRoomData()->GetTilesetForRoom(roomnum)->GetData();
    auto blockset = m_g->GetRoomData()->GetCombinedBlocksetForRoom(roomnum);
    m_rpalette = PreparePalettes(roomnum);

    m_warp_poly.clear();
    m_link_poly.clear();
    m_layers.clear();
    m_width = map->GetPixelWidth();
    m_height = map->GetPixelHeight();
    UpdateBuffer();

    if (m_layer_opacity[Layer::BACKGROUND1] > 0)
    {
        m_layer_bufs[Layer::BACKGROUND1]->Resize(m_width, m_height);
        m_layer_bufs[Layer::BACKGROUND1]->Insert3DMapLayer(0, 0, 0, Tilemap3D::Layer::BG, map, tileset, blockset);
        m_layers.insert({ Layer::BACKGROUND1, m_layer_bufs[Layer::BACKGROUND1]->MakeBitmap(m_rpalette,
            true, m_layer_opacity[Layer::BACKGROUND1]) });
    }
    if (m_layer_opacity[Layer::BACKGROUND2] > 0)
    {
        m_layer_bufs[Layer::BACKGROUND2]->Resize(m_width, m_height);
        m_layer_bufs[Layer::BACKGROUND2]->Insert3DMapLayer(0, 0, 0, Tilemap3D::Layer::FG, map, tileset, blockset);
        m_layers.insert({ Layer::BACKGROUND2, m_layer_bufs[Layer::BACKGROUND2]->MakeBitmap(m_rpalette,
            true, m_layer_opacity[Layer::BACKGROUND2], 0) });
    }
    if (m_layer_opacity[Layer::FOREGROUND] > 0)
    {
        m_layer_bufs[Layer::FOREGROUND]->Resize(m_width, m_height);
        m_layer_bufs[Layer::FOREGROUND]->Insert3DMapLayer(0, 0, 0, Tilemap3D::Layer::FG, map, tileset, blockset);
        m_layers.insert({ Layer::FOREGROUND, m_layer_bufs[Layer::FOREGROUND]->MakeBitmap(m_rpalette,
            true, 0, m_layer_opacity[Layer::FOREGROUND]) });
    }
    if (m_layer_opacity[Layer::HEIGHTMAP] > 0)
    {
        m_layers.insert({ Layer::HEIGHTMAP, DrawHeightmapVisualisation(map, m_layer_opacity[Layer::HEIGHTMAP]) });
    }
    if (m_layer_opacity[Layer::FG_SPRITES] > 0)
    {
        RedrawAllSprites();
    }
}

void RoomViewerCtrl::RedrawAllSprites()
{
    m_layer_bufs[Layer::FG_SPRITES]->Resize(m_width, m_height);
    m_layer_bufs[Layer::BG_SPRITES]->Resize(m_width, m_height);
    m_rpalette = PreparePalettes(m_roomnum);
    auto q = PrepareSprites(m_roomnum);
    DrawSpriteHitboxes(q);
    DrawSprites(q);
    UpdateLayer(Layer::BG_SPRITES, m_layer_bufs[Layer::FG_SPRITES]->MakeBitmap(m_rpalette,
        true, m_layer_opacity[Layer::FG_SPRITES], m_layer_opacity[Layer::FG_SPRITES]));
    UpdateLayer(Layer::FG_SPRITES, m_layer_bufs[Layer::FG_SPRITES]->MakeBitmap(m_rpalette,
        true, m_layer_opacity[Layer::FG_SPRITES], m_layer_opacity[Layer::FG_SPRITES]));
}

void RoomViewerCtrl::UpdateLayer(const Layer& layer, std::shared_ptr<wxBitmap> bmp)
{
    if (m_layers.find(layer) == m_layers.cend())
    {
        m_layers.insert({ layer, bmp });
    }
    else
    {
        m_layers[layer] = bmp;
    }
}

std::vector<std::shared_ptr<Palette>> RoomViewerCtrl::PreparePalettes(uint16_t roomnum)
{
    auto palette = std::vector<std::shared_ptr<Palette>>{ m_g->GetRoomData()->GetPaletteForRoom(roomnum)->GetData() };
    palette.emplace_back();
    palette.emplace_back(m_g->GetGraphicsData()->GetPlayerPalette()->GetData());
    palette.emplace_back(m_g->GetGraphicsData()->GetHudPalette()->GetData());
    m_errors.clear();
    std::array<int, 3> sprite_palette_alloc = { -1, -1, -1 };
    m_layer_bufs[Layer::FG_SPRITES]->Resize(m_width, m_height);
    int i = 0;
    for (const auto& entity : m_entities)
    {
        auto s_pal = m_g->GetSpriteData()->GetSpritePaletteIdxs(entity.GetType());
        const uint8_t pal_slot = entity.GetPalette();
        if (pal_slot == 1 || pal_slot == 3)
        {
            const int lo = s_pal.first;
            const int hi = s_pal.second;
            if (lo != -1)
            {
                if (sprite_palette_alloc[pal_slot - 1] == -1)
                {
                    sprite_palette_alloc[pal_slot - 1] = lo;
                }
                else if (sprite_palette_alloc[pal_slot - 1] != lo)
                {
                    m_errors.push_back(StrPrintf("Possible Palette Clash - Slot%d Lo orig %02X, req %02X.",
                        pal_slot, sprite_palette_alloc[pal_slot - 1], lo));
                }
            }
            if (hi != -1) // Hi Palette
            {
                if (pal_slot == 3)
                {
                    m_errors.push_back(StrPrintf("Possible Palette Clash - Slot%d Hi specified, req %02X.",
                            pal_slot, hi));
                }
                else
                {
                    if (sprite_palette_alloc[pal_slot] == -1)
                    {
                        sprite_palette_alloc[pal_slot] = hi;
                    }
                    else if (sprite_palette_alloc[pal_slot] != hi)
                    {
                        m_errors.push_back(StrPrintf("Possible Palette Clash - Slot%d Hi orig %02X, req %02X.",
                            pal_slot, sprite_palette_alloc[pal_slot], hi));
                    }
                }
            }
        }
    }
    palette[3] = std::make_shared<Palette>(std::vector<std::shared_ptr<Palette>>{ palette[3], m_g->GetSpriteData()->GetSpritePalette(sprite_palette_alloc[2], -1) });
    palette[1] = m_g->GetSpriteData()->GetSpritePalette(sprite_palette_alloc[0], sprite_palette_alloc[1]);
    return palette;
}

std::vector<RoomViewerCtrl::SpriteQ> RoomViewerCtrl::PrepareSprites(uint16_t roomnum)
{
    auto map = m_g->GetRoomData()->GetMapForRoom(roomnum)->GetData();
    std::vector<SpriteQ> sprites;
    int i = 1;
    for (const auto& entity : m_entities)
    {
        SpriteQ s;
        if (m_g->GetSpriteData()->HasFrontAndBack(entity.GetType()))
        {
            auto sprite = m_g->GetSpriteData()->GetSpriteFromEntity(entity.GetType());
            int anim = 0;
            if ((entity.GetOrientation() == Orientation::SW || entity.GetOrientation() == Orientation::SE))
            {
                anim = 1;
            }
            auto frame = m_g->GetSpriteData()->GetSpriteFrame(sprite, anim, 0);
            s.frame = frame->GetData();
        }
        else
        {
            auto frame = m_g->GetSpriteData()->GetDefaultEntityFrame(entity.GetType());
            s.frame = frame->GetData();
        }
        s.id = i;
        s.entity = entity;
        s.palette = entity.GetPalette();
        s.hflip = (entity.GetOrientation() == Orientation::NW || entity.GetOrientation() == Orientation::SE);
        s.background = false;
        s.selected = (i == m_selected);

        auto hitbox = m_g->GetSpriteData()->GetEntityHitbox(entity.GetType());

        int x = entity.GetX() + 0x080;
        int y = entity.GetY() - 0x080;
        int z = entity.GetZ();

        if (hitbox.first >= 0x0C)
        {
            x += 0x80;
            y += 0x80;
        }

        auto xy = map->EntityPositionToPixel(x, y, z);
        s.x = xy.x;
        s.y = xy.y;
        sprites.push_back(s);
        i++;
    }
    // Fix draw order
    std::sort(sprites.begin(), sprites.end(), [this](const SpriteQ& lhs, const SpriteQ& rhs) -> bool
        {
            if (lhs.selected || rhs.selected)
            {
                return rhs.selected;
            }
            // Draw objects furthest away from camera first
            auto hitbox_lhs = m_g->GetSpriteData()->GetEntityHitbox(lhs.entity.GetType());
            auto hitbox_rhs = m_g->GetSpriteData()->GetEntityHitbox(rhs.entity.GetType());
            int dist_lhs = lhs.entity.GetX() + lhs.entity.GetY() + hitbox_lhs.first;
            int dist_rhs = rhs.entity.GetX() + rhs.entity.GetY() + hitbox_rhs.first;
            if (dist_lhs != dist_rhs)
            {
                return dist_lhs < dist_rhs;
            }
            // Next draw left-most objects
            int left_lhs = lhs.entity.GetY() - hitbox_lhs.first;
            int left_rhs = rhs.entity.GetY() - hitbox_rhs.first;
            if (left_lhs < left_rhs)
            {
                return left_lhs < left_rhs;
            }
            // Finally, sort by height
            int height_lhs = lhs.entity.GetZ() + hitbox_lhs.second;
            int height_rhs = rhs.entity.GetZ() + hitbox_rhs.second;
            return height_lhs < height_rhs;
        });
    return sprites;
}

void RoomViewerCtrl::DrawSpriteHitboxes(const std::vector<SpriteQ>& q)
{
    std::unordered_map<Layer, wxImage> layers;
    std::unordered_map<Layer, wxGraphicsContext*> ctxs;
    layers.insert({ Layer::BG_SPRITES_WIREFRAME_BG, wxImage(m_buffer_width, m_buffer_height) });
    layers.insert({ Layer::BG_SPRITES_WIREFRAME_FG, wxImage(m_buffer_width, m_buffer_height) });
    layers.insert({ Layer::FG_SPRITES_WIREFRAME_BG, wxImage(m_buffer_width, m_buffer_height) });
    layers.insert({ Layer::FG_SPRITES_WIREFRAME_FG, wxImage(m_buffer_width, m_buffer_height) });
    
    m_entity_poly.clear();
    
    for (auto& img : layers)
    {
        img.second.InitAlpha();
        SetOpacity(img.second, 0x00);
        auto ctx = wxGraphicsContext::Create(img.second);
        ctx->Scale(m_zoom, m_zoom);
        ctx->SetPen(*wxTRANSPARENT_PEN);
        ctx->SetBrush(*wxBLACK_BRUSH);
        ctxs.insert({ img.first, ctx });
    }
    for (const auto& s : q)
    {
        auto hitbox = m_g->GetSpriteData()->GetEntityHitbox(s.entity.GetType());

        wxPoint2DDouble shadow_points[] = {
            wxPoint2DDouble(s.x + hitbox.first * 2, s.y),
            wxPoint2DDouble(s.x, s.y + hitbox.first),
            wxPoint2DDouble(s.x - hitbox.first * 2, s.y),
            wxPoint2DDouble(s.x, s.y - hitbox.first),
            wxPoint2DDouble(s.x + hitbox.first * 2, s.y)
        };
        wxPoint2DDouble hitbox_fg_points[] = {
            wxPoint2DDouble(s.x + hitbox.first * 2, s.y),
            wxPoint2DDouble(s.x, s.y + hitbox.first),
            wxPoint2DDouble(s.x - hitbox.first * 2, s.y),
            wxPoint2DDouble(s.x - hitbox.first * 2, s.y - hitbox.second),
            wxPoint2DDouble(s.x, s.y + hitbox.first - hitbox.second),
            wxPoint2DDouble(s.x, s.y + hitbox.first),
            wxPoint2DDouble(s.x, s.y + hitbox.first - hitbox.second),
            wxPoint2DDouble(s.x + hitbox.first * 2, s.y - hitbox.second),
            wxPoint2DDouble(s.x + hitbox.first * 2, s.y),
            wxPoint2DDouble(s.x + hitbox.first * 2, s.y - hitbox.second),
            wxPoint2DDouble(s.x, s.y - hitbox.first - hitbox.second),
            wxPoint2DDouble(s.x - hitbox.first * 2, s.y - hitbox.second)
        };

        m_entity_poly.push_back({ s.id, {
            {(double)(s.x + hitbox.first * 2), (double)(s.y)},
            {(double)(s.x + hitbox.first * 2), (double)(s.y - hitbox.second)},
            {(double)(s.x), (double)(s.y - hitbox.first - hitbox.second)},
            {(double)(s.x - hitbox.first * 2), (double)(s.y - hitbox.second)},
            {(double)(s.x - hitbox.first * 2), (double)(s.y)},
            {(double)(s.x), (double)(s.y + hitbox.first)},
            {(double)(s.x + hitbox.first * 2), (double)(s.y)}
        } });

        //hm_gc->DrawLines(sizeof(shadow_points) / sizeof(shadow_points[0]), shadow_points);
        auto fg_ctx = s.background ? ctxs[Layer::BG_SPRITES_WIREFRAME_FG] : ctxs[Layer::FG_SPRITES_WIREFRAME_FG];
        auto bg_ctx = s.background ? ctxs[Layer::BG_SPRITES_WIREFRAME_BG] : ctxs[Layer::FG_SPRITES_WIREFRAME_BG];
        auto dotted_pen = bg_ctx->CreatePen(wxGraphicsPenInfo(s.selected ? wxColor(0xFF, 50, 50) : wxColor(0xA0, 0xA0, 0xA0)).Style(wxPENSTYLE_DOT));
        bg_ctx->SetPen(dotted_pen);
        bg_ctx->StrokeLine(s.x + hitbox.first * 2, s.y, s.x, s.y - hitbox.first);
        bg_ctx->StrokeLine(s.x, s.y - hitbox.first, s.x - hitbox.first * 2, s.y);
        bg_ctx->StrokeLine(s.x, s.y - hitbox.first, s.x, s.y - hitbox.first - hitbox.second);
        auto solid_pen = bg_ctx->CreatePen(wxGraphicsPenInfo(s.selected ? wxColor(0xFF, 50, 50) : wxColor(0xA0, 0xA0, 0xA0)).Style(wxPENSTYLE_SOLID));
        fg_ctx->SetPen(solid_pen);
        fg_ctx->StrokeLines(sizeof(hitbox_fg_points) / sizeof(hitbox_fg_points[0]), hitbox_fg_points);
    }
    for (auto& ctx : ctxs)
    {
        delete ctx.second;
    }
    ctxs.clear();
    for (auto& layer : layers)
    {
        if (m_layers.find(layer.first) == m_layers.cend())
        {
            m_layers.insert({ layer.first, std::make_shared<wxBitmap>(layer.second) });
        }
        else
        {
            m_layers[layer.first] = std::make_shared<wxBitmap>(layer.second);
        }
    }
}

void RoomViewerCtrl::DrawSprites(const std::vector<SpriteQ>& q)
{
    for (const auto& s : q)
    {
        if (s.frame != nullptr)
        {
            if (s.background)
            {
                m_layer_bufs[Layer::BG_SPRITES]->InsertSprite(s.x, s.y, s.palette, *s.frame, s.hflip);
            }
            else
            {
                m_layer_bufs[Layer::FG_SPRITES]->InsertSprite(s.x, s.y, s.palette, *s.frame, s.hflip);
            }
        }
    }
}

void RoomViewerCtrl::UpdateRoomDescText(uint16_t roomnum)
{
    if (m_g == nullptr)
    {
        return;
    }
    auto rd = m_g->GetRoomData()->GetRoom(roomnum);
    std::ostringstream ss;
    ss << "Room: " << std::dec << std::uppercase << std::setw(3) << std::setfill('0') << rd->index
        << " Tileset: 0x" << std::hex << std::uppercase << std::setw(2) << std::setfill('0') << static_cast<unsigned>(rd->tileset)
        << " Palette: 0x" << std::hex << std::uppercase << std::setw(2) << std::setfill('0') << static_cast<unsigned>(rd->room_palette)
        << " PriBlockset: 0x" << std::hex << std::uppercase << std::setw(1) << std::setfill('0') << static_cast<unsigned>(rd->pri_blockset)
        << " SecBlockset: 0x" << std::hex << std::uppercase << std::setw(1) << std::setfill('0') << static_cast<unsigned>(rd->sec_blockset)
        << " BGM: 0x" << std::hex << std::uppercase << std::setw(2) << std::setfill('0') << static_cast<unsigned>(rd->bgm)
        << " Map: " << rd->map;
    m_status_text = ss.str();
    FireEvent(EVT_STATUSBAR_UPDATE);
}

std::shared_ptr<wxBitmap> RoomViewerCtrl::DrawRoomWarps(uint16_t roomnum)
{
    auto map = m_g->GetRoomData()->GetMapForRoom(roomnum)->GetData();
    auto warps = m_g->GetRoomData()->GetWarpsForRoom(roomnum);

    wxImage img(m_buffer_width, m_buffer_height);
    img.InitAlpha();
    SetOpacity(img, 0x00);
    wxGraphicsContext* gc = wxGraphicsContext::Create(img);
    gc->Scale(m_zoom, m_zoom);
    gc->SetPen(*wxWHITE_PEN);
    gc->SetBrush(*wxBLACK_BRUSH);
    for (const auto& warp : warps)
    {
        DrawWarp(*gc, warp, map, TILE_WIDTH, TILE_HEIGHT);
    }
    wxColour bkColor(*wxBLACK);
    wxColour textColor(*wxWHITE);
    int line = 0;
    if (m_g->GetRoomData()->HasClimbDestination(roomnum))
    {
        AddRoomLink(gc, "Climb Destination:", m_g->GetRoomData()->GetClimbDestination(roomnum), 5, 5 + line * 16);
        line++;
    }
    if (m_g->GetRoomData()->HasFallDestination(roomnum))
    {
        AddRoomLink(gc, "Fall Destination:", m_g->GetRoomData()->GetFallDestination(roomnum), 5, 5 + line * 16);
        line++;
    }
    auto txns = m_g->GetRoomData()->GetTransitions(roomnum);
    for (const auto& t : txns)
    {
        std::string label = StrPrintf("Transition when flag %04d is %s:", t.second, (t.first.first == roomnum) ? "SET" : "CLEAR");
        uint16_t dest = (t.first.first == roomnum) ? t.first.second : t.first.first;
        AddRoomLink(gc, label, dest, 5, 5 + line * 16);
        line++;
    }
    delete gc;
    return std::make_shared<wxBitmap>(img);
}

void RoomViewerCtrl::DrawRoomHeightmap(uint16_t roomnum)
{
    if (m_g == nullptr)
    {
        return;
    }

    auto map = m_g->GetRoomData()->GetMapForRoom(roomnum)->GetData();

    m_warp_poly.clear();
    m_link_poly.clear();

    m_layers.clear();
    m_width = (map->GetHeightmapWidth() + map->GetHeightmapHeight()) * TILE_WIDTH + 1;
    m_height = (map->GetHeightmapWidth() + map->GetHeightmapHeight()) * TILE_HEIGHT + 1;
    UpdateBuffer();
    m_layers.insert({ Layer::HEIGHTMAP, DrawHeightmapGrid(map) });
}

std::shared_ptr<wxBitmap> RoomViewerCtrl::DrawHeightmapVisualisation(std::shared_ptr<Tilemap3D> map, uint8_t opacity)
{
    wxImage hm_img(m_buffer_width, m_buffer_height);
    hm_img.InitAlpha();
    SetOpacity(hm_img, 0x00);
    wxGraphicsContext* hm_gc = wxGraphicsContext::Create(hm_img);
    hm_gc->Scale(m_zoom, m_zoom);
    hm_gc->SetPen(*wxWHITE_PEN);
    hm_gc->SetBrush(*wxBLACK_BRUSH);
    for (int y = 0; y < map->GetHeightmapHeight(); ++y)
    {
        for (int x = 0; x < map->GetHeightmapWidth(); ++x)
        {
            // Only display cells that are not completely restricted
            if ((map->GetHeight({ x, y }) > 0 || (map->GetCellProps({ x, y }) != 0x04)))
            {
                int z = map->GetHeight({ x, y });
                auto xy(map->Iso3DToPixel({ x + 12, y + 12, z }));
                DrawHeightmapCell(*hm_gc, xy.x, xy.y, z, TILE_WIDTH, TILE_HEIGHT,
                    map->GetCellProps({ x,y }), map->GetCellType({ x,y }));
            }
        }
    }
    delete hm_gc;
    SetOpacity(hm_img, opacity);
    return std::make_shared<wxBitmap>(hm_img);
}

std::shared_ptr<wxBitmap> RoomViewerCtrl::DrawHeightmapGrid(std::shared_ptr<Tilemap3D> map)
{

    wxImage img(m_buffer_width, m_buffer_height);
    wxGraphicsContext* gc = wxGraphicsContext::Create(img);
    gc->SetBrush(*wxBLACK);
    gc->SetPen(*wxTRANSPARENT_PEN);
    gc->DrawRectangle(0, 0, m_buffer_width, m_buffer_height);
    gc->SetPen(*wxWHITE_PEN);
    gc->Scale(m_zoom, m_zoom);
    auto font = wxFont(TILE_HEIGHT / 2, wxFONTFAMILY_TELETYPE, wxFONTSTYLE_NORMAL,
        wxFONTWEIGHT_NORMAL, false);
    // Draw grid
    for (int y = 0; y < map->GetHeightmapHeight(); ++y)
        for (int x = 0; x < map->GetHeightmapWidth(); ++x)
        {
            int xx = (x - y + map->GetHeightmapHeight() - 1) * TILE_WIDTH;
            int xy = (x + y) * TILE_HEIGHT;
            DrawHeightmapCell(*gc, xx, xy, 0, TILE_WIDTH * 2, TILE_HEIGHT * 2, -1, -1, false, wxColor(0x444444));
            std::string lbl1 = StrPrintf("%02X", map->GetCellType({ x,y }));
            std::string lbl2 = StrPrintf("%X %X", map->GetCellType({ x,y }), map->GetCellProps({ x,y }));
        }

    // Draw cells
    for (int y = 0; y < map->GetHeightmapHeight(); ++y)
        for (int x = 0; x < map->GetHeightmapWidth(); ++x)
        {
            // Only display cells that are not completely restricted
            if ((map->GetHeight({ x, y }) > 0 || (map->GetCellProps({ x, y }) != 0x04)))
            {
                int xx = (x - y + map->GetHeightmapHeight() - 1) * TILE_WIDTH;
                int xy = (x + y) * TILE_HEIGHT;
                DrawHeightmapCell(*gc, xx, xy, map->GetHeight({x,y}), TILE_WIDTH * 2, TILE_HEIGHT * 2, map->GetCellProps({x,y}), -1, false);
                std::string lbl1 = StrPrintf("%02X", map->GetCellType({ x,y }));
                std::string lbl2 = StrPrintf("%X", map->GetHeight({ x,y }));
                std::string lbl3 = StrPrintf("%X", map->GetCellProps({x,y}));
                double tw, th;
                gc->SetFont(font, map->GetCellType({ x,y }) == 0 ? *wxLIGHT_GREY : wxColor(0xFF00FF));
                gc->GetTextExtent(lbl1, &tw, &th);
                gc->DrawText(lbl1, xx + TILE_WIDTH - tw / 2, xy + TILE_HEIGHT / 2 - th / 2 + 2);
                gc->SetFont(font, *wxCYAN);
                gc->GetTextExtent(lbl2, &tw, &th);
                gc->DrawText(lbl2, xx + TILE_WIDTH - 5 * tw / 4, xy + 5 * TILE_HEIGHT / 4 - th / 2 + 2);
                gc->SetFont(font, map->GetCellProps({ x,y }) == 0 ? *wxLIGHT_GREY : *wxYELLOW);
                gc->GetTextExtent(lbl3, &tw, &th);
                gc->DrawText(lbl3, xx + TILE_WIDTH + 1 * tw / 4, xy + 5 * TILE_HEIGHT / 4 - th / 2 + 2);
            }
        }
    delete gc;

    return std::make_shared<wxBitmap>(img);
}

void RoomViewerCtrl::DrawHeightmapCell(wxGraphicsContext& gc, int x, int y, int zz, int width, int height, int restrictions, int classification, bool draw_walls, wxColor border_colour)
{
    int z = draw_walls ? zz : 0;
    wxPoint2DDouble tile_points[] = {
        wxPoint2DDouble(x + width / 2, y),
        wxPoint2DDouble(x + width    , y + height / 2),
        wxPoint2DDouble(x + width / 2, y + height),
        wxPoint2DDouble(x            , y + height / 2),
        wxPoint2DDouble(x + width / 2, y)
    };
    wxPoint2DDouble left_wall[] = {
        wxPoint2DDouble(x            , y + height / 2),
        wxPoint2DDouble(x + width / 2, y + height),
        wxPoint2DDouble(x + width / 2, y + height + z * height),
        wxPoint2DDouble(x            , y + height / 2 + z * height),
        wxPoint2DDouble(x            , y + height / 2)
    };
    wxPoint2DDouble right_wall[] = {
        wxPoint2DDouble(x + width / 2, y + height),
        wxPoint2DDouble(x + width    , y + height / 2),
        wxPoint2DDouble(x + width    , y + height / 2 + z * height),
        wxPoint2DDouble(x + width / 2, y + height + z * height),
        wxPoint2DDouble(x + width / 2, y + height)
    };
    wxColor bg;
    switch(restrictions)
    {
    case -1:
        bg = *wxBLACK;
        break;
    case 0x0:
        bg.Set(zz * 3, 48 + zz * 8, zz * 3);
        break;
    case 0x4:
        bg.Set(48 + zz * 8, zz * 3, zz * 3);
        break;
    case 0x2:
        bg.Set(32 + zz * 3, 32 + zz * 3, 48 + zz * 12);
        break;
    case 0x6:
        bg.Set(48 + zz * 8, 32 + zz * 3, 48 + zz * 8);
        break;
    default:
        bg.Set(48 + zz * 8, 48 + zz * 8, zz * 3);
        break;
    }
    gc.SetBrush(wxBrush(bg));
    gc.SetPen(border_colour);
    //gc.SetTextForeground(*wxWHITE);
    gc.DrawLines(sizeof(tile_points) / sizeof(tile_points[0]), tile_points);
    bg = bg.ChangeLightness(70);
    gc.SetBrush(wxBrush(bg));
    gc.DrawLines(sizeof(left_wall) / sizeof(left_wall[0]), left_wall);
    bg = bg.ChangeLightness(70);
    gc.SetBrush(wxBrush(bg));
    gc.DrawLines(sizeof(right_wall) / sizeof(right_wall[0]), right_wall);
    if (classification > 0)
    {
        // Set font and transparent colour
        gc.SetFont(wxFont(height / 2, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL,
            wxFONTWEIGHT_NORMAL, false),
            wxColour(255, 255, 255, 255));
        std::ostringstream ss;
        ss << std::hex << std::setw(2) << std::setfill('0') << std::uppercase << classification;
        double textwidth, textheight;
        gc.GetTextExtent(ss.str(), &textwidth, &textheight);
        gc.DrawText(ss.str(), x + (width - textwidth) / 2, y + (height - textheight) / 2);
    }
}

void RoomViewerCtrl::DrawWarp(wxGraphicsContext& gc, const WarpList::Warp& warp, std::shared_ptr<Tilemap3D> map, int tile_width, int tile_height)
{
    int x = 0;
    int y = 0;
    if (warp.room1 == m_roomnum)
    {
        x = warp.x1;
        y = warp.y1;
    }
    else if (warp.room2 == m_roomnum)
    {
        x = warp.x2;
        y = warp.y2;
    }
    int z = map->GetHeight({ x - 12,y - 12 });
    auto xy = map->Iso3DToPixel({ x,y,z });
    int width = tile_width;
    int height = tile_height;
    std::vector<wxPoint2DDouble> tile_points = {
        wxPoint2DDouble(xy.x + tile_width / 2, xy.y),
        wxPoint2DDouble(xy.x + (warp.x_size + 1) * tile_width / 2, xy.y + warp.x_size * tile_height / 2),
        wxPoint2DDouble(xy.x + (warp.x_size - warp.y_size + 1) * tile_width / 2, xy.y + (warp.x_size + warp.y_size) * tile_height / 2),
        wxPoint2DDouble(xy.x + (1 - warp.y_size) * tile_width / 2, xy.y + warp.y_size * tile_height / 2),
        wxPoint2DDouble(xy.x + tile_width / 2, xy.y)
    };
    switch (warp.type)
    {
    case WarpList::Warp::Type::NORMAL:
        gc.SetPen(*wxYELLOW_PEN);
        break;
    case WarpList::Warp::Type::STAIR_SE:
        gc.SetPen(*wxGREEN_PEN);
        break;
    case WarpList::Warp::Type::STAIR_SW:
        gc.SetPen(*wxCYAN_PEN);
        break;
    default:
        gc.SetPen(*wxRED_PEN);
        break;
    }
    gc.SetBrush(*wxTRANSPARENT_BRUSH);
    gc.DrawLines(tile_points.size(), &tile_points[0]);
    m_warp_poly.push_back({ warp, tile_points });
}



void RoomViewerCtrl::AddRoomLink(wxGraphicsContext* gc, const std::string& label, uint16_t room, int x, int y)
{
    wxFont font(10, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);
    double w, h;
    gc->SetFont(font, *wxWHITE);
    gc->GetTextExtent(label, &w, &h);
    const double LINK_X = x + std::max<double>(145, w + 20);
    const double LINK_Y = y;
    gc->DrawText(label, x, y);
    gc->SetFont(font, *wxYELLOW);
    auto roomlabel = StrPrintf("Room %03d", room);
    gc->DrawText(roomlabel, LINK_X, LINK_Y);
    gc->GetTextExtent(roomlabel, &w, &h);
    m_link_poly.push_back({ room, {
        {LINK_X, LINK_Y},
        {LINK_X + w, LINK_Y},
        {LINK_X + w, LINK_Y + h},
        {LINK_X, LINK_Y + h},
        {LINK_X, LINK_Y}
    } });
}

void RoomViewerCtrl::SetOpacity(wxImage& image, uint8_t opacity)
{
    uint8_t* alpha = image.GetAlpha();
    for (int i = 0; i < (image.GetHeight() * image.GetWidth()); i++)
    {
        *alpha = (*alpha < opacity) ? *alpha : opacity;
        alpha++;
    }
}

void RoomViewerCtrl::ForceRedraw()
{
    m_redraw = true;
    m_repaint = true;
    Refresh(true);
}

void RoomViewerCtrl::ForceRepaint()
{
    m_repaint = true;
    Refresh(true);
}

void RoomViewerCtrl::UpdateScroll()
{
    m_scroll_rate = std::ceil(SCROLL_RATE * m_zoom);
    SetVirtualSize(std::ceil(m_zoom * m_width), std::ceil(m_zoom * m_height));
    SetScrollRate(m_scroll_rate, m_scroll_rate);
    AdjustScrollbars();
}

void RoomViewerCtrl::UpdateBuffer()
{
    m_buffer_width = std::ceil(m_width * m_zoom);
    m_buffer_height = std::ceil(m_height * m_zoom);
    m_bmp->Create(m_buffer_width, m_buffer_height);
}

void RoomViewerCtrl::RefreshGraphics()
{
    if (m_g == nullptr)
    {
        return;
    }
    switch (m_mode)
    {
    case Mode::HEIGHTMAP:
        DrawRoomHeightmap(m_roomnum);
        break;
    case Mode::WARPS:
        DrawRoom(m_roomnum);
        m_layers[Layer::WARPS] = DrawRoomWarps(m_roomnum);
        break;
    case Mode::NORMAL:
        DrawRoom(m_roomnum);
        break;
    }
    UpdateScroll();
    ForceRedraw();
}

int RoomViewerCtrl::GetErrorCount() const
{
    return m_errors.size();
}

std::string RoomViewerCtrl::GetErrorText(int errnum) const
{
    return m_errors.at(errnum);
}

const std::string& RoomViewerCtrl::GetStatusText() const
{
    return m_status_text;
}


bool RoomViewerCtrl::Pnpoly(const std::vector<wxPoint2DDouble>& poly, int x, int y)
{
    int i, j;
    bool c = false;
    for (i = 0, j = poly.size() - 1; i < poly.size(); j = i++) {
        if (((poly[i].m_y > y) != (poly[j].m_y > y)) &&
            (x < (poly[j].m_x - poly[i].m_x) * (y - poly[i].m_y) / (poly[j].m_y - poly[i].m_y) + poly[i].m_x))
            c = !c;
    }
    return c;
}

void RoomViewerCtrl::GoToRoom(uint16_t room)
{
    auto name = m_g->GetRoomData()->GetRoom(room)->name;
    FireEvent(EVT_GO_TO_NAV_ITEM, "Rooms/" + name + "/Warps");
}

void RoomViewerCtrl::FireEvent(const wxEventType& e, long userdata)
{
    wxCommandEvent evt(e);
    evt.SetExtraLong(userdata);
    evt.SetClientData(this->GetParent());
    wxPostEvent(this->GetParent(), evt);
}

void RoomViewerCtrl::FireEvent(const wxEventType& e, const std::string& userdata)
{
    wxCommandEvent evt(e);
    evt.SetString(userdata);
    evt.SetClientData(this->GetParent());
    wxPostEvent(this->GetParent(), evt);
}

void RoomViewerCtrl::FireEvent(const wxEventType& e)
{
    wxCommandEvent evt(e);
    evt.SetClientData(this->GetParent());
    wxPostEvent(this->GetParent(), evt);
}

void RoomViewerCtrl::OnDraw(wxDC& dc)
{
    int sx, sy;
    GetViewStart(&sx, &sy);
    sx *= m_scroll_rate;
    sy *= m_scroll_rate;
    int sw, sh;
    GetClientSize(&sw, &sh);
    wxMemoryDC mdc(*m_bmp);
    if (m_redraw)
    {
        mdc.SetBackground(*wxBLACK_BRUSH);
        mdc.Clear();
        for (const auto& layer : m_layers)
        {
            if (layer.first == Layer::BACKGROUND1 ||
                layer.first == Layer::BACKGROUND2 ||
                layer.first == Layer::FOREGROUND ||
                layer.first == Layer::BG_SPRITES ||
                layer.first == Layer::FG_SPRITES)
            {
                mdc.SetUserScale(m_zoom, m_zoom);
            }
            else
            {
                mdc.SetUserScale(1.0, 1.0);
            }
            mdc.DrawBitmap(*layer.second, 0, 0, true);
        }
        m_redraw = false;
    }
    mdc.SetUserScale(1.0, 1.0);
    dc.SetBackground(*wxBLACK_BRUSH);
    dc.Clear();
    dc.Blit(sx, sy, sw, sh, &mdc, sx, sy, wxCOPY);
    mdc.SelectObject(wxNullBitmap);
}

void RoomViewerCtrl::OnPaint(wxPaintEvent& evt)
{
	wxBufferedPaintDC dc(this);
	this->PrepareDC(dc);
	this->OnDraw(dc);
}

void RoomViewerCtrl::OnEraseBackground(wxEraseEvent& evt)
{
    //evt.Skip();
}

void RoomViewerCtrl::OnSize(wxSizeEvent& evt)
{
	Refresh(false);
	evt.Skip();
}

void RoomViewerCtrl::OnMouseMove(wxMouseEvent& evt)
{
    int x, y;
    GetViewStart(&x, &y);
    x *= m_scroll_rate;
    y *= m_scroll_rate;
    x += evt.GetX();
    y += evt.GetY();
    x = std::ceil(x / m_zoom);
    y = std::ceil(y / m_zoom);
    std::string status_text = m_status_text;
    if (m_g != nullptr)
    {
        auto map = m_g->GetRoomData()->GetMapForRoom(m_roomnum)->GetData();
        if (map)
        {
            auto r = map->PixelToHMPoint({ x,y });
            status_text = StrPrintf("(%d,%d) [%d,%d]", x, y, r.x, r.y);
            SetCursor(wxCURSOR_ARROW);
            for (const auto& wp : m_warp_poly)
            {
                if (Pnpoly(wp.second, x, y))
                {
                    uint16_t room = (wp.first.room1 == m_roomnum) ? wp.first.room2 : wp.first.room1;
                    uint8_t wx = (wp.first.room1 == m_roomnum) ? wp.first.x2 : wp.first.x1;
                    uint8_t wy = (wp.first.room1 == m_roomnum) ? wp.first.y2 : wp.first.y1;
                    status_text += StrPrintf(" - Warp to room %03d (%d,%d)", room, wx, wy);
                    SetCursor(wxCURSOR_HAND);
                    if (status_text != m_status_text)
                    {
                        m_status_text = status_text;
                        FireEvent(EVT_STATUSBAR_UPDATE);
                    }
                    evt.Skip();
                    return;
                }
            }
            if (status_text != m_status_text)
            {
                m_status_text = status_text;
                FireEvent(EVT_STATUSBAR_UPDATE);
            }
            for (const auto& lp : m_link_poly)
            {
                if (Pnpoly(lp.second, x, y))
                {
                    SetCursor(wxCURSOR_HAND);
                    evt.Skip();
                    return;
                }
            }
            for (const auto& ep : m_entity_poly)
            {
                if (Pnpoly(ep.second, x, y))
                {
                    SetCursor(wxCURSOR_HAND);
                    status_text += StrPrintf(" - Entity(%d)", ep.first);
                    if (status_text != m_status_text)
                    {
                        m_status_text = status_text;
                        FireEvent(EVT_STATUSBAR_UPDATE);
                    }
                    evt.Skip();
                    return;
                }
            }
        }
    }
    SetCursor(wxCURSOR_ARROW);
    evt.Skip();
}

void RoomViewerCtrl::UpdateEntityProperties(int entity)
{
    EntityPropertiesWindow dlg(m_parent, entity, &m_entities[entity - 1]);
    if (dlg.ShowModal() == wxID_OK)
    {
        m_g->GetSpriteData()->SetRoomEntities(m_roomnum, m_entities);
        FireEvent(EVT_STATUSBAR_UPDATE);
        RedrawAllSprites();
        ForceRedraw();
    }
}

void RoomViewerCtrl::OnMouseLeave(wxMouseEvent& evt)
{
    SetCursor(wxCURSOR_ARROW);
    evt.Skip();
}

void RoomViewerCtrl::OnLeftClick(wxMouseEvent& evt)
{
    int x, y;
    GetViewStart(&x, &y);
    x *= m_scroll_rate;
    y *= m_scroll_rate;
    x += evt.GetX();
    y += evt.GetY();
    x = std::ceil(x / m_zoom);
    y = std::ceil(y / m_zoom);
    int selected = -1;
    if (m_g != nullptr)
    {
        auto map = m_g->GetRoomData()->GetMapForRoom(m_roomnum)->GetData();
        if (map)
        {
            for (const auto& wp : m_warp_poly)
            {
                if (Pnpoly(wp.second, x, y))
                {
                    SetCursor(wxCURSOR_ARROW);
                    uint16_t room = (wp.first.room1 == m_roomnum) ? wp.first.room2 : wp.first.room1;
                    GoToRoom(room);
                    break;
                }
            }
            for (const auto& lp : m_link_poly)
            {
                if (Pnpoly(lp.second, x, y))
                {
                    SetCursor(wxCURSOR_ARROW);
                    GoToRoom(lp.first);
                    break;
                }
            }
            for (const auto& ep : m_entity_poly)
            {
                if (Pnpoly(ep.second, x, y))
                {
                    selected = ep.first;
                    break;
                }
            }
        }
    }
    if (m_selected != selected)
    {
        m_selected = selected;
        FireEvent(EVT_STATUSBAR_UPDATE);
        RedrawAllSprites();
        ForceRedraw();
    }
    evt.Skip();
}

void RoomViewerCtrl::OnKeyDown(wxKeyEvent& evt)
{
    if (m_redraw)
    {
        evt.Skip();
        return;
    }
    bool refresh_sprites = false;
    switch (evt.GetKeyCode())
    {
    case WXK_TAB:
        if (evt.GetModifiers() == wxMOD_SHIFT)
        {
            m_selected--;
            if (m_selected < 1)
            {
                m_selected = m_entities.size();
            }
            refresh_sprites = true;
        }
        else if (evt.GetModifiers() == 0)
        {
            m_selected++;
            if (m_selected > m_entities.size() || m_selected < 1)
            {
                m_selected = 1;
            }
            refresh_sprites = true;
        }
        break;
    case WXK_ESCAPE:
        m_selected = -1;
        refresh_sprites = true;
        break;
    case WXK_INSERT:
        if (m_entities.size() < 15)
        {
            m_entities.push_back(Entity());
            m_selected = m_entities.size();
            refresh_sprites = true;
        }
    }
    if (!refresh_sprites && m_selected > 0 && m_selected <= m_entities.size())
    {
        auto& ent = m_entities[m_selected - 1];
        switch (evt.GetKeyCode())
        {
        case WXK_UP:
            ent.SetX(ent.GetX() - 0x80);
            refresh_sprites = true;
            break;
        case WXK_DOWN:
            ent.SetX(ent.GetX() + 0x80);
            refresh_sprites = true;
            break;
        case WXK_LEFT:
            ent.SetY(ent.GetY() + 0x80);
            refresh_sprites = true;
            break;
        case WXK_RIGHT:
            ent.SetY(ent.GetY() - 0x80);
            refresh_sprites = true;
            break;
        case WXK_PAGEUP:
            ent.SetZ(ent.GetZ() + 0x80);
            refresh_sprites = true;
            break;
        case WXK_PAGEDOWN:
            ent.SetZ(ent.GetZ() - 0x80);
            refresh_sprites = true;
            break;
        case WXK_NUMPAD_ADD:
        case WXK_ADD:
            ent.SetType((ent.GetType() + 1) & 0xFF);
            refresh_sprites = true;
            break;
        case WXK_NUMPAD_SUBTRACT:
        case WXK_SUBTRACT:
            ent.SetType((ent.GetType() - 1) & 0xFF);
            refresh_sprites = true;
            break;
        case WXK_DELETE:
            if (evt.GetModifiers() == wxMOD_SHIFT)
            {
                m_entities.erase(m_entities.begin() + (m_selected - 1));
                if (m_entities.size() == 0)
                {
                    m_selected = -1;
                }
                else if (m_selected > m_entities.size())
                {
                    m_selected = m_entities.size();
                }
                refresh_sprites = true;
            }
            break;
        case 'R':
            if (evt.GetModifiers() == wxMOD_SHIFT)
            {
                ent.SetOrientation(static_cast<Orientation>(
                    (static_cast<int>(ent.GetOrientation()) + 1) & 0x03));
                refresh_sprites = true;
            }
            else if (evt.GetModifiers() == 0)
            {
                ent.SetOrientation(static_cast<Orientation>(
                    (static_cast<int>(ent.GetOrientation()) - 1) & 0x03));
                refresh_sprites = true;
            }
            break;
        case 'P':
            if (evt.GetModifiers() == wxMOD_SHIFT)
            {
                ent.SetPalette((ent.GetPalette() + 1) & 0x03);
                refresh_sprites = true;
            }
            else if (evt.GetModifiers() == 0)
            {
                ent.SetPalette((ent.GetPalette() - 1) & 0x03);
                refresh_sprites = true;
            }
            break;
        case 'D':
            if (evt.GetModifiers() == wxMOD_CONTROL)
            {
                if (m_entities.size() < 15)
                {
                    m_entities.push_back(ent);
                    m_entities.back().SetX(m_entities.back().GetX() + 0x100);
                    m_selected = m_entities.size();
                    refresh_sprites = true;
                }
            }
            break;
        case '[':
            if (m_selected > 1)
            {
                std::swap(ent, m_entities[m_selected - 2]);
                m_selected--;
                refresh_sprites = true;
            }
            break;
        case ']':
            if (m_selected < m_entities.size())
            {
                std::swap(ent, m_entities[m_selected]);
                m_selected++;
                refresh_sprites = true;
            }
            break;
        case WXK_RETURN:
            UpdateEntityProperties(m_selected);
            break;
        }
    }
    if (refresh_sprites)
    {
        m_g->GetSpriteData()->SetRoomEntities(m_roomnum, m_entities);
        FireEvent(EVT_STATUSBAR_UPDATE);
        RedrawAllSprites();
        ForceRedraw();
        evt.Skip(false);
    }
    else
    {
        evt.Skip();
    }
}

void RoomViewerCtrl::OnLeftDblClick(wxMouseEvent& evt)
{
    int x, y;
    GetViewStart(&x, &y);
    x *= m_scroll_rate;
    y *= m_scroll_rate;
    x += evt.GetX();
    y += evt.GetY();
    x = std::ceil(x / m_zoom);
    y = std::ceil(y / m_zoom);

    for (const auto& ep : m_entity_poly)
    {
        if (Pnpoly(ep.second, x, y) && ep.first == m_selected)
        {
            UpdateEntityProperties(m_selected);
            break;
        }
    }
    evt.Skip();
}