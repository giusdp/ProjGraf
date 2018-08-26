#ifndef PROJGRAF_MESH_H
#define PROJGRAF_MESH_H

#include <vector>
#include "point3.h"
#include "texture.h"
//classe UV:
//i punti delle texture
class UV
{
  public:
    float u, v;
    UV(float u, float v) : u(u), v(v) {}
};
// classe Vertex:
// i vertici della mesh
class Vertex
{
  public:
    Point3 p; // posizione

    // attributi per vertice
    Vector3 n; // normale (per vertice)
};

class Edge
{
  public:
    Vertex *v[2]; // due puntatori a Vertice (i due estremi dell'edge)
                  // attributi per edge:
};

class Face
{
  public:
    Vertex *v[3]; // tre puntatori a Vertice (i tre vertici del triangolo)

    UV *uv[3];

    // costruttore
    Face(Vertex *a, Vertex *b, Vertex *c)
    {
        v[0] = a;
        v[1] = b;
        v[2] = c;
    }
    Face(Vertex *a, Vertex *b, Vertex *c, UV *ta, UV *tb, UV *tc)
    {
        v[0] = a;
        v[1] = b;
        v[2] = c;
        uv[0] = ta;
        uv[1] = tb;
        uv[2] = tc;
    }

    // attributi per faccia
    Vector3 n; // normale (per faccia)

    // computa la normale della faccia
    void ComputeNormal()
    {
        n = -((v[1]->p - v[0]->p) % (v[2]->p - v[0]->p)).Normalize();
    }

    // attributi per wedge
};

class Mesh
{
    std::vector<Vertex> v; // vettore di vertici
    std::vector<Face> f;   // vettore di facce
    std::vector<Edge> e;   // vettore di edge (per ora, non usato)

    std::vector<UV> textures; // vettore delle textures

  public:
    // costruttore con caricamento
    Mesh(char *filename)
    {
        LoadFromObj(filename);
        ComputeNormalsPerFace();
        ComputeNormalsPerVertex();
        ComputeBoundingBox();
    }

    Mesh(std::vector<Vertex> &vs, std::vector<Face> &fs) : v(vs), f(fs)
    {
        ComputeNormalsPerFace();
        ComputeNormalsPerVertex();
        ComputeBoundingBox();
    }

    // metodi
    void RenderNxF();  // manda a schermo la mesh Normali x Faccia
    void RenderNxV();  // manda a schermo la mesh Normali x Vertice
    void RenderWire(); // manda a schermo la mesh in wireframe

    void RenderNxVxT(Texture texture); // come RenderNxV ma con una texture

    bool LoadFromObj(char *filename); //  carica la mesh da un file OBJ

    void ComputeNormalsPerFace();
    void ComputeNormalsPerVertex();

    void ComputeBoundingBox();

    // centro del axis aligned bounding box
    Point3 Center() { return (bbmin + bbmax) / 2.0; };

    Point3 bbmin, bbmax; // bounding box
};
#endif //PROJGRAF_MESH_H