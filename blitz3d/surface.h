#ifndef SURFACE_H
#define SURFACE_H

#include "model.h"

#define MAX_SURFACE_BONES 4

class Surface
{
public:
    struct Vertex
    {
        Vector coords;
        Vector normal;
        unsigned color;
        float tex_coords[2][2];
        unsigned char bone_bones[MAX_SURFACE_BONES];
        float bone_weights[MAX_SURFACE_BONES];

        Vertex() : color(~0)
        {
            bone_bones[0] = 255;
            memset(tex_coords, 0, sizeof(tex_coords));
        }

        bool operator<(const Vertex& t) const
        {
            return memcmp(this, &t, sizeof(*this)) == -1;
        }
    };

    struct Triangle
    {
        unsigned short verts[3];
    };

    struct Bone
    {
        Transform coord_tform;
        Matrix normal_tform;
    };

    struct Monitor
    {
        int brush_changes, geom_changes;
    };

    Surface();

    Surface(Monitor* mon);

    ~Surface();

    void setName(const std::string& t);

    void setBrush(const Brush& b);

    void clear(bool verts, bool tris);

    void addVertex(const Vertex& v)
    {
        vertices.push_back(v);
        ++mon->geom_changes;
    }

    void setVertex(const int n, const Vertex& v)
    {
        vertices[n] = v;
        if (n < valid_vs) valid_vs = n;
        ++mon->geom_changes;
    }

    void setCoords(const int n, const Vector& v)
    {
        vertices[n].coords = v;
        if (n < valid_vs) valid_vs = n;
        ++mon->geom_changes;
    }

    void setNormal(const int n, const Vector& v)
    {
        vertices[n].normal = v;
        if (n < valid_vs) valid_vs = n;
    }

    void setColor(const int n, const unsigned argb)
    {
        vertices[n].color = argb;
        if (n < valid_vs) valid_vs = n;
    }

    void setTexCoords(const int n, const Vector& v, const int i)
    {
        vertices[n].tex_coords[i][0] = v.x;
        vertices[n].tex_coords[i][1] = v.y;
        if (n < valid_vs) valid_vs = n;
    }

    void addTriangle(const Triangle& t)
    {
        triangles.push_back(t);
        ++mon->geom_changes;
    }

    void setTriangle(const int n, const Triangle& t)
    {
        triangles[n] = t;
        if (n < valid_ts) valid_ts = n;
        ++mon->geom_changes;
    }

    Vector getColor(int index) const;

    void setColor(int index, const Vector& v);

    void addVertices(const std::vector<Vertex>& verts);

    void addTriangles(const std::vector<Triangle>& tris);

    void updateNormals();

    gxMesh* getMesh();

    gxMesh* getMesh(const std::vector<Bone>& bones);

    std::string getName() const { return name; }

    const Brush& getBrush() const { return brush; }

    int numVertices() const { return vertices.size(); }

    int numTriangles() const { return triangles.size(); }

    const Vertex& getVertex(const int n) const { return vertices[n]; }

    const Triangle& getTriangle(const int n) const { return triangles[n]; }

private:
    Brush brush;
    std::string name;
    gxMesh* mesh;
    std::vector<Vertex> vertices;
    std::vector<Triangle> triangles;
    int mesh_vs, mesh_ts;
    int valid_vs, valid_ts;
    Monitor* mon;
};

#endif
