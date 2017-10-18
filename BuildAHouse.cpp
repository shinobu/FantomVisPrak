#include <fantom/algorithm.hpp>
#include <fantom/register.hpp>
#include <fantom/fields.hpp>
using namespace fantom;
namespace
{
    class BuildAHouse : public DataAlgorithm
    {
    public:
        struct Options : public DataAlgorithm::Options
        {
            Options( Control& control ) : DataAlgorithm::Options( control )
            {
                // Handle user options
                //add<int>("#Generate_x_times", "generation counter", 4);
            }
        };

        //================================================================================================================

        struct DataOutputs : public DataAlgorithm::DataOutputs
        {
            DataOutputs( Control& control ): DataAlgorithm::DataOutputs( control )
            {
              // Define outputs
                add< Grid< 3 > >( "Grid" );
            }
        };

        //================================================================================================================

        //Constructor ACHTUNG
        BuildAHouse( InitData& data ): DataAlgorithm( data )
        {
          // Keine Ahnung
        }

        //================================================================================================================

        //dynamisch Domaine generieren
        std::vector<Tensor<double,3>> domainByRoomDepth(size_t depth)
        {
          std::vector<Tensor<double,3>> raum_punkte;
          double coord[3];

          for (size_t x = 0; x < depth; x++)
          {
            for (size_t y = 0; y < depth; y++)
            {
              for (size_t z = 0; z < depth; z++)
              {
                coord[0] = x;
                coord[1] = y;
                coord[2] = z;
                Tensor<double,3> tensor(coord);
                raum_punkte.push_back(tensor);
              }
            }
          }

          return raum_punkte;
        }

        //================================================================================================================

        struct Church
        {

        };

        //================================================================================================================

        struct Tower
        {

        };

        //================================================================================================================

        struct Castle
        {

        };

        //================================================================================================================

