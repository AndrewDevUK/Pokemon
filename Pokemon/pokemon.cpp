#include <iostream>

enum Type {Normal, Fire, Water, Grass};
enum WhosThatPokemon {Charmander, Squirtle, Bulbasaur};
enum MoveType {Damage, AttackUp, DefenseUp, AttackDown, DefenseDown};

const char* TypeToString(Type type)
{
    switch(type)
    {
        case Normal:
            return "Normal";
            break;
        case Fire:
            return "Fire";
            break;
        case Water:
            return "Water";
            break;
        case Grass:
            return "Grass";
            break;
    }
}

const char* MoveTypeToString(MoveType mType)
{
    switch(mType)
    {
        case Damage:
            return "Damage";
            break;
        case AttackUp:
            return "Increase Attack";
            break;
        case DefenseUp:
            return "Increase Defense";
            break;
        case AttackDown:
            return "Decrease Opponant Attack";
            break;
        case DefenseDown:
            return "Decrease Opponant Defense";
            break;
    }
}

struct Stats
{
    int Health;
    int Attack;
    int Defense;
};

struct Move
{
    const char* Name;
    int Power;
    Type type;
    MoveType mType;

    // Default constructor creates the 'Tackle' move.
    Move(const char* name = "Tackle", int power = 15, Type etype  = Normal, MoveType movType = Damage)
            :Name(name), Power(power), type(etype), mType(movType)
    {
    }
};

class Pokemon
{
public:
    Type m_type;
    const char* m_name;
    Move m_Moves[4];
    Stats stats;

    Pokemon(WhosThatPokemon pokemon)
    {
        switch(pokemon)
        {
            case Charmander:
                m_name = "Charmander";
                m_type = Fire;

                stats.Health = 100;
                stats.Attack = 10;
                stats.Defense = 10;

                m_Moves[0] = Move("Scratch", 15, Normal, Damage);
                m_Moves[1] = Move("Ember", 15, Fire, Damage);
                m_Moves[2] = Move("Leer", 0, Normal, DefenseDown);
                m_Moves[3] = Move("Sharpen", 0, Normal, AttackUp);

                break;

            case Squirtle:
                m_name = "Squirtle";
                m_type = Water;

                stats.Health = 100;
                stats.Attack = 10;
                stats.Defense = 10;

                m_Moves[0] = Move(); // Tackle
                m_Moves[1] = Move("Water Gun", 15, Water, Damage);
                m_Moves[2] = Move("Bulk Up", 0, Normal, DefenseUp);
                m_Moves[3] = Move("Leer", 0, Normal, AttackDown);

                break;

            case Bulbasaur:
                m_name = "Bulbasaur";
                m_type = Grass;

                stats.Health = 100;
                stats.Attack = 10;
                stats.Defense = 10;

                m_Moves[0] = Move(); // Tackle
                m_Moves[1] = Move("Vine Whip", 15, Grass, Damage);
                m_Moves[2] = Move("Bulk Up", 0, Normal, DefenseUp);
                m_Moves[3] = Move("Sharpen", 0, Normal, AttackUp);

                break;
        }
    }

    float GetDamageMultiplier(Type target, Move move)
    {
        float multiplier;

        if(m_type == move.type)     // Check for STAB damage multiplier.
            multiplier = 1.5f;
        else
            multiplier = 1.0f;

        // Check for type advantage/disadvantage multiplier.
        if(move.type == Normal)
            multiplier *= 1;
        else if(move.type == target)
        {
            multiplier *= 0.5f;
            std::cout << "It's not very effective!" << std::endl;
        }
        else if(move.type == Fire && target == Water)
        {
            multiplier *= 0.5f;
            std::cout << "It's not very effective!" << std::endl;
        }
        else if(move.type == Fire && target == Grass)
        {
            multiplier *= 2.0f;
            std::cout << "It's super effective!" << std::endl;
        }
        else if(move.type == Water && target == Fire)
        {
            multiplier *= 2.0f;
            std::cout << "It's super effective!" << std::endl;
        }
        else if(move.type == Water && target == Grass)
        {
            multiplier *= 0.5f;
            std::cout << "It's not very effective!" << std::endl;
        }
        else if(move.type == Grass && target == Fire)
        {
            multiplier *= 0.5f;
            std::cout << "It's not very effective!" << std::endl;
        }
        else if(move.type == Grass && target == Water)
        {
            multiplier *= 2.0f;
            std::cout << "It's super effective!" << std::endl;
        }

        return multiplier;
    }

