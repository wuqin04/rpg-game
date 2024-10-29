#include <iostream>
#include <string>
#include <cassert>
#include "random.h"

class Creature{
public:
    Creature(std::string_view name, char symbol, int health, int damage, int gold)
        : m_name{ name }, m_symbol{ symbol }, m_health{ health }, m_damage{ damage }, m_gold{ gold }
        {}

    const std::string& getName() const {
        return m_name;
    }

    char getSymbol() const {
        return m_symbol;
    }

    int getHealth() const {
        return m_health;
    }

    int getDamage() const {
        return m_damage;
    }

    int getGold() const {
        return m_gold;
    }

    void reduceHealth(int amount) {
        m_health -= amount;
    }

    bool isDead() {
        return m_health <= 0;
    }

    void addGold(int amount) {
        m_gold += amount;
    }

protected:
    std::string m_name{};
    char m_symbol{};
    int m_health{};
    int m_damage{};
    int m_gold{};
};

class Player: public Creature{
public:
    Player(std::string_view name, char symbol = '@', int health = 20, int damage = 1, int gold = 0)
        : Creature{ name, symbol, health, damage, gold }
        {
            if (m_name == "cheat") 
                m_health = 10000;
        }

    int getLevel() const {
        return m_level;
    }

    void levelUp() {
        m_level++;
        m_damage++;
    }

    bool hasWon() {
        return m_level == 20;
    }

private:
    int m_level{ 1 };
};

class Monster: public Creature{
public:
    enum Type{
        dragon,
        orc,
        slime,
        max_types,
    };
    
    explicit Monster(Type type)
        : Creature{ monsterData[type] }
        {}

    static Monster getRandomMonster() {
        int generateMonster{ Random::get(0, max_types - 1) };

        return Monster{ static_cast<Type>(generateMonster) };
    }

private:
    static inline Creature monsterData[] {
        { "dragon", 'D', 20, 4, 100 },
        { "orc", 'o', 4, 2, 25 },
        { "slime", 's', 1, 1, 10 }
    };

    static_assert(std::size(monsterData) == max_types);
};


void attackPlayer(Monster& monster, Player& player) {
    if (monster.isDead())
        return;

    player.reduceHealth(monster.getDamage());
    std::cout << "The " << monster.getName() << " hit you for " << monster.getDamage() << " damage.\n";
}

void attackMonster(Monster& monster, Player& player) {
    if (player.isDead()) 
        return;

    monster.reduceHealth(player.getDamage());
    std::cout << "You hit the " << monster.getName() << " for " << player.getDamage() << " damage.\n";

    if (monster.isDead()) {
        std::cout << "You killed the " << monster.getName() << ".\n";
        player.levelUp();
        std::cout << "You are now level " << player.getLevel() << ".\n";
        std::cout << "You found " << monster.getGold() << ".\n";
        player.addGold(monster.getGold());
    }
}

void fightMonster(Player& player) {
    Monster monster{ Monster::getRandomMonster() };

    std::cout << "You have encountered a " << monster.getName() << " (s).\n";

    while (!monster.isDead() && !player.isDead()) {
        std::cout << "(R)un or (F)ight: ";
        char input{};
        std::cin >> input;

        if (!std::cin) {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cin.clear();
            continue;
        }

        input = toupper(input);

        if (input == 'R') {

            if (Random::get(1, 2) == 1) {
                std::cout << "You successfully fled.\n";
                return;
            }
            else {
                std::cout << "You failed to flee.\n";
                attackPlayer(monster, player);
                continue;
            }
        }
        
        if (input == 'F') {
            attackMonster(monster, player);
            attackPlayer(monster, player);
        }

    }
}

int main() {
	std::cout << "Enter your name: ";
    std::string name{};
    std::cin >> name;

    Player player{ name };

    std::cout << "Welcome, " << name << ".\n";
    std::cout << "You have " << player.getHealth() << " health and are carrying " << player.getGold() << " gold.\n";

    while (!player.isDead() && !player.hasWon())
        fightMonster(player);

    if (player.isDead()) {
        std::cout << "You died at level " << player.getLevel() << " and with " << player.getGold() << " gold.\n";
        std::cout << "Too bad you can't take it with you! \n";
    }
    else {
        std::cout << "You won the game with " << player.getGold() << " gold!\n";
    }
}