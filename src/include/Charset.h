﻿#ifndef _CHARSET_H_
#define _CHARSET_H_

#include "RomOffsets.h"
#include "LSString.h"

namespace Charset
{

	const LSString::CharacterSet DEFAULT_ENGLISH_CHARSET = {
		{ 0, L" "}, { 1, L"0"}, { 2, L"1"}, { 3, L"2"}, { 4, L"3"}, { 5, L"4"}, { 6, L"5"}, { 7, L"6"},
		{ 8, L"7"}, { 9, L"8"}, {10, L"9"}, {11, L"A"}, {12, L"B"}, {13, L"C"}, {14, L"D"}, {15, L"E"},
		{16, L"F"}, {17, L"G"}, {18, L"H"}, {19, L"I"}, {20, L"J"}, {21, L"K"}, {22, L"L"}, {23, L"M"},
		{24, L"N"}, {25, L"O"}, {26, L"P"}, {27, L"Q"}, {28, L"R"}, {29, L"S"}, {30, L"T"}, {31, L"U"},
		{32, L"V"}, {33, L"W"}, {34, L"X"}, {35, L"Y"}, {36, L"Z"}, {37, L"a"}, {38, L"b"}, {39, L"c"},
		{40, L"d"}, {41, L"e"}, {42, L"f"}, {43, L"g"}, {44, L"h"}, {45, L"i"}, {46, L"j"}, {47, L"k"},
		{48, L"l"}, {49, L"m"}, {50, L"n"}, {51, L"o"}, {52, L"p"}, {53, L"q"}, {54, L"r"}, {55, L"s"},
		{56, L"t"}, {57, L"u"}, {58, L"v"}, {59, L"w"}, {60, L"x"}, {61, L"y"}, {62, L"z"}, {63, L"*"},
		{64, L"."}, {65, L","}, {66, L"?"}, {67, L"!"}, {68, L"/"}, {69, L"<"}, {70, L">"}, {71, L":"},
		{72, L"-"}, {73, L"\'"}, {74, L"\""}, {75, L"%"}, {76, L"#"}, {77, L"&"}, {78, L"("}, {79, L")"},
		{80, L"="}, {81, L"↖"}, {82, L"↗"}, {83, L"↘"}, {84, L"↙"}
	};

	const LSString::CharacterSet DEFAULT_FRENCH_CHARSET = {
		{ 0, L" "}, { 1, L"0"}, { 2, L"1"}, { 3, L"2"}, { 4, L"3"}, { 5, L"4"}, { 6, L"5"}, { 7, L"6"},
		{ 8, L"7"}, { 9, L"8"}, {10, L"9"}, {11, L"A"}, {12, L"B"}, {13, L"C"}, {14, L"D"}, {15, L"E"},
		{16, L"F"}, {17, L"G"}, {18, L"H"}, {19, L"I"}, {20, L"J"}, {21, L"K"}, {22, L"L"}, {23, L"M"},
		{24, L"N"}, {25, L"O"}, {26, L"P"}, {27, L"Q"}, {28, L"R"}, {29, L"S"}, {30, L"T"}, {31, L"U"},
		{32, L"V"}, {33, L"W"}, {34, L"X"}, {35, L"Y"}, {36, L"Z"}, {37, L"a"}, {38, L"b"}, {39, L"c"},
		{40, L"d"}, {41, L"e"}, {42, L"f"}, {43, L"g"}, {44, L"h"}, {45, L"i"}, {46, L"j"}, {47, L"k"},
		{48, L"l"}, {49, L"m"}, {50, L"n"}, {51, L"o"}, {52, L"p"}, {53, L"q"}, {54, L"r"}, {55, L"s"},
		{56, L"t"}, {57, L"u"}, {58, L"v"}, {59, L"w"}, {60, L"x"}, {61, L"y"}, {62, L"z"}, {63, L"*"},
		{64, L"."}, {65, L","}, {66, L"?"}, {67, L"!"}, {68, L"/"}, {69, L"<"}, {70, L">"}, {71, L":"},
		{72, L"-"}, {73, L"\'"}, {74, L"\""}, {75, L"%"}, {76, L"#"}, {77, L"&"}, {78, L"("}, {79, L")"},
		{80, L"="}, {81, L"↖"}, {82, L"↗"}, {83, L"↘"}, {84, L"↙"}, {85, L"è"}, {86, L"á"}, {87, L"à"},
		{88, L"ù"}, {89, L"â"}, {90, L"ê"}, {91, L"î"}, {92, L"ô"}, {93, L"û"}, {94, L"ç"}, {95, L"ü"},
		{96, L";"}, {97, L"`"}
	};

