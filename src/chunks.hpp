#pragma once

#include "mesh.hpp"
#include "vec3.hpp"
#include <vector>
#include "mesher.hpp"
#include "shader.hpp"
#include <cmath>

extern Camera *camera;

class Chunk {
public:
    Mesh *mesh;
    glm::vec3 worldPosition;
    glm::i32vec3 tilePosition;

    bool forceLoad = false;

    Chunk(Mesh *mesh, glm::vec3 wp, glm::i32vec3 tp) {
        worldPosition = wp;
        tilePosition = tp;
        this->mesh = mesh;
    }

    void onAdd() {

    }

    void onRemove() {

    }

    void update() {
        // idk
    }

    void render() {
        mesh->render(camera);
    }

    ~Chunk() {
        delete mesh;
    }
};

namespace chunk {
    int renderDistance = 16;
    int chunkSize = 16;

    bool disposeChunks = true;

    std::vector<Chunk*> chunks = std::vector<Chunk*>();
    std::vector<int> forDelete = std::vector<int>();

    Shader *shader;

    void update() {
        bool tiles[renderDistance*2+1][renderDistance*2+1];
        for (int x = 0; x < renderDistance*2+1; x++) {
            for (int z = 0; z < renderDistance * 2 + 1; z++) {
                tiles[x][z] = false;
            }
        }

        glm::vec3 cameraPosition = camera->position;
        glm::vec3 cp = cameraPosition / (float) chunkSize;
        glm::i32vec3 cameraTilePosition = glm::i32vec3(floor(cp.x), floor(cp.y), floor(cp.z));

        forDelete.clear();
        int pos = 0;
        for (Chunk *chunk: chunks) {
            glm::i32vec3 relativeChunkTilePosition = (chunk->tilePosition - cameraTilePosition);

            if ((abs(relativeChunkTilePosition.x) > renderDistance || abs(relativeChunkTilePosition.z) > renderDistance)) {
                if (!chunk->forceLoad && disposeChunks) {
                    forDelete.push_back(pos);
                }
            } else {
                tiles[relativeChunkTilePosition.x + renderDistance][relativeChunkTilePosition.z + renderDistance] = true;
            }
            pos++;
        }

        for (uint64_t i = 0; i < forDelete.size(); i++) {
            int ind = forDelete[forDelete.size() - i - 1];
            delete chunks[ind];
            chunks.erase(chunks.cbegin() + ind);
        }
        chunks.shrink_to_fit();

        forDelete.clear();
        forDelete.shrink_to_fit();

        for (int x = 0; x < renderDistance*2+1; x++) {
            for (int z = 0; z < renderDistance*2+1; z++) {
                if (tiles[x][z])
                    continue;

                glm::i32vec3 tilePosition = glm::i32vec3(x - renderDistance, 0, z - renderDistance) + cameraTilePosition;
                glm::vec3 worldPosition = tilePosition * chunkSize;

                Chunk *chunk = new Chunk(mesher::chunk(tilePosition.x, tilePosition.z, chunkSize, shader), worldPosition, tilePosition);

                chunks.push_back(chunk);
            }
        }

        for (Chunk *chunk: chunks)
            chunk->update();
    }

    void render() {
        for (Chunk *chunk: chunks)
            chunk->render();
    }
}