    void Attack(Pokemon& target, Move move)
    {
        std::cout << m_name << " used " << move.Name << "!" << std::endl;

        switch(move.mType)
        {
            int damage;
            float attackMult, defMult;

            case Damage:
                attackMult = stats.Attack / static_cast<float>(10);
                defMult = target.stats.Defense / static_cast<float>(10);

                damage = ((move.Power * GetDamageMultiplier(target.m_type, move)) * attackMult) / defMult;
                target.stats.Health -= damage;

                std::cout << "Dealt " << damage << " damage!" << std::endl;

                if(target.stats.Health < 0)
                    target.stats.Health = 0;
                break;
            
            case AttackUp:
                if (stats.Attack < 18)
                {
                    stats.Attack += 2;
                    std::cout << "It's attack rose!" << std::endl;
                    break;
                }
                else
                {
                    std::cout << "Attack cannot go any higher!" << std::endl;
                    break;
                }
            
            case DefenseUp:
                if (stats.Defense < 18)
                {
                    stats.Defense += 2;
                    std::cout << "It's defense rose!" << std::endl;
                    break;
                }
                else
                {
                    std::cout << "Defense cannot go any higher!" << std::endl;
                    break;
                }

            case AttackDown:
                if (target.stats.Attack > 2)
                {
                    target.stats.Attack -= 2;
                    std::cout << target.m_name << "'s attack fell!" << std::endl;
                    break;
                }
                else
                {
                    std::cout << "Attack cannot go any lower!" << std::endl;
                    break;
                }

            case DefenseDown:
                if (target.stats.Defense > 2)
                {
                    target.stats.Defense -= 2;
                    std::cout << target.m_name << "'s defense fell!" << std::endl;
                    break;
                }
                else
                {
                    std::cout << "Defense cannot go any lower!" << std::endl;
                    break;
                }
        }
    }
};

class Game
{
public:

    Pokemon *player1, *player2;


    bool MainMenu()
    {
        char menuOption;

        system("cls");
        std::cout << "******************************************************" << std::endl;
        std::cout << "               Pokemon Battle Simulator      ver 1.0  " << std::endl;
        std::cout << "Written by Andrew Morgan                  ©Nintendo   " << std::endl;
        std::cout << "******************************************************" << std::endl << std::endl;

        std::cout << "Play? (y/n):";
        std::cin >> menuOption;

        if (menuOption == 'y' || menuOption == 'Y')
            return true;
        else
            return false;
    }

    void DisplayHeader()
    {
        system("cls");
        std::cout << "******************************************************" << std::endl;
        std::cout << player1->m_name << " - " << player1->stats.Health << "HP" << "\t\t" << player2->m_name << " - " << player2->stats.Health << "HP" << std::endl;
        std::cout << "******************************************************" << std::endl << std::endl;

    }

