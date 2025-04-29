#include <iostream>
#include <cmath>
#include <algorithm>
#include "PolygonalMesh.hpp"
#include "Utils.hpp"
#include "UCDUtilities.hpp"

using namespace std;
using namespace Eigen;
using namespace PolygonalLibrary;

/// test sui marker

bool TestMarkers(const PolygonalMesh& mesh)
{
    cout << "Marker registrati:" << endl;
    for (const auto& [marker, list_id] : mesh.MarkerCell0Ds)
    {
        cout << "Marker0D: " << marker << " IDs = I";
        for (auto& id: list_id)
            cout << ' ' << id;
        cout << " ]" << endl;
    };
                      
    cout << endl;
                      
    for (const auto& [marker, list_id] : mesh.MarkerCell1Ds)
    {
        cout << "Marker1D: " << marker << " IDs = [";
        for (auto& id: list_id)
            cout << ' ' << id;
        cout << " ]" << endl;
    }
    cout << endl;
    return true;
                      
}


/// test sulla lunghezza dei lati

bool TestEdges(const PolygonalMesh& mesh)
{
    for (unsigned int i = 0; i < mesh.NumCell1Ds; ++i)
    {
        unsigned int id0 = mesh.Cell1DsExtrema(0, i);
        unsigned int id1 = mesh.Cell1DsExtrema(1, i);

        // Protezione contro indici fuori dai limiti
        if (id0 >= mesh.Cell0DsCoordinates.cols() || id1 >= mesh.Cell0DsCoordinates.cols())
        {
            cerr << "Errore: indice vertice fuori dai limiti in Cell1DsExtrema: "
                 << "id0 = " << id0 << ", id1 = " << id1 << ", max = " << mesh.Cell0DsCoordinates.cols() - 1 << endl;
            return false;
        }

        Vector2d P0 = mesh.Cell0DsCoordinates.block<2, 1>(0, id0);
        Vector2d P1 = mesh.Cell0DsCoordinates.block<2, 1>(0, id1);

        if ((P1 - P0).norm() < 1e-16)
            return false;
    }

    return true;
}


/// Test sull'area

bool TestPolygons(const PolygonalMesh& mesh)
{
    for (const auto& [id, verticiIds] : mesh.Cell2DsVertices)
    {
        if (verticiIds.size() < 3)
            return false;

        double area = 0.0;

        for (size_t j = 0; j < verticiIds.size(); ++j)
        {
            unsigned int id0 = verticiIds[j];
            unsigned int id1 = verticiIds[(j + 1) % verticiIds.size()];

            Vector2d P0 = mesh.Cell0DsCoordinates.block<2, 1>(0, id0);
            Vector2d P1 = mesh.Cell0DsCoordinates.block<2, 1>(0, id1);

            area += P0.x() * P1.y() - P1.x() * P0.y();
        }

        area = fabs(area) * 0.5;

        if (area < 1e-16)
            return false;
    }

    return true;
}



int main()
{
    PolygonalMesh mesh;

    if(!ImportMesh(mesh))
    {
        cerr << "file not found" << endl;
        return 1;
    }
    /// Test 1
        if (!TestMarkers(mesh))
        {
            cerr << "Errore: marker non validi." << endl;
            return 2;
        }else {
            cout << "nessun marker non valido" << endl;
        }

    /// Test 2
        if (!TestEdges(mesh))
        {
            cerr << "Errore: esistono spigoli di lunghezza nulla." << endl;
            return 3;
        }else {
            cout << "nessun spigolo di lunghezza nulla" << endl;
        }

    /// Test 3
        if (!TestPolygons(mesh))
        {
            cerr << "Errore: esistono poligoni con area nulla." << endl;
            return 4;
        } else {
            cout << "nessun poligono ha area nulla" << endl;
        }


    Gedim::UCDUtilities utilities;

        utilities.ExportPoints("./Cell0Ds.inp", mesh.Cell0DsCoordinates);

        utilities.ExportSegments("./Cell1Ds.inp", mesh.Cell0DsCoordinates, mesh.Cell1DsExtrema);
    

    return 0;
}
