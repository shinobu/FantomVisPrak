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
                add< Grid< 3 > >( "Church" );
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

          for (size_t z = 0; z < depth; z++)
          {
            for (size_t y = 0; y < depth; y++)
            {
              for (size_t x = 0; x < depth; x++)
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

        

        //================================================================================================================

        struct getChurch
        {

        };

        //================================================================================================================

        struct Tower
        {
          // Construct Tower
        };

        //================================================================================================================

        struct Castle
        {
          // Construct Castle
        };

        //================================================================================================================

        virtual void execute( const Algorithm::Options& /*options*/, const volatile bool& /*abortFlag*/) override
        {

          // ##### Domain staff #####
          int depth = 4;
          std::shared_ptr< const DiscreteDomain< 3 > > mDomain = DomainFactory::makeDomainArbitrary( domainByRoomDepth(depth), Precision::FLOAT64 );


          // ##### Anzahl Zelltypen #####
          size_t t = (size_t) 3;

          // ##### Welcher Zelltyp #####
          const std::pair< Cell::Type, size_t > voxel_pair(Cell::HEXAHEDRON, (size_t)2);
          const std::pair< Cell::Type, size_t > prism_pair(Cell::PRISM, (size_t)1);
          const std::pair< Cell::Type, size_t > pyramid_pair(Cell::PYRAMID, (size_t)1);
          const std::pair< Cell::Type, size_t > cellCounts[3] = {voxel_pair, prism_pair, pyramid_pair};

          // ##### Zell indices #####
          std::vector<size_t> church_vec;

          // HEXAHEDRON 1 (8 Elem)          //(x,y,z)
          church_vec.push_back((size_t) 0); //(0,0,1)
          church_vec.push_back((size_t) 1); //(1,0,1)
          church_vec.push_back((size_t) 2); //(1,0,0)
          church_vec.push_back((size_t) 3); //(0,0,0)
          church_vec.push_back((size_t) 4); //(0,1,0)
          church_vec.push_back((size_t) 5); //(1,1,0)
          church_vec.push_back((size_t) 6); //(1,1,1)
          church_vec.push_back((size_t) 7); //(0,1,1)

          // HEXAHEDRON 2 (8 Elem)
          church_vec.push_back((size_t) 1); //(,,)
          church_vec.push_back((size_t) 5); //(,,)
          church_vec.push_back((size_t) 2); //(,,)
          church_vec.push_back((size_t) 6); //(,,)
          church_vec.push_back((size_t) 8); //(,,)
          church_vec.push_back((size_t) 9); //(,,)
          church_vec.push_back((size_t) 10); //(,,)
          church_vec.push_back((size_t) 11); //(,,)

          // PYRAMID 1  (5 Elem)
          church_vec.push_back((size_t) 4); //(,,)
          church_vec.push_back((size_t) 5); //(,,)
          church_vec.push_back((size_t) 6); //(,,)
          church_vec.push_back((size_t) 7); //(,,)
          church_vec.push_back((size_t) 12); //(,,)

          //PRISM 1 (6 Elem)
          church_vec.push_back((size_t) 5); //(,,)
          church_vec.push_back((size_t) 6); //(,,)
          church_vec.push_back((size_t) 10); //(,,)
          church_vec.push_back((size_t) 11); //(,,)
          church_vec.push_back((size_t) 13); //(,,)
          church_vec.push_back((size_t) 14); //(,,)

          //##### Make Object #####
          DefaultValueArray< size_t >* indArray = new DefaultValueArray< size_t >( church_vec, Precision::FLOAT64);
          std::unique_ptr< ValueArray< size_t > > indices((ValueArray<size_t>*) indArray);

          std::shared_ptr< const Grid< 3 > > mGrid = DomainFactory::makeGridUnstructured(*mDomain, t, cellCounts, std::move(indices));
          setResult( "Church", mGrid );

        }
    };

    AlgorithmRegister< BuildAHouse > dummy( "Praktikum/Church", "Little City" );
}
