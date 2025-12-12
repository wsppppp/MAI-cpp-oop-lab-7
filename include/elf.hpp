#pragma once

#include <string>
#include <memory>
#include "npc.hpp"

struct Elf : public NPC {
    Elf() = default;
    Elf(const std::string &name_, int x_, int y_);
    Elf(std::istream &is);

    bool accept(const std::shared_ptr<NPC>& attacker) override;
    bool visit_elf(const std::shared_ptr<Elf>& defender) override;
    bool visit_dragon(const std::shared_ptr<Dragon>& defender) override;
    bool visit_druid(const std::shared_ptr<Druid>& defender) override;

    void print() const override;
    void save(std::ostream& os) const override;

    int step() const override { return 10; }        
    int kill_radius() const override { return 50; }  
};