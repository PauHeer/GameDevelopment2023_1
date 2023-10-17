
#include "App.h"
#include "Render.h"
#include "Textures.h"
#include "Map.h"
#include "Physics.h"

#include "Defs.h"
#include "Log.h"

#include <math.h>
#include "SDL_image/include/SDL_image.h"

Map::Map() : Module(), mapLoaded(false)
{
    name.Create("map");
}

// Destructor
Map::~Map()
{}

// Called before render is available
bool Map::Awake(pugi::xml_node& config)
{
    LOG("Loading Map Parser");
    bool ret = true;

    mapFileName = config.child("mapfile").attribute("path").as_string();
    mapFolder = config.child("mapfolder").attribute("path").as_string();

    return ret;
}

bool Map::Update(float dt)
{
    if(mapLoaded == false)
        return false;

    ListItem<MapLayer*>* mapLayerItem;
    mapLayerItem = mapData.maplayers.start;

    while (mapLayerItem != NULL) {

        if (mapLayerItem->data->properties.GetProperty("Draw") != NULL && mapLayerItem->data->properties.GetProperty("Draw")->value) {

            for (int x = 0; x < mapLayerItem->data->width; x++)
            {
                for (int y = 0; y < mapLayerItem->data->height; y++)
                {
                    int gid = mapLayerItem->data->Get(x, y);
                    TileSet* tileset = GetTilesetFromTileId(gid);

                    SDL_Rect r = tileset->GetTileRect(gid);
                    iPoint pos = MapToWorld(x, y);

                    app->render->DrawTexture(tileset->texture,
                        pos.x,
                        pos.y,
                        &r);
                }
            }
        }
        mapLayerItem = mapLayerItem->next;

    }

    return true;
}

iPoint Map::MapToWorld(int x, int y) const
{
    iPoint ret;

    ret.x = x * mapData.tileWidth;
    ret.y = y * mapData.tileHeight;

    return ret;
}

iPoint Map::WorldToMap(int x, int y) 
{
    iPoint ret(0, 0);

    //

    return ret;
}

// Get relative Tile rectangle
SDL_Rect TileSet::GetTileRect(int gid) const
{
    SDL_Rect rect = { 0 };
    int relativeIndex = gid - firstgid;

    rect.w = tileWidth;
    rect.h = tileHeight;
    rect.x = margin + (tileWidth + spacing) * (relativeIndex % columns);
    rect.y = margin + (tileWidth + spacing) * (relativeIndex / columns);

    return rect;
}

TileSet* Map::GetTilesetFromTileId(int gid) const
{
    ListItem<TileSet*>* item = mapData.tilesets.start;
    TileSet* set = NULL;

    while (item)
    {
        set = item->data;
        if (gid < (item->data->firstgid + item->data->tilecount))
        {
            break;
        }
        item = item->next;
    }

    return set;
}

// Called before quitting
bool Map::CleanUp()
{
    LOG("Unloading map");

	ListItem<TileSet*>* item;
	item = mapData.tilesets.start;

	while (item != NULL)
	{
		RELEASE(item->data);
		item = item->next;
	}
	mapData.tilesets.Clear();

    // Remove all layers
    ListItem<MapLayer*>* layerItem;
    layerItem = mapData.maplayers.start;

    while (layerItem != NULL)
    {
        RELEASE(layerItem->data);
        layerItem = layerItem->next;
    }

    return true;
}

