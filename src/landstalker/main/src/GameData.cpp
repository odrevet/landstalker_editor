#include <landstalker/main/include/GameData.h>

#include <algorithm>

#include <landstalker/main/include/RomLabels.h>

GameData::GameData(const filesystem::path& asm_file)
	: DataManager(asm_file),
	  m_rd(std::make_shared<RoomData>(asm_file)),
	  m_gd(std::make_shared<GraphicsData>(asm_file)),
	  m_sd(std::make_shared<StringData>(asm_file)),
	  m_spd(std::make_shared<SpriteData>(asm_file)),
	  m_scd(std::make_shared<ScriptData>(asm_file))
{
	m_data.push_back(m_rd);
	m_data.push_back(m_gd);
	m_data.push_back(m_sd);
	m_data.push_back(m_spd);
	m_data.push_back(m_scd);
	CacheData();
	SetDefaults();
}

GameData::GameData(const Rom& rom)
	: DataManager(rom),
	  m_rd(std::make_shared<RoomData>(rom)),
	  m_gd(std::make_shared<GraphicsData>(rom)),
	  m_sd(std::make_shared<StringData>(rom)),
	  m_spd(std::make_shared<SpriteData>(rom)),
	  m_scd(std::make_shared<ScriptData>(rom))
{
	m_data.push_back(m_rd);
	m_data.push_back(m_gd);
	m_data.push_back(m_sd);
	m_data.push_back(m_spd);
	m_data.push_back(m_scd);
	CacheData();
	SetDefaults();
}

bool GameData::Save(const filesystem::path& dir)
{
	auto success = std::all_of(m_data.begin(), m_data.end(), [&](auto& d)
		{
			return d->Save(dir);
		});
	return success;
}

bool GameData::Save()
{
	return std::all_of(m_data.begin(), m_data.end(), [](auto& d)
		{
			return d->Save();
		});
}

PendingWrites GameData::GetPendingWrites() const
{
	PendingWrites retval;
	std::for_each(m_data.begin(), m_data.end(), [&](auto& d)
		{
			auto w = d->GetPendingWrites();
			retval.insert(retval.end(), w.cbegin(), w.cend());
		});
	return retval;
}

bool GameData::WillFitInRom(const Rom& rom) const
{
	return std::all_of(m_data.begin(), m_data.end(), [&](auto& d)
		{
			return d->WillFitInRom(rom);
		});
}

bool GameData::HasBeenModified() const
{
	return std::any_of(m_data.begin(), m_data.end(), [](auto& d)
		{
			return d->HasBeenModified();
		});
}

bool GameData::InjectIntoRom(Rom& rom)
{
	return std::all_of(m_data.begin(), m_data.end(), [&](auto& d)
		{
			return d->InjectIntoRom(rom);
		});
}

void GameData::RefreshPendingWrites(const Rom& rom)
{
	std::for_each(m_data.begin(), m_data.end(), [&](auto& d)
		{
			d->RefreshPendingWrites(rom);
		});
}

const std::map<std::string, std::shared_ptr<PaletteEntry>>& GameData::GetAllPalettes() const
{
	return m_palettes;
}

const std::map<std::string, std::shared_ptr<TilesetEntry>>& GameData::GetAllTilesets() const
{
	return m_tilesets;
}

const std::map<std::string, std::shared_ptr<AnimatedTilesetEntry>>& GameData::GetAllAnimatedTilesets() const
{
	return m_anim_tilesets;
}

const std::map<std::string, std::shared_ptr<Tilemap2DEntry>>& GameData::GetAllTilemaps() const
{
	return m_tilemaps;
}

std::shared_ptr<PaletteEntry> GameData::GetPalette(const std::string& name) const
{
	if (m_palettes.find(name) == m_palettes.cend())
	{
		return nullptr;
	}
	return m_palettes.at(name);
}

std::shared_ptr<TilesetEntry> GameData::GetTileset(const std::string& name) const
{
	return m_tilesets.at(name);
}

std::shared_ptr<AnimatedTilesetEntry> GameData::GetAnimatedTileset(const std::string& name) const
{
	return m_anim_tilesets.at(name);
}

std::shared_ptr<Tilemap2DEntry> GameData::GetTilemap(const std::string& name) const
{
	return m_tilemaps.at(name);
}

