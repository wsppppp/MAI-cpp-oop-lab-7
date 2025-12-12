#include "../include/factory.hpp"
#include "../include/elf.hpp"
#include "../include/dragon.hpp"
#include "../include/druid.hpp"

std::shared_ptr<NPC> factory(NpcType type, const std::string &name, int x, int y) {
    switch (type) {
        case ElfType:     return std::make_shared<Elf>(name, x, y);
        case DragonType:  return std::make_shared<Dragon>(name, x, y);
        case DruidType:   return std::make_shared<Druid>(name, x, y);
        default: throw std::runtime_error("Unknown NpcType");
    }
}

std::shared_ptr<NPC> factory(std::istream &is) {
    int t;
    is >> t;
    NpcType type = static_cast<NpcType>(t);
    switch (type) {
        case ElfType:     return std::make_shared<Elf>(is);
        case DragonType:  return std::make_shared<Dragon>(is);
        case DruidType:   return std::make_shared<Druid>(is);
        default: throw std::runtime_error("Unknown NpcType");
    }
}