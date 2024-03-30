#pragma once

#include <GL/glew.h>
#include "vec3.hpp"
#include <vector>
#include <cmath>
#include "mesh.hpp"
#include "OpenSimplexNoise.h"

namespace mesher {
    GLuint VID, TID;

    OpenSimplexNoise::Noise noise;

    struct GeometryInfo {
        std::vector<GLfloat> vertices = std::vector<GLfloat>();
        std::vector<GLuint> indices = std::vector<GLuint>();
    };

    struct Vertex {
        GLuint id;
        glm::vec3 p;

        Vertex(glm::vec3 p) {
            this->p = p;
            id = VID++;
        }

        void handle(GeometryInfo &geometry) {
            geometry.vertices.push_back(p.x);
            geometry.vertices.push_back(p.y);
            geometry.vertices.push_back(p.z);
            geometry.vertices.push_back(0);
            geometry.vertices.push_back(1);
            geometry.vertices.push_back(0);
        }
    };

    struct Tri {
        GLuint id;
        GLuint a, b, c;

        Tri(Vertex &a, Vertex &b, Vertex &c) {
            this->a = a.id;
            this->b = b.id;
            this->c = c.id;
            id = TID++;
        }

        void handle(GeometryInfo &geometry) {
            geometry.indices.push_back(a);
            geometry.indices.push_back(b);
            geometry.indices.push_back(c);
        }
    };

    struct BuildingInfo {
        GeometryInfo &geometry;
        std::vector<Vertex> vertexes = std::vector<Vertex>();
        std::vector<Tri> tris = std::vector<Tri>();

        explicit BuildingInfo(GeometryInfo &geometry) : geometry(geometry) { }
    };

    Vertex &vert(BuildingInfo &info, float x, float y, float z) {
        info.vertexes.emplace_back(glm::vec3(x, y, z));
        return info.vertexes.back();
    }

    Tri &tri(BuildingInfo &info, Vertex &a, Vertex &b, Vertex &c) {
        info.tris.emplace_back(a, b, c);
        return info.tris.back();
    }

    void quad(BuildingInfo &info, Vertex &a, Vertex &b, Vertex &c, Vertex &d) {
        tri(info, a, b, c);
        tri(info, c, d, a);
    }

    void initMeshConstruct(BuildingInfo &info) {
        VID = 0;
        TID = 0;
    }

    void endMeshConstruct(BuildingInfo &info) {
        for (Vertex vert: info.vertexes)
            vert.handle(info.geometry);
        for (Tri tri: info.tris)
            tri.handle(info.geometry);

        info.vertexes.clear();
        info.tris.clear();
    }

    Mesh* chunk(int x, int z, int chunkSize, Shader *shader) {
        float xx = x * chunkSize;
        float zz = z * chunkSize;
        GeometryInfo geometry = GeometryInfo();
        BuildingInfo info = BuildingInfo(geometry);

        initMeshConstruct(info);

        for (int x = 0; x < chunkSize; x += 1) {
            for (int z = 0; z < chunkSize; z += 1) {
                vert(info, x + xx, noise.eval(x + xx, z + zz), z + zz);
                if (x >= 1 && z >= 1) {
                    quad(info,
                         info.vertexes[(x) * chunkSize + (z)],
                         info.vertexes[(x - 1) * chunkSize + (z)],
                         info.vertexes[(x - 1) * chunkSize + (z - 1)],
                         info.vertexes[(x) * chunkSize + (z - 1)]
                    );
                }
            }
        }

        endMeshConstruct(info);

        Mesh *mesh = new Mesh(geometry.vertices.data(), geometry.vertices.size(),
                         geometry.indices.data(), geometry.indices.size(),
                         shader);

        return mesh;
    }
}