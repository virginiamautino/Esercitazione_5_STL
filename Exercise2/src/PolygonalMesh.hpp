#pragma once
#include <unordered_map>
#include <map>
#include <vector>
#include <Eigen/Eigen>
#include <list>

using namespace std;
using namespace Eigen;

namespace PolygonalLibrary {

    struct PolygonalMesh {
        // Membri relativi alle celle
        unsigned int NumCell0Ds = 0; ///< number of Cell0D
        std::vector<unsigned int> Cell0DsId = {}; ///< Cell0D id, size 1 x NumberCell0D
        Eigen::MatrixXd Cell0DsCoordinates = {}; ///< Cell0D coordinates, size 2 x NumberCell0D (x,y)
        std::map<unsigned int, list<unsigned int>> MarkerCell0Ds = {}; ///< Cell0D markers

        unsigned int NumCell1Ds = 0; ///< number of Cell1D
        std::vector<unsigned int> Cell1DsId = {}; ///< Cell1D id, size 1 x NumberCell1D
        Eigen::MatrixXi Cell1DsExtrema = {}; ///< Cell1D vertices indices, size 2 x NumberCell1D (fromId,toId)
        std::map<unsigned int, list<unsigned int>> MarkerCell1Ds = {}; ///< Cell1D properties
        
        
        
        unsigned int NumCell2Ds = 0; ///< number of Cell2D
        std::vector<unsigned int> Cell2DsId = {}; ///< Cell2D id, size 1 x NumberCell2D
        std::unordered_map<unsigned int, std::vector<unsigned int>> Cell2DsVertices; // id → vertici
        std::unordered_map<unsigned int, std::vector<unsigned int>> Cell2DsEdges;    // id → spigoli

    };

}

