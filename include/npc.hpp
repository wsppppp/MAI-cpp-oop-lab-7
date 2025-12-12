#pragma once

#include <iostream>
#include <memory>
#include <vector>
#include <mutex>
#include <shared_mutex>

struct Elf;
struct Dragon;
struct Druid;

struct IFightObserver {
  virtual void on_fight(const std::shared_ptr<struct NPC>& attacker, const std::shared_ptr<struct NPC>& defender, bool win) = 0;
  virtual ~IFightObserver() = default;
};

enum NpcType {
  ElfType = 1,
  DragonType = 2,
  DruidType = 3
};

struct NPC : public std::enable_shared_from_this<NPC> {
public:
  std::string name;
  NpcType type;
  int x{0};
  int y{0};
protected:
  bool alive{true};
  mutable std::shared_mutex mtx_pos;
  std::vector<std::shared_ptr<IFightObserver>> observers;
public:
  NPC() = default;
  NPC(const std::string &name_, int x_, int y_);
  virtual ~NPC() = default;

  void subscribe(std::shared_ptr<IFightObserver> observer);
  void fight_notify(const std::shared_ptr<NPC>& defender, bool win);

  bool is_close(const std::shared_ptr<NPC>& other, size_t distance) const;
  std::pair<int, int> position() const;
  void move(int dx, int dy, int max_x, int max_y);
  bool is_alive() const;
  void die();

  virtual int step() const = 0;
  virtual int kill_radius() const = 0;

  virtual bool accept(const std::shared_ptr<NPC> &attacker) = 0;
  virtual bool visit_elf(const std::shared_ptr<Elf>& defender) = 0;
  virtual bool visit_dragon(const std::shared_ptr<Dragon>& defender) = 0;
  virtual bool visit_druid(const std::shared_ptr<Druid>& defender) = 0;

  virtual void print() const = 0;
  virtual void save(std::ostream& os) const;

  friend std::ostream& operator<<(std::ostream& os, const NPC& npc);
};