        virtual void execute( const Algorithm::Options& options, const volatile bool& abortFlag) override
        {

          // ##### Domain staff #####
          std::vector<Tensor<double,3>> raum_punkte;
/*
          Tensor<double, 3> t1(double[] {0,0,0});
          Tensor<double, 3> t2(double[] {1,0,0});
          Tensor<double, 3> t3(double[] {2,0,0});
          Tensor<double, 3> t4(double[] {0,1,0});
          Tensor<double, 3> t5(double[] {1,1,0});
          Tensor<double, 3> t6(double[] {2,1,0}));
          Tensor<double, 3> t7(double[] {0,1,1});
          Tensor<double, 3> t8(double[] {1,1,1});
          Tensor<double, 3> t9(double[] {2,1,1});
          Tensor<double, 3> t10(double[] {0,0,1});
          Tensor<double, 3> t11(double[] {1,0,1});
          Tensor<double, 3> t12(double[] {2,0,1});
          Tensor<double, 3> t13(double[] {0,1,2});
          Tensor<double, 3> t14(double[] {1,1,2});
          Tensor<double, 3> t15(double[] {2,1,2});
          Tensor<double, 3> t16(double[] {0,0,2});
          Tensor<double, 3> t17(double[] {1,0,2});
          Tensor<double, 3> t18(double[] {2,0,2});
*/
          //Domain coordinates



          //Domain coordinates indices
/*
          double coord_1[3] = {-0.5, -0.5, 0.5};
          double coord_2[3] = {0.5, -0.5, 0.5};
          double coord_3[3] = {0.5, -0.5, -0.5};
          double coord_4[3] = {-0.5, -0.5, -0.5};
          double coord_5[3] = {-0.5, 0.5, -0.5};
          double coord_6[3] = {0.5, 0.5, -0.5};
          double coord_7[3] = {0.5, 0.5, 0.5};
          double coord_8[3] = {-0.5, 0.5, 0.5};
          double coord_9[3] = {0, 1, 0};
          Tensor<double, 3> tensor_1(coord_1);
          Tensor<double, 3> tensor_2(coord_2);
          Tensor<double, 3> tensor_3(coord_3);
          Tensor<double, 3> tensor_4(coord_4);
          Tensor<double, 3> tensor_5(coord_5);
          Tensor<double, 3> tensor_6(coord_6);
          Tensor<double, 3> tensor_7(coord_7);
          Tensor<double, 3> tensor_8(coord_8);
          Tensor<double, 3> tensor_9(coord_9);
          std::vector< Tensor<double, 3>  > points;
          points.push_back(tensor_1);
          points.push_back(tensor_2);
          points.push_back(tensor_3);
          points.push_back(tensor_4);
          points.push_back(tensor_5);
          points.push_back(tensor_6);
          points.push_back(tensor_7);
          points.push_back(tensor_8);
          points.push_back(tensor_9);
          //std::shared_ptr< const DiscreteDomain< 3 > > mDomain = DomainFactory::makeDomainArbitrary(points, Precision::FLOAT64);

*/
          std::shared_ptr< const DiscreteDomain< 3 > > mDomain = DomainFactory::makeDomainArbitrary( domainByRoomDepth(4), Precision::FLOAT64 );


          // ##### Anzahl Zelltypen #####
          size_t t = (size_t) 3;

          // ##### Welcher Zelltyp #####
          const std::pair< Cell::Type, size_t > voxel_pair(Cell::HEXAHEDRON,(size_t) 2);
          const std::pair< Cell::Type, size_t > prism_pair(Cell::PRISM,(size_t) 1);
          const std::pair< Cell::Type, size_t > pyramid_pair(Cell::PYRAMID,(size_t) 1);
          const std::pair< Cell::Type, size_t > cellCounts[3] = {voxel_pair, prism_pair, pyramid_pair};

          //const std::pair< Cell::Type, size_t > cellCounts[3] =
          //{
          //  std::make_pair(Cell::HEXAHEDRON,(size_t) 2),
          //  std::make_pair(Cell::PRISM,(size_t) 1),
          //  std::make_pair(Cell::PYRAMID,(size_t) 1)
          //};

          // ##### Zell indices #####
          std::vector<size_t> vertice_vec = std::vector<size_t>();
          vertice_vec.resize(27);

          // HEXAHEDRON 1
          vertice_vec.push_back((size_t) 0);
          vertice_vec.push_back((size_t) 1);
          vertice_vec.push_back((size_t) 2);
          vertice_vec.push_back((size_t) 3);
          vertice_vec.push_back((size_t) 4);
          vertice_vec.push_back((size_t) 5);
          vertice_vec.push_back((size_t) 6);
          vertice_vec.push_back((size_t) 7);

          // HEXAHEDRON 2
          vertice_vec.push_back((size_t) 1);
          vertice_vec.push_back((size_t) 5);
          vertice_vec.push_back((size_t) 2);
          vertice_vec.push_back((size_t) 6);
          vertice_vec.push_back((size_t) 8);
          vertice_vec.push_back((size_t) 9);
          vertice_vec.push_back((size_t) 10);
          vertice_vec.push_back((size_t) 11);

          // PYRAMID 1
          vertice_vec.push_back((size_t) 4);
          vertice_vec.push_back((size_t) 5);
          vertice_vec.push_back((size_t) 6);
          vertice_vec.push_back((size_t) 7);
          vertice_vec.push_back((size_t) 12);

          //PRISM 1
          vertice_vec.push_back((size_t) 5);
          vertice_vec.push_back((size_t) 6);
          vertice_vec.push_back((size_t) 10);
          vertice_vec.push_back((size_t) 11);
          vertice_vec.push_back((size_t) 13);
          vertice_vec.push_back((size_t) 14);

          //##### Make Object #####
          DefaultValueArray< size_t >* indArray = new DefaultValueArray< size_t >( vertice_vec, Precision::FLOAT64);
          std::unique_ptr< ValueArray< size_t > > indices((ValueArray<size_t>*) indArray);

          std::shared_ptr< const Grid< 3 > > mGrid = DomainFactory::makeGridUnstructured(*mDomain, t, cellCounts, std::move(indices));
          setResult( "Grid", mGrid );

        }
    };

    AlgorithmRegister< BuildAHouse > dummy( "Praktikum/Test", "Little City" );
}