	const LSString::CharacterSet DEFAULT_GERMAN_CHARSET = {
		{ 0, L" "}, { 1, L"0"}, { 2, L"1"}, { 3, L"2"}, { 4, L"3"}, { 5, L"4"}, { 6, L"5"}, { 7, L"6"},
		{ 8, L"7"}, { 9, L"8"}, {10, L"9"}, {11, L"A"}, {12, L"B"}, {13, L"C"}, {14, L"D"}, {15, L"E"},
		{16, L"F"}, {17, L"G"}, {18, L"H"}, {19, L"I"}, {20, L"J"}, {21, L"K"}, {22, L"L"}, {23, L"M"},
		{24, L"N"}, {25, L"O"}, {26, L"P"}, {27, L"Q"}, {28, L"R"}, {29, L"S"}, {30, L"T"}, {31, L"U"},
		{32, L"V"}, {33, L"W"}, {34, L"X"}, {35, L"Y"}, {36, L"Z"}, {37, L"*"}, {38, L"."}, {39, L","},
		{40, L"?"}, {41, L"!"}, {42, L"/"}, {43, L"<"}, {44, L">"}, {45, L":"},	{46, L"-"}, {47, L"\'"},
		{48, L"\""}, {49, L"%"}, {50, L"#"}, {51, L"&"}, {52, L"("}, {53, L")"}, {54, L"="}, {55, L"↖"},
		{56, L"↗"}, {57, L"↘"}, {58, L"↙"}, {59, L"Ä"}, {60, L"Ö"}, {61, L"Ü"}, {62, L"ß"}
	};