// Load new map
bool Map::Load()
{
    bool ret = true;

    pugi::xml_document mapFileXML;
    pugi::xml_parse_result result = mapFileXML.load_file(mapFileName.GetString());

    if(result == NULL)
    {
        LOG("Could not load map xml file %s. pugi error: %s", mapFileName, result.description());
        ret = false;
    }

    if(ret == true)
    {
        ret = LoadMap(mapFileXML);
    }

    if (ret == true)
    {
        ret = LoadTileSet(mapFileXML);
    }

    if (ret == true)
    {
        ret = LoadAllLayers(mapFileXML.child("map"));
    }
    
    // NOTE: Later you have to create a function here to load and create the colliders from the map



    //COLLIDERS CREATIONS

    
    //Platforms

    PhysBody* ptf1_ptf = app->physics->CreateRectangle(224 + 128, 544 + 8, 254, 16, STATIC, ColliderType::PLATFORM);
    ptf1_ptf->ctype = ColliderType::PLATFORM;
    PhysBody* ptf1 = app->physics->CreateRectangle(224 + 128, 544 + 16, 256, 32, STATIC, ColliderType::WALL);
    ptf1->ctype = ColliderType::WALL;

    PhysBody* ptf2_ptf = app->physics->CreateRectangle(928 + 16, 576 + 8, 222, 16, STATIC, ColliderType::PLATFORM);
    ptf2_ptf->ctype = ColliderType::PLATFORM;
    PhysBody* ptf2 = app->physics->CreateRectangle(928 + 16, 576 + 16, 224, 32, STATIC, ColliderType::WALL);
    ptf2->ctype = ColliderType::WALL;

    PhysBody* ptf3_ptf = app->physics->CreateRectangle(864 - 16, 256 - 24, 30, 16, STATIC, ColliderType::PLATFORM);
    ptf3_ptf->ctype = ColliderType::PLATFORM;
    PhysBody* ptf3 = app->physics->CreateRectangle(864 - 16, 256 - 16, 32, 32, STATIC, ColliderType::WALL);
    ptf3->ctype = ColliderType::WALL;

    PhysBody* ptf4_ptf = app->physics->CreateRectangle(832, 480 - 24, 254, 16, STATIC, ColliderType::PLATFORM);
    ptf4_ptf->ctype = ColliderType::PLATFORM;
    PhysBody* ptf4 = app->physics->CreateRectangle(832, 480 - 16, 256, 32, STATIC, ColliderType::WALL);
    ptf4->ctype = ColliderType::WALL;

    


    //Islands

    PhysBody* isl1_ptf = app->physics->CreateRectangle(384 + 16, 384 + 8, 94, 16, STATIC, ColliderType::PLATFORM);
    isl1_ptf->ctype = ColliderType::PLATFORM;
    PhysBody* isl1_1 = app->physics->CreateRectangle(384 + 16, 384 + 16, 96, 32, STATIC, ColliderType::WALL);
    isl1_1->ctype = ColliderType::WALL;
    PhysBody* isl1_2 = app->physics->CreateRectangle(384 + 16, 384 + 32, 32, 64, STATIC, ColliderType::WALL);
    isl1_2->ctype = ColliderType::WALL;
    PhysBody* circIsl1_1 = app->physics->CreateCircle(384, 416, 32, bodyType::STATIC);
    circIsl1_1->ctype = ColliderType::WALL;
    PhysBody* circIsl1_2 = app->physics->CreateCircle(416, 416, 32, bodyType::STATIC);
    circIsl1_2->ctype = ColliderType::WALL;

    PhysBody* isl2_ptf = app->physics->CreateRectangle(640 - 16, 360 - 32, 96, 16, STATIC, ColliderType::PLATFORM);
    isl2_ptf->ctype = ColliderType::PLATFORM;
    PhysBody* isl2_1 = app->physics->CreateRectangle(640 - 16, 368 - 32, 96, 32, STATIC, ColliderType::WALL);
    isl2_1->ctype = ColliderType::WALL;
    PhysBody* isl2_2 = app->physics->CreateRectangle(640 - 16, 384 - 32, 32, 64, STATIC, ColliderType::WALL);
    isl2_2->ctype = ColliderType::WALL;
    PhysBody* circIsl2_1 = app->physics->CreateCircle(608, 384 - 32, 32, bodyType::STATIC);
    circIsl2_1->ctype = ColliderType::WALL;
    PhysBody* circIsl2_2 = app->physics->CreateCircle(608 + 33, 384 - 32, 32, bodyType::STATIC);
    circIsl2_2->ctype = ColliderType::WALL;


    //Towers

    PhysBody* twr1_ptf = app->physics->CreateRectangle(128 - 16, 448 - 24, 94, 16, STATIC, ColliderType::PLATFORM);
    twr1_ptf->ctype = ColliderType::PLATFORM;
    PhysBody* twr1 = app->physics->CreateRectangle(128 - 16, 448 - 16, 96, 32, STATIC, ColliderType::WALL);
    twr1->ctype = ColliderType::WALL;


    //Walls

    PhysBody* w1 = app->physics->CreateRectangle(32, 384, 64, 2000, STATIC, ColliderType::WALL);
    w1->ctype = ColliderType::WALL;
    PhysBody* w2 = app->physics->CreateRectangle(2016, 384, 64, 2000, STATIC, ColliderType::WALL);
    w2->ctype = ColliderType::WALL;


    //Floor

    PhysBody* flr1_ptf = app->physics->CreateRectangle(256 + 16, 704 + 8, 542, 16, STATIC, ColliderType::PLATFORM);
    flr1_ptf->ctype = ColliderType::PLATFORM;
    PhysBody* flr1 = app->physics->CreateRectangle(256 + 16, 704 + 32, 544, 64, STATIC, ColliderType::WALL);
    flr1->ctype = ColliderType::WALL;
    
    PhysBody* flr2_ptf = app->physics->CreateRectangle(864, 704 + 8, 446, 16, STATIC, ColliderType::PLATFORM);
    flr2_ptf->ctype = ColliderType::PLATFORM;
    PhysBody* flr2 = app->physics->CreateRectangle(864, 704 + 32, 448, 64, STATIC, ColliderType::WALL);
    flr2->ctype = ColliderType::WALL;

    PhysBody* flr3_ptf = app->physics->CreateRectangle(1632, 704 + 8, 830, 16, STATIC, ColliderType::PLATFORM);
    flr3_ptf->ctype = ColliderType::PLATFORM;
    PhysBody* flr3 = app->physics->CreateRectangle(1632, 704 + 32, 832, 64, STATIC, ColliderType::WALL);
    flr3->ctype = ColliderType::WALL;

    PhysBody* roof = app->physics->CreateRectangle(1024, -500, 2048, 64, STATIC, ColliderType::WALL);
    roof->ctype = ColliderType::WALL;
    
    if(ret == true)
    {
        LOG("Successfully parsed map XML file :%s", mapFileName.GetString());
        LOG("width : %d height : %d",mapData.width,mapData.height);
        LOG("tile_width : %d tile_height : %d",mapData.tileWidth, mapData.tileHeight);
        
        LOG("Tilesets----");

        ListItem<TileSet*>* tileset;
        tileset = mapData.tilesets.start;

        while (tileset != NULL) {
            LOG("name : %s firstgid : %d",tileset->data->name.GetString(), tileset->data->firstgid);
            LOG("tile width : %d tile height : %d", tileset->data->tileWidth, tileset->data->tileHeight);
            LOG("spacing : %d margin : %d", tileset->data->spacing, tileset->data->margin);
            tileset = tileset->next;
        }

        ListItem<MapLayer*>* mapLayer;
        mapLayer = mapData.maplayers.start;

        while (mapLayer != NULL) {
            LOG("id : %d name : %s", mapLayer->data->id, mapLayer->data->name.GetString());
            LOG("Layer width : %d Layer height : %d", mapLayer->data->width, mapLayer->data->height);
            mapLayer = mapLayer->next;
        }
    }

    if(mapFileXML) mapFileXML.reset();

    mapLoaded = ret;

    return ret;
}

