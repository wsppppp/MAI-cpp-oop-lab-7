#include <gtest/gtest.h>
#include <memory>
#include <sstream>
#include <vector>
#include "../include/elf.hpp"
#include "../include/dragon.hpp"
#include "../include/druid.hpp"
#include "../include/factory.hpp"
#include "../include/observers.hpp"

// Проверка дистанций (таблица)
TEST(NpcStats, Distances) {
    Elf e("Legolas", 0, 0);
    Dragon g("Smaug", 0, 0);
    Druid dr("Malorne", 0, 0);

    EXPECT_EQ(e.step(), 10);
    EXPECT_EQ(e.kill_radius(), 50);

    EXPECT_EQ(g.step(), 50);
    EXPECT_EQ(g.kill_radius(), 30);

    EXPECT_EQ(dr.step(), 10);
    EXPECT_EQ(dr.kill_radius(), 10);
}

// Логика «кто кого убивает»
TEST(FightLogic, WhoKillsWhom) {
    auto elf    = std::make_shared<Elf>("Elf", 0, 0);
    auto dragon = std::make_shared<Dragon>("Dragon", 0, 0);
    auto druid  = std::make_shared<Druid>("Druid", 0, 0);

    EXPECT_TRUE(elf->accept(dragon));   // Дракон → Эльф
    EXPECT_TRUE(druid->accept(elf));    // Эльф → Друид
    EXPECT_TRUE(dragon->accept(druid)); // Друид → Дракон

    EXPECT_FALSE(dragon->accept(elf));
    EXPECT_FALSE(elf->accept(druid));
    EXPECT_FALSE(druid->accept(dragon));

    EXPECT_FALSE(elf->accept(elf));
    EXPECT_FALSE(dragon->accept(dragon));
    EXPECT_FALSE(druid->accept(druid));
}

// Геометрия (is_close)
TEST(Geometry, IsClose) {
    auto elf = std::make_shared<Elf>("Elf", 0, 0);
    auto dragon = std::make_shared<Dragon>("Dragon", 0, 0);

    auto far_elf = std::make_shared<Elf>("FarElf", 10, 0);
    EXPECT_TRUE(elf->is_close(far_elf, 10));
    EXPECT_FALSE(elf->is_close(far_elf, 9));

    auto close_elf = std::make_shared<Elf>("CloseElf", 15, 20); // dist = 25
    EXPECT_TRUE(dragon->is_close(close_elf, dragon->kill_radius())); // 25 <= 30
    EXPECT_TRUE(dragon->is_close(far_elf, dragon->kill_radius()));   // 10 <= 30
}

// Проверка move: не выходит за границы и смещает позицию
TEST(NpcMovement, ClampAndShift) {
    Elf e("E", 0, 0);
    e.move(-1000, -1000, 100, 100);
    auto [x1, y1] = e.position();
    EXPECT_EQ(x1, 0);
    EXPECT_EQ(y1, 0);

    e.move(150, 200, 100, 100);
    auto [x2, y2] = e.position();
    EXPECT_EQ(x2, 100);
    EXPECT_EQ(y2, 100);

    e.move(-5, -7, 100, 100);
    auto [x3, y3] = e.position();
    EXPECT_EQ(x3, 95);
    EXPECT_EQ(y3, 93);
}

// Жизненный цикл: is_alive / die
TEST(NpcLifecycle, DieStopsAliveFlag) {
    Dragon g("G", 5, 5);
    EXPECT_TRUE(g.is_alive());
    g.die();
    EXPECT_FALSE(g.is_alive());
}

// Сериализация/десериализация через factory(is)
TEST(Serialization, SaveLoadViaFactory) {
    std::vector<std::shared_ptr<NPC>> src = {
        std::make_shared<Elf>("E1", 1, 2),
        std::make_shared<Dragon>("G1", 3, 4),
        std::make_shared<Druid>("D1", 5, 6),
    };
    std::stringstream ss;
    for (auto &p : src) p->save(ss);

    std::vector<std::shared_ptr<NPC>> dst;
    for (size_t i = 0; i < src.size(); ++i) {
        dst.push_back(factory(ss));
    }
    ASSERT_EQ(dst.size(), src.size());

    EXPECT_EQ(dst[0]->type, ElfType);
    EXPECT_EQ(dst[0]->name, "E1");
    EXPECT_EQ(dst[0]->position(), std::make_pair(1, 2));

    EXPECT_EQ(dst[1]->type, DragonType);
    EXPECT_EQ(dst[1]->name, "G1");
    EXPECT_EQ(dst[1]->position(), std::make_pair(3, 4));

    EXPECT_EQ(dst[2]->type, DruidType);
    EXPECT_EQ(dst[2]->name, "D1");
    EXPECT_EQ(dst[2]->position(), std::make_pair(5, 6));
}

// Обсервер: on_fight вызывается при notify
struct MockObserver : public IFightObserver {
    int calls = 0;
    std::shared_ptr<NPC> last_att;
    std::shared_ptr<NPC> last_def;
    bool last_win = false;
    void on_fight(const std::shared_ptr<NPC>& attacker,
                  const std::shared_ptr<NPC>& defender,
                  bool win) override {
        ++calls;
        last_att = attacker;
        last_def = defender;
        last_win = win;
    }
};

TEST(Observers, FightNotify) {
    auto obs = std::make_shared<MockObserver>();
    auto e = std::make_shared<Elf>("E", 0, 0);
    auto d = std::make_shared<Druid>("D", 1, 1);
    e->subscribe(obs);
    e->fight_notify(d, true);
    EXPECT_EQ(obs->calls, 1);
    EXPECT_EQ(obs->last_att, e);
    EXPECT_EQ(obs->last_def, d);
    EXPECT_TRUE(obs->last_win);
}