	inline const LSString::CharacterSet DEFAULT_JAPANESE_CHARSET =
	{
		{  0, L"　"}, {  1, L"０"}, {  2, L"１"}, {  3, L"２"}, {  4, L"３"}, {  5, L"４"}, {  6, L"５"}, {  7, L"６"},
		{  8, L"７"}, {  9, L"８"}, { 10, L"９"}, { 11, L"あ"}, { 12, L"い"}, { 13, L"う"}, { 14, L"え"}, { 15, L"お"},
		{ 16, L"か"}, { 17, L"き"}, { 18, L"く"}, { 19, L"け"}, { 20, L"こ"}, { 21, L"さ"}, { 22, L"し"}, { 23, L"す"},
		{ 24, L"せ"}, { 25, L"そ"}, { 26, L"た"}, { 27, L"ち"}, { 28, L"つ"}, { 29, L"て"}, { 30, L"と"}, { 31, L"な"},
		{ 32, L"に"}, { 33, L"ぬ"}, { 34, L"ね"}, { 35, L"の"}, { 36, L"は"}, { 37, L"ひ"}, { 38, L"ふ"}, { 39, L"へ"},
		{ 40, L"ほ"}, { 41, L"ま"}, { 42, L"み"}, { 43, L"む"}, { 44, L"め"}, { 45, L"も"}, { 46, L"や"}, { 47, L"ゆ"},
		{ 48, L"よ"}, { 49, L"ら"}, { 50, L"り"}, { 51, L"る"}, { 52, L"れ"}, { 53, L"ろ"}, { 54, L"わ"}, { 55, L"を"},
		{ 56, L"ん"}, { 57, L"ぁ"}, { 58, L"ぃ"}, { 59, L"ぅ"}, { 60, L"ぇ"}, { 61, L"ぉ"}, { 62, L"ゃ"}, { 63, L"ゅ"},
		{ 64, L"ょ"}, { 65, L"ゎ"}, { 66, L"ア"}, { 67, L"イ"}, { 68, L"ウ"}, { 69, L"エ"}, { 70, L"オ"}, { 71, L"カ"},
		{ 72, L"キ"}, { 73, L"ク"}, { 74, L"ケ"}, { 75, L"コ"}, { 76, L"サ"}, { 77, L"シ"}, { 78, L"ス"}, { 79, L"セ"},
		{ 80, L"ソ"}, { 81, L"タ"}, { 82, L"チ"}, { 83, L"ツ"}, { 84, L"テ"}, { 85, L"ト"}, { 86, L"ナ"}, { 87, L"ニ"},
		{ 88, L"ヌ"}, { 89, L"ネ"}, { 90, L"ノ"}, { 91, L"ハ"}, { 92, L"ヒ"}, { 93, L"フ"}, { 94, L"ヘ"}, { 95, L"ホ"},
		{ 96, L"マ"}, { 97, L"ミ"}, { 98, L"ム"}, { 99, L"メ"}, {100, L"モ"}, {101, L"ヤ"}, {102, L"ユ"}, {103, L"ヨ"},
		{104, L"ラ"}, {105, L"リ"}, {106, L"ル"}, {107, L"レ"}, {108, L"ロ"}, {109, L"ワ"}, {110, L"ヲ"}, {111, L"ン"},
		{112, L"ァ"}, {113, L"ィ"}, {114, L"ゥ"}, {115, L"ェ"}, {116, L"ォ"}, {117, L"ャ"}, {118, L"ュ"}, {119, L"ョ"},
		{120, L"ッ"}, {121, L"、"}, {122, L"。"}, {123, L"゛"}, {124, L"゜"}, {125, L"ー"}, {126, L"！"}, {127, L"？"},
		{128, L"※"}, {129, L"東"}, {130, L"西"}, {131, L"南"}, {132, L"北"}, {133, L"上"}, {134, L"中"}, {135, L"下"},
		{136, L"道"}, {137, L"具"}, {138, L"屋"}, {139, L"教"}, {140, L"会"}, {141, L"宿"}, {142, L"神"}, {143, L"父"},
		{144, L"冒"}, {145, L"険"}, {146, L"記"}, {147, L"録"}, {148, L"毒"}, {149, L"呪"}, {150, L"治"}, {151, L"療"},
		{152, L"金"}, {153, L"貨"}, {154, L"枚"}, {155, L"買"}, {156, L"階"}, {157, L"本"}, {158, L"売"}, {159, L"泊"},
		{160, L"客"}, {161, L"品"}, {162, L"男"}, {163, L"女"}, {164, L"子"}, {165, L"供"}, {166, L"人"}, {167, L"族"},
		{168, L"殿"}, {169, L"公"}, {170, L"爵"}, {171, L"領"}, {172, L"主"}, {173, L"兵"}, {174, L"悪"}, {175, L"霊"},
		{176, L"年"}, {177, L"月"}, {178, L"日"}, {179, L"財"}, {180, L"宝"}, {181, L"地"}, {182, L"図"}, {183, L"実"},
		{184, L"灯"}, {185, L"台"}, {186, L"家"}, {187, L"店"}, {188, L"町"}, {189, L"村"}, {190, L"滝"}, {191, L"岬"},
		{192, L"島"}, {193, L"海"}, {194, L"沼"}, {195, L"湖"}, {196, L"港"}, {197, L"城"}, {198, L"塔"}, {199, L"森"},
		{200, L"橋"}, {201, L"団"}, {202, L"気"}, {203, L"船"}, {204, L"箱"}, {205, L"魔"}, {206, L"命"}, {207, L"危"},
		{208, L"美"}, {209, L"長"}, {210, L"古"}, {211, L"老"}, {212, L"作"}, {213, L"名"}, {214, L"商"}, {215, L"大"},
		{216, L"・"}, {217, L"「"}, {218, L"」"}, {219, L"↘"}, {220, L"↖"}, {221, L"↙"}, {222, L"↗"}, {223, L"王"},
		{224, L"剣"}, {225, L"士"}, {226, L"国"}, {227, L"本"}, {228, L"法"}, {229, L"Ａ"}, {230, L"Ｂ"}, {231, L"Ｃ"},
		{232, L"．"}, {236, L"╳"}															   
	};