bool Map::LoadMap(pugi::xml_node mapFile)
{
    bool ret = true;
    pugi::xml_node map = mapFile.child("map");

    if (map == NULL)
    {
        LOG("Error parsing map xml file: Cannot find 'map' tag.");
        ret = false;
    }
    else
    {
        //Load map general properties
        mapData.height = map.attribute("height").as_int();
        mapData.width = map.attribute("width").as_int();
        mapData.tileHeight = map.attribute("tileheight").as_int();
        mapData.tileWidth = map.attribute("tilewidth").as_int();
        mapData.type = MAPTYPE_UNKNOWN;
    }

    return ret;
}

bool Map::LoadTileSet(pugi::xml_node mapFile){

    bool ret = true; 

    pugi::xml_node tileset;
    for (tileset = mapFile.child("map").child("tileset"); tileset && ret; tileset = tileset.next_sibling("tileset"))
    {
        TileSet* set = new TileSet();

        set->name = tileset.attribute("name").as_string();
        set->firstgid = tileset.attribute("firstgid").as_int();
        set->margin = tileset.attribute("margin").as_int();
        set->spacing = tileset.attribute("spacing").as_int();
        set->tileWidth = tileset.attribute("tilewidth").as_int();
        set->tileHeight = tileset.attribute("tileheight").as_int();
        set->columns = tileset.attribute("columns").as_int();
        set->tilecount = tileset.attribute("tilecount").as_int();

        SString tmp("%s%s", mapFolder.GetString(), tileset.child("image").attribute("source").as_string());
        set->texture = app->tex->Load(tmp.GetString());

        mapData.tilesets.Add(set);
    }

    return ret;
}

bool Map::LoadLayer(pugi::xml_node& node, MapLayer* layer)
{
    bool ret = true;

    //Load the attributes
    layer->id = node.attribute("id").as_int();
    layer->name = node.attribute("name").as_string();
    layer->width = node.attribute("width").as_int();
    layer->height = node.attribute("height").as_int();

    LoadProperties(node, layer->properties);

    //Reserve the memory for the data 
    layer->data = new uint[layer->width * layer->height];
    memset(layer->data, 0, layer->width * layer->height);

    //Iterate over all the tiles and assign the values
    pugi::xml_node tile;
    int i = 0;
    for (tile = node.child("data").child("tile"); tile && ret; tile = tile.next_sibling("tile"))
    {
        layer->data[i] = tile.attribute("gid").as_int();
        i++;
    }

    return ret;
}

bool Map::LoadAllLayers(pugi::xml_node mapNode) {
    bool ret = true;

    for (pugi::xml_node layerNode = mapNode.child("layer"); layerNode && ret; layerNode = layerNode.next_sibling("layer"))
    {
        //Load the layer
        MapLayer* mapLayer = new MapLayer();
        ret = LoadLayer(layerNode, mapLayer);

        //add the layer to the map
        mapData.maplayers.Add(mapLayer);
    }

    return ret;
}

bool Map::LoadProperties(pugi::xml_node& node, Properties& properties)
{
    bool ret = false;

    for (pugi::xml_node propertieNode = node.child("properties").child("property"); propertieNode; propertieNode = propertieNode.next_sibling("property"))
    {
        Properties::Property* p = new Properties::Property();
        p->name = propertieNode.attribute("name").as_string();
        p->value = propertieNode.attribute("value").as_bool(); // (!!) I'm assuming that all values are bool !!

        properties.list.Add(p);
    }

    return ret;
}

Properties::Property* Properties::GetProperty(const char* name)
{
    ListItem<Property*>* item = list.start;
    Property* p = NULL;

    while (item)
    {
        if (item->data->name == name) {
            p = item->data;
            break;
        }
        item = item->next;
    }

    return p;
}


