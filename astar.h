
#pragma once

#include <cmath>
#include <deque>
#include <iostream>
#include <map>
#include <set>
#include <vector>

////////////////////////////////////////////////////////////////////////////////
//
// A dynamic A* algorithm custom customized for blueprint the game.
//
// First of all the algorithm will support A* in the normal way.
// This includes doing the following:
//
// 1. Add the starting square to the open list.
// 2. Repeat:
// a) Look for the lowest F cost square on the open list.
//    Refer to this as the current square.
// b) Switch it to the closed list.
// c) For each of the 8 squares adjacent to this current square:
//    If it is not walkable or if it is on the closed list, ignore it.
// Otherwise do the following:
// If it isn t on the open list, add it to the open list.
// Make the current square the parent of this square.
// Record the F, G, and H costs of the square.
// If it is on the open list already, check to see if this path to that square
// is better, using G cost as the measure.
// A lower G cost means that this is a better path.
// If so, change the parent of the square to the current square and recalculate
// the G and F scores of the square.
// If you are keeping your open list sorted by F score, you may need to
// resort the list to account for the change.
// d) Stop when you:
// Add the target square to the closed list, in which case the path has
// been found (see note below), or Fail to find the target square and
//  the open list is empty. In this case, there is no path.
//
////////////////////////////////////////////////////////////////////////
namespace star
{
#ifdef _DEBUG
#define DEBUG(x)        \
    do                  \
    {                   \
        std::cerr << x; \
    } while (0)
#else
#define DEBUG(x)
#endif

using LayerID = int;
using SubLayer = int;
using WeightValue = int;
using WeightValues = WeightValue*;
using Position = int;
using FScore = int;

enum MAIN_LAYERS
{
    M_LAYER_NONE,
    M_LAYER_RUBBLE,
    M_LAYER_MUD,
    M_LAYER_LOW_GRASS,
    M_LAYER_HIGH_GRASS,
    M_LAYER_FOREST,
    M_LAYER_THICKFOREST
};

enum SUB_LAYERS
{
    S_LAYER_NONE
};

// MEMORIES
// BAD AREA LIST
// BAD ENTITY LIST
enum ENTITY_MODIFIERS
{
    EM_NONE,
    EM_CAREFUL,
    EM_SCARED
};

struct Costs
{
    int g_score {};
    int h_score {};
};

struct ScoreModifiers
{
    // Attributes contains information on when to add the Score Modifiers.
    // Not all layers may use it in actual path changing ways.
    LayerID layer;
    // SubLayer sub_layer;
    // Attributes attr;
    int penalty_modifier {};
    // std::map<std::string, int> value;
};

struct Entry
{
    bool blocked = false;
    Position position {};
    Costs costs {};
    Entry* parent = nullptr;
};

class AStar
{
public:
    AStar();
    AStar(WeightValues array, int x, int y);
    ~AStar();

    /**
     * @brief Bind bind a map to be used
     * @param Array pointer to the map
     * @param x Size in x
     * @param y Size in y
     * @param copy true of false;
     * @return
     */
    bool bind(WeightValues array, int x, int y, bool copy = false);

    /**
     * @brief setSize Sets the size of the area to be used by A*
     *
     * @param x X Size of area
     * @param y Y Size of area
     */
    void size(int x, int y);

    /**
     * @brief Attaches an array of weights/penalties to the existing
     * array and recalculates the Costs
     *
     * @param array Array of Integer values;
     */
    LayerID add_penalty_layer(const WeightValues array, size_t size);

    /**
     * @brief Removes a penalty layer
     * @param id ID of the layer to remove.
     *
     * @return bool true if layer sucessfully was removed
     */
    bool remove_penalty_layer(LayerID id);

    /**
     * @brief Calculate the A* path
     * @param from From coordinate
     * @param to To coordinate
     * @param ignore_blocked Ignore blocked flag, this could be used
     * for fastest path, like for flying creatures.
     *
     */
    bool astar(int from_x, int from_y, int to_x, int to_y, bool ignore_blocked);

    /**
     * Return result
     */
    [[nodiscard]] const std::vector<int>& get_final_path() const;

    /**
     * @brief enableDebugDump
     * @param enable Enables output of ascii map for debug purposes
     */
    void enable_debug_dump(bool enable);

private:
    /**
     * @brief reset_data resets arrays before returning
     */
    void reset_data();
    /**
     * @brief debugDump outputs a map of the estimated path
     */
    void debugDump();

    /**
     * @brief add_open_list_entry_by_position
     * @param pos Poition to add
     * @param entry to add
     */
    void add_open_list_entry_by_position(Position pos, Entry* entry);

     void add_closed_list_entry_by_position(Position pos, Entry* entry);
    bool change_open_to_closed(Entry* entry);

    /**
     * @brief build_open_list
     * @return returns false if no path was found
     */
    bool build_open_list();
    bool buildPath(const Entry* parentEntry);
    void cleanup();

    bool m_enable_debug;
    // Currently bound area

    int* m_current_bound = nullptr;
    int m_map_size_x {};
    int m_map_size_y {};
    int* m_path_copy = nullptr;

    Position m_best_scored_position;
    Position m_start_position;
    Position m_end_position;
    Position m_active_position;

    typedef std::multimap<FScore, Entry*> ScoreList;
    typedef std::map<Position, Entry*> PositionList;

    ScoreList m_open_score_list;
    PositionList m_open_position_list;
    ScoreList m_closed_score_list;
    PositionList m_closed_position_list;
    std::deque<Position> m_path_result;
    std::map<LayerID, int*> m_layers;
    std::vector<int> final_path_;
};

} // namespace star

