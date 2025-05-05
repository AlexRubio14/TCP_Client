#pragma once
#include "Cell.h"

#define NUM_CELLS 80
#define WHITE_CELLS 44
#define COLORED_CELLS_PER_COLOR 9
#define SAVE_ZONE 4
#define SEPARATION_BETWEEN_SAVE_ZONE 10
#define SPAWN_BLUE 49

class Map
{
private:
	std::vector<std::shared_ptr<Cell>> cells;

	//Map
	std::vector<sf::RectangleShape> squares;
	std::vector<sf::CircleShape> bases;
	std::vector<std::shared_ptr<sf::Text>> names;
	sf::Font font;
	
	sf::Color SetCellColor(int _id);
	void CreateCells();
	void SetNextCells();
	void CreateBases();
	void SetClientsName();
	void CreateBoard(sf::RenderWindow& window);
	void Render(sf::RenderWindow& window);
	void SetCellsPosition(sf::RenderWindow& window);
	void SetWhiteCellsPosition(sf::Vector2f& currentPosition);
	void SetBlueCells(sf::RenderWindow& window, sf::Vector2f& currentPosition);
	void SetYellowCells(sf::RenderWindow& window, sf::Vector2f& currentPosition);
	void SetRedCells(sf::RenderWindow& window, sf::Vector2f& currentPosition);
	void SetGreenCells(sf::RenderWindow& window, sf::Vector2f& currentPosition);
	void SetHomePositions(sf::Vector2f& currentPosition, int initId, int baseId);

public:
	Map(sf::RenderWindow& window);
	~Map();

	void Update(sf::RenderWindow& window);
	
	inline std::vector<std::shared_ptr<Cell>> GetCells() const { return cells; }
	inline void SetName(const int id, const std::string name) { names[id]->setString(name); }
};

