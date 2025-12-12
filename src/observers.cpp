#include "observers.hpp"

std::shared_ptr<IFightObserver> TextObserver::get() {
  static TextObserver instance;
  return std::shared_ptr<IFightObserver>(&instance, [](IFightObserver *) {});
}

void TextObserver::on_fight(const std::shared_ptr<NPC>& attacker, const std::shared_ptr<NPC>& defender, bool win) {
  if (win) {
    std::cout << std::endl << "Murder --------" << std::endl;
    attacker->print();
    defender->print();
  }
}

FileObserver::FileObserver() { file.open("log.txt", std::ios::app); }

std::shared_ptr<IFightObserver> FileObserver::get() {
  static FileObserver instance;
  return std::shared_ptr<IFightObserver>(&instance, [](IFightObserver *) {});
}

void FileObserver::on_fight(const std::shared_ptr<NPC>& attacker, const std::shared_ptr<NPC>& defender, bool win) {
  if (!file.is_open()) return;
  if (win) {
    file << "Murder --------" << std::endl;
    file << attacker->name << " vs " << defender->name << std::endl;
  }
  file.flush();
}