#include "PolygonalMesh.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <vector>
#include <list>
#include <Eigen/Dense>
#include "Utils.hpp"

namespace PolygonalLibrary {

    bool ImportMesh(PolygonalMesh& mesh) {
        if (!ImportCell0Ds(mesh))
            return false;

        if (!ImportCell1Ds(mesh))
            return false;

        if (!ImportCell2Ds(mesh))
            return false;

        return true;
    }

    // ***************************************************************************
bool ImportCell0Ds(PolygonalMesh& mesh)
{
    ifstream file("./Cell0Ds.csv");

    if(file.fail())
        return false;

    list<string> listLines;

    string line;
    while (getline(file, line))
        listLines.push_back(line);

    file.close();

    // remove header
    listLines.pop_front();

    mesh.NumCell0Ds = listLines.size();

    if (mesh.NumCell0Ds == 0)
    {
        cerr << "There is no cell 0D" << endl;
        return false;
    }

    mesh.Cell0DsId.reserve(mesh.NumCell0Ds);
    mesh.Cell0DsCoordinates = Eigen::MatrixXd::Zero(3, mesh.NumCell0Ds);

    for (string& line : listLines)
    {
        replace(line.begin(), line.end(), ';', ' ');
        istringstream converter(line);

        unsigned int id;
        unsigned int marker;
        Vector2d coord;

        converter >>  id >> marker >> mesh.Cell0DsCoordinates(0, id) >> mesh.Cell0DsCoordinates(1, id);

        mesh.Cell0DsId.push_back(id);

        /// Memorizza i marker
        if(marker != 0)
        {
            const auto it = mesh.MarkerCell0Ds.find(marker);
            if(it == mesh.MarkerCell0Ds.end())
            {
                mesh.MarkerCell0Ds.insert({marker, {id}});
            }
            else
            {
                // mesh.MarkerCell0Ds[marker].push_back(id);
                it->second.push_back(id);
            }
        }

    }

    return true;
}

    // ***************************************************************************
bool ImportCell1Ds(PolygonalMesh& mesh) {
    ifstream file("./Cell1Ds.csv");

    if(file.fail())
        return false;

    list<string> listLines;
    string line;
    while (getline(file, line))
        listLines.push_back(line);

    file.close();

    // remove header
    listLines.pop_front();

    mesh.NumCell1Ds = listLines.size();

    if (mesh.NumCell1Ds == 0)
    {
        cerr << "There is no cell 1D" << endl;
        return false;
    }

    mesh.Cell1DsId.reserve(mesh.NumCell1Ds);
    mesh.Cell1DsExtrema = Eigen::MatrixXi(2, mesh.NumCell1Ds);

    for (string& line : listLines)
    {
        replace(line.begin(), line.end(), ';', ' ');
        istringstream converter(line);

        unsigned int id;
        unsigned int marker;
        Vector2i vertices;

        converter >>  id >> marker >>  mesh.Cell1DsExtrema(0, id) >>  mesh.Cell1DsExtrema(1, id);
        mesh.Cell1DsId.push_back(id);

        /// Memorizza i marker
        if(marker != 0)
        {
            const auto it = mesh.MarkerCell1Ds.find(marker);
            if(it == mesh.MarkerCell1Ds.end())
            {
                mesh.MarkerCell1Ds.insert({marker, {id}});
            }
            else
            {
                // mesh.MarkerCell1Ds[marker].push_back(id);
                it->second.push_back(id);
            }
        }
    }

    return true;
    }

    // ***************************************************************************
bool ImportCell2Ds(PolygonalMesh& mesh) {
    std::ifstream file("./Cell2Ds.csv");

    if (file.fail())
        return false;

    std::list<std::string> listLines;
    std::string line;
    while (std::getline(file, line))
        listLines.push_back(line);

    file.close();

    // Remove header
    listLines.pop_front();

    if (listLines.empty()) {
        std::cerr << "There is no cell 2D" << std::endl;
        return false;
    }

    // Pulisce eventuali dati preesistenti
    mesh.Cell2DsVertices.clear();
    mesh.Cell2DsEdges.clear();

    for (std::string& line : listLines)
    {
        replace(line.begin(), line.end(), ';', ' ');
        std::istringstream converter(line);

        unsigned int id;
        unsigned int marker;
        unsigned int NumVertices;
        unsigned int NumEdges;

        converter >> id >> marker >> NumVertices;

        std::vector<unsigned int> vertices(NumVertices);
        for (unsigned int i = 0; i < NumVertices; ++i)
            converter >> vertices[i];

        converter >> NumEdges;

        std::vector<unsigned int> edges(NumEdges);
        for (unsigned int i = 0; i < NumEdges; ++i)
            converter >> edges[i];

        // Inserisce nei dizionari
        mesh.Cell2DsVertices[id] = vertices;
        mesh.Cell2DsEdges[id] = edges;
    }

    // Puoi aggiornare NumCell2Ds alla fine
    mesh.NumCell2Ds = mesh.Cell2DsVertices.size();

    return true;
}

}