    void ChoosePokemon()
    {
        int p1Choice, p2Choice;

        do
        {              
            system("cls");
            std::cout << "Player 1, choose your pokemon!" << std::endl;
            std::cout << "1, Charmander" << std::endl;
            std::cout << "2, Squirtle" << std::endl;
            std::cout << "3, Bulbasaur" << std::endl;

            std::cin >> p1Choice;
        }while(!(p1Choice == 1 || p1Choice == 2 || p1Choice == 3));

        do
        {              
            system("cls");
            std::cout << "Player 2, choose your pokemon!" << std::endl;
            std::cout << "1, Charmander" << std::endl;
            std::cout << "2, Squirtle" << std::endl;
            std::cout << "3, Bulbasaur" << std::endl;

            std::cin >> p2Choice;
        }while(!(p2Choice == 1 || p2Choice == 2 || p2Choice == 3));

        switch(p1Choice)
        {
            case 1:
                player1 =  new Pokemon(Charmander);
                break;
            case 2:
                player1 = new Pokemon(Squirtle);
                break;
            case 3:
                player1 = new Pokemon(Bulbasaur);
                break;
        }

        switch(p2Choice)
        {
            case 1:
                player2 = new Pokemon(Charmander);
                break;
            case 2:
                player2 = new  Pokemon(Squirtle);
                break;
            case 3:
                player2 = new Pokemon(Bulbasaur);
                break;
        }
    }

    void DisplayMoveInfo(Pokemon& pokemon)
    {
        std::cout << "1, " << pokemon.m_Moves[0].Name << " \t | " << pokemon.m_Moves[0].Power << " \t | " << MoveTypeToString(pokemon.m_Moves[0].mType) << " \t | " << TypeToString(pokemon.m_Moves[0].type) << std::endl;
        std::cout << "2, " << pokemon.m_Moves[1].Name << " \t | " << pokemon.m_Moves[1].Power << " \t | " << MoveTypeToString(pokemon.m_Moves[1].mType) << " \t | " << TypeToString(pokemon.m_Moves[1].type) << std::endl;
        std::cout << "3, " << pokemon.m_Moves[2].Name << " \t | " << pokemon.m_Moves[2].Power << " \t | " << MoveTypeToString(pokemon.m_Moves[2].mType) << " \t | " << TypeToString(pokemon.m_Moves[2].type) << std::endl;
        std::cout << "4, " << pokemon.m_Moves[3].Name << " \t | " << pokemon.m_Moves[3].Power << " \t | " << MoveTypeToString(pokemon.m_Moves[3].mType) << " \t | " << TypeToString(pokemon.m_Moves[3].type) << std::endl;
    }

    void RunBattle()
    {
        int p1Choice, p2Choice;

        std::cout << "Prepare for Battle!" << std::endl;
        std::cin.get();

        while(player1->stats.Health > 0 && player2->stats.Health > 0)
        {
            do 
            {
                system("cls");
                DisplayHeader();
                std::cout << player1->m_name << "'s Turn!" << std::endl << std::endl;
                DisplayMoveInfo(*player1);
                std::cout << ">";

                std::cin >> p1Choice;
                
            }while(!(p1Choice == 1 || p1Choice == 2 || p1Choice == 3 || p1Choice == 4));

            player1->Attack(*player2, player1->m_Moves[p1Choice -1]);
            std::cin.ignore();
            std::cin.get();
            
            if(player2->stats.Health == 0)
                break;

            do 
            {
                system("cls");
                DisplayHeader();
                std::cout << player2->m_name << "'s Turn!" << std::endl << std::endl;
                DisplayMoveInfo(*player2);
                std::cout << ">";

                std::cin >> p2Choice;
                
            }while(!(p2Choice == 1 || p2Choice == 2 || p2Choice == 3 || p2Choice == 4));

            player2->Attack(*player1, player2->m_Moves[p2Choice -1]);
            std::cin.ignore();
            std::cin.get();

            if(player1->stats.Health == 0)
                break;
        }

        if(player1->stats.Health == 0)
        {
            system("cls");
            DisplayHeader();
            std::cout << player2->m_name << " won!" << std::endl;
            std::cin.get();
        }
        else if(player2->stats.Health == 0)
        {
            system("cls");
            DisplayHeader();
            std::cout << player1->m_name << " won!" << std::endl;
            std::cin.get();
        }
    }
};

int main()
{
    Game gameObj;

    while(1)
    {
        if(!gameObj.MainMenu())
            break;
        
        gameObj.ChoosePokemon();
        gameObj.RunBattle();
    }
}
