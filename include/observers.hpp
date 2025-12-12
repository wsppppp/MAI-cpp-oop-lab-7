#pragma once

#include <fstream>

#include "npc.hpp"

struct TextObserver : public IFightObserver {
  static std::shared_ptr<IFightObserver> get();
  void on_fight(const std::shared_ptr<NPC>& attacker, const std::shared_ptr<NPC>& defender, bool win) override;
private:
  TextObserver() {}
};

struct FileObserver : public IFightObserver {
  static std::shared_ptr<IFightObserver> get();
  void on_fight(const std::shared_ptr<NPC>& attacker, const std::shared_ptr<NPC>& defender, bool win) override;
private:
  std::ofstream file;
  FileObserver();
};