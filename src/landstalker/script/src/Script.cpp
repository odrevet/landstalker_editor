#include <landstalker/script/include/Script.h>

Script::Script()
{
}

Script::Script(const std::vector<uint8_t>& bytes)
{
	//m_table.reserve(bytes.size() >> 1);
	for (std::size_t i = 0; i < bytes.size(); i += 2)
	{
		uint16_t word = (bytes.at(i) << 8) | bytes.at(i + 1);
		m_table.push_back(std::move(ScriptTableEntry::FromBytes(word)));
	}
}

Script::Script(const Script& rhs)
{
	Copy(rhs);
}

bool Script::operator==(const Script& rhs) const
{
	if (this->m_table.size() != rhs.m_table.size())
	{
		return false;
	}
	for (std::size_t i = 0; i < this->m_table.size(); ++i)
	{
		if (this->m_table[i]->ToBytes() != rhs.m_table[i]->ToBytes())
		{
			return false;
		}
	}
	return true;
}

Script& Script::operator=(const Script& rhs)
{
	Copy(rhs);
	return *this;
}

bool Script::operator!=(const Script& rhs) const
{
	return !(*this == rhs);
}

std::vector<uint8_t> Script::ToBytes() const
{
	std::vector<uint8_t> bytes;
	bytes.reserve(m_table.size() << 1);
	for (const auto& e : m_table)
	{
		uint16_t word = e->ToBytes();
		bytes.push_back(word >> 8);
		bytes.push_back(word & 0xFF);
	}
	return bytes;
}

void Script::Copy(const Script& rhs)
{
	m_table.clear();
	m_table.reserve(rhs.m_table.size());
	for (const auto& e : rhs.m_table)
	{
		m_table.push_back(std::move(ScriptTableEntry::FromBytes(e->ToBytes())));
	}
}
