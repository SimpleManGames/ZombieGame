#include "Dungeon.h"

namespace Dungeon {

	Dungeon::Dungeon(int width, int height) :
		_width(width),
		_height(height),
		_tiles(width * height, Unused),
		_rooms(),
		_exits() {
		save("Levels/level.txt");
	}

	void Dungeon::generate(int maxFeatures) {
		// Places the first room in the middle
		if (!makeRoom(_width / 2, _height / 2, static_cast<Direction>(c_Random::randomInt(4), true))) {
			std::cout << "Unable to place the first room." << std::endl;
			return;
		}
		// Place the other features
		for (int i = 1; i < maxFeatures; ++i) {
			if (!createFeature()) {
				std::cout << "Unable to place more features (placed" << i << ")." << std::endl;
				break;
			}
		}
		if (!placeObject('@')) {
			std::cout << "Unable to place player" << std::endl;
			return;
		}
		if (!placeObject('Z')) {
			std::cout << "Unable to place zombie" << std::endl;
			return;
		}
		if (!placeObject('R')) {
			std::cout << "Unable to place rifle" << std::endl;
			return;
		}
		if (!placeObject('S')) {
			std::cout << "Unable to place shotgun" << std::endl;
			return;
		}
		for (int i = 0; i < 20; i++)
			if (!placeObject('A')) {
				std::cout << "Unable to place ammo" << std::endl;
				return;
		}
		if (!placeObject(UpStairs)) {
			std::cout << "Unable to place up stairs" << std::endl;
			return;
		}
		if (!placeObject(DownStairs)) {
			std::cout << "Unable to place down stairs" << std::endl;
			return;
		}
		for (char& tile : _tiles) {
			if (tile == Unused) {
				tile = ' ';
			}
			else if (tile == Floor || tile == Corridor) {
				tile = '.';
			}
		}
	}

	void Dungeon::print() {
		for (int y = 0; y < _height; ++y) {
			for (int x = 0; x < _width; ++x) {
				std::cout << getTile(x, y);
			}
			std::cout << std::endl;
		}
	}

	void Dungeon::save(std::string filePath) {
		std::ofstream output_File(filePath.c_str());
		if (output_File.fail()) {
			std::cout << "Failed to open file." << std::endl;
			exit(1);
		}
		// OUTPUT SIZE OF MAP TO LOAD
		
		//output_File.write(static_cast<char *>(&(_tiles[0])), _tiles.size() * sizeof(char));
	
		/*int widthCount = 0;
		for (std::vector<char>::const_iterator i = _tiles.begin(); i != _tiles.end(); ++i) {
			output_File << *i;
			if (widthCount == width) {
				output_File << "\n";
				widthCount = 0;
			}
			else
				widthCount++;
		}*/
		for (int y = 0; y < _height; ++y) {
			for (int x = 0; x < _width; ++x) {
				output_File << getTile(x, y);
			}
			output_File << '\n';
		}
	}

	char Dungeon::getTile(int x, int y) const {
		if (x < 0 || y < 0 || x >= _width || y >= _height) {
			return Unused;
		}
		return _tiles[x + y * _width];
	}

	void Dungeon::setTile(int x, int y, char tile) {
		_tiles[x + y * _width] = tile;
	}

	bool Dungeon::createFeature() {
		for (int i = 0; i < 1000; ++i) {
			if (_exits.empty()) {
				break;
			}
			// Picks a random side of a random room/corridor
			int r = c_Random::randomInt(_exits.size());
			int x = c_Random::randomInt(_exits[r].x, _exits[r].x + _exits[r].width - 1);
			int y = c_Random::randomInt(_exits[r].y, _exits[r].y + _exits[r].height - 1);

			// North, South, West, East
			for (int j = 0; j < DirectionCount; ++j) {
				if (createFeature(x, y, static_cast<Direction>(j))) {
					_exits.erase(_exits.begin() + r);
					return true;
				}
			}
		}
		return false;
	}

	bool Dungeon::createFeature(int x, int y, Direction dir) {
		static const int roomChance = 50;

		int dx = 0;
		int dy = 0;

		if (dir == North) { dy = 1; }
		else if (dir == South) { dy = -1; }
		else if (dir == West) { dx = 1; }
		else if (dir == East) { dx = -1; }

		if (getTile(x + dx, y + dy) != Floor && getTile(x + dx, y + dy) != Corridor) {
			return false;
		}
		if (c_Random::randomInt(100) < roomChance) {
			if (makeRoom(x, y, dir)) {
				setTile(x, y, ClosedDoor);
				return true;
			}
		}
		else {
			if (makeCorridor(x, y, dir)) {
				if (getTile(x + dx, y + dy) == Floor) {
					setTile(x, y, ClosedDoor);
				}
				else {
					setTile(x, y, Corridor);
				}
				return true;
			}
		}
		return false;
	}

