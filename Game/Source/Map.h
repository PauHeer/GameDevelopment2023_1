#ifndef __MAP_H__
#define __MAP_H__

#include "Module.h"
#include "List.h"
#include "Point.h"
#include "Pathfinding.h"

#include "PugiXml\src\pugixml.hpp"

// Ignore Terrain Types and Tile Types for now, but we want the image!
struct TileSet
{
	SString	name;
	int	firstgid;
	int margin;
	int	spacing;
	int	tileWidth;
	int	tileHeight;
	int columns;
	int tilecount;

	SDL_Texture* texture;
	SDL_Rect GetTileRect(int gid) const;
};

//  We create an enum for map type, just for convenience,
// NOTE: Platformer game will be of type ORTHOGONAL
enum MapTypes
{
	MAPTYPE_UNKNOWN = 0,
	MAPTYPE_ORTHOGONAL,
	MAPTYPE_ISOMETRIC,
	MAPTYPE_STAGGERED
};

struct Properties
{
	struct Property
	{
		SString name;
		bool value;
	};

	~Properties()
	{
		//...
		ListItem<Property*>* item;
		item = list.start;

		while (item != NULL)
		{
			RELEASE(item->data);
			item = item->next;
		}

		list.Clear();
	}

	Property* GetProperty(const char* name);

	List<Property*> list;
};

struct MapLayer
{
	SString	name;
	int id; 
	int width;
	int height;
	uint* data;

	Properties properties;

	MapLayer() : data(NULL)
	{}

	~MapLayer()
	{
		RELEASE(data);
	}

	inline uint Get(int x, int y) const
	{
		return data[(y * width) + x];
	}
};

struct ObjectLayer
{
	SString	name;
	int id;
	int x;
	int y;
	int width;
	int height;
	uint* data;

	Properties properties;

	ObjectLayer() : data(NULL)
	{}

	~ObjectLayer()
	{
		RELEASE(data);
	}
};

struct MapData
{
	int x;
	int y;
	int width;
	int	height;
	int	tileWidth;
	int	tileHeight;
	List<TileSet*> tilesets;
	MapTypes type;

	List<MapLayer*> maplayers;
	List<ObjectLayer*> objectlayers;
};

class Map : public Module
{
public:

    Map();

    // Destructor
    virtual ~Map();

    // Called before render is available
    bool Awake(pugi::xml_node& conf);

	// Called before the first frame
	bool Start();

	// Called each loop iteration
	bool Update(float dt);

    // Called before quitting
    bool CleanUp();

    // Load new map
    bool Load();

	void CreateNavigationMap(int& width, int& height, uchar** buffer) const;

	bool isDoorColliderActive = true;

	

	iPoint MapToWorld(int x, int y) const;
	iPoint Map::WorldToMap(int x, int y);
	PhysBody* door;


private:

	bool LoadMap(pugi::xml_node mapFile);
	bool LoadTileSet(pugi::xml_node mapFile);
	bool LoadLayer(pugi::xml_node& node, MapLayer* layer);
	bool LoadGroup(pugi::xml_node& node, ObjectLayer* object);
	bool LoadAllLayers(pugi::xml_node mapNode);
	bool LoadAllGroups(pugi::xml_node mapNode);
	TileSet* GetTilesetFromTileId(int gid) const;
	bool LoadProperties(pugi::xml_node& node, Properties& properties);

public: 

	MapData mapData;
	PathFinding* pathfinding;

private:

    SString mapFileName;
	SString mapFolder;
	MapLayer* navigationLayer;
    bool mapLoaded;
	bool firstRender = false;
	int blockedGid = 1;
};

#endif // __MAP_H__