#include "../include/dragon.hpp"
#include "factory.hpp"
#include "../include/elf.hpp"
#include "../include/druid.hpp"

Dragon::Dragon(const std::string &name_, int x_, int y_) : NPC(name_, x_, y_) {
  type = DragonType;
}

Dragon::Dragon(std::istream &is) {
  is >> name;
  is >> x >> y;
  type = DragonType;
}

bool Dragon::accept(const std::shared_ptr<NPC>& attacker) {
  return attacker->visit_dragon(std::static_pointer_cast<Dragon>(shared_from_this()));
}

// Дракон убивает эльфа
bool Dragon::visit_elf(const std::shared_ptr<Elf>& defender) { return true; }
// Дракон не бьёт дракона
bool Dragon::visit_dragon(const std::shared_ptr<Dragon>& defender) { return false; }
// Дракон не убивает друида
bool Dragon::visit_druid(const std::shared_ptr<Druid>& defender) { return false; }

void Dragon::print() const { std::cout << "Dragon: " << *this << std::endl; }

void Dragon::save(std::ostream& os) const {
  os << DragonType << std::endl;
  NPC::save(os);
}