// file mesh.cpp
//
// Implementazione dei metodi di Mesh

#include <cstdio>
#include <string>

#ifdef __APPLE__
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#include <iostream>

#endif

#include "mesh.h"

extern bool useWireframe; // quick hack: una var globale definita altrove

void Mesh::ComputeNormalsPerFace()
{
    for (int i = 0; i < f.size(); i++)
    {
        f[i].ComputeNormal();
    }
}

void Mesh::RenderNxVxT(Texture texture) // come RenderNxV ma con una texture
{
    if (useWireframe)
    {
        glDisable(GL_TEXTURE_2D);
        glColor3f(.5, .5, .5);
        RenderWire();
        glColor3f(1, 1, 1);
    }
    else
    {
        glDisable(GL_LIGHTING);
        glColor3f(1,1,1);
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, texture.getTextureID());
        
        // for (UV uv : textures){
        //     glTexCoord2f(uv.u, uv.v);
        // }

        glBegin(GL_TRIANGLES);

        for (int i = 0; i < f.size(); i++)
        {
            glTexCoord2f(f[i].uv[0]->u, f[i].uv[0]->v);
            (f[i].v[0])->p.SendAsVertex();
            //(f[i].v[0])->n.SendAsNormal(); // gouroud shading (o phong?)

            glTexCoord2f(f[i].uv[1]->u, f[i].uv[1]->v);
            (f[i].v[1])->p.SendAsVertex();
            //(f[i].v[1])->n.SendAsNormal();

            glTexCoord2f(f[i].uv[2]->u, f[i].uv[2]->v);
            (f[i].v[2])->p.SendAsVertex();
            //(f[i].v[2])->n.SendAsNormal();
        }
        glEnd();

        glDisable(GL_TEXTURE_2D);
        glEnable(GL_LIGHTING);

    }
}
// Computo normali per vertice
// (come media rinormalizzata delle normali delle facce adjacenti)
void Mesh::ComputeNormalsPerVertex()
{
    // uso solo le strutture di navigazione FV (da Faccia a Vertice)!

    // fase uno: ciclo sui vertici, azzero tutte le normali
    for (int i = 0; i < v.size(); i++)
    {
        v[i].n = Point3(0, 0, 0);
    }

    // fase due: ciclo sulle facce: accumulo le normali di F nei 3 V corrispondenti
    for (int i = 0; i < f.size(); i++)
    {
        f[i].v[0]->n = f[i].v[0]->n + f[i].n;
        f[i].v[1]->n = f[i].v[1]->n + f[i].n;
        f[i].v[2]->n = f[i].v[2]->n + f[i].n;
    }

    // fase tre: ciclo sui vertici; rinormalizzo
    // la normale media rinormalizzata e' uguale alla somma delle normnali, rinormalizzata
    for (int i = 0; i < v.size(); i++)
    {
        v[i].n = v[i].n.Normalize();
    }
}

// renderizzo la mesh in wireframe
void Mesh::RenderWire()
{
    glLineWidth(1.0);
    // (nota: ogni edge viene disegnato due volte.
    // sarebbe meglio avere ed usare la struttura edge)
    for (int i = 0; i < f.size(); i++)
    {
        glBegin(GL_LINE_LOOP);
        f[i].n.SendAsNormal();
        (f[i].v[0])->p.SendAsVertex();
        (f[i].v[1])->p.SendAsVertex();
        (f[i].v[2])->p.SendAsVertex();
        glEnd();
    }
}

// Render usando la normale per faccia (FLAT SHADING)
void Mesh::RenderNxF()
{
    if (useWireframe)
    {
        glDisable(GL_TEXTURE_2D);
        glColor3f(.5, .5, .5);
        RenderWire();
        glColor3f(1, 1, 1);
    }

    // mandiamo tutti i triangoli a schermo
    glBegin(GL_TRIANGLES);
    for (int i = 0; i < f.size(); i++)
    {
        f[i].n.SendAsNormal(); // flat shading

        (f[i].v[0])->p.SendAsVertex();

        (f[i].v[1])->p.SendAsVertex();

        (f[i].v[2])->p.SendAsVertex();
    }
    glEnd();
}

// Render usando la normale per vertice (GOURAUD SHADING)
void Mesh::RenderNxV()
{
    if (useWireframe)
    {
        glDisable(GL_TEXTURE_2D);
        glColor3f(.5, .5, .5);
        RenderWire();
        glColor3f(1, 1, 1);
    }
    else
    {
        // mandiamo tutti i triangoli a schermo
        glBegin(GL_TRIANGLES);
        for (int i = 0; i < f.size(); i++)
        {
            (f[i].v[0])->n.SendAsNormal(); // gouroud shading (o phong?)
            (f[i].v[0])->p.SendAsVertex();

            (f[i].v[1])->n.SendAsNormal();
            (f[i].v[1])->p.SendAsVertex();

            (f[i].v[2])->n.SendAsNormal();
            (f[i].v[2])->p.SendAsVertex();
        }
        glEnd();
    }
}

