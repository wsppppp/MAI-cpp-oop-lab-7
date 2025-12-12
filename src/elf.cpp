#include "../include/elf.hpp"
#include "factory.hpp"
#include "../include/dragon.hpp"
#include "../include/druid.hpp"

Elf::Elf(const std::string &name_, int x_, int y_) : NPC(name_, x_, y_) {
  type = ElfType;
}

Elf::Elf(std::istream &is) {
  is >> name;
  is >> x >> y;
  type = ElfType;
}

bool Elf::accept(const std::shared_ptr<NPC>& attacker) {
  return attacker->visit_elf(std::static_pointer_cast<Elf>(shared_from_this()));
}

// Эльф не бьёт эльфа
bool Elf::visit_elf(const std::shared_ptr<Elf>& defender) { return false; }
// Эльф не может убить дракона
bool Elf::visit_dragon(const std::shared_ptr<Dragon>& defender) { return false; }
// Эльф убивает друида
bool Elf::visit_druid(const std::shared_ptr<Druid>& defender) { return true; }

void Elf::print() const { std::cout << "Elf: " << *this << std::endl; }

void Elf::save(std::ostream& os) const {
  os << ElfType << std::endl;
  NPC::save(os);
}