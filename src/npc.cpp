#include "npc.hpp"
#include <cmath>
#include <shared_mutex>

NPC::NPC(const std::string &name_, int x_, int y_)
    : name(name_), x(x_), y(y_) {}

void NPC::subscribe(std::shared_ptr<IFightObserver> observer) {
    observers.push_back(observer);
}

void NPC::fight_notify(const std::shared_ptr<NPC>& defender, bool win) {
    for (const auto& o : observers) {
        if (o) {o->on_fight(shared_from_this(), defender, win);}
    }
}

std::pair<int, int> NPC::position() const {
    std::shared_lock<std::shared_mutex> lk(mtx_pos);
    return {x, y};
}

bool NPC::is_close(const std::shared_ptr<NPC> &other, size_t distance) const {
    auto [ox, oy] = other->position(); 
    std::shared_lock<std::shared_mutex> lk(mtx_pos);
    long long dx = static_cast<long long>(x) - ox;
    long long dy = static_cast<long long>(y) - oy;
    return dx * dx + dy * dy <= 1LL * distance * distance;
}

void NPC::move(int dx, int dy, int max_x, int max_y) {
    std::lock_guard<std::shared_mutex> lk(mtx_pos);
    int nx = x + dx;
    int ny = y + dy;
    if (nx < 0) nx = 0;
    if (ny < 0) ny = 0;
    if (nx > max_x) nx = max_x;
    if (ny > max_y) ny = max_y;
    x = nx;
    y = ny;
}

bool NPC::is_alive() const {
    std::shared_lock<std::shared_mutex> lk(mtx_pos);
    return alive;
}

void NPC::die() {
    std::lock_guard<std::shared_mutex> lk(mtx_pos);
    alive = false;
}

void NPC::save(std::ostream &os) const {
    os << name << std::endl;
    os << x << " " << y << std::endl;
}

std::ostream &operator<<(std::ostream &os, const NPC &npc) {
    os << "{ name: " << npc.name
       << ", x: " << npc.x
       << ", y: " << npc.y
       << " }";
    return os;
}