// trova l'AXIS ALIGNED BOUNDIG BOX
void Mesh::ComputeBoundingBox()
{
    // basta trovare la min x, y, e z, e la max x, y, e z di tutti i vertici
    // (nota: non e' necessario usare le facce: perche?)
    if (v.empty())
        return;
    bbmin = bbmax = v[0].p;
    for (auto &i : v)
    {
        for (int k = 0; k < 3; k++)
        {
            if (bbmin.coord[k] > i.p.coord[k])
                bbmin.coord[k] = i.p.coord[k];
            if (bbmax.coord[k] < i.p.coord[k])
                bbmax.coord[k] = i.p.coord[k];
        }
    }
}

// carica la mesh da un file in formato Obj
//   Nota: nel file, possono essere presenti sia quads che tris
//   ma nella rappresentazione interna (classe Mesh) abbiamo solo tris.
//
bool Mesh::LoadFromObj(char *filename)
{
    std::vector<UV *> t;

    FILE *file = fopen(filename, "rt"); // apriamo il file in lettura
    if (!file)
    {
        std::fprintf(stderr, "Errore apertura file %s.\n", filename);
        return false;
    }
    //make a first pass through the file to get a count of the number
    //of vertices, normals, texcoords & triangles
    char buf[128];
    int nv, nf, nt;
    float x, y, z;
    float u_textcoord, v_textcoord;
    int va, vb, vc, vd;
    int na, nb, nc, nd;
    int ta, tb, tc, td;

    nv = 0;
    nf = 0;
    nt = 0;
    while (fscanf(file, "%s", buf) != EOF)
    {
        switch (buf[0])
        {
        case '#': // comment
            // eat up rest of line
            fgets(buf, sizeof(buf), file);
            break;
        case 'v': // v, vn, vt
            switch (buf[1])
            {
            case '\0': // vertex
                // eat up rest of line
                fgets(buf, sizeof(buf), file);
                nv++;
                break;
            case 't': // vt
                // eat up rest of line
                fgets(buf, sizeof(buf), file);
                nt++;
                break;
            default:
                break;
            }
            break;
        case 'f': // face
            fscanf(file, "%s", buf);
            // can be one of %d, %d//%d, %d/%d, %d/%d/%d %d//%d
            if (strstr(buf, "//"))
            {
                // v//n
                sscanf(buf, "%d//%d", &va, &na);
                fscanf(file, "%d//%d", &vb, &nb);
                fscanf(file, "%d//%d", &vc, &nc);
                nf++;
                nt++;
                while (fscanf(file, "%d//%d", &vd, &nd) > 0)
                {
                    nt++;
                }
            }
            else if (sscanf(buf, "%d/%d/%d", &va, &ta, &na) == 3)
            {
                // v/t/n
                fscanf(file, "%d/%d/%d", &vb, &tb, &nb);
                fscanf(file, "%d/%d/%d", &vc, &tc, &nc);
                nf++;
            }
            else if (sscanf(buf, "%d/%d", &va, &ta) == 2)
            {
                // v/t
                fscanf(file, "%d/%d", &vb, &tb);
                fscanf(file, "%d/%d", &vc, &tc);
                nf++;
                nt++;
                while (fscanf(file, "%d/%d", &vd, &td) > 0)
                {
                    nt++;
                }
            }
            else
            {
                // v
                fscanf(file, "%d", &va);
                fscanf(file, "%d", &vb);
                nf++;
                nt++;
                while (fscanf(file, "%d", &vc) > 0)
                {
                    nt++;
                }
            }
            break;

        default:
            // eat up rest of line
            fgets(buf, sizeof(buf), file);
            break;
        }
    }

    //std::printf("dopo FirstPass nv=%d nf=%d nt=%d %s\n", nv, nf, nt, filename);

    // allochiamo spazio per nv vertici
    v.resize(nv);

    // rewind to beginning of file and read in the data this pass
    rewind(file);

    //on the second pass through the file, read all the data into the
    //allocated arrays

    nv = 0;
    nt = 0;
    while (fscanf(file, "%s", buf) != EOF)
    {
        switch (buf[0])
        {
        case '#': // comment
            // eat up rest of line
            fgets(buf, sizeof(buf), file);
            break;
        case 'v': // v, vn, vt
            switch (buf[1])
            {
            case '\0': // vertex
                fscanf(file, "%f %f %f", &x, &y, &z);
                v[nv].p = Point3(x, y, z);
                nv++;
                break;
            case 't': /* texcoords */
                fscanf(file, "%f %f", &u_textcoord, &v_textcoord);
                t.push_back(new UV(u_textcoord, v_textcoord));
                nt++;
                break;
            default:
                break;
            }
            break;
        case 'f': // face
            fscanf(file, "%s", buf);
            // can be one of %d, %d//%d, %d/%d, %d/%d/%d %d//%d
            if (strstr(buf, "//"))
            {
                // v//n
                sscanf(buf, "%d//%d", &va, &na);
                fscanf(file, "%d//%d", &vb, &nb);
                fscanf(file, "%d//%d", &vc, &nc);
                va--;
                vb--;
                vc--;
                Face newface(&(v[va]), &(v[vc]), &(v[vb])); // invoco il costruttore di faccia
                f.push_back(newface);                       // inserico la nuova faccia in coda al vettore facce
                vb = vc;
                while (fscanf(file, "%d//%d", &vc, &nc) > 0)
                {
                    vc--;
                    Face newface(&(v[va]), &(v[vc]), &(v[vb])); // invoco il costruttore di faccia
                    f.push_back(newface);                       // inserico la nuova faccia in coda al vettore facce
                    vb = vc;
                }
            }
            else if (sscanf(buf, "%d/%d/%d", &va, &ta, &na) == 3)
            {
                // v/t/n
                fscanf(file, "%d/%d/%d", &vb, &tb, &nb);
                fscanf(file, "%d/%d/%d", &vc, &tc, &nc);
                va--;
                vb--;
                vc--;
                ta--;
                tb--;
                tc--;

                Face newface(&(v[va]), &(v[vc]), &(v[vb]), t[ta], t[tc], t[tb]); // invoco il costruttore di faccia

                textures.push_back(*t[ta]);
                textures.push_back(*t[tb]);
                textures.push_back(*t[tc]);
                f.push_back(newface); // inserico la nuova faccia in coda al vettore facce
                vb = vc;
                while (fscanf(file, "%d/%d/%d", &vc, &tc, &nc) > 0)
                {
                    //std::printf("More than 3\n");
                    vc--;
                    tc--;
                    Face newface(&(v[va]), &(v[vc]), &(v[vb]), t[ta], t[tc], t[tb]); // invoco il costruttore di faccia
                    f.push_back(newface);                                            // inserico la nuova faccia in coda al vettore facce
                    textures.push_back(*t[tc]);
                    vb = vc;
                    tb = tc;
                }
            }
            else if (sscanf(buf, "%d/%d", &va, &ta) == 2)
            {
                // v/t
                fscanf(file, "%d/%d", &va, &ta);
                fscanf(file, "%d/%d", &va, &ta);
                va--;
                vb--;
                vc--;
                Face newface(&(v[va]), &(v[vc]), &(v[vb])); // invoco il costruttore di faccia
                f.push_back(newface);                       // inserico la nuova faccia in coda al vettore facce
                nt++;
                vb = vc;
                while (fscanf(file, "%d/%d", &vc, &tc) > 0)
                {
                    vc--;
                    Face newface(&(v[va]), &(v[vc]), &(v[vb])); // invoco il costruttore di faccia
                    f.push_back(newface);                       // inserico la nuova faccia in coda al vettore facce
                    nt++;
                    vb = vc;
                }
            }
            else
            {
                // v
                sscanf(buf, "%d", &va);
                fscanf(file, "%d", &vb);
                fscanf(file, "%d", &vc);
                va--;
                vb--;
                vc--;
                Face newface(&(v[va]), &(v[vc]), &(v[vb])); // invoco il costruttore di faccia
                f.push_back(newface);                       // inserico la nuova faccia in coda al vettore facce
                nt++;
                vb = vc;
                while (fscanf(file, "%d", &vc) > 0)
                {
                    vc--;
                    Face newface(&(v[va]), &(v[vc]), &(v[vb])); // invoco il costruttore di faccia
                    f.push_back(newface);                       // inserico la nuova faccia in coda al vettore facce
                    nt++;
                    vb = vc;
                }
            }
            break;

        default:
            // eat up rest of line
            fgets(buf, sizeof(buf), file);
            break;
        }
    }

    //std::printf("dopo SecondPass nv=%d nt=%d\n", nv, nt);

    for (UV *u : t)
        delete u;
    t.clear();
    fclose(file);
    return true;
}
