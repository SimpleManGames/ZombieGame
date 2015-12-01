#ifndef _DUNGEON_H
#define _DUNGEON_H

#ifndef _VECTOR_
#include <vector>
#endif // !_VECTOR_

#include <iostream>
#include <fstream>
#include <random>

namespace c_Random {
	static std::random_device rd;
	static std::mt19937 mt(rd());

	static int randomInt(int max) {
		std::uniform_int_distribution<> dist(0, max - 1);
		return dist(mt);
	}

	static int randomInt(int min, int max) {
		std::uniform_int_distribution<> dist(0, max - min);
		return dist(mt) + min;
	}

	static bool randomBool(double probability = 0.5) {
		 std::bernoulli_distribution dist(probability);
		 return dist(c_Random::mt);
	}
}

namespace Dungeon {

	struct Rect {
		int x, y;
		int width, height;
	};

	class Dungeon
	{
	public: // Public Enums
		enum Tile {
			Unused = ' ',
			Floor = '.',
			Corridor = ',',
			Wall = '#',
			ClosedDoor = '+',
			OpenDoor = '-',
			UpStairs = '<',
			DownStairs = '>'
		};
		enum Direction {
			North,
			South,
			West,
			East,
			DirectionCount
		};

	public: // Public construtors and functions
		Dungeon() :
			_width(79),
			_height(24),
			_tiles(79 * 24, Unused),
			_rooms(),
			_exits() { }
		Dungeon(int width, int height);

		void generate(int maxFeatures);
		void print();
		void save(std::string filePath);
		std::vector<char> load(std::string fileName); // TO DO: IMPLEMENT 
		std::vector<std::string> getTileString() {
			//std::vector<std::string> returnV(_tiles.begin(), _tiles.end());
			std::vector<std::string> returnV;
			returnV.resize(_tiles.size());
			std::copy(_tiles.begin(), _tiles.end(), returnV.begin());
			return returnV;
		}


	private: // Private Functions
		char getTile(int x, int y) const;
		void setTile(int x, int y, char tile);
		bool createFeature();
		bool createFeature(int x, int y, Direction dir);
		bool makeRoom(int x, int y, Direction dir, bool firstRoom = false);
		bool makeCorridor(int x, int y, Direction dir);
		bool placeRect(const Rect& rect, char tile);
		bool placeObject(char tile);

	private: // Private Varibles
		int _width, _height;
		std::vector<char> _tiles;
		std::vector<Rect> _rooms; // Rooms for place stairs or enemies
		std::vector<Rect> _exits; // 4 sides of the rooms/corridors
	};
}
#endif // !_DUNGEON_H
