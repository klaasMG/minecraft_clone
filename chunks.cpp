#include "chunks.h"
#include "vec3.hpp"
#include "mat4x4.hpp"
#include "ext/matrix_transform.hpp"
#ifndef CHUNK_SIZE
#define CHUNK_SIZE 16.0
#endif


glm::mat4x4 createModelMatrix(const glm::i64vec2& worldPos) {
    glm::vec3 position(
        static_cast<float>(worldPos.x) * CHUNK_SIZE,
        0.0f,
        static_cast<float>(worldPos.y) * CHUNK_SIZE
    );

    glm::mat4 model(1.0f);
    model = glm::translate(model, position);
    return model;
}

chunk ChunkManager::create_chunk(const glm::i64vec2& world_pos){
    auto result = std::make_unique<chunk>();
    result->world_pos = world_pos;

    for (size_t i = 0; i < result->chunk_layers.size(); ++i){
        chunk_layer layer{};
        layer.height = static_cast<uint8_t>(i);

        if (i < 16)
            layer.blocks.fill(Block{ .block_id = 1 });
        else
            layer.blocks.fill(Block{ .block_id = 0 });

        result->chunk_layers[i] = std::move(layer);
    }

    return std::move(*result);
}

std::vector<ChunkRenderDate> ChunkManager::get_chunk_render_date(const glm::i64vec2& chunk_pos, int radius) {
    std::vector<ChunkRenderDate> result;
    size_t offset = 0;

    for (int dx = -radius; dx <= radius; ++dx) {
        for (int dz = -radius; dz <= radius; ++dz) {
            glm::i64vec2 pos(chunk_pos.x + dx, chunk_pos.y + dz);
            
            chunk& c = get_chunk(pos);
            std::vector<glm::vec4>& mesh = get_mesh(c);
            glm::mat4x4 model = get_model_matrix(pos);
            
            ChunkRenderDate data;
            data.model_matrix = model;
            data.num_vertices = mesh.size();
            data.vertex_offset = offset;
            
            result.push_back(data);
            offset += mesh.size();
        }
    }

    return result;
}

std::vector<std::reference_wrapper<chunk>> ChunkManager::get_chunks(const glm::i64vec2& center, int radius){
    std::vector<std::reference_wrapper<chunk>> chunk_result;

    for (int dx = -radius; dx <= radius; ++dx){
        for (int dz = -radius; dz <= radius; ++dz){
            chunk& return_chunk = get_chunk(glm::i64vec2(center.x + dx, center.y + dz));
            chunk_result.emplace_back(return_chunk);
        }
    }

    return chunk_result;
}

std::vector<std::vector<glm::vec4>> ChunkManager::get_meshes(const glm::i64vec2& center, int radius){
    std::vector<std::vector<glm::vec4>> mesh_results;

    for (int dx = -radius; dx <= radius; ++dx){
        for (int dz = -radius; dz <= radius; ++dz){
            glm::i64vec2 pos(center.x + dx, center.y + dz);

            chunk& c = get_chunk(pos);
            mesh_results.push_back(get_mesh(c));
        }
    }

    return mesh_results;
}

std::vector<glm::mat4x4> ChunkManager::get_model_matricies(const glm::i64vec2& chunk_pos, int radius) {
    std::vector<glm::mat4x4> model_matricies_result;

    for (int dx = -radius; dx <= radius; ++dx){
        for (int dz = -radius; dz <= radius; ++dz){
            glm::i64vec2 pos(chunk_pos.x + dx, chunk_pos.y + dz);
            model_matricies_result.emplace_back(get_model_matrix(pos));
        }
    }

    return model_matricies_result;
}

glm::mat4x4 ChunkManager::get_model_matrix(const glm::i64vec2& chunk_pos) {
    chunk& model_chunk = get_chunk(chunk_pos);
    glm::i64vec2 world_pos = model_chunk.world_pos;
    glm::mat4x4 model_matrix = createModelMatrix(world_pos);
    return model_matrix;
}

