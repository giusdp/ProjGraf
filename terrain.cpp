//
// Created by giuseppe on 19/08/18.
//

#include "terrain.h"
#include "utils.h"
extern bool useWireframe;

Terrain::Terrain() {
    generateTerrain();
}

void Terrain::generateTerrain() {

    for (int z = 0; z < VERTEX_COUNT; z++) {
        for (int x = 0; x < VERTEX_COUNT; x++) {
            float grid_x = x * SIZE;
            float grid_z = z * SIZE;
            Vertex v = Vertex();
            v.p = Point3(grid_x, 0, grid_z);
            v.n = Vector3(0, 1, 0);
            grid.push_back(v);
        }
    }

    for (int z = 0; z < VERTEX_COUNT - 1; z++) {
        for (int x = 0; x < VERTEX_COUNT - 1; x++) {
            Face f = Face(&grid[x + z * VERTEX_COUNT], &grid[x + 1 + z * VERTEX_COUNT],
                          &grid[(z + 1) * VERTEX_COUNT + x]);
            f.ComputeNormal();
            faces.push_back(f);
        }
    }

    // Creo le facce mancanti, però così disegno più volte gli stessi vertici e linee.
    // Per il lato destro e basso della griglia risolvo il problema della linea mancante
    // a discapito del ridisegno dei vertici in comune delle facce.
    if (!useWireframe) {
        for (int z = 0; z < VERTEX_COUNT - 1; z++) {
            for (int x = (int) VERTEX_COUNT - 1; x > 0; x--) {
                Face f = Face(&grid[x + z * VERTEX_COUNT], &grid[x + (z + 1) * VERTEX_COUNT],
                              &grid[(x - 1) + (z + 1) * VERTEX_COUNT]);
                f.ComputeNormal();
                faces.push_back(f);
            }
        }
    }

}

double flying = 0;

void Terrain::render() {

    glPushMatrix();
    glDisable(GL_TEXTURE_2D);
    glColor3f(.5, .5, .5);
    glLineWidth(1.0);

    glRotatef(180, 0, 1, 0);
    glTranslatef(xOffset, -10, 0);

    // Calcolo nuova altezza per ogni vertice con perlin noise.
    // Flying è un trucchetto per far sembrare che si sta volando sopra il terreno
    double xOff = 0, yOff = flying, zOff = 0;
    float noise;
    for (int z = 0; z < VERTEX_COUNT - 1; z++) {
        xOff = 0;
        for (int x = 0; x < VERTEX_COUNT - 1; x++) {
            auto index = (int) (x + z * (VERTEX_COUNT - 1));
            noise = (float) perlinNoise.noise(xOff, yOff, zOff);
            faces[index].v[0]->p.coord[1] = mapRange(noise, 0, 1, -maxHeight, maxHeight);
            xOff += 0.05f;
            //zOff += 0.01;
            noise = (float) perlinNoise.noise(xOff, yOff, zOff);
            faces[index].v[1]->p.coord[1] = mapRange(noise, 0, 1, -maxHeight, maxHeight);
            xOff += 0.05f;
            //zOff += 0.01;
            noise = (float) perlinNoise.noise(xOff, yOff, zOff);
            faces[index].v[2]->p.coord[1] = mapRange(noise, 0, 1, -maxHeight, maxHeight);
            //xOff += 0.05f;
            //zOff += 0.01;
        }
        yOff += 0.1;
    }
    if (flying > 10000) flying = 0; // per non rischiare buffer overflow
    flying += 0.01;


    // Potevo usare usare vertici e facce per creare un oggetto mesh
    // e poi utilizzare i metodi Render.
    // Però volevo mettere mano ai singoli vertici per poter cambiare i colori
    // Quindi ho scritto il codice per il rendering.
    GLenum mode = (useWireframe ? GL_LINE_LOOP : GL_TRIANGLES);
    // Disegnamo tutte le facce
    for (auto &face : faces) {
        glBegin(mode);

        // Mandando solo la normale della faccia si ha flat shading
        // Per avere uno stile "low poly" non è male, anzi.
        // face.n.SendAsNormal();
        // Mandando una normale per vertice invece si ha uno smooth shading
        vertexColor(face.v[0]->p.coord[1]);
        face.v[0]->n.SendAsNormal();
        face.v[0]->p.SendAsVertex();

        vertexColor(face.v[1]->p.coord[1]);
        face.v[1]->n.SendAsNormal();
        face.v[1]->p.SendAsVertex();

        vertexColor(face.v[2]->p.coord[1]);
        face.v[2]->n.SendAsNormal();
        face.v[2]->p.SendAsVertex();

        glEnd();
    }

    glColor3f(1, 1, 1);
    glPopMatrix();
}

void Terrain::vertexColor(float y) {
    float red = 1, green = 1, blue = 1;
    if (y >= 0 && y <= maxHeight) {
// interpolate between (1.0f, 0.0f, 0.0f) and (0.0f, 1.0f, 0.0f)
        green = y / maxHeight;
        blue = 1.0f - green;
        red = 0.0f;

    } else if (y >= -maxHeight && y < 0) {
// interpolate between (0.0f, 1.0f, 0.0f) and (0.0f, 0.0f, 1.0f)
        red = 0.0f;
        green = (-y) / maxHeight;
        blue = 1.0f - green;
    }

    glColor3f(red, green, blue);
}



