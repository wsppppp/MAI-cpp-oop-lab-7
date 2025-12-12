#pragma once
#include "npc.hpp"

std::shared_ptr<NPC> factory(NpcType type, const std::string &name, int x, int y);
std::shared_ptr<NPC> factory(std::istream &is);