chunk& ChunkManager::get_chunk(const glm::i64vec2& chunk_pos) {
    auto it = chunks.find(chunk_pos);

    if (it == chunks.end()) {
        auto new_chunk = std::make_unique<chunk>();
        *new_chunk = create_chunk(chunk_pos);
        auto [insert_it, _] = chunks.emplace(chunk_pos, std::move(new_chunk));
        return *insert_it->second;
    }

    return *it->second;
}

std::vector<glm::vec4>& ChunkManager::get_mesh(const chunk& mesh_chunk) {
    std::map<glm::i64vec2, std::unique_ptr<std::vector<glm::vec4>>, Vec2Compare>::iterator it = meshes.find(mesh_chunk.world_pos);

    if (it != meshes.end()) {
        return *(it->second);
    }

    std::unique_ptr<std::vector<glm::vec4>> vertices =
        std::make_unique<std::vector<glm::vec4>>();

    vertices->reserve(10000);

    for (size_t cy = 0; cy < mesh_chunk.chunk_layers.size(); ++cy) {
        const chunk_layer& layer = mesh_chunk.chunk_layers[cy];
        int y = static_cast<int>(cy) * 16;

        for (size_t i = 0; i < layer.blocks.size(); ++i) {
            const Block& b = layer.blocks[i];

            if (b.block_id == 0)
                continue;

            int x = static_cast<int>(i % 16);
            int z = static_cast<int>(i / 16);

            float id = static_cast<float>(b.block_id);

            glm::vec3 p[8] = {
                glm::vec3(x,     y,     z),
                glm::vec3(x + 1, y,     z),
                glm::vec3(x + 1, y + 1, z),
                glm::vec3(x,     y + 1, z),
                glm::vec3(x,     y,     z + 1),
                glm::vec3(x + 1, y,     z + 1),
                glm::vec3(x + 1, y + 1, z + 1),
                glm::vec3(x,     y + 1, z + 1)
            };

            // bottom
            vertices->push_back(glm::vec4(p[0], id));
            vertices->push_back(glm::vec4(p[1], id));
            vertices->push_back(glm::vec4(p[2], id));

            vertices->push_back(glm::vec4(p[2], id));
            vertices->push_back(glm::vec4(p[3], id));
            vertices->push_back(glm::vec4(p[0], id));

            // top
            vertices->push_back(glm::vec4(p[4], id));
            vertices->push_back(glm::vec4(p[5], id));
            vertices->push_back(glm::vec4(p[6], id));

            vertices->push_back(glm::vec4(p[6], id));
            vertices->push_back(glm::vec4(p[7], id));
            vertices->push_back(glm::vec4(p[4], id));

            // front
            vertices->push_back(glm::vec4(p[0], id));
            vertices->push_back(glm::vec4(p[1], id));
            vertices->push_back(glm::vec4(p[5], id));

            vertices->push_back(glm::vec4(p[5], id));
            vertices->push_back(glm::vec4(p[4], id));
            vertices->push_back(glm::vec4(p[0], id));

            // back
            vertices->push_back(glm::vec4(p[2], id));
            vertices->push_back(glm::vec4(p[3], id));
            vertices->push_back(glm::vec4(p[7], id));

            vertices->push_back(glm::vec4(p[7], id));
            vertices->push_back(glm::vec4(p[6], id));
            vertices->push_back(glm::vec4(p[2], id));

            // left
            vertices->push_back(glm::vec4(p[0], id));
            vertices->push_back(glm::vec4(p[3], id));
            vertices->push_back(glm::vec4(p[7], id));

            vertices->push_back(glm::vec4(p[7], id));
            vertices->push_back(glm::vec4(p[4], id));
            vertices->push_back(glm::vec4(p[0], id));

            // right
            vertices->push_back(glm::vec4(p[1], id));
            vertices->push_back(glm::vec4(p[2], id));
            vertices->push_back(glm::vec4(p[6], id));

            vertices->push_back(glm::vec4(p[6], id));
            vertices->push_back(glm::vec4(p[5], id));
            vertices->push_back(glm::vec4(p[1], id));
        }
    }

    std::vector<glm::vec4>& ref = *vertices;
    auto inserted = meshes.emplace(mesh_chunk.world_pos, std::move(vertices));
    return *(inserted.first->second);
}