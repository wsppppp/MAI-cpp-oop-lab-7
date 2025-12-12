#include "../include/druid.hpp"
#include "factory.hpp"
#include "../include/elf.hpp"
#include "../include/dragon.hpp"

Druid::Druid(const std::string &name_, int x_, int y_) : NPC(name_, x_, y_) {
  type = DruidType;
}

Druid::Druid(std::istream &is) {
  is >> name;
  is >> x >> y;
  type = DruidType;
}

bool Druid::accept(const std::shared_ptr<NPC>& attacker) {
  return attacker->visit_druid(std::static_pointer_cast<Druid>(shared_from_this()));
}

// Друид атакует Дракона
bool Druid::visit_dragon(const std::shared_ptr<Dragon>& defender) { return true; }
// Друид не атакует Эльфа
bool Druid::visit_elf(const std::shared_ptr<Elf>& defender) { return false; }
// Друид не бьёт друида
bool Druid::visit_druid(const std::shared_ptr<Druid>& defender) { return false; }

void Druid::print() const { std::cout << "Druid: " << *this << std::endl; }

void Druid::save(std::ostream& os) const {
  os << DruidType << std::endl;
  NPC::save(os);
}