	bool Dungeon::makeRoom(int x, int y, Direction dir, bool firstRoom) {
		static const int minRoomSize = 3;
		static const int maxRoomSIze = 7;

		Rect room;
		room.width = c_Random::randomInt(minRoomSize, maxRoomSIze);
		room.height = c_Random::randomInt(minRoomSize, maxRoomSIze);

		if (dir == North) {
			room.x = x - room.width / 2;
			room.y = y - room.height;
		}
		else if (dir == South) {
			room.x = x - room.width / 2;
			room.y = y + 1;
		}
		else if (dir == West) {
			room.x = x - room.width;
			room.y = y - room.height / 2;
		}
		else if (dir == East) {
			room.x = x + 1;
			room.y = y - room.height / 2;
		}
		if (placeRect(room, Floor)) {
			_rooms.emplace_back(room);

			if (dir != South || firstRoom) _exits.emplace_back(Rect{ room.x, room.y - 1, room.width, 1 });
			if (dir != North || firstRoom) _exits.emplace_back(Rect{ room.x, room.y + room.height, room.width, 1 });
			if (dir != East || firstRoom) _exits.emplace_back(Rect{ room.x - 1, room.y, 1, room.height });
			if (dir != West || firstRoom) _exits.emplace_back(Rect{ room.x + room.width, room.y, 1, room.height });

			return true;
		}
		return false;
	}

	bool Dungeon::makeCorridor(int x, int y, Direction dir) {
		static const int minCorridorLength = 3;
		static const int maxCorridorLength = 6;

		Rect corridor;
		corridor.x = x;
		corridor.y = y;

		if (c_Random::randomBool()) {
			corridor.width = c_Random::randomInt(minCorridorLength, maxCorridorLength);
			corridor.height = 1;

			if (dir == North) {
				corridor.y = y - 1;
				if (c_Random::randomBool()) {
					corridor.x = x - corridor.width + 1;
				}
			}
			else if (dir == South) {
				corridor.y = y - 1;
				if (c_Random::randomBool()) {
					corridor.x = x - corridor.width + 1;
				}
			}
			else if (dir == West) {
				corridor.x = x - corridor.width;
			}
			else if (dir == East) {
				corridor.x = x + 1;
			}
		}
		else {
			corridor.width = 1;
			corridor.height = c_Random::randomInt(minCorridorLength, maxCorridorLength);

			if (dir == North) {
				corridor.y = y - corridor.height;
			}
			else if (dir == South) {
				corridor.y = y + 1;
			}
			else if (dir == West) {
				corridor.y = y + 1;
				if (c_Random::randomBool()) {
					corridor.y = y - corridor.height + 1;
				}
			}
			else if (dir == East) {
				corridor.x = x + 1;
				if (c_Random::randomBool()) {
					corridor.y = y - corridor.height + 1;
				}
			}
		}
		if (placeRect(corridor, Corridor)) {
			if (dir != South && corridor.width != 1) _exits.emplace_back(Rect{ corridor.x, corridor.y - 1, corridor.width, 1 });
			if (dir != North && corridor.width != 1) _exits.emplace_back(Rect{ corridor.x, corridor.y + corridor.height, corridor.width, 1 });
			if (dir != East && corridor.height != 1) _exits.emplace_back(Rect{ corridor.x - 1, corridor.y, 1, corridor.height });
			if (dir != West && corridor.height != 1) _exits.emplace_back(Rect{ corridor.x + corridor.width, corridor.y, 1, corridor.height });

			return true;
		}
		return false;
	}

	bool Dungeon::placeRect(const Rect & rect, char tile) {
		if (rect.x < 1 || rect.y < 1 || rect.x + rect.width > _width - 1 || rect.y + rect.height > _height - 1)
			return false;

		for (int y = rect.y; y < rect.y + rect.height; ++y)
			for (int x = rect.x; x < rect.x + rect.width; ++x) {
				if (getTile(x, y) != Unused) return false; // the area already used
			}

		for (int y = rect.y - 1; y < rect.y + rect.height + 1; ++y)
			for (int x = rect.x - 1; x < rect.x + rect.width + 1; ++x) {
				if (x == rect.x - 1 || y == rect.y - 1 || x == rect.x + rect.width || y == rect.y + rect.height) setTile(x, y, Wall);
				else setTile(x, y, tile);
			}

		return true;
	}

	bool Dungeon::placeObject(char tile) {
		if (_rooms.empty())
			return false;

		int r = c_Random::randomInt(_rooms.size()); // choose a random room
		int x = c_Random::randomInt(_rooms[r].x + 1, _rooms[r].x + _rooms[r].width - 2);
		int y = c_Random::randomInt(_rooms[r].y + 1, _rooms[r].y + _rooms[r].height - 2);

		if (getTile(x, y) == Floor) {
			setTile(x, y, tile);
			// place one object in one room (optional)
			_rooms.erase(_rooms.begin() + r);
			return true;
		}
		return false;
	}
}