void GameData::CacheData()
{
	auto room_pals = m_rd->GetAllPalettes();
	m_palettes.insert(room_pals.cbegin(), room_pals.cend());
	auto gfx_pals = m_gd->GetAllPalettes();
	m_palettes.insert(gfx_pals.cbegin(), gfx_pals.cend());
	const auto& sprite_pals = m_spd->GetAllPalettes();
	m_palettes.insert(sprite_pals.cbegin(), sprite_pals.cend());

	auto room_ts = m_rd->GetAllTilesets();
	m_tilesets.insert(room_ts.cbegin(), room_ts.cend());
	auto gfx_ts = m_gd->GetAllTilesets();
	m_tilesets.insert(gfx_ts.cbegin(), gfx_ts.cend());
	auto font_ts = m_sd->GetAllTilesets();
	m_tilesets.insert(font_ts.cbegin(), font_ts.cend());

	auto anim_ts = m_rd->GetAllAnimatedTilesets();
	m_anim_tilesets.insert(anim_ts.cbegin(), anim_ts.cend());

	auto maps = m_gd->GetAllMaps();
	m_tilemaps.insert(maps.cbegin(), maps.cend());
	auto textbox_maps = m_sd->GetAllMaps();
	m_tilemaps.insert(textbox_maps.cbegin(), textbox_maps.cend());
}

