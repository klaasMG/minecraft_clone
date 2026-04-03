#ifndef MINECRAFT_CLONE_BLOCKS_H
#define MINECRAFT_CLONE_BLOCKS_H

#include "chunks.h"
#include "vec2.hpp"
#include "vec4.hpp"
#include <cstdint>
#include <array>
#include <vector>
#include <map>
#include <memory>
#include <variant>

struct Vec2Compare {
    bool operator()(const glm::i64vec2& a, const glm::i64vec2& b) const {
        if (a.x != b.x) return a.x < b.x;
        return a.y < b.y;
    }
};

struct Block {
    uint32_t block_id;
};

struct chunk_layer {
    uint8_t height;
    std::array<Block, 4096> blocks;
};

struct chunk {
    std::array<chunk_layer, 64> chunk_layers;
    glm::i64vec2 world_pos;
};

enum class chunk_data_type {
    CHUNKS,
    MESHES,
};

class ChunkManager {
public:
    std::vector<glm::vec4>& get_mesh(const chunk& mesh_chunk);
    chunk& get_chunk(const glm::i64vec2& chunk_pos);
    std::vector<std::reference_wrapper<chunk>> get_chunks(const glm::i64vec2& center, int radius);
    std::vector<std::vector<glm::vec4>> get_meshes(const glm::i64vec2& center, int radius);
    static chunk create_chunk(const glm::i64vec2& world_pos);
private:
    std::map<glm::i64vec2, std::unique_ptr<chunk>, Vec2Compare> chunks;
    std::map<glm::i64vec2, std::unique_ptr<std::vector<glm::vec4>>, Vec2Compare> meshes;
};

#endif //MINECRAFT_CLONE_BLOCKS_H