	inline const LSString::DiacriticMap JAPANESE_DIACRITIC_MAP =
	{
		{L"゛",{{L"か",L"が"}, {L"き",L"ぎ"}, {L"く",L"ぐ"}, {L"け",L"げ"}, {L"こ",L"ご"}, {L"さ",L"ざ"}, {L"し",L"じ"},
		        {L"す",L"ず"}, {L"せ",L"ぜ"}, {L"そ",L"ぞ"}, {L"た",L"だ"}, {L"ち",L"ぢ"}, {L"つ",L"づ"}, {L"て",L"で"},
		        {L"と",L"ど"}, {L"は",L"ば"}, {L"ひ",L"び"}, {L"ふ",L"ぶ"}, {L"へ",L"べ"}, {L"ほ",L"ぼ"}, {L"カ",L"ガ"},
		        {L"キ",L"ギ"}, {L"ク",L"グ"}, {L"ケ",L"ゲ"}, {L"コ",L"ゴ"}, {L"サ",L"ザ"}, {L"シ",L"ジ"}, {L"ス",L"ズ"},
		        {L"セ",L"ゼ"}, {L"ソ",L"ゾ"}, {L"タ",L"ダ"}, {L"チ",L"ヂ"}, {L"ツ",L"ヅ"}, {L"テ",L"デ"}, {L"ト",L"ド"},
		        {L"ハ",L"バ"}, {L"ヒ",L"ビ"}, {L"フ",L"ブ"}, {L"ヘ",L"ベ"}, {L"ホ",L"ボ"}, {L"ウ",L"ヴ"}}},
		{L"゜",{{L"は",L"ぱ"}, {L"ひ",L"ぴ"}, {L"ふ",L"ぷ"}, {L"へ",L"ぺ"}, {L"へ",L"ぺ"}, {L"ほ",L"ぽ"}, {L"ハ",L"パ"},
		        {L"ヒ",L"ピ"}, {L"フ",L"プ"}, {L"ヘ",L"ペ"}, {L"ホ",L"ポ"}}}
	};

	inline const LSString::DiacriticMap DEFAULT_DIACRITIC_MAP = {};

	inline RomOffsets::Region DeduceRegion(uint32_t printable_charset_size)
	{
		switch (printable_charset_size)
		{
		case 237:
			return RomOffsets::Region::JP;
		case 98:
			return RomOffsets::Region::FR;
		case 63:
			return RomOffsets::Region::DE;
		case 85:
		default:
			return RomOffsets::Region::US;
		}
	}

	inline const LSString::CharacterSet& GetDefaultCharset(RomOffsets::Region region)
	{
		switch (region)
		{
		case RomOffsets::Region::JP:
			return DEFAULT_JAPANESE_CHARSET;
		case RomOffsets::Region::FR:
			return DEFAULT_FRENCH_CHARSET;
		case RomOffsets::Region::DE:
			return DEFAULT_GERMAN_CHARSET;
		case RomOffsets::Region::US:
		case RomOffsets::Region::UK:
		case RomOffsets::Region::US_BETA:
		default:
			return DEFAULT_ENGLISH_CHARSET;
		}
	}

	inline uint8_t GetEOSChar(RomOffsets::Region region)
	{
		switch (region)
		{
		case RomOffsets::Region::JP:
			return 0xE9;
		case RomOffsets::Region::FR:
			return 0x64;
		case RomOffsets::Region::DE:
			return 0x41;
		case RomOffsets::Region::US:
		case RomOffsets::Region::UK:
		case RomOffsets::Region::US_BETA:
		default:
			return 0x55;
		}
	}

	inline const LSString::DiacriticMap& GetDiacriticMap(RomOffsets::Region region)
	{
		if (region == RomOffsets::Region::JP)
		{
			return JAPANESE_DIACRITIC_MAP;
		}
		return DEFAULT_DIACRITIC_MAP;
	}

} // namespace Charset

#endif // _CHARSET_H_