void GameData::SetDefaults()
{
	m_tilemaps[RomLabels::Graphics::HUD_TILEMAP]->SetTileset(RomLabels::Graphics::HUD_TILESET);
	m_tilemaps[RomLabels::Graphics::INV_TILEMAP]->SetTileset(RomLabels::Graphics::HUD_TILESET);
	m_tilemaps[RomLabels::Graphics::TEXTBOX_2LINE_MAP]->SetTileset(RomLabels::Graphics::HUD_TILESET);
	m_tilemaps[RomLabels::Graphics::TEXTBOX_3LINE_MAP]->SetTileset(RomLabels::Graphics::HUD_TILESET);
	m_tilemaps[RomLabels::Graphics::ISLAND_MAP_BG_MAP]->SetTileset(RomLabels::Graphics::ISLAND_MAP_BG_TILES);
	m_tilemaps[RomLabels::Graphics::ISLAND_MAP_FG_MAP]->SetTileset(RomLabels::Graphics::ISLAND_MAP_FG_TILES);
	m_tilemaps[RomLabels::Graphics::LITHOGRAPH_MAP]->SetTileset(RomLabels::Graphics::LITHOGRAPH_TILES);
	m_tilemaps[RomLabels::Graphics::END_CREDITS_MAP]->SetTileset(RomLabels::Graphics::END_CREDITS_LOGOS);
	m_tilemaps[RomLabels::Graphics::CLIMAX_LOGO_MAP]->SetTileset(RomLabels::Graphics::CLIMAX_LOGO_TILES);
	m_tilemaps[RomLabels::Graphics::TITLE_1_MAP]->SetTileset(RomLabels::Graphics::TITLE_1_TILES);
	m_tilemaps[RomLabels::Graphics::TITLE_2_MAP]->SetTileset(RomLabels::Graphics::TITLE_2_TILES);
	m_tilemaps[RomLabels::Graphics::TITLE_3_MAP]->SetTileset(RomLabels::Graphics::TITLE_3_TILES);
	m_tilemaps[RomLabels::Graphics::GAME_LOAD_MAP]->SetTileset(RomLabels::Graphics::GAME_LOAD_TILES);

	m_tilesets[RomLabels::Graphics::HUD_TILESET]->SetDefaultPalette(RomLabels::Graphics::HUD_PAL);
	m_tilesets[RomLabels::Graphics::MAIN_FONT]->SetDefaultPalette(RomLabels::Graphics::HUD_PAL);
	m_tilesets[RomLabels::Graphics::MAIN_FONT]->SetPalIndicies("10,1");
	m_tilesets[RomLabels::Graphics::INV_FONT]->SetDefaultPalette(RomLabels::Graphics::HUD_PAL);
	m_tilesets[RomLabels::Graphics::INV_FONT]->SetPalIndicies("10,1");
	m_tilesets[RomLabels::Tilesets::INTRO_FONT]->SetDefaultPalette(RomLabels::Graphics::HUD_PAL);
	if (m_tilesets.find(RomLabels::Graphics::END_CREDITS_FONT) != m_tilesets.end())
	{
		m_tilesets[RomLabels::Graphics::END_CREDITS_FONT]->SetDefaultPalette(RomLabels::Graphics::HUD_PAL);
	}
	m_tilesets[RomLabels::Graphics::END_CREDITS_FONT]->SetDefaultPalette(RomLabels::Graphics::END_CREDITS_PAL);
	m_tilesets[RomLabels::Graphics::DOWN_ARROW]->SetDefaultPalette(RomLabels::Graphics::HUD_PAL);
	m_tilesets[RomLabels::Graphics::RIGHT_ARROW]->SetDefaultPalette(RomLabels::Graphics::HUD_PAL);
	m_tilesets[RomLabels::Graphics::INV_ARROW]->SetDefaultPalette(RomLabels::Graphics::HUD_PAL);
	m_tilesets[RomLabels::Graphics::INV_ARROW]->SetPalIndicies("0,11,14,15");
	m_tilesets[RomLabels::Graphics::INV_CURSOR]->SetDefaultPalette(RomLabels::Graphics::INV_ITEM_PAL);
	m_tilesets[RomLabels::Graphics::INV_CURSOR]->SetPalIndicies("0,5,10,15");
	if (m_tilesets.find(RomLabels::Graphics::INV_UNUSED1) != m_tilesets.cend())
	{
		m_tilesets[RomLabels::Graphics::INV_UNUSED1]->SetDefaultPalette(RomLabels::Graphics::HUD_PAL);
	}
	if (m_tilesets.find(RomLabels::Graphics::INV_UNUSED2) != m_tilesets.cend())
	{
		m_tilesets[RomLabels::Graphics::INV_UNUSED2]->SetDefaultPalette(RomLabels::Graphics::HUD_PAL);
	}
	m_tilesets[RomLabels::Graphics::SWORD_MAGIC]->SetDefaultPalette(RomLabels::Graphics::SWORD_PAL_SWAPS + ":1");
	m_tilesets[RomLabels::Graphics::SWORD_THUNDER]->SetDefaultPalette(RomLabels::Graphics::SWORD_PAL_SWAPS + ":2");
	m_tilesets[RomLabels::Graphics::SWORD_ICE]->SetDefaultPalette(RomLabels::Graphics::SWORD_PAL_SWAPS + ":3");
	m_tilesets[RomLabels::Graphics::SWORD_GAIA]->SetDefaultPalette(RomLabels::Graphics::SWORD_PAL_SWAPS + ":4");
	m_tilesets[RomLabels::Graphics::COINFALL]->SetDefaultPalette(RomLabels::Graphics::SWORD_PAL_SWAPS + ":5");
	m_tilesets[RomLabels::Graphics::ISLAND_MAP_BG_TILES]->SetDefaultPalette(RomLabels::Graphics::ISLAND_MAP_BG_PAL);
	m_tilesets[RomLabels::Graphics::ISLAND_MAP_FG_TILES]->SetDefaultPalette(RomLabels::Graphics::ISLAND_MAP_FG_PAL);
	m_tilesets[RomLabels::Graphics::ISLAND_MAP_DOTS]->SetDefaultPalette(RomLabels::Graphics::ISLAND_MAP_BG_PAL);
	m_tilesets[RomLabels::Graphics::ISLAND_MAP_FRIDAY]->SetDefaultPalette(RomLabels::Graphics::PLAYER_PAL);
	m_tilesets[RomLabels::Graphics::LITHOGRAPH_TILES]->SetDefaultPalette(RomLabels::Graphics::LITHOGRAPH_PAL);
	m_tilesets[RomLabels::Graphics::SEGA_LOGO_TILES]->SetDefaultPalette(RomLabels::Graphics::SEGA_LOGO_PAL);
	m_tilesets[RomLabels::Graphics::CLIMAX_LOGO_TILES]->SetDefaultPalette(RomLabels::Graphics::CLIMAX_LOGO_PAL);
	m_tilesets[RomLabels::Graphics::TITLE_1_TILES]->SetDefaultPalette(RomLabels::Graphics::TITLE_PALETTE_BLUE);
	m_tilesets[RomLabels::Graphics::TITLE_2_TILES]->SetDefaultPalette(RomLabels::Graphics::TITLE_PALETTE_YELLOW);
	m_tilesets[RomLabels::Graphics::TITLE_3_TILES]->SetDefaultPalette(RomLabels::Graphics::TITLE_3_PAL);
	m_tilesets[RomLabels::Graphics::GAME_LOAD_CHARS]->SetDefaultPalette(RomLabels::Graphics::GAME_LOAD_PALETTE);
	m_tilesets[RomLabels::Graphics::GAME_LOAD_TILES]->SetDefaultPalette(RomLabels::Graphics::GAME_LOAD_PALETTE);

	for (auto& t : m_gd->GetStatusEffects())
	{
		t->SetDefaultPalette(RomLabels::Graphics::PLAYER_PAL